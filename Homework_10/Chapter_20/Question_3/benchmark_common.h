// Elijah Rosal - CS4080 - Homework 10, Chapter 20 Question 3
// 4.16.2026
/*
Shared helpers for hash-table benchmarks.
*/

#ifndef clox_benchmark_common_h
#define clox_benchmark_common_h

#include <stdint.h>
#include "object.h"

uint64_t nowNanos(void);
ObjString* makeKey(int id);
ObjString* makeForcedHashKey(int id, uint32_t forcedHash);

#endif
