// Elijah Rosal - CS4080 - Homework 10, Chapter 20 Question 3
// 4.16.2026
/*
Benchmark 3: compares normal keys versus forced-collision keys.
*/

#include <stdio.h>
#include <stdlib.h>

#include "benchmark_common.h"
#include "table.h"
#include "value.h"
#include "vm.h"

static void runCase(const char* name, ObjString** keys, int n) {
    Table table;
    initTable(&table);

    uint64_t start = nowNanos();
    for (int i = 0; i < n; i++) {
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

    printf("%s n=%d\n", name, n);
    printf("  insert: %.2f ms\n", (double)insertNs / 1e6);
    printf("  lookup: %.2f ms, found=%d\n", (double)lookupNs / 1e6, found);

    freeTable(&table);
}

int main(int argc, char** argv) {
    int n = 50000;
    if (argc > 1) n = atoi(argv[1]);
    if (n <= 0) n = 50000;

    initVM();

    ObjString** normal = malloc(sizeof(ObjString*) * (size_t)n);
    ObjString** collide = malloc(sizeof(ObjString*) * (size_t)n);

    for (int i = 0; i < n; i++) {
        normal[i] = makeKey(i);
        collide[i] = makeForcedHashKey(i, 1u);
    }

    runCase("normal", normal, n);
    runCase("forced_collision", collide, n);

    free(collide);
    free(normal);
    freeVM();
    return 0;
}
