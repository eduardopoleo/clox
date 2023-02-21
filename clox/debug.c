//
//  debug.c
//  clox
//
//  Created by Eduardo Poleo on 2023-02-17.
//

#include "debug.h"

void disassembleChunk(Chunk *chunk, char *name) {
    printf("== %s ==\n", name);
    
    for (int offset = 0; offset < chunk->count;) {
        offset = disassembleInstruction(chunk, offset);
    }
}

/*
    Does not need to be defined in the header but it obeys
    definition order. So it must be defined before where's used.
*/

static int simpleInstruction(const char* name, int offset) {
    printf("%s\n", name);
    return offset + 1;
}

static int constantInstruction(const char *name, Chunk *chunk, int offset) {
    // this printf ths constant index. The contant instruction has 2 parts
    // the operation and the index in the contant array where the constant is
    // stored. This is that second piece
    uint8_t constantIndex = chunk->code[offset + 1];
    printf("%-16s %4d '", name, constantIndex);
    printValue(chunk->constants.values[constantIndex]);
    printf("'\n");
    // burns 2 places in the code count
    return offset + 2;
}

int disassembleInstruction(Chunk *chunk, int offset) {
//  offset so where is the instruction located in the array of code
    printf("%04d ", offset);

    // if it's on the same line as before just print a  |
    if (offset > 0 && chunk->lines[offset] == chunk->lines[offset - 1]) {
    // Basically if we're on the same line as before
        printf("   | ");
    } else {
    // else print the line location for this.
        printf("%4d ", chunk->lines[offset]);
    }

    uint8_t instruction = chunk->code[offset];
    switch(instruction) {
        case OP_CONSTANT:
            return constantInstruction("OP_CONSTANT", chunk, offset);
        case OP_RETURN:
            return simpleInstruction("OP_RETURN\n", offset);
        case OP_NEGATE:
            return simpleInstruction("OP_NEGATE", offset);
        case OP_ADD:
            return simpleInstruction("OP_ADD", offset);
        case OP_SUBTRACT:
            return simpleInstruction("OP_ADD", offset);
        case OP_MULTIPLY:
            return simpleInstruction("OP_ADD", offset);
        case OP_DIVIDE:
            return simpleInstruction("OP_ADD", offset);
        default:
            printf("Unknown op code %d\n", instruction);
            return offset + 1;
    }
}

