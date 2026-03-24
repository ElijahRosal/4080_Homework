// Elijah Rosal - CS4080 - Homework 7, Chapter 14 Question 1
// 3.24.2026
/*
Code below has been modified for Question 1 for Chapter 14 of Crafting Interpreters.

Replaces per-instruction line storage with run-length encoding in writeChunk(),
adds getLine() lookup for instruction-to-source mapping, and updates allocation
and cleanup logic for the compressed line run table.
*/

#include <stdlib.h>
#include "chunk.h"
#include "memory.h"
void initChunk(Chunk* chunk) {
    chunk->count = 0;
    chunk->capacity = 0;
    chunk->lineCount = 0;
    chunk->lineCapacity = 0;
    chunk->code = NULL;
    chunk->lines = NULL;
    initValueArray(&chunk->constants);
}
void freeChunk(Chunk* chunk) {
    FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
    FREE_ARRAY(LineRun, chunk->lines, chunk->lineCapacity);
    freeValueArray(&chunk->constants);
    initChunk(chunk);
}
void writeChunk(Chunk* chunk, uint8_t byte, int line) {
    if (chunk->capacity < chunk->count + 1) {
        int oldCapacity = chunk->capacity;
        chunk->capacity = GROW_CAPACITY(oldCapacity);
        chunk->code = GROW_ARRAY(uint8_t, chunk->code, oldCapacity, chunk->capacity);
    }
    chunk->code[chunk->count] = byte;
    if (chunk->lineCount > 0 &&
            chunk->lines[chunk->lineCount - 1].line == line) {
        chunk->lines[chunk->lineCount - 1].count++;
    } else {
        if (chunk->lineCapacity < chunk->lineCount + 1) {
            int oldCapacity = chunk->lineCapacity;
            chunk->lineCapacity = GROW_CAPACITY(oldCapacity);
            chunk->lines = GROW_ARRAY(
                    LineRun, chunk->lines, oldCapacity, chunk->lineCapacity);
        }
        chunk->lines[chunk->lineCount].line = line;
        chunk->lines[chunk->lineCount].count = 1;
        chunk->lineCount++;
    }
    chunk->count++;
}

int getLine(Chunk* chunk, int instruction) {
    int offset = instruction;

    for (int i = 0; i < chunk->lineCount; i++) {
        if (offset < chunk->lines[i].count) {
            return chunk->lines[i].line;
        }
        offset -= chunk->lines[i].count;
    }

    return -1;
}

int addConstant(Chunk* chunk, Value value) {
    writeValueArray(&chunk->constants, value);
    return chunk->constants.count - 1;
}