// Elijah Rosal - CS4080 - Homework 7, Chapter 14 Question 2
// 3.24.2026
/*
Code below has been modified for Question 2 for Chapter 14 of Crafting Interpreters.

Uses writeConstant() so constant emission automatically selects OP_CONSTANT or
OP_CONSTANT_LONG depending on constant table index size.
*/

#include "common.h"
#include "chunk.h"
#include "debug.h"
int main(int argc, const char* argv[]) {
    Chunk chunk;
    initChunk(&chunk);
    writeConstant(&chunk, 1.2, 123);
    writeChunk(&chunk, OP_RETURN, 123);
    disassembleChunk(&chunk, "test chunk");
    freeChunk(&chunk);
    return 0;
}
