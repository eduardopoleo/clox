//
//  value.c
//  clox
//
//  Created by Eduardo Poleo on 2023-02-17.
//

#include "value.h"
#include "memory.h"

void initValueArray(ValueArray *array) {
    array->count = 0;
    array->capacity = 0;
    array->values = NULL;
}

void writeValueArray(ValueArray *array, Value value) {
    if (array->count + 1 > array->capacity) {
        int oldCapacity = array->capacity;
        int newCapacity = GROW_CAPACITY(oldCapacity);
        array->capacity = newCapacity;
        array->values = GROW_ARRAY(Value, array->values, oldCapacity, newCapacity);
    }
    
    array->values[array->count] = value;
    array->count++;
}

void freeValueArray(ValueArray *array) {
    FREE_ARRAY(Value, array->values, array->capacity);
    initValueArray(array);
}

void printValue(Value value) {
    printf("%g", value);
}
