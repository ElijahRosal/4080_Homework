// Elijah Rosal - CS4080 - Homework 10, Chapter 20 Question 3
// 4.16.2026
/*
Benchmark 1: bulk insert and lookup with unique keys.
*/

#include <stdio.h>
#include <stdlib.h>

#include "benchmark_common.h"
#include "table.h"
#include "value.h"
#include "vm.h"

int main(int argc, char** argv) {
    int n = 100000;
    if (argc > 1) n = atoi(argv[1]);
    if (n <= 0) n = 100000;

    initVM();

    Table table;
    initTable(&table);

    ObjString** keys = malloc(sizeof(ObjString*) * (size_t)n);

    uint64_t start = nowNanos();
    for (int i = 0; i < n; i++) {
        keys[i] = makeKey(i);
        tableSet(&table, keys[i], NUMBER_VAL((double)i));
    }
    uint64_t insertNs = nowNanos() - start;

    start = nowNanos();
    int found = 0;
    for (int i = 0; i < n; i++) {
        Value value;
        if (tableGet(&table, keys[i], &value)) found++;
    }
    uint64_t lookupNs = nowNanos() - start;

    printf("bulk n=%d\n", n);
    printf("insert: %.2f ms (%.2f Mops/s)\n",
           (double)insertNs / 1e6,
           (double)n / ((double)insertNs / 1e9) / 1e6);
    printf("lookup: %.2f ms (%.2f Mops/s), found=%d\n",
           (double)lookupNs / 1e6,
           (double)n / ((double)lookupNs / 1e9) / 1e6,
           found);

    free(keys);
    freeTable(&table);
    freeVM();
    return 0;
}
