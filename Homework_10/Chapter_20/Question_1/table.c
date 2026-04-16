#include <stdlib.h>
#include <string.h>
#include "memory.h"
#include "object.h"
#include "table.h"
#include "value.h"
#define TABLE_MAX_LOAD 0.75

#define EMPTY_KEY ((Value){VAL_OBJ, {.obj = NULL}})

static bool isEmptyKey(Value key) {
    return IS_OBJ(key) && AS_OBJ(key) == NULL;
}

static uint32_t hashValue(Value key) {
    switch (key.type) {
        case VAL_BOOL:
            return AS_BOOL(key) ? 3u : 5u;
        case VAL_NIL:
            return 7u;
        case VAL_NUMBER: {
            double number = AS_NUMBER(key);
            if (number == 0.0) number = 0.0;

            uint64_t bits;
            memcpy(&bits, &number, sizeof(double));
            uint32_t mixed = (uint32_t)(bits ^ (bits >> 32));
            return mixed * 16777619u;
        }
        case VAL_OBJ:
            if (IS_STRING(key)) return AS_STRING(key)->hash;

            uintptr_t pointer = (uintptr_t)AS_OBJ(key);
            return (uint32_t)(pointer ^ (pointer >> 32));
    }

    return 0;
}

void initTable(Table* table) {
    table->count = 0;
    table->capacity = 0;
    table->entries = NULL;
}
void freeTable(Table* table) {
    FREE_ARRAY(Entry, table->entries, table->capacity);
    initTable(table);
}
static Entry* findEntry(Entry* entries, int capacity,
 Value key) {
    uint32_t index = hashValue(key) % capacity;
    Entry* tombstone = NULL;

    for (;;) {
        Entry* entry = &entries[index];

        if (isEmptyKey(entry->key)) {
            if (IS_NIL(entry->value)) {
                // Empty entry.
                return tombstone != NULL ? tombstone : entry;
            } else {
                // We found a tombstone.
                if (tombstone == NULL) tombstone = entry;
            }
        } else if (valuesEqual(entry->key, key)) {
            // We found the key.
            return entry;
        }

        index = (index + 1) % capacity;
    }
}

bool tableGet(Table* table, Value key, Value* value) {
    if (table->count == 0) return false;

    Entry* entry = findEntry(table->entries, table->capacity,
   key);
    if (isEmptyKey(entry->key)) return false;

    *value = entry->value;
    return true;
}

static void adjustCapacity(Table* table, int capacity) {
    Entry* entries = ALLOCATE(Entry, capacity);

    for (int i = 0; i < capacity; i++) {
        entries[i].key = EMPTY_KEY;
        entries[i].value = NIL_VAL;
    }

    table->count = 0;

    for (int i = 0; i < table->capacity; i++) {
        Entry* entry = &table->entries[i];

        if (isEmptyKey(entry->key)) continue;

        Entry* dest = findEntry(entries, capacity, entry->key);
        dest->key = entry->key;
        dest->value = entry->value;
        table->count++;
    }

    FREE_ARRAY(Entry, table->entries, table->capacity);
    table->entries = entries;
    table->capacity = capacity;
}

bool tableSet(Table* table, Value key, Value value) {
    if (table->count + 1 > table->capacity * TABLE_MAX_LOAD) {
        int capacity = GROW_CAPACITY(table->capacity);
        adjustCapacity(table, capacity);
    }

    Entry* entry = findEntry(table->entries, table->capacity,key);
    bool isNewKey = isEmptyKey(entry->key);

    if (isNewKey && IS_NIL(entry->value)) table->count++;

    entry->key = key;
    entry->value = value;
    return isNewKey;
}

bool tableDelete(Table* table, Value key) {
    if (table->count == 0) return false;

    // Find the entry.
    Entry* entry = findEntry(table->entries, table->capacity,
   key);
    if (isEmptyKey(entry->key)) return false;

    // Place a tombstone in the entry.
    entry->key = EMPTY_KEY;
    entry->value = BOOL_VAL(true);
    return true;
}

void tableAddAll(Table* from, Table* to) {
    for (int i = 0; i < from->capacity; i++) {
        Entry* entry = &from->entries[i];

        if (!isEmptyKey(entry->key)) {
            tableSet(to, entry->key, entry->value);
        }
    }
}

ObjString* tableFindString(Table* table, const char* chars,
 int length, uint32_t hash) {
    if (table->count == 0) return NULL;

    uint32_t index = hash % table->capacity;

    for (;;) {
        Entry* entry = &table->entries[index];

        if (isEmptyKey(entry->key)) {
            // Stop if we find an empty non-tombstone entry.
            if (IS_NIL(entry->value)) return NULL;
        } else if (IS_STRING(entry->key)) {
            ObjString* key = AS_STRING(entry->key);
            if (key->length == length &&
            key->hash == hash &&
            memcmp(key->chars, chars, length) == 0) {
            // We found it.
                return key;
            }
        }

        index = (index + 1) % table->capacity;
    }
}
