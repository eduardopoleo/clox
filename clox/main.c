//
//  main.c
//  clox
//
//  Created by Eduardo Poleo on 2023-02-17.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "vm.h"

static void repl() {
    char line[1024];
    
    for(;;) {
        printf("> ");
/*
    Reads the content from the stream (stdin) and stores
    into the provided buffer. Not sure what happens when this
    breaks. cuz it does not finish the repl.
*/
        if (!fgets(line, sizeof(line), stdin)) {
            printf("\n");
            break;
        }

        interpret(line);
    }
}

static char *readfile(const char *path) {
//  opens the file
    FILE *file = fopen(path, "rb");
//  move the reader cursor to the end of the file
    if (file == NULL) {
        fprintf(stderr, "Could not open file \"%s\". \n", path);
        exit(74);
    }
    fseek(file, 0L, SEEK_END);
//  gets the size of the file based on the cursor
    size_t fileSize = ftell(file);
//  rewinds it back to the beginning
    rewind(file);
// allocate a buffer to dump all the file contents into it
    char *buffer = (char*)malloc(fileSize + 1);
    if (buffer == NULL) {
        fprintf(stderr, "Not enough memory to read \"%s\".\n", path);
        exit(74);
    }
// dumps the file content into the buffer
    size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);

    if (bytesRead < fileSize) {
        fprintf(stderr, "Could not read file \"%s\".\n", path);
        exit(74);
    }
// adds terminal character
    buffer[bytesRead] = '\0';
// dismisses the file
    fclose(file);
// returns the buffer ready to be processed.
    return buffer;
}

static void runFile(const char *path) {
    char *source = readfile(path);
    InterpretResult result = interpret(source);
    free(source);
    
    if (result  == INTERPRET_COMPILE_ERROR) exit(65);
    if (result  == INTERPRET_RUNTIME_ERROR) exit(70);
}

int main(int argc, const char * argv[]) {
    initVM();
//  if no file is passed fire up the repl
    if (argc == 1) {
        repl();
    } else if (argc == 2) {
// if file is passed dump it into a buffer and interpret it
        runFile(argv[1]);
    } else {
        fprintf(stderr, "Usage of Clox [path]\n");
    }

    freeVM();
    return 0;
}
