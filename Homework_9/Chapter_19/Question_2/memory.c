// Elijah Rosal - CS4080 - Homework 8, Chapter 17 Question 3
// 3.26.2026
/*
Code below has been modified for Question 3 for Chapter 17 of Crafting Interpreters.

Hooks the ternary conditional operator (?:) into the Pratt parser by
adding precedence, parse rules, and operand parsing without bytecode emission.
*/

#include <stdlib.h>
#include "memory.h"
void* reallocate(void* pointer, size_t oldSize, size_t newSize) {
    if (newSize == 0) {
        free(pointer);
        return NULL;
    }
    void* result = realloc(pointer, newSize);
    if (result == NULL) exit(1);
    return result;
}
static void freeObject(Obj* object) {
    switch (object->type) {
        case OBJ_STRING: {
            ObjString* string = (ObjString*)object;
            if (string->ownsChars) {
                FREE_ARRAY(char, (char*)string->chars, string->length + 1);
            }
            FREE(ObjString, object);
            break;
        }
    }
}
void freeObjects() {
    Obj* object = vm.objects;
    while (object != NULL) {
        Obj* next = object->next;
        freeObject(object);
        object = next;
    }
}