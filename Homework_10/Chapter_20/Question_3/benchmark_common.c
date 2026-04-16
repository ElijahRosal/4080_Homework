// Elijah Rosal - CS4080 - Homework 10, Chapter 20 Question 3
// 4.16.2026
/*
Shared helpers for hash-table benchmarks.
*/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <time.h>
#endif

#include "memory.h"
#include "object.h"
#include "vm.h"

uint64_t nowNanos(void) {
#ifdef _WIN32
    static LARGE_INTEGER frequency;
    static int initialized = 0;
    LARGE_INTEGER counter;

    if (!initialized) {
        QueryPerformanceFrequency(&frequency);
        initialized = 1;
    }

    QueryPerformanceCounter(&counter);
    return (uint64_t)((counter.QuadPart * 1000000000ULL) / frequency.QuadPart);
#else
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000000000ULL + (uint64_t)ts.tv_nsec;
#endif
}

ObjString* makeKey(int id) {
    char buffer[32];
    int length = snprintf(buffer, sizeof(buffer), "key_%d", id);
    return copyString(buffer, length);
}

ObjString* makeForcedHashKey(int id, uint32_t forcedHash) {
    char buffer[32];
    int length = snprintf(buffer, sizeof(buffer), "collide_%d", id);

    ObjString* string = ALLOCATE(ObjString, 1);
    char* chars = ALLOCATE(char, length + 1);

    memcpy(chars, buffer, (size_t)length + 1);

    string->obj.type = OBJ_STRING;
    string->obj.next = vm.objects;
    vm.objects = (Obj*)string;

    string->length = length;
    string->chars = chars;
    string->hash = forcedHash;

    return string;
}
