//
//  value.h
//  clox
//
//  Created by Eduardo Poleo on 2023-02-17.
//

#ifndef value_h
#define value_h

#include <stdio.h>
#include <stdbool.h>
#include "common.h"

// Forward declaration to be able to include files / concepts into each other
typedef struct Obj Obj;
typedef struct ObjString ObjString;

typedef enum {
    VAL_BOOL,
    VAL_NIL,
    VAL_NUMBER,
    VAL_OBJ
} ValueType;

/*
    The main idea here is to create an interface over all the value types
    we're gonna use. That way in the VM we can hold any type of them in the
    stack so that they can be processed.
 */
typedef struct {
    ValueType type;
    union {
        bool boolean;
        double number;
        Obj *obj;
    } as;
} Value;

#define IS_BOOL(value)   ((value).type == VAL_BOOL)
#define IS_NIL(value)    ((value).type == VAL_NIL)
#define IS_NUMBER(value) ((value).type == VAL_NUMBER)
#define IS_OBJ(value)    ((value).type == VAL_OBJ)

// gets the value from the correct label in the union
#define AS_BOOL(value)   ((value).as.boolean)
#define AS_NUMBER(value) ((value).as.number)
#define AS_OBJ(value)    ((value).as.obj)

// instantiate a new value struct
#define BOOL_VAL(value)   ((Value) {VAL_BOOL, {.boolean = value}})
#define NIL_VAL           ((Value) {VAL_NIL, {.number = 0}})
#define NUMBER_VAL(value) ((Value) {VAL_NUMBER, {.number = value}})
#define OBJ_VAL(object)    ((Value) {VAL_OBJ, {.obj = (Obj*)object}})

typedef struct {
    int capacity;
    int count;
    Value *values;
} ValueArray;

bool valuesEqual(Value a, Value b);
void initValueArray(ValueArray *array);
void writeValueArray(ValueArray *array, Value value);
void freeValueArray(ValueArray *array);
void printValue(Value value);

#endif /* value_h */
