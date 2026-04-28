// Elijah Rosal - CS4080 - Homework 12, Chapter 24 Question 2
// 4.28.2026
/*
Code below has been modified for Question 2 for Chapter 24 of Crafting Interpreters.

Extends ObjNative structure to include an arity field for argument count
validation in native function calls.
*/

#ifndef clox_object_h
#define clox_object_h
#include "common.h"
#include "value.h"
#include "chunk.h"
#define OBJ_TYPE(value) (AS_OBJ(value)->type)
#define IS_FUNCTION(value) isObjType(value, OBJ_FUNCTION)
#define IS_NATIVE(value) isObjType(value, OBJ_NATIVE)
#define IS_STRING(value) isObjType(value, OBJ_STRING)
#define AS_FUNCTION(value) ((ObjFunction*)AS_OBJ(value))
#define AS_NATIVE(value) \
(((ObjNative*)AS_OBJ(value))->function)
#define AS_STRING(value) ((ObjString*)AS_OBJ(value))
#define AS_CSTRING(value) (((ObjString*)AS_OBJ(value))->chars)
typedef enum {
    OBJ_FUNCTION,
    OBJ_NATIVE,
    OBJ_STRING,
   } ObjType;
struct Obj {
    ObjType type;
    struct Obj* next;
};
typedef struct {
 Obj obj;
 int arity;
 Chunk chunk;
 ObjString* name;
} ObjFunction;
typedef Value (*NativeFn)(int argCount, Value* args);
typedef struct {
    Obj obj;
    NativeFn function;
    int arity;
} ObjNative;
struct ObjString {
    Obj obj;
    int length;
    char* chars;
    uint32_t hash;
};
ObjFunction* newFunction();
ObjNative* newNative(NativeFn function);
ObjString* takeString(char* chars, int length);
ObjString* copyString(const char* chars, int length);
static inline bool isObjType(Value value, ObjType type) {
    return IS_OBJ(value) && AS_OBJ(value)->type == type;
}
void printObject(Value value);
#endif