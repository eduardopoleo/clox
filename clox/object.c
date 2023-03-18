//
//  object.c
//  clox
//
//  Created by Eduardo Poleo on 2023-03-18.
//

#include <stdio.h>
#include <string.h>

#include "memory.h"
#include "object.h"
#include "value.h"
#include "vm.h"

/*
    This whole setup is just so that we can build / allocate objects
    of different kind. The methods used make is to that we can get around
    the strong types without having to write multiple helper function for
    every type of object.
*/

/*
    Whats the point of this macro if it does the same thing a below
*/
#define ALLOCATE_OBJ(type, objectType) \
    (type*)allocateObject(sizeof(type), objectType)

static Obj* allocateObject(size_t size, ObjType type) {
    Obj* object = (Obj*)reallocate(NULL, 0, size);
    object->type = type;
    return object;
}

static ObjString* allocateString(char* chars, int length) {
    ObjString *string = ALLOCATE_OBJ(ObjString, OBJ_STRING);
    string->length = length;
    string->chars = chars;
    return string;
}

ObjString *copyString(const char *chars, int length) {
    //allocates enough memory for the string
    char *heapChars = ALLOCATE(char, length + 1);
    // copy the memory from the string into the new buffer
    memcpy(heapChars, chars, length);
    // terminates the string for good measure
    heapChars[length] = '\0';
    //??
    return allocateString(heapChars, length);
}

ObjString *takeString(char *chars, int length) {
    return allocateString(chars, length);
}

void printObject(Value value) {
    switch (OBJ_TYPE(value)) {
        case OBJ_STRING:
            printf("%s", AS_CSTRING(value));
            break;
    }
}
