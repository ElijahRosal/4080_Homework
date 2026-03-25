// Elijah Rosal - CS4080 - Homework 7, Chapter 14 Question 3
// 3.24.2026
/*
Code below has been modified for Question 3 for Chapter 14 of Crafting Interpreters.

Declares a custom allocator lifecycle for a single large heap region used by
reallocate(), avoiding direct malloc/realloc/free calls inside reallocate().
*/

#ifndef clox_memory_h
#define clox_memory_h
#include "common.h"
#define GROW_CAPACITY(capacity) \
((capacity) < 8 ? 8 : (capacity) * 2)
#define GROW_ARRAY(type, pointer, oldCount, newCount) \
(type*)reallocate(pointer, sizeof(type) * (oldCount), \
sizeof(type) * (newCount))
#define FREE_ARRAY(type, pointer, oldCount) \
reallocate(pointer, sizeof(type) * (oldCount), 0)
void initAllocator(void);
void shutdownAllocator(void);
void* reallocate(void* pointer, size_t oldSize, size_t newSize);
#endif