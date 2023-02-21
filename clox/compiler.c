//
//  compiler.c
//  clox
//
//  Created by Eduardo Poleo on 2023-02-20.
//

#include <stdio.h>
#include "compiler.h"
#include "common.h"
#include "scanner.h"

void compile(const char *source) {
    initScanner(source);
    int line = -1;
    Token token = scanToken();

    for (;;) {
        Token token = scanToken();
        if (token.line != line) {
            printf("%4d ", token.line);
            line = token.line;
        } else {
            printf("   | ");
        }
        printf("%2d '%.*s'\n", token.type, token.length, token.start);
        if (token.type == TOKEN_EOF) break;
    }
}

