// Elijah Rosal - CS4080 - Homework 7, Chapter 14 Question 1
// 3.24.2026
/*
Code below has been modified for Question 1 for Chapter 14 of Crafting Interpreters.

Switches disassembly line reporting to use getLine() so source line display
works with the new run-length encoded line metadata in Chunk.
*/

#include <stdio.h>
#include "debug.h"
#include "value.h"
void disassembleChunk(Chunk* chunk, const char* name) {
    printf("== %s ==\n", name);
    for (int offset = 0; offset < chunk->count;) {
        offset = disassembleInstruction(chunk, offset);
    }
}
static int simpleInstruction(const char* name, int offset) {
    printf("%s\n", name);
    return offset + 1;
}
static int constantInstruction(const char* name, Chunk* chunk,
 int offset) {
    uint8_t constant = chunk->code[offset + 1];
    printf("%-16s %4d '", name, constant);
    printValue(chunk->constants.values[constant]);
    printf("'\n");
    return offset + 2;
}
int disassembleInstruction(Chunk* chunk, int offset) {
    printf("%04d ", offset);
    int line = getLine(chunk, offset);
    int previousLine = offset > 0 ? getLine(chunk, offset - 1) : -1;
    if (offset > 0 && line == previousLine) {
        printf(" | ");
    } else {
        printf("%4d ", line);
    }
    uint8_t instruction = chunk->code[offset];
    switch (instruction) {
        case OP_CONSTANT:
            return constantInstruction("OP_CONSTANT", chunk,
           offset);
        case OP_RETURN:
            return simpleInstruction("OP_RETURN", offset);
        default:
            printf("Unknown opcode %d\n", instruction);
            return offset + 1;
    }
}

