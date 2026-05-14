// Elijah Rosal - CS4080 - Homework 15, Chapter 30 Question 2
// 5.13.2026
/*
Added benchmark for tiny string allocation performance.
*/

#include <stdio.h>
#include <time.h>

#include "object.h"
#include "vm.h"

static double runCase(int iterations, bool optimized, volatile uint64_t* checksum) {
    clock_t start = clock();

    for (int i = 0; i < iterations; i++) {
        char s[3];
        s[0] = (char)('a' + (i % 26));
        s[1] = (char)('a' + ((i / 26) % 26));
        s[2] = '\0';

        Value value = optimized ? copyStringValue(s, 2) : copyHeapStringValue(s, 2);
        char buffer[SMALL_STRING_INLINE_CAP + 1];
        copyStringValueChars(value, buffer);
        *checksum += (uint64_t)(uint8_t)buffer[0];
    }

    clock_t end = clock();
    return (double)(end - start) / CLOCKS_PER_SEC;
}

int main(void) {
    const int iterations = 5000000;
    volatile uint64_t checksum = 0;

    initVM();
    double heapSeconds = runCase(iterations, false, &checksum);
    freeVM();

    initVM();
    double inlineSeconds = runCase(iterations, true, &checksum);
    freeVM();

    printf("bench_small_vs_heap iterations=%d\n", iterations);
    printf("heap-only:   %.6f s (%.2f ns/op)\n", heapSeconds,
           (heapSeconds * 1e9) / iterations);
    printf("inline-small %.6f s (%.2f ns/op)\n", inlineSeconds,
           (inlineSeconds * 1e9) / iterations);
    printf("speedup:     %.2fx\n", heapSeconds / inlineSeconds);
    printf("checksum:    %llu\n", (unsigned long long)checksum);
    return 0;
}
