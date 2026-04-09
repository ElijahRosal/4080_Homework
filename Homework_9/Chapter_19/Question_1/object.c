// Elijah Rosal - CS4080 - Homework 8, Chapter 17 Question 3
// 3.26.2026
/*
Code below has been modified for Question 3 for Chapter 17 of Crafting Interpreters.

Hooks the ternary conditional operator (?:) into the Pratt parser by
adding precedence, parse rules, and operand parsing without bytecode emission.
*/

#include <stdio.h>
#include <string.h>
#include "memory.h"
#include "object.h"
#include "value.h"
#include "vm.h"
#define ALLOCATE_OBJ(type, objectType) \
(type*)allocateObject(sizeof(type), objectType)
static Obj* allocateObject(size_t size, ObjType type) {
    Obj* object = (Obj*)reallocate(NULL, 0, size);
    object->type = type;
    object->next = vm.objects;
    vm.objects = object;
    return object;
}
static ObjString* allocateString(const char* chars, int length) {
    ObjString* string = (ObjString*)allocateObject(
            sizeof(ObjString) + sizeof(char) * (length + 1), OBJ_STRING);
    string->length = length;
    memcpy(string->chars, chars, length);
    string->chars[length] = '\0';
    return string;
}
ObjString* takeString(char* chars, int length) {
    ObjString* string = allocateString(chars, length);
    FREE_ARRAY(char, chars, length + 1);
    return string;
}
ObjString* copyString(const char* chars, int length) {
    return allocateString(chars, length);
}
void printObject(Value value) {
    switch (OBJ_TYPE(value)) {
        case OBJ_STRING:
            printf("%s", AS_CSTRING(value));
            break;
    }
}
