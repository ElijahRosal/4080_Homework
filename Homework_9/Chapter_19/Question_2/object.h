// Elijah Rosal - CS4080 - Homework 8, Chapter 17 Question 3
// 3.26.2026
/*
Code below has been modified for Question 3 for Chapter 17 of Crafting Interpreters.

Hooks the ternary conditional operator (?:) into the Pratt parser by
adding precedence, parse rules, and operand parsing without bytecode emission.
*/

#ifndef clox_object_h
#define clox_object_h
#include "common.h"
#include "value.h"
#define OBJ_TYPE(value) (AS_OBJ(value)->type)
#define IS_STRING(value) isObjType(value, OBJ_STRING)
#define AS_STRING(value) ((ObjString*)AS_OBJ(value))
#define AS_CSTRING(value) (((ObjString*)AS_OBJ(value))->chars)
typedef enum {
    OBJ_STRING,
   } ObjType;
struct Obj {
    ObjType type;
    struct Obj* next;
};
struct ObjString {
    Obj obj;
    int length;
    const char* chars;
    bool ownsChars;
};
ObjString* takeString(char* chars, int length);
ObjString* copyString(const char* chars, int length);
ObjString* constantString(const char* chars, int length);
static inline bool isObjType(Value value, ObjType type) {
    return IS_OBJ(value) && AS_OBJ(value)->type == type;
}
void printObject(Value value);
#endif