// Elijah Rosal - CS4080 - Homework 7, Chapter 14 Question 3
// 3.24.2026
/*
Code below has been modified for Question 3 for Chapter 14 of Crafting Interpreters.

Bootstraps and tears down the custom single-region allocator used by
reallocate() in memory.c.
*/

#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "memory.h"
int main(int argc, const char* argv[]) {
    initAllocator();

    Chunk chunk;
    initChunk(&chunk);
    int constant = addConstant(&chunk, 1.2);
    writeChunk(&chunk, OP_CONSTANT, 123);
    writeChunk(&chunk, constant, 123);
    writeChunk(&chunk, OP_RETURN, 123);
    disassembleChunk(&chunk, "test chunk");
    freeChunk(&chunk);

    shutdownAllocator();
    return 0;
}
