// Elijah Rosal - CS4080 - Homework 7, Chapter 14 Question 2
// 3.24.2026
/*
Code below has been modified for Question 2 for Chapter 14 of Crafting Interpreters.

Implements writeConstant() to emit OP_CONSTANT for 8-bit operands and
OP_CONSTANT_LONG for 24-bit operands, allowing chunks to address many more
constants while keeping the small instruction form for the common case.
*/

#include <stdlib.h>
#include <stdio.h>
#include "chunk.h"
#include "memory.h"
void initChunk(Chunk* chunk) {
    chunk->count = 0;
    chunk->capacity = 0;
    chunk->code = NULL;
    chunk->lines = NULL;
    initValueArray(&chunk->constants);
}
void freeChunk(Chunk* chunk) {
    FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
    FREE_ARRAY(int, chunk->lines, chunk->capacity);
    freeValueArray(&chunk->constants);
    initChunk(chunk);
}
void writeChunk(Chunk* chunk, uint8_t byte, int line) {
    if (chunk->capacity < chunk->count + 1) {
        int oldCapacity = chunk->capacity;
        chunk->capacity = GROW_CAPACITY(oldCapacity);
        chunk->code = GROW_ARRAY(uint8_t, chunk->code, oldCapacity, chunk->capacity);
        chunk->lines = GROW_ARRAY(int, chunk->lines, oldCapacity, chunk->capacity);
    }
    chunk->code[chunk->count] = byte;
    chunk->lines[chunk->count] = line;
    chunk->count++;
}
int addConstant(Chunk* chunk, Value value) {
    writeValueArray(&chunk->constants, value);
    return chunk->constants.count - 1;
}

void writeConstant(Chunk* chunk, Value value, int line) {
    int constant = addConstant(chunk, value);

    if (constant <= UINT8_MAX) {
        writeChunk(chunk, OP_CONSTANT, line);
        writeChunk(chunk, (uint8_t) constant, line);
        return;
    }

    if (constant <= 0xFFFFFF) {
        writeChunk(chunk, OP_CONSTANT_LONG, line);
        writeChunk(chunk, (uint8_t) ((constant >> 16) & 0xFF), line);
        writeChunk(chunk, (uint8_t) ((constant >> 8) & 0xFF), line);
        writeChunk(chunk, (uint8_t) (constant & 0xFF), line);
        return;
    }

    fprintf(stderr, "Too many constants in one chunk.\n");
    exit(1);
}