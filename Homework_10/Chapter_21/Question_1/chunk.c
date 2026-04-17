// Elijah Rosal - CS4080 - Homework 10, Chapter 20 Question 1
// 4.16.2026
/*
Code below has been modified for Question 1 for Chapter 20 of Crafting Interpreters.

Updates string interning table insertion to use the generalized Value-key
table API.
*/
#include <stdlib.h>
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
static int findConstant(Chunk* chunk, Value value) {
    for (int i = 0; i < chunk->constants.count; i++) {
        if (valuesEqual(chunk->constants.values[i], value)) return i;
    }
    return -1;
}
int addConstant(Chunk* chunk, Value value) {
    int existing = findConstant(chunk, value);
    if (existing != -1) return existing;
    writeValueArray(&chunk->constants, value);
    return chunk->constants.count - 1;
}