// Elijah Rosal - CS4080 - Homework 13, Chapter 26 Question 3
// 5.7.2026
/*
Code below has been modified for Question 3 for Chapter 26 of Crafting Interpreters.

Implements reference counting in place of the original mark-sweep collector
and releases objects immediately when their last reference is dropped.
*/

#include <stdlib.h>
#include "memory.h"
#include "vm.h"

void* reallocate(void* pointer, size_t oldSize, size_t newSize) {
    if (newSize > oldSize) {
        vm.bytesAllocated += newSize - oldSize;
    } else {
        vm.bytesAllocated -= oldSize - newSize;
    }

    if (newSize == 0) {
        free(pointer);
        return NULL;
    }

    void* result = realloc(pointer, newSize);
    if (result == NULL) exit(1);
    return result;
}

void retainObject(Obj* object) {
    if (object == NULL) return;
    object->refCount++;
}

void retainValue(Value value) {
    if (IS_OBJ(value)) {
        retainObject(AS_OBJ(value));
    }
}

static void unlinkObject(Obj* object) {
    if (vm.objects == object) {
        vm.objects = object->next;
        return;
    }

    Obj* previous = vm.objects;
    while (previous != NULL && previous->next != object) {
        previous = previous->next;
    }

    if (previous != NULL) {
        previous->next = object->next;
    }
}

static void freeObject(Obj* object) {
    switch (object->type) {
        case OBJ_CLOSURE: {
            ObjClosure* closure = (ObjClosure*)object;
            for (int i = 0; i < closure->upvalueCount; i++) {
                releaseObject((Obj*)closure->upvalues[i]);
            }
            FREE_ARRAY(ObjUpvalue*, closure->upvalues,
            closure->upvalueCount);
            releaseObject((Obj*)closure->function);
            FREE(ObjClosure, object);
            break;
        }
        case OBJ_FUNCTION: {
            ObjFunction* function = (ObjFunction*)object;
            freeChunk(&function->chunk);
            releaseObject((Obj*)function->name);
            FREE(ObjFunction, object);
            break;
        }
        case OBJ_NATIVE:
            FREE(ObjNative, object);
            break;
        case OBJ_STRING: {
            ObjString* string = (ObjString*)object;
            FREE_ARRAY(char, string->chars, string->length + 1);
            FREE(ObjString, object);
            break;
        }
        case OBJ_UPVALUE: {
            ObjUpvalue* upvalue = (ObjUpvalue*)object;
            releaseValue(upvalue->closed);
            FREE(ObjUpvalue, object);
            break;
        }
    }
}

void releaseObject(Obj* object) {
    if (object == NULL) return;
    if (object->refCount <= 0) return;

    object->refCount--;
    if (object->refCount > 0) return;

    unlinkObject(object);
    freeObject(object);
}

void releaseValue(Value value) {
    if (IS_OBJ(value)) {
        releaseObject(AS_OBJ(value));
    }
}

void freeObjects() {
    Obj* object = vm.objects;
    while (object != NULL) {
        Obj* next = object->next;
        switch (object->type) {
            case OBJ_CLOSURE: {
                ObjClosure* closure = (ObjClosure*)object;
                FREE_ARRAY(ObjUpvalue*, closure->upvalues,
                closure->upvalueCount);
                FREE(ObjClosure, object);
                break;
            }
            case OBJ_FUNCTION: {
                ObjFunction* function = (ObjFunction*)object;
                FREE_ARRAY(uint8_t, function->chunk.code,
                function->chunk.capacity);
                FREE_ARRAY(int, function->chunk.lines,
                function->chunk.capacity);
                FREE_ARRAY(Value, function->chunk.constants.values,
                function->chunk.constants.capacity);
                FREE(ObjFunction, object);
                break;
            }
            case OBJ_NATIVE:
                FREE(ObjNative, object);
                break;
            case OBJ_STRING: {
                ObjString* string = (ObjString*)object;
                FREE_ARRAY(char, string->chars, string->length + 1);
                FREE(ObjString, object);
                break;
            }
            case OBJ_UPVALUE:
                FREE(ObjUpvalue, object);
                break;
        }
        object = next;
    }
    free(vm.grayStack);
}