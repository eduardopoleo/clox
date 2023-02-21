//
//  vm.h
//  clox
//
//  Created by Eduardo Poleo on 2023-02-18.
//

#ifndef vm_h
#define vm_h

#include <stdio.h>
#include "chunk.h"
#include "value.h"

#define STACK_MAX 256

typedef struct {
    // A reference to the set of chunks to be processed
    Chunk *chunk;
    uint8_t* ip;
    Value stack[STACK_MAX];
    Value *stackTop;
} VM;

typedef enum {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR,
}InterpretResult;

void initVM(void);
void freeVM(void);
// This is weird by this point I would've imagined that we have
// a set of instructions to process
InterpretResult interpret(const char *source);
void push(Value value);
Value pop(void);

#endif /* vm_h */
