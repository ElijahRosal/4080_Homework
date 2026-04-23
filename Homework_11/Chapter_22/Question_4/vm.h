// Elijah Rosal - CS4080 - Homework 11, Chapter 22 Question 4
// 4.23.2026
/*
Code below has been modified for Question 4 for Chapter 22 of Crafting Interpreters.

Raises VM stack capacity to support the expanded local slot range.
*/

#ifndef clox_vm_h
#define clox_vm_h
#include "chunk.h"
#include "table.h"
#include "value.h"
#define STACK_MAX UINT16_COUNT
typedef struct {
 Chunk* chunk;
 uint8_t* ip;
 Value stack[STACK_MAX];
 Value* stackTop;
 Table globals;
 Table strings;
 Obj* objects;
} VM;
typedef enum {
 INTERPRET_OK,
 INTERPRET_COMPILE_ERROR,
 INTERPRET_RUNTIME_ERROR
} InterpretResult;
extern VM vm;
void initVM();
void freeVM();
InterpretResult interpret(const char* source);
void push(Value value);
Value pop();
#endif