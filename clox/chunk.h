//
//  chunk.h
//  clox
//
//  Created by Eduardo Poleo on 2023-02-17.
//

#ifndef chunk_h
#define chunk_h

#include <stdio.h>
#include "common.h"
#include "value.h"

typedef enum {
    OP_RETURN,
    OP_NEGATE,
    OP_CONSTANT,
    OP_NIL,
    OP_TRUE,
    OP_FALSE,
    OP_NOT,
    OP_EQUAL,
    OP_GREATER,
    OP_LESS,
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
} OpCode;

typedef struct {
    int capacity;
    int count;
    uint8_t *code; // Unsigned single byte
    int *lines;
    ValueArray constants;
    // Still confused as to what a chunk is. How come we can store the constants right here
    
} Chunk;

void initChunk(Chunk *chunk);
void writeChunk(Chunk *chunk, uint8_t byte, int line);
void freeChunk(Chunk *chunk);
int addConstant(Chunk *chunk, Value value);

#endif /* chunk_h */
