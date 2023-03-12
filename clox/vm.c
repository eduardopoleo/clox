//
//  vm.c
//  clox
//
//  Created by Eduardo Poleo on 2023-02-18.
//

#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include "common.h"
#include "compiler.h"
#include "debug.h"
#include "value.h"
#include "vm.h"


/*
    Not necessarily the best practice but
    keeps the code simple
*/

VM vm;

static void resetStack() {
    vm.stackTop = vm.stack;
}

/*
    A function that takes a variable number of arguments, just like
    printf(...)
 */
static void runtimeError(const char* format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    fputs("\n", stderr);
    
    size_t instruction = vm.ip - vm.chunk->code - 1;
    int line = vm.chunk->lines[instruction];
    fprintf(stderr, "[line %d] in script\n", line);
    resetStack();
}

void initVM(void) {
    resetStack();
}

void freeVM(void) {
}

void push(Value value) {
    *vm.stackTop = value;
    vm.stackTop++;
}

Value pop(void) {
    vm.stackTop--;
    return *vm.stackTop;
}

static Value peek(int distance) {
    return vm.stackTop[-1 - distance];
}

static bool isFalsey(Value value) {
    return IS_NIL(value) || (IS_BOOL(value) && !AS_BOOL(value));
}

static InterpretResult run() {
//  Why just why ??
//  The original value gets return but ip gets increased
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
//    The do / while loop ensures that all the statements get
//    in the same scope when the macro expands
/*
    Notice that you can pass macros as arguments to other macros??
    this is wild
 */
#define BINARY_OP(valueType, op) \
    do { \
      if (!IS_NUMBER(peek(0)) || !IS_NUMBER(peek(1))) { \
        runtimeError("Operands must be numbers."); \
        return INTERPRET_RUNTIME_ERROR; \
      } \
      double b = AS_NUMBER(pop()); \
      double a = AS_NUMBER(pop()); \
      push(valueType(a op b)); \
    } while (false)

    // equivalent to an infite while loop
    for(;;) {
#ifdef DEBUG_TRACE_EXECUTION
    printf("        ");
        for (Value* slot = vm.stack; slot < vm.stackTop; slot++) {
            printf("[ ");
            printValue(*slot);
            printf(" ]");
        }
        printf("\n");
    disassembleInstruction(vm.chunk, (int)(vm.ip - vm.chunk->code));
#endif
        uint8_t instruction;
        switch (instruction = READ_BYTE()) {
            case OP_CONSTANT: {
                /*
                 The first byte reads the instruction OP_CONSTANT
                 the second byte (consumed by inside read constant
                 contains the index where the constant gets stored.
                 So this is like a 2 byte instruction.
                 */
                Value constant = READ_CONSTANT();
                push(constant);
                printValue(constant);
                printf("\n");
                break;
            }
            case OP_NIL: push(NIL_VAL); break;
            case OP_TRUE: push(BOOL_VAL(true)); break;
            case OP_EQUAL:{
                Value b = pop();
                Value a = pop();
                push(BOOL_VAL(valuesEqual(a, b)));
                break;
            }
            case OP_FALSE: push(BOOL_VAL(false)); break;
            case OP_NEGATE: {
                if (!IS_NUMBER(peek(0))) {
                    runtimeError("Operand must be a number.");
                    return INTERPRET_RUNTIME_ERROR;
                }
                push(NUMBER_VAL(-AS_NUMBER(pop())));
                break;
            }
            case OP_ADD:      BINARY_OP(NUMBER_VAL, +); break;
            case OP_SUBTRACT: BINARY_OP(NUMBER_VAL, -); break;
            case OP_MULTIPLY: BINARY_OP(NUMBER_VAL, *); break;
            case OP_DIVIDE:   BINARY_OP(NUMBER_VAL, /); break;
            case OP_NOT:
                push(BOOL_VAL(isFalsey(pop()))); break;
            case OP_RETURN:
                printValue(pop());
                printf("\n");
                return INTERPRET_OK;
        }
    }
#undef READ_CONSTANT
#undef READ_BYTE
#undef BINARY_OP
}

InterpretResult interpret(const char *source) {
    Chunk chunk;
    /*
        Chunk is a set of instructions in terms of lifecycle it just
        need to live while the function is active so no need to
        alloc memory
     */
    
    initChunk(&chunk);
    
    /*
        Compiles the souce into the chunk. This is a one pass compiler:
        - Scanning happens on demand. Tokens are requested on demand
        - Parser "parses" which in this case means it compiles the source
        into instructions bytes and inserts them in the right order into the chunk
        - Instructions are 8bit (1byte) integer basically C enums
     */
    if (!compile(source, &chunk)) {
        freeChunk(&chunk);
        return INTERPRET_COMPILE_ERROR;
    }
    
    vm.chunk = &chunk;
    /*
        Initializes the ip to the beginning of the chunk array
        at this point the stack is still empty
     */
    vm.ip = vm.chunk->code;
    
    /*
        The VM processes these chunks in order using a stack
     */
    InterpretResult result = run();
    freeChunk(&chunk);
    return result;
}
