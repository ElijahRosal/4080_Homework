// Elijah Rosal - CS4080 - Homework 7, Chapter 15 Question 3
// 3.24.2026
/*
Code below has been modified for Question 3 for Chapter 15 of Crafting Interpreters.

Implements dynamic VM stack growth by allocating the stack on the heap,
growing capacity on demand during push operations, and freeing stack memory
when the VM is released.
*/

#include <stdio.h>
#include "common.h"
#include "debug.h"
#include "memory.h"
#include "vm.h"
VM vm;

static void resetStack() {
    vm.stackTop = vm.stack;
}

static void growStack() {
    int oldCapacity = vm.stackCapacity;
    int count = (int)(vm.stackTop - vm.stack);
    vm.stackCapacity = GROW_CAPACITY(oldCapacity);
    vm.stack = GROW_ARRAY(Value, vm.stack, oldCapacity, vm.stackCapacity);
    vm.stackTop = vm.stack + count;
}

void initVM() {
    vm.stack = NULL;
    vm.stackCapacity = 0;
    resetStack();
}

void freeVM() {
    FREE_ARRAY(Value, vm.stack, vm.stackCapacity);
    vm.stack = NULL;
    vm.stackTop = NULL;
    vm.stackCapacity = 0;
}

void push(Value value) {
    if (vm.stackTop - vm.stack >= vm.stackCapacity) {
        growStack();
    }

    *vm.stackTop = value;
    vm.stackTop++;
}

Value pop() {
    vm.stackTop--;
    return *vm.stackTop;
}
static InterpretResult run() {
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
#define BINARY_OP(op) \
do { \
double b = pop(); \
double a = pop(); \
push(a op b); \
} while (false)
    for (;;) {
        #ifdef DEBUG_TRACE_EXECUTION
        printf(" ");
        for (Value* slot = vm.stack; slot < vm.stackTop; slot++) {
            printf("[ ");
            printValue(*slot);
            printf(" ]");
        }
        printf("\n");
        disassembleInstruction(vm.chunk,
        (int)(vm.ip - vm.chunk->code));
        #endif
        uint8_t instruction;
        switch (instruction = READ_BYTE()) {
            case OP_CONSTANT: {
                Value constant = READ_CONSTANT();
                push(constant);
                break;
            }
            case OP_ADD: BINARY_OP(+); break;
            case OP_SUBTRACT: BINARY_OP(-); break;
            case OP_MULTIPLY: BINARY_OP(*); break;
            case OP_DIVIDE: BINARY_OP(/); break;
            case OP_NEGATE: push(-pop()); break;
            case OP_RETURN: {
                return INTERPRET_OK;
            }
        }
    }
#undef READ_BYTE
#undef READ_CONSTANT
#undef BINARY_OP
}
InterpretResult interpret(Chunk* chunk) {
    vm.chunk = chunk;
    vm.ip = vm.chunk->code;
    return run();
}