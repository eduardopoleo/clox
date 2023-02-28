//
//  compiler.c
//  clox
//
//  Created by Eduardo Poleo on 2023-02-20.
//

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "compiler.h"
#include "common.h"
#include "scanner.h"

#ifdef DEBUG_PRINT_CODE
#include "debug.h"
#endif

typedef struct {
    Token current;
    Token previous;
    bool hadError;
    bool panicMode;
} Parser;

Parser parser;

/*
 These need to be in order of precedence otherwise
 some of the tricks used by the main function won't work
 */
typedef enum {
    PREC_NONE,
    PREC_ASSIGNMENT,  // =
    PREC_OR,          // or
    PREC_AND,         // and
    PREC_EQUALITY,    // == !=
    PREC_COMPARISON,  // < > <= >=
    PREC_TERM,        // + -
    PREC_FACTOR,      // * /
    PREC_UNARY,       // ! -
    PREC_CALL,        // . ()
    PREC_PRIMARY
} Precedence;

typedef void (*ParseFn)(void);

typedef struct {
    ParseFn prefix;
    ParseFn infix;
    Precedence precedence;
} ParseRule;

Chunk *compilingChunk;

static Chunk *currentChunk() {
    return compilingChunk;
}
 
static void errorAt(Token *token, const char *message) {
    // Done to avoid cascading errors;
    if (parser.panicMode) return;

    parser.panicMode = true;
    fprintf(stderr, "[line %d] Error", token->line);
    
    if (token->type == TOKEN_EOF) {
        fprintf(stderr, " at end");
    } else if (token->type == TOKEN_ERROR) {
        
    } else {
        fprintf(stderr, "  at '%.*s'", token->length, token->start);
    }
    
    fprintf(stderr, ": %s\n", message);
    parser.hadError = true;
}

static void error(const char *message) {
    errorAt(&parser.previous, message);
}

static void errorAtCurrent(const char *message) {
    errorAt(&parser.current, message);
}

static void advance() {
    parser.previous = parser.current;
    
    for(;;) {
        parser.current = scanToken();
        if (parser.current.type != TOKEN_ERROR) break;
        
        errorAtCurrent(parser.current.start);
    }
}

static void consume(TokenType type, const char *message) {
    if (parser.current.type == type) {
        advance();
        return;
    }
    
    errorAtCurrent(message);
}

static void emitByte(uint8_t byte) {
    writeChunk(currentChunk(), byte, parser.previous.line);
}

static void emitBytes(uint8_t byte1, uint8_t byte2) {
    emitByte(byte1);
    emitByte(byte2);
}

static void emitReturn() {
    emitByte(OP_RETURN);
}

static void endCompiler() {
    emitReturn();
    #ifdef DEBUG_PRINT_CODE
        if (!parser.hadError) {
            disassembleChunk(currentChunk(), "code");
        }
    #endif
}

/*
    Forward declaration cuz otherwise
    functions won't be able to call each other
 */
static void expression(void);
static ParseRule *getRule(TokenType type);
static void parsePrecedence(Precedence precedence);

static uint8_t makeConstant(Value value) {
    int constant = addConstant(currentChunk(), value);
    if (constant > UINT8_MAX) {
        error("Too many constants in one chunk");
        return 0;
    }
    return (uint8_t)constant;
}

static void emitConstant(Value value) {
    /*
     Emits 1 byte for the instruction and 1 byte to the location
     of the constant in the chunks constant array
     */
    emitBytes(OP_CONSTANT, makeConstant(value));
}

static void parsePrecedence(Precedence precedence) {
    /*
        This probably to get around off by 1 errors and to make
        the other methods simpler
     */
    advance();
    /*
     The actual token we care is the previous one in this case
     if we're parsing 1 + 2:
     - previous : Token(number) 1
     - current  : Token(PLUS) +
     */
    ParseFn prefixRule = getRule(parser.previous.type)->prefix;
    if (prefixRule == NULL) {
        error("Expected Expression");
        return;
    }
    
    /*
        for the 1 + 2 example the first expression is number
        we call it number() -> just emits a constant instruction
     */
    prefixRule();
    
    /*
        We now do the check on current? Again probably to off by 1 and get simplify things
        token is + and is infix.
     */
    
    /*
     typedef enum {
         PREC_NONE,
         PREC_ASSIGNMENT,  // =
         PREC_OR,          // or
         PREC_AND,         // and
         PREC_EQUALITY,    // == !=
         PREC_COMPARISON,  // < > <= >=
         PREC_TERM,        // + -
         PREC_FACTOR,      // * /
         PREC_UNARY,       // ! -
         PREC_CALL,        // . ()
         PREC_PRIMARY
     } Precedence;
    */
    
    while (getRule(parser.current.type)->precedence > precedence){
        /*
            previous = +
            current = 2
        */
        advance();
        ParseFn infixRule = getRule(parser.previous.type)->infix;
        /*
            we call the infix rule which in this case if binary
         */
        infixRule();
    }
}


static void expression() {
    parsePrecedence(PREC_ASSIGNMENT);
}

static void grouping() {
    expression();
    consume(TOKEN_RIGHT_PAREN, "Expect ')' after expression");
}

static void unary() {
    TokenType operatorType = parser.previous.type;
//    compiles the actual operand -(operand)
    parsePrecedence(PREC_UNARY);
    
    switch(operatorType) {
        case TOKEN_MINUS: emitByte(OP_NEGATE); break;
        default: return;
    }
}

