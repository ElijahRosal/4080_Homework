// Elijah Rosal - CS4080 - Homework 13, Chapter 26 Question 3
// 5.7.2026
/*
Code below has been modified for Question 3 for Chapter 26 of Crafting Interpreters.

Replaces the mark-sweep GC API with retain/release ownership helpers used by
the reference-counting memory manager.
*/

#ifndef clox_memory_h
#define clox_memory_h
#include "common.h"
#include "object.h"
#include "vm.h"
#define ALLOCATE(type, count) \
(type*)reallocate(NULL, 0, sizeof(type) * (count))
#define FREE(type, pointer) reallocate(pointer, sizeof(type), 0)
#define GROW_CAPACITY(capacity) \
((capacity) < 8 ? 8 : (capacity) * 2)
#define GROW_ARRAY(type, pointer, oldCount, newCount) \
(type*)reallocate(pointer, sizeof(type) * (oldCount), \
sizeof(type) * (newCount))
#define FREE_ARRAY(type, pointer, oldCount) \
reallocate(pointer, sizeof(type) * (oldCount), 0)
void* reallocate(void* pointer, size_t oldSize, size_t newSize);
void retainObject(Obj* object);
void releaseObject(Obj* object);
void retainValue(Value value);
void releaseValue(Value value);
void freeObjects();
#endif