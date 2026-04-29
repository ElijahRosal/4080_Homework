// Elijah Rosal - CS4080 - Homework 12, Chapter 25 Question 1
// 4.28.2026
/*
Code below has been modified for Question 1 for Chapter 25 of Crafting Interpreters.

Functions without upvalues are executed directly, so each call frame keeps the
underlying ObjFunction alongside an optional ObjClosure pointer. That lets the
VM avoid allocating a closure for plain functions while still supporting upvalue
capture when closures are actually required.
*/

#ifndef clox_vm_h
#define clox_vm_h
#include "object.h"
#include "table.h"
#include "value.h"
#define FRAMES_MAX 64
#define STACK_MAX (FRAMES_MAX * UINT8_COUNT)
typedef struct {
 ObjClosure* closure;
 ObjFunction* function;
 uint8_t* ip;
 Value* slots;
} CallFrame;
typedef struct {
 CallFrame frames[FRAMES_MAX];
 int frameCount;
 Value stack[STACK_MAX];
 Value* stackTop;
 Table globals;
 Table strings;
 ObjUpvalue* openUpvalues;
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