static void binary() {
    TokenType operatorType = parser.previous.type;
    // this is a bit weird cuz getRule got us here in the first place
    // he's also avoiding passing any argument so that we can make use
    // of the function pointer.
    // long story short we need to get the rule again just to determine the
    // precedence
    ParseRule *rule = getRule(operatorType);
    // compiles the actual right operan left + (right) <- this
    // this is the trick on this algo to make things left associative
    printf("binary\n");
    parsePrecedence((Precedence)(rule->precedence + 1));
    
    switch (operatorType) {
        case TOKEN_PLUS: emitByte(OP_ADD); break;
        case TOKEN_MINUS: emitByte(OP_SUBTRACT); break;
        case TOKEN_STAR: emitByte(OP_MULTIPLY); break;
        case TOKEN_SLASH: emitByte(OP_DIVIDE); break;
        default: return;
    }
}

static void number() {
    double value = strtod(parser.previous.start, NULL);
    printf("number %f\n", value);
    emitConstant(value);
}

/*
    Array of ParseRules structs. It's indexed using
    the TokenTypes.
    Each struct contains function pointers or a pointer
    to the function used to parse the given expression.
    We'll use the function and call it with args
    It's equivalent to a dynamic send.
 
    TOKEN = prefix |  infix  | precedence
 */

ParseRule rules[] = {
  [TOKEN_LEFT_PAREN]    = {grouping, NULL,   PREC_NONE},
  [TOKEN_RIGHT_PAREN]   = {NULL,     NULL,   PREC_NONE},
  [TOKEN_LEFT_BRACE]    = {NULL,     NULL,   PREC_NONE},
  [TOKEN_RIGHT_BRACE]   = {NULL,     NULL,   PREC_NONE},
  [TOKEN_COMMA]         = {NULL,     NULL,   PREC_NONE},
  [TOKEN_DOT]           = {NULL,     NULL,   PREC_NONE},
  [TOKEN_MINUS]         = {unary,    binary, PREC_TERM},
  [TOKEN_PLUS]          = {NULL,     binary, PREC_TERM},
  [TOKEN_SEMICOLON]     = {NULL,     NULL,   PREC_NONE},
  [TOKEN_SLASH]         = {NULL,     binary, PREC_FACTOR},
  [TOKEN_STAR]          = {NULL,     binary, PREC_FACTOR},
  [TOKEN_BANG]          = {NULL,     NULL,   PREC_NONE},
  [TOKEN_BANG_EQUAL]    = {NULL,     NULL,   PREC_NONE},
  [TOKEN_EQUAL]         = {NULL,     NULL,   PREC_NONE},
  [TOKEN_EQUAL_EQUAL]   = {NULL,     NULL,   PREC_NONE},
  [TOKEN_GREATER]       = {NULL,     NULL,   PREC_NONE},
  [TOKEN_GREATER_EQUAL] = {NULL,     NULL,   PREC_NONE},
  [TOKEN_LESS]          = {NULL,     NULL,   PREC_NONE},
  [TOKEN_LESS_EQUAL]    = {NULL,     NULL,   PREC_NONE},
  [TOKEN_IDENTIFIER]    = {NULL,     NULL,   PREC_NONE},
  [TOKEN_STRING]        = {NULL,     NULL,   PREC_NONE},
  [TOKEN_NUMBER]        = {number,   NULL,   PREC_NONE},
  [TOKEN_AND]           = {NULL,     NULL,   PREC_NONE},
  [TOKEN_CLASS]         = {NULL,     NULL,   PREC_NONE},
  [TOKEN_ELSE]          = {NULL,     NULL,   PREC_NONE},
  [TOKEN_FALSE]         = {NULL,     NULL,   PREC_NONE},
  [TOKEN_FOR]           = {NULL,     NULL,   PREC_NONE},
  [TOKEN_FUN]           = {NULL,     NULL,   PREC_NONE},
  [TOKEN_IF]            = {NULL,     NULL,   PREC_NONE},
  [TOKEN_NIL]           = {NULL,     NULL,   PREC_NONE},
  [TOKEN_OR]            = {NULL,     NULL,   PREC_NONE},
  [TOKEN_PRINT]         = {NULL,     NULL,   PREC_NONE},
  [TOKEN_RETURN]        = {NULL,     NULL,   PREC_NONE},
  [TOKEN_SUPER]         = {NULL,     NULL,   PREC_NONE},
  [TOKEN_THIS]          = {NULL,     NULL,   PREC_NONE},
  [TOKEN_TRUE]          = {NULL,     NULL,   PREC_NONE},
  [TOKEN_VAR]           = {NULL,     NULL,   PREC_NONE},
  [TOKEN_WHILE]         = {NULL,     NULL,   PREC_NONE},
  [TOKEN_ERROR]         = {NULL,     NULL,   PREC_NONE},
/*
    Sentinel so that the algo does not explote when we do not have
    more tokens.
 */
  [TOKEN_EOF]           = {NULL,     NULL,   PREC_NONE},
};

static ParseRule *getRule(TokenType type) {
    return &rules[type];
}

bool compile(const char *source, Chunk *chunk) {
    /*
        Both start and current starts at the beginning of the source
        start = source; current = source
     */
    initScanner(source);
    compilingChunk = chunk;
    parser.panicMode = false;
    parser.hadError = false;
    /*
        Scans a token to get us started
     */
    advance();
    // parsing // compilation starts here
    expression();
    consume(TOKEN_EOF, "Expect end of expression.");
    endCompiler();
    return !parser.hadError;
}

