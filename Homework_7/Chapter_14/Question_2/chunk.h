// Elijah Rosal - CS4080 - Homework 7, Chapter 14 Question 2
// 3.24.2026
/*
Code below has been modified for Question 2 for Chapter 14 of Crafting Interpreters.

Adds OP_CONSTANT_LONG for 24-bit constant indices and declares writeConstant(),
which selects the short or long constant instruction based on operand size.
*/

#ifndef clox_chunk_h
#define clox_chunk_h
#include "common.h"
#include "value.h"
typedef enum {
    OP_CONSTANT,
    OP_CONSTANT_LONG,
    OP_RETURN,
   } OpCode;
typedef struct {
    int count;
    int capacity;
    int* lines;
    uint8_t* code;
    ValueArray constants;
} Chunk;
void initChunk(Chunk* chunk);
void freeChunk(Chunk* chunk);
void writeChunk(Chunk* chunk, uint8_t byte, int line);
int addConstant(Chunk* chunk, Value value);
void writeConstant(Chunk* chunk, Value value, int line);
#endif
