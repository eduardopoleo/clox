//
//  value.h
//  clox
//
//  Created by Eduardo Poleo on 2023-02-17.
//

#ifndef value_h
#define value_h

#include <stdio.h>
#include "value.h"

// Value is just a double ah! he remapped the type
typedef double Value;

typedef struct {
    int capacity;
    int count;
    Value *values;
} ValueArray;

void initValueArray(ValueArray *array);
void writeValueArray(ValueArray *array, Value value);
void freeValueArray(ValueArray *array);
void printValue(Value value);

#endif /* value_h */
