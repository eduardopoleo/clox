//
//  debug.h
//  clox
//
//  Created by Eduardo Poleo on 2023-02-17.
//

#ifndef debug_h
#define debug_h

#include <stdio.h>
#include "chunk.h"
#include "scanner.h"

void disassembleChunk(Chunk *chunk, char *name);
int disassembleInstruction(Chunk *chunk, int offset);
void printToken(Token token);

#endif /* debug_h */
