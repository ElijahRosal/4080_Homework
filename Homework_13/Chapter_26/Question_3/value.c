// Elijah Rosal - CS4080 - Homework 13, Chapter 26 Question 3
// 5.7.2026
/*
Code below has been modified for Question 3 for Chapter 26 of Crafting Interpreters.

Retains object values when they are stored in arrays and releases them when the
array is freed.
*/

#include <stdio.h>
#include <string.h>
#include "object.h"
#include "memory.h"
#include "value.h"
void initValueArray(ValueArray* array) {
    array->values = NULL;
    array->capacity = 0;
    array->count = 0;
}
void writeValueArray(ValueArray* array, Value value) {
    if (array->capacity < array->count + 1) {
        int oldCapacity = array->capacity;
        array->capacity = GROW_CAPACITY(oldCapacity);
        array->values = GROW_ARRAY(Value, array->values,
        oldCapacity, array->capacity);
    }
    array->values[array->count] = value;
    retainValue(value);
    array->count++;
}
void freeValueArray(ValueArray* array) {
    for (int i = 0; i < array->count; i++) {
        releaseValue(array->values[i]);
    }
    FREE_ARRAY(Value, array->values, array->capacity);
    initValueArray(array);
}
void printValue(Value value) {
    switch (value.type) {
        case VAL_BOOL:
            printf(AS_BOOL(value) ? "true" : "false");
            break;
        case VAL_NIL: printf("nil"); break;
        case VAL_NUMBER: printf("%g", AS_NUMBER(value)); break;
        case VAL_OBJ: printObject(value); break;
    }
}
bool valuesEqual(Value a, Value b) {
    if (a.type != b.type) return false;
    switch (a.type) {
        case VAL_BOOL: return AS_BOOL(a) == AS_BOOL(b);
        case VAL_NIL: return true;
        case VAL_NUMBER: return AS_NUMBER(a) == AS_NUMBER(b);
        case VAL_OBJ: return AS_OBJ(a) == AS_OBJ(b);
        default: return false; // Unreachable.
    }
}