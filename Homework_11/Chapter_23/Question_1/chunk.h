// Elijah Rosal - CS4080 - Homework 11, Chapter 22 Question 1
// 4.23.2026
/*
Code below has been modified for Question 1 for Chapter 22 of Crafting Interpreters.

Replaces linear local-variable resolution with a hash-bucket based lookup
structure to reduce average identifier lookup cost while preserving scope
and shadowing behavior.
*/

#ifndef clox_chunk_h
#define clox_chunk_h
#include "common.h"
#include "value.h"
typedef enum {
    OP_CONSTANT,
    OP_NIL,
    OP_TRUE,
    OP_FALSE,
    OP_POP,
    OP_DUP,
    OP_GET_LOCAL,
    OP_SET_LOCAL,
    OP_GET_GLOBAL,
    OP_DEFINE_GLOBAL,
    OP_SET_GLOBAL,
    OP_EQUAL,
    OP_GREATER,
    OP_LESS,
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_NOT,
    OP_NEGATE,
    OP_PRINT,
    OP_JUMP,
    OP_JUMP_IF_FALSE,
    OP_LOOP,
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
#endif
