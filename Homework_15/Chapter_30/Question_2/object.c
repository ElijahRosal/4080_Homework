// Elijah Rosal - CS4080 - Homework 15, Chapter 30 Question 2
// 5.13.2026
/*
Added small-string inline value optimization support.
*/

#include <stdio.h>
#include <string.h>
#include "memory.h"
#include "object.h"
#include "table.h"
#include "value.h"
#include "vm.h"
#define ALLOCATE_OBJ(type, objectType) \
(type*)allocateObject(sizeof(type), objectType)
static Obj* allocateObject(size_t size, ObjType type) {
    Obj* object = (Obj*)reallocate(NULL, 0, size);
    object->type = type;
    object->isMarked = false;
    object->next = vm.objects;
    vm.objects = object;
#ifdef DEBUG_LOG_GC
    printf("%p allocate %zu for %d\n", (void*)object, size, type);
#endif
    return object;
}
ObjBoundMethod* newBoundMethod(Value receiver,
 ObjClosure* method) {
    ObjBoundMethod* bound = ALLOCATE_OBJ(ObjBoundMethod,
    OBJ_BOUND_METHOD);
    bound->receiver = receiver;
    bound->method = method;
    return bound;
}
ObjClass* newClass(ObjString* name) {
    ObjClass* klass = ALLOCATE_OBJ(ObjClass, OBJ_CLASS);
    klass->name = name;
    initTable(&klass->methods);
    return klass;
}
ObjClosure* newClosure(ObjFunction* function) {
    ObjUpvalue** upvalues = ALLOCATE(ObjUpvalue*, function->upvalueCount);
    for (int i = 0; i < function->upvalueCount; i++) {
        upvalues[i] = NULL;
    }
    ObjClosure* closure = ALLOCATE_OBJ(ObjClosure, OBJ_CLOSURE);
    closure->function = function;
    closure->upvalues = upvalues;
    closure->upvalueCount = function->upvalueCount;
    return closure;
}
ObjFunction* newFunction() {
    ObjFunction* function = ALLOCATE_OBJ(ObjFunction, OBJ_FUNCTION);
    function->arity = 0;
    function->upvalueCount = 0;
    function->name = NULL;
    initChunk(&function->chunk);
    return function;
}
ObjInstance* newInstance(ObjClass* klass) {
    ObjInstance* instance = ALLOCATE_OBJ(ObjInstance, OBJ_INSTANCE);
    instance->klass = klass;
    initTable(&instance->fields);
    return instance;
}
ObjNative* newNative(NativeFn function) {
    ObjNative* native = ALLOCATE_OBJ(ObjNative, OBJ_NATIVE);
    native->function = function;
    return native;
}

ObjString* allocateString(char* chars, int length, uint32_t hash) {
    ObjString* string = ALLOCATE_OBJ(ObjString, OBJ_STRING);
    string->length = length;
    string->chars = chars;
    string->hash = hash;
    push(OBJ_VAL(string));
    tableSet(&vm.strings, string, NIL_VAL);
    pop();
    return string;
}
static uint32_t hashString(const char* key, int length) {
    uint32_t hash = 2166136261u;
    for (int i = 0; i < length; i++) {
        hash ^= (uint8_t)key[i];
        hash *= 16777619;
    }
    return hash;
}
ObjString* takeString(char* chars, int length) {
    uint32_t hash = hashString(chars, length);
    ObjString* interned = tableFindString(&vm.strings, chars, length,hash);
    if (interned != NULL) {
        FREE_ARRAY(char, chars, length + 1);
        return interned;
    }
    return allocateString(chars, length, hash);
}
ObjString* copyString(const char* chars, int length) {
    uint32_t hash = hashString(chars, length);
    ObjString* interned = tableFindString(&vm.strings, chars, length,hash);
    if (interned != NULL) return interned;
    char* heapChars = ALLOCATE(char, length + 1);
    memcpy(heapChars, chars, length);
    heapChars[length] = '\0';
    return allocateString(heapChars, length, hash);
}

Value takeHeapStringValue(char* chars, int length) {
    return OBJ_VAL(takeString(chars, length));
}

Value copyHeapStringValue(const char* chars, int length) {
    return OBJ_VAL(copyString(chars, length));
}

Value takeStringValue(char* chars, int length) {
#ifdef NAN_BOXING
    return OBJ_VAL(takeString(chars, length));
#else
    if (length <= SMALL_STRING_INLINE_CAP) {
        Value value;
        value.type = VAL_SMALL_STRING;
        value.as.smallString.length = (uint8_t)length;
        memcpy(value.as.smallString.chars, chars, length);
        value.as.smallString.chars[length] = '\0';
        FREE_ARRAY(char, chars, length + 1);
        return value;
    }
    return OBJ_VAL(takeString(chars, length));
#endif
}

Value copyStringValue(const char* chars, int length) {
#ifdef NAN_BOXING
    return OBJ_VAL(copyString(chars, length));
#else
    if (length <= SMALL_STRING_INLINE_CAP) {
        Value value;
        value.type = VAL_SMALL_STRING;
        value.as.smallString.length = (uint8_t)length;
        memcpy(value.as.smallString.chars, chars, length);
        value.as.smallString.chars[length] = '\0';
        return value;
    }
    return OBJ_VAL(copyString(chars, length));
#endif
}

int stringValueLength(Value value) {
#ifdef NAN_BOXING
    return AS_STRING(value)->length;
#else
    if (IS_SMALL_STRING(value)) return AS_SMALL_LENGTH(value);
    return AS_STRING(value)->length;
#endif
}

void copyStringValueChars(Value value, char* dest) {
#ifdef NAN_BOXING
    memcpy(dest, AS_STRING(value)->chars, AS_STRING(value)->length);
    dest[AS_STRING(value)->length] = '\0';
#else
    if (IS_SMALL_STRING(value)) {
        memcpy(dest, AS_SMALL_CHARS(value), AS_SMALL_LENGTH(value));
        dest[AS_SMALL_LENGTH(value)] = '\0';
        return;
    }
    memcpy(dest, AS_STRING(value)->chars, AS_STRING(value)->length);
    dest[AS_STRING(value)->length] = '\0';
#endif
}

ObjUpvalue* newUpvalue(Value* slot) {
    ObjUpvalue* upvalue = ALLOCATE_OBJ(ObjUpvalue, OBJ_UPVALUE);
    upvalue->closed = NIL_VAL;
    upvalue->location = slot;
    upvalue->next = NULL;
    return upvalue;
}
static void printFunction(ObjFunction* function) {
    if (function->name == NULL) {
        printf("<script>");
        return;
    }
    printf("<fn %s>", function->name->chars);
}
void printObject(Value value) {
    switch (OBJ_TYPE(value)) {
        case OBJ_BOUND_METHOD:
            printFunction(AS_BOUND_METHOD(value)->method->function);
            break;
        case OBJ_CLASS:
            printf("%s", AS_CLASS(value)->name->chars);
            break;
        case OBJ_INSTANCE:
            printf("%s instance", AS_INSTANCE(value)->klass->name->chars);
            break;
        case OBJ_CLOSURE:
            printFunction(AS_CLOSURE(value)->function);
            break;
        case OBJ_FUNCTION:
            printFunction(AS_FUNCTION(value));
            break;
        case OBJ_NATIVE:
            printf("<native fn>");
            break;
        case OBJ_STRING:
            printf("%s", AS_CSTRING(value));
            break;
        case  OBJ_UPVALUE:
            printf("upvalue");
            break;
    }
}

