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
static ObjString* allocateString(const char* chars, int length, bool ownsChars) {
    ObjString* string = ALLOCATE_OBJ(ObjString, OBJ_STRING);
    string->length = length;
    string->chars = chars;
    string->ownsChars = ownsChars;
    return string;
}
ObjString* takeString(char* chars, int length) {
    return allocateString(chars, length, true);
}
ObjString* copyString(const char* chars, int length) {
    char* heapChars = ALLOCATE(char, length + 1);
    memcpy(heapChars, chars, length);
    heapChars[length] = '\0';
    return allocateString(heapChars, length, true);
}
ObjString* constantString(const char* chars, int length) {
    return allocateString(chars, length, false);
}
void printObject(Value value) {
    switch (OBJ_TYPE(value)) {
        case OBJ_STRING:
            printf("%.*s", AS_STRING(value)->length, AS_STRING(value)->chars);
            break;
    }
}
