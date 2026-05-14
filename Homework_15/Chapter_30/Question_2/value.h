// Elijah Rosal - CS4080 - Homework 15, Chapter 30 Question 2
// 5.13.2026
/*
Added small-string inline value optimization support.
*/

#ifndef clox_value_h
#define clox_value_h
#include <string.h>
#include "common.h"
typedef struct Obj Obj;
typedef struct ObjString ObjString;
#ifdef NAN_BOXING
#define SIGN_BIT ((uint64_t)0x8000000000000000)
#define QNAN ((uint64_t)0x7ffc000000000000)
#define TAG_NIL 1 // 01.
#define TAG_FALSE 2 // 10.
#define TAG_TRUE 3 // 11.
typedef uint64_t Value;
#define IS_BOOL(value) (((value) | 1) == TRUE_VAL)
#define IS_NIL(value) ((value) == NIL_VAL)
#define IS_NUMBER(value) (((value) & QNAN) != QNAN)
#define IS_OBJ(value) \
(((value) & (QNAN | SIGN_BIT)) == (QNAN | SIGN_BIT))
#define AS_BOOL(value) ((value) == TRUE_VAL)
#define AS_NUMBER(value) valueToNum(value)
#define AS_OBJ(value) \
((Obj*)(uintptr_t)((value) & ~(SIGN_BIT | QNAN)))
#define BOOL_VAL(b) ((b) ? TRUE_VAL : FALSE_VAL)
#define FALSE_VAL ((Value)(uint64_t)(QNAN | TAG_FALSE))
#define TRUE_VAL ((Value)(uint64_t)(QNAN | TAG_TRUE))
#define NIL_VAL ((Value)(uint64_t)(QNAN | TAG_NIL))
#define NUMBER_VAL(num) numToValue(num)
#define OBJ_VAL(obj) \
(Value)(SIGN_BIT | QNAN | (uint64_t)(uintptr_t)(obj))
static inline double valueToNum(Value value) {
    double num;
    memcpy(&num, &value, sizeof(Value));
    return num;
}
static inline Value numToValue(double num) {
    Value value;
    memcpy(&value, &num, sizeof(double));
    return value;
}
#else
#define SMALL_STRING_INLINE_CAP 6

typedef enum {
    VAL_BOOL,
    VAL_NIL,
    VAL_NUMBER,
    VAL_OBJ,
    VAL_SMALL_STRING
   } ValueType;

typedef struct {
    uint8_t length;
    char chars[SMALL_STRING_INLINE_CAP + 1];
} SmallString;

typedef struct {
    ValueType type;
    union {
        bool boolean;
        double number;
        Obj* obj;
        SmallString smallString;
    } as;
} Value;
#define IS_BOOL(value) ((value).type == VAL_BOOL)
#define IS_NIL(value) ((value).type == VAL_NIL)
#define IS_NUMBER(value) ((value).type == VAL_NUMBER)
#define IS_OBJ(value) ((value).type == VAL_OBJ)
#define IS_SMALL_STRING(value) ((value).type == VAL_SMALL_STRING)
#define AS_OBJ(value) ((value).as.obj)
#define AS_BOOL(value) ((value).as.boolean)
#define AS_NUMBER(value) ((value).as.number)
#define AS_SMALL_STRING(value) ((value).as.smallString)
#define AS_SMALL_CHARS(value) ((value).as.smallString.chars)
#define AS_SMALL_LENGTH(value) ((int)(value).as.smallString.length)
#define BOOL_VAL(value) ((Value){VAL_BOOL, {.boolean = value}})
#define NIL_VAL ((Value){VAL_NIL, {.number = 0}})
#define NUMBER_VAL(value) ((Value){VAL_NUMBER, {.number = value}})
#define OBJ_VAL(object) ((Value){VAL_OBJ, {.obj = (Obj*)object}})
#define SMALL_STRING_VAL(len, cstr) ((Value){VAL_SMALL_STRING, {.smallString = {(uint8_t)(len), cstr}}})
#endif

typedef struct {
    int capacity;
    int count;
    Value* values;
} ValueArray;
bool valuesEqual(Value a, Value b);
void initValueArray(ValueArray* array);
void writeValueArray(ValueArray* array, Value value);
void freeValueArray(ValueArray* array);
void printValue(Value value);
#endif