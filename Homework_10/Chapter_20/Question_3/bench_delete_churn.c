// Elijah Rosal - CS4080 - Homework 10, Chapter 20 Question 3
// 4.16.2026
/*
Benchmark 2: deletion-heavy churn with random key reuse.
*/

#include <stdio.h>
#include <stdlib.h>

#include "benchmark_common.h"
#include "table.h"
#include "value.h"
#include "vm.h"

int main(int argc, char** argv) {
    int pool = 100000;
    int ops = 300000;

    if (argc > 1) pool = atoi(argv[1]);
    if (argc > 2) ops = atoi(argv[2]);
    if (pool < 1000) pool = 1000;
    if (ops < 1000) ops = 1000;

    srand(1337);
    initVM();

    Table table;
    initTable(&table);

    ObjString** keys = malloc(sizeof(ObjString*) * (size_t)pool);
    unsigned char* present = calloc((size_t)pool, 1);

    for (int i = 0; i < pool; i++) keys[i] = makeKey(i);

    for (int i = 0; i < pool / 2; i++) {
        tableSet(&table, keys[i], NUMBER_VAL((double)i));
        present[i] = 1;
    }

    int inserts = 0;
    int deletes = 0;
    int gets = 0;
    int getHits = 0;

    uint64_t start = nowNanos();
    for (int i = 0; i < ops; i++) {
        int idx = rand() % pool;

        if (present[idx]) {
            if (tableDelete(&table, keys[idx])) {
                present[idx] = 0;
                deletes++;
            }
        } else {
            tableSet(&table, keys[idx], NUMBER_VAL((double)idx));
            present[idx] = 1;
            inserts++;
        }

        idx = rand() % pool;
        Value value;
        if (tableGet(&table, keys[idx], &value)) getHits++;
        gets++;
    }
    uint64_t elapsedNs = nowNanos() - start;

    printf("delete_churn pool=%d ops=%d\n", pool, ops);
    printf("mixed ops: %.2f ms (%.2f Mops/s)\n",
           (double)elapsedNs / 1e6,
           (double)(ops * 2) / ((double)elapsedNs / 1e9) / 1e6);
    printf("inserts=%d deletes=%d gets=%d get_hits=%d\n",
           inserts, deletes, gets, getHits);

    free(present);
    free(keys);
    freeTable(&table);
    freeVM();
    return 0;
}
