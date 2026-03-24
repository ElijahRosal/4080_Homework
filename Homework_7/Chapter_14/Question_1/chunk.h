// Elijah Rosal - CS4080 - Homework 7, Chapter 14 Question 1
// 3.24.2026
/*
Code below has been modified for Question 1 for Chapter 14 of Crafting Interpreters.

Introduces run-length encoded line metadata (LineRun) for bytecode chunks and
adds the getLine() declaration used to map instruction offsets back to source lines.
*/

#ifndef clox_chunk_h
#define clox_chunk_h
#include "common.h"
#include "value.h"
typedef enum {
    OP_CONSTANT,
    OP_RETURN,
   } OpCode;

typedef struct {
    int line;
    int count;
} LineRun;

typedef struct {
    int count;
    int capacity;
    int lineCount;
    int lineCapacity;
    LineRun* lines;
    uint8_t* code;
    ValueArray constants;
} Chunk;
void initChunk(Chunk* chunk);
void freeChunk(Chunk* chunk);
void writeChunk(Chunk* chunk, uint8_t byte, int line);
int getLine(Chunk* chunk, int instruction);
int addConstant(Chunk* chunk, Value value);
#endif
