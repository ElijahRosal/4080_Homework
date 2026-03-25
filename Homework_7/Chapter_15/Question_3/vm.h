// Elijah Rosal - CS4080 - Homework 7, Chapter 15 Question 3
// 3.24.2026
/*
Code below has been modified for Question 3 for Chapter 15 of Crafting Interpreters.

Replaces the fixed-size VM stack array with a heap-allocated stack that tracks
capacity and can grow at runtime as needed.
*/

#ifndef clox_vm_h
#define clox_vm_h
#include "chunk.h"
#include "value.h"

typedef struct {
 Chunk* chunk;
 uint8_t* ip;
 Value* stack;
 int stackCapacity;
 Value* stackTop;
} VM;
typedef enum {
 INTERPRET_OK,
 INTERPRET_COMPILE_ERROR,
 INTERPRET_RUNTIME_ERROR
} InterpretResult;
void initVM();
void freeVM();
InterpretResult interpret(Chunk* chunk);
void push(Value value);
Value pop();
#endif