// Elijah Rosal - CS4080 - Homework 7, Chapter 14 Question 3
// 3.24.2026
/*
Code below has been modified for Question 3 for Chapter 14 of Crafting Interpreters.

Implements a custom allocator backed by one large heap block acquired once at
startup. reallocate() manages split/merge free blocks inside this region using
an explicit free list and does not call malloc/realloc/free.
*/

#include <stdlib.h>
#include <string.h>
#include "memory.h"

#define VM_HEAP_SIZE (1024 * 1024)
#define ALIGNMENT 8

typedef struct Block {
    size_t size;
    bool isFree;
    struct Block* prev;
    struct Block* next;
    struct Block* prevFree;
    struct Block* nextFree;
} Block;

static uint8_t* heapStart = NULL;
static Block* firstBlock = NULL;
static Block* freeListHead = NULL;

static size_t alignUp(size_t size) {
    return (size + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1);
}

static void* payloadFromBlock(Block* block) {
    return (void*) (block + 1);
}

static Block* blockFromPayload(void* pointer) {
    return ((Block*) pointer) - 1;
}

static void removeFree(Block* block) {
    if (block->prevFree != NULL) {
        block->prevFree->nextFree = block->nextFree;
    } else {
        freeListHead = block->nextFree;
    }

    if (block->nextFree != NULL) {
        block->nextFree->prevFree = block->prevFree;
    }

    block->prevFree = NULL;
    block->nextFree = NULL;
}

static void insertFree(Block* block) {
    block->isFree = true;
    block->prevFree = NULL;
    block->nextFree = freeListHead;

    if (freeListHead != NULL) {
        freeListHead->prevFree = block;
    }

    freeListHead = block;
}

static void splitBlock(Block* block, size_t requestedSize) {
    size_t minimumRemainder = sizeof(Block) + ALIGNMENT;
    if (block->size < requestedSize + minimumRemainder) {
        return;
    }

    uint8_t* remainderAddress = (uint8_t*) payloadFromBlock(block) + requestedSize;
    Block* remainder = (Block*) remainderAddress;
    remainder->size = block->size - requestedSize - sizeof(Block);
    remainder->isFree = true;
    remainder->prev = block;
    remainder->next = block->next;
    remainder->prevFree = NULL;
    remainder->nextFree = NULL;

    if (block->next != NULL) {
        block->next->prev = remainder;
    }

    block->next = remainder;
    block->size = requestedSize;

    insertFree(remainder);
}

static Block* coalesceWithNext(Block* block) {
    Block* next = block->next;
    if (next == NULL || !next->isFree) {
        return block;
    }

    removeFree(next);
    block->size += sizeof(Block) + next->size;
    block->next = next->next;

    if (block->next != NULL) {
        block->next->prev = block;
    }

    return block;
}

static void freeBlock(Block* block) {
    block->isFree = true;

    if (block->next != NULL && block->next->isFree) {
        block = coalesceWithNext(block);
    }

    if (block->prev != NULL && block->prev->isFree) {
        block = coalesceWithNext(block->prev);
    }

    insertFree(block);
}

static void* allocateBlock(size_t requestedSize) {
    Block* current = freeListHead;
    while (current != NULL) {
        if (current->size >= requestedSize) {
            removeFree(current);
            current->isFree = false;
            splitBlock(current, requestedSize);
            return payloadFromBlock(current);
        }
        current = current->nextFree;
    }

    return NULL;
}

static bool growIntoNextFree(Block* block, size_t requestedSize) {
    Block* next = block->next;
    if (next == NULL || !next->isFree) {
        return false;
    }

    size_t combined = block->size + sizeof(Block) + next->size;
    if (combined < requestedSize) {
        return false;
    }

    coalesceWithNext(block);
    splitBlock(block, requestedSize);
    return true;
}

void initAllocator(void) {
    if (heapStart != NULL) {
        return;
    }

    heapStart = (uint8_t*) malloc(VM_HEAP_SIZE);
    if (heapStart == NULL) {
        exit(1);
    }

    firstBlock = (Block*) heapStart;
    firstBlock->size = VM_HEAP_SIZE - sizeof(Block);
    firstBlock->isFree = true;
    firstBlock->prev = NULL;
    firstBlock->next = NULL;
    firstBlock->prevFree = NULL;
    firstBlock->nextFree = NULL;
    freeListHead = firstBlock;
}

void shutdownAllocator(void) {
    if (heapStart == NULL) {
        return;
    }

    free(heapStart);
    heapStart = NULL;
    firstBlock = NULL;
    freeListHead = NULL;
}

void* reallocate(void* pointer, size_t oldSize, size_t newSize) {
    if (heapStart == NULL) {
        initAllocator();
    }

    if (pointer == NULL) {
        if (newSize == 0) {
            return NULL;
        }

        void* result = allocateBlock(alignUp(newSize));
        if (result == NULL) {
            exit(1);
        }
        return result;
    }

    Block* block = blockFromPayload(pointer);

    if (newSize == 0) {
        freeBlock(block);
        return NULL;
    }

    size_t requestedSize = alignUp(newSize);
    if (requestedSize <= block->size) {
        splitBlock(block, requestedSize);
        return pointer;
    }

    if (growIntoNextFree(block, requestedSize)) {
        return pointer;
    }

    void* destination = allocateBlock(requestedSize);
    if (destination == NULL) {
        exit(1);
    }

    size_t copySize = oldSize < newSize ? oldSize : newSize;
    if (copySize > block->size) {
        copySize = block->size;
    }
    memcpy(destination, pointer, copySize);

    freeBlock(block);
    return destination;
}