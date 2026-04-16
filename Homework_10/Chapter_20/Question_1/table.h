// Elijah Rosal - CS4080 - Homework 10, Chapter 20 Question 1
// 4.16.2026
/*
Code below has been modified for Question 1 for Chapter 20 of Crafting Interpreters.

Generalizes clox's hash table key support from strings only to include
other primitive key types used by Lox values.
*/

#ifndef clox_table_h
#define clox_table_h
#include "common.h"
#include "value.h"
typedef struct {
    Value key;
    Value value;
} Entry;
typedef struct {
    int count;
    int capacity;
    Entry* entries;
} Table;
void initTable(Table* table);
void freeTable(Table* table);
bool tableGet(Table* table, Value key, Value* value);
bool tableSet(Table* table, Value key, Value value);
bool tableDelete(Table* table, Value key);
void tableAddAll(Table* from, Table* to);
ObjString* tableFindString(Table* table, const char* chars, int length, uint32_t hash);
#endif