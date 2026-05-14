#include <stdio.h>
#include <string.h>
#include <time.h>

#include "memory.h"
#include "object.h"
#include "vm.h"

static Value concatValues(Value a, Value b, bool optimized) {
    int aLength = stringValueLength(a);
    int bLength = stringValueLength(b);
    int length = aLength + bLength;

    char* chars = ALLOCATE(char, length + 1);
    copyStringValueChars(a, chars);
    copyStringValueChars(b, chars + aLength);
    chars[length] = '\0';

    return optimized ? takeStringValue(chars, length)
                     : takeHeapStringValue(chars, length);
}

static double runCase(int iterations, bool optimized, volatile uint64_t* checksum) {
    clock_t start = clock();

    for (int i = 0; i < iterations; i++) {
        char leftChars[2] = {(char)('a' + (i % 26)), '\0'};
        char rightChars[2] = {(char)('a' + ((i / 26) % 26)), '\0'};

        Value left = optimized ? copyStringValue(leftChars, 1)
                               : copyHeapStringValue(leftChars, 1);
        Value right = optimized ? copyStringValue(rightChars, 1)
                                : copyHeapStringValue(rightChars, 1);
        Value joined = concatValues(left, right, optimized);

        *checksum += (uint64_t)stringValueLength(joined);
    }

    clock_t end = clock();
    return (double)(end - start) / CLOCKS_PER_SEC;
}

int main(void) {
    const int iterations = 3000000;
    volatile uint64_t checksum = 0;

    initVM();
    double heapSeconds = runCase(iterations, false, &checksum);
    freeVM();

    initVM();
    double inlineSeconds = runCase(iterations, true, &checksum);
    freeVM();

    printf("bench_small_concat iterations=%d\n", iterations);
    printf("heap-only:   %.6f s (%.2f ns/op)\n", heapSeconds,
           (heapSeconds * 1e9) / iterations);
    printf("inline-small %.6f s (%.2f ns/op)\n", inlineSeconds,
           (inlineSeconds * 1e9) / iterations);
    printf("speedup:     %.2fx\n", heapSeconds / inlineSeconds);
    printf("checksum:    %llu\n", (unsigned long long)checksum);
    return 0;
}
