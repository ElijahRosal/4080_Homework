// Elijah Rosal - CS4080 - Homework 12, Chapter 24 Question 3
// 4.28.2026
/*
Code below has been modified for Question 3 for Chapter 24 of Crafting Interpreters.

Extends the VM structure to support error signaling from native functions.
Adds error state management allowing native functions to report runtime errors
back to the interpreter, enabling type checking and validation at the C level.
*/

#ifndef clox_vm_h
#define clox_vm_h
#include "object.h"
#include "table.h"
#include "value.h"
#define FRAMES_MAX 64
#define STACK_MAX (FRAMES_MAX * UINT8_COUNT)
#define NATIVE_ERROR_MAX 512

typedef struct {
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
 Obj* objects;
 bool hasNativeError;
 char nativeError[NATIVE_ERROR_MAX];
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
void setNativeError(const char* format, ...);
bool getNativeError();
#endif