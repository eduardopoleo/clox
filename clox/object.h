//
//  object.h
//  clox
//
//  Created by Eduardo Poleo on 2023-03-18.
//

#ifndef object_h
#define object_h

#include "common.h"
#include "value.h"

#define OBJ_TYPE(value)   (AS_OBJ(value)->type)
#define IS_STRING(value)  isObjType(value, OBJ_STRING)
#define AS_STRING(value)  ((ObjString*)AS_OBJ(value))
#define AS_CSTRING(value) (((ObjString*)AS_OBJ(value))->chars)

typedef enum {
    OBJ_STRING,
} ObjType;

/*
    This is a weird way to do it. I wonder why he does not push
    all this separation directly into the values. It might be
    that at somepoint he wants to do something specific to objects
*/
struct Obj {
    ObjType type;
};

struct ObjString {
    /*
        This is sort of the C way of doing inheritance. By placing
        a reference to the Obj as a field on the object type, we can
        typecast this object into a global object or viceversa depending
        of what the code needs to return etc.
        This probably anwsers the question above.
     */
    Obj Obj;
    int length;
    char *chars;
};

ObjString *takeString(char *chars, int length);

ObjString* copyString(const char* chars, int length);
void printObject(Value value);

static inline bool isObjType(Value value, ObjType type) {
    return IS_OBJ(value) && AS_OBJ(value)->type == type;
}

#endif /* object_h */
