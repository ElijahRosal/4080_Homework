// Elijah Rosal - CS4080 - Homework 10, Chapter 20 Question 3
// 4.16.2026
/*
Benchmark 4: scaling study across multiple table sizes.
*/

#include <stdio.h>
#include <stdlib.h>

#include "benchmark_common.h"
#include "table.h"
#include "value.h"
#include "vm.h"

static void runScale(int n) {
    Table table;
    initTable(&table);

    ObjString** keys = malloc(sizeof(ObjString*) * (size_t)n);
    for (int i = 0; i < n; i++) keys[i] = makeKey(i);

    uint64_t start = nowNanos();
    for (int i = 0; i < n; i++) tableSet(&table, keys[i], NUMBER_VAL((double)i));
    uint64_t insertNs = nowNanos() - start;

    start = nowNanos();
    for (int i = 0; i < n; i++) {
        Value value;
        tableGet(&table, keys[i], &value);
    }
    uint64_t lookupNs = nowNanos() - start;

    printf("n=%d  insert_ns/op=%.1f  lookup_ns/op=%.1f\n",
           n,
           (double)insertNs / n,
           (double)lookupNs / n);

    free(keys);
    freeTable(&table);
}

int main(void) {
    const int sizes[] = {1000, 5000, 20000, 100000, 300000};
    const int count = (int)(sizeof(sizes) / sizeof(sizes[0]));

    initVM();

    printf("scale benchmark\n");
    for (int i = 0; i < count; i++) runScale(sizes[i]);

    freeVM();
    return 0;
}
