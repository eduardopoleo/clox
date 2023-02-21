//
//  memory.c
//  clox
//
//  Created by Eduardo Poleo on 2023-02-17.
//

#include <stdio.h>
#include <stdlib.h>

#include "memory.h"

void *reallocate(void *pointer, size_t oldSize, size_t newSize) {
    if (newSize == 0) {
        free(pointer);
        // This counts as the void pointer.
        return NULL;
    }

    // this still needs to be assigned even if you're passing the pointer
    // to it. It does not magically assign the memory to the existing pointer
    void *result = realloc(pointer, newSize);
    // safety exit
    if (result == NULL) exit(1);
    return result;
}

