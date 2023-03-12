//
//  debug.c
//  clox
//
//  Created by Eduardo Poleo on 2023-02-17.
//

#include "debug.h"
#include "scanner.h"

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

void printToken(Token token) {
//    TOKEN_LEFT_PAREN, TOKEN_RIGHT_PAREN,
//    TOKEN_LEFT_BRACE, TOKEN_RIGHT_BRACE,
//    TOKEN_COMMA, TOKEN_DOT, TOKEN_MINUS, TOKEN_PLUS,
//    TOKEN_SEMICOLON, TOKEN_SLASH, TOKEN_STAR,
//    // One or two character tokens.
//    TOKEN_BANG, TOKEN_BANG_EQUAL,
//    TOKEN_EQUAL, TOKEN_EQUAL_EQUAL,
//    TOKEN_GREATER, TOKEN_GREATER_EQUAL,
//    TOKEN_LESS, TOKEN_LESS_EQUAL,
//    // Literals.
//    TOKEN_IDENTIFIER, TOKEN_STRING, TOKEN_NUMBER,
//    // Keywords.
//    TOKEN_AND, TOKEN_CLASS, TOKEN_ELSE, TOKEN_ELSE,
//    TOKEN_FOR, TOKEN_FUN, TOKEN_IF, TOKEN_NIL, TOKEN_OR,
//    TOKEN_PRINT, TOKEN_RETURN, TOKEN_SUPER, TOKEN_THIS,
//    TOKEN_TRUE, TOKEN_VAR, TOKEN_WHILE,
//
//    TOKEN_ERROR, TOKEN_EOF
    switch(token.type) {
        case TOKEN_NIL: printf("TOK_NIL\n"); break;
        case TOKEN_LEFT_PAREN: printf("TOKEN_LEFT_PAREN\n"); break;
        case TOKEN_RIGHT_PAREN: printf("TOKEN_RIGHT_PAREN\n"); break;
        case TOKEN_LEFT_BRACE: printf("TOKEN_LEFT_BRACE\n"); break;
        case TOKEN_RIGHT_BRACE: printf("TOKEN_RIGHT_BRACE\n"); break;
        case TOKEN_COMMA: printf("TOKEN_COMMA\n"); break;
        case TOKEN_DOT: printf("TOKEN_DOT\n"); break;
        case TOKEN_MINUS: printf("TOKEN_MINUS\n"); break;
        case TOKEN_PLUS: printf("TOKEN_PLUS\n"); break;
        case TOKEN_SEMICOLON: printf("TOKEN_SEMICOLON\n"); break;
        case TOKEN_SLASH: printf("TOKEN_SLASH\n"); break;
        case TOKEN_BANG_EQUAL: printf("TOKEN_BANG_EQUAL\n"); break;
        case TOKEN_GREATER: printf("TOKEN_GREATER\n"); break;
        case TOKEN_GREATER_EQUAL: printf("TOKEN_GREATER_EQUAL\n"); break;
        case TOKEN_LESS: printf("TOKEN_LESS\n"); break;
        case TOKEN_LESS_EQUAL: printf("TOKEN_LESS_EQUAL\n"); break;
        case TOKEN_STRING: printf("TOKEN_STRING\n"); break;
        case TOKEN_NUMBER: printf("TOKEN_NUMBER\n"); break;
        case TOKEN_AND: printf("TOKEN_AND\n"); break;
        case TOKEN_CLASS: printf("TOKEN_CLASS\n"); break;
        case TOKEN_ELSE: printf("TOKEN_ELSE\n"); break;
        case TOKEN_FOR: printf("TOKEN_FOR\n"); break;
        case TOKEN_FUN: printf("TOKEN_FUN\n"); break;
        case TOKEN_IF: printf("TOKEN_IF\n"); break;
        case TOKEN_OR: printf("TOKEN_OR\n"); break;
        case TOKEN_PRINT: printf("TOKEN_PRINT\n"); break;
        case TOKEN_RETURN: printf("TOKEN_RETURN\n"); break;
        case TOKEN_SUPER: printf("TOKEN_SUPER\n"); break;
        case TOKEN_THIS: printf("TOKEN_THIS\n"); break;
        case TOKEN_TRUE: printf("TOKEN_TRUE\n"); break;
        case TOKEN_VAR: printf("TOKEN_VAR\n"); break;
        case TOKEN_WHILE: printf("TOKEN_WHILE\n"); break;
    }
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
        case OP_EQUAL:
            return simpleInstruction("OP_EQUAL", offset);
        case OP_GREATER:
            return simpleInstruction("OP_GREATER", offset);
        case OP_LESS:
            return simpleInstruction("OP_LESS", offset);
        case OP_ADD:
            return simpleInstruction("OP_ADD", offset);
        case OP_SUBTRACT:
            return simpleInstruction("OP_SUBTRACT", offset);
        case OP_MULTIPLY:
            return simpleInstruction("OP_MULTIPLY", offset);
        case OP_DIVIDE:
            return simpleInstruction("OP_DIVIDE", offset);
        case OP_NIL:
            return simpleInstruction("OP_NIL", offset);
        case OP_TRUE:
            return simpleInstruction("OP_TRUE", offset);
        case OP_FALSE:
            return simpleInstruction("OP_FALSE", offset);
        case OP_NOT:
            return simpleInstruction("OP_NOT", offset);
        default:
            printf("Unknown op code %d\n", instruction);
            return offset + 1;
    }
}

