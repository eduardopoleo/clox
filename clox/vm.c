//
//  vm.c
//  clox
//
//  Created by Eduardo Poleo on 2023-02-18.
//

#include <stdio.h>
#include "common.h"
#include "compiler.h"
#include "debug.h"
#include "vm.h"
#include <stdbool.h>

/*
    Not necessarily the best practice but
    keeps the code simple
*/

VM vm;

static void resetStack() {
    vm.stackTop = vm.stack;
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

static InterpretResult run() {
//  Why just why ??
//  The original value gets return but ip gets increased
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
//    The do / while loop ensures that all the statements get
//    in the same scope when the macro expands
#define BINARY_OP(op)\
    do { \
        double b = pop(); \
        double a = pop(); \
        push(a op b); \
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
                // remember that the seconde byte is the constant
                // so we burn and print if for now
                Value constant = READ_CONSTANT();
                push(constant);
                printValue(constant);
                printf("\n");
                break;
            }
            case OP_NEGATE: {
                push(-pop());
                break;
            }
            case OP_ADD: BINARY_OP(+); break;
            case OP_SUBTRACT: BINARY_OP(-); break;
            case OP_MULTIPLY: BINARY_OP(*); break;
            case OP_DIVIDE: BINARY_OP(/); break;
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
    compile(source);
    return INTERPRET_OK;
}