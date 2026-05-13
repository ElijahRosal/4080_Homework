// Elijah Rosal - CS4080 - Homework 14, Chapter 28 Question 1
// 5.12.2026
/*
Benchmark program to measure the performance difference between:
1. Hash table lookup for init method (original implementation)
2. Cached init method lookup (optimized implementation)

This benchmark creates many classes with init methods and creates many instances,
measuring the total time for both implementations.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "common.h"
#include "compiler.h"
#include "vm.h"
#include "object.h"

#define NUM_CLASSES 100
#define INSTANCES_PER_CLASS 10000
#define TOTAL_INSTANCES (NUM_CLASSES * INSTANCES_PER_CLASS)

// Test the original implementation using table lookup
void benchmark_table_lookup() {
    printf("=== Benchmark: Hash Table Lookup (Original) ===\n");
    printf("Creating %d classes...\n", NUM_CLASSES);
    printf("Creating %d instances per class...\n", INSTANCES_PER_CLASS);
    printf("Total instances to create: %d\n\n", TOTAL_INSTANCES);
    
    clock_t start = clock();
    
    // Create test classes
    ObjClass** classes = (ObjClass**)malloc(NUM_CLASSES * sizeof(ObjClass*));
    for (int i = 0; i < NUM_CLASSES; i++) {
        char name[32];
        snprintf(name, sizeof(name), "Class_%d", i);
        ObjString* className = copyString(name, strlen(name));
        
        ObjClass* klass = newClass(className);
        
        // Create and add an init method
        ObjFunction* initFunc = newFunction();
        initFunc->name = copyString("init", 4);
        initFunc->arity = 0;
        ObjClosure* initClosure = newClosure(initFunc);
        
        Value initValue = OBJ_VAL(initClosure);
        tableSet(&klass->methods, vm.initString, initValue);
        
        classes[i] = klass;
    }
    
    clock_t creation_time = clock();
    double class_creation_seconds = (double)(creation_time - start) / CLOCKS_PER_SEC;
    printf("Class creation time: %.6f seconds\n", class_creation_seconds);
    
    // Benchmark instance creation with table lookups
    clock_t instance_start = clock();
    
    for (int i = 0; i < NUM_CLASSES; i++) {
        ObjClass* klass = classes[i];
        for (int j = 0; j < INSTANCES_PER_CLASS; j++) {
            // Create instance
            ObjInstance* instance = newInstance(klass);
            
            // Simulate table lookup for init method (original way)
            Value unused;
            tableGet(&klass->methods, vm.initString, &unused);
        }
    }
    
    clock_t instance_end = clock();
    double instance_seconds = (double)(instance_end - instance_start) / CLOCKS_PER_SEC;
    
    clock_t total_end = clock();
    double total_seconds = (double)(total_end - start) / CLOCKS_PER_SEC;
    
    printf("Instance creation + table lookup time: %.6f seconds\n", instance_seconds);
    printf("Total time: %.6f seconds\n\n", total_seconds);
    
    free(classes);
}

// Test the optimized implementation using cached method
void benchmark_cached_lookup() {
    printf("=== Benchmark: Cached Init Method (Optimized) ===\n");
    printf("Creating %d classes...\n", NUM_CLASSES);
    printf("Creating %d instances per class...\n", INSTANCES_PER_CLASS);
    printf("Total instances to create: %d\n\n", TOTAL_INSTANCES);
    
    clock_t start = clock();
    
    // Create test classes
    ObjClass** classes = (ObjClass**)malloc(NUM_CLASSES * sizeof(ObjClass*));
    for (int i = 0; i < NUM_CLASSES; i++) {
        char name[32];
        snprintf(name, sizeof(name), "Class_%d", i);
        ObjString* className = copyString(name, strlen(name));
        
        ObjClass* klass = newClass(className);
        
        // Create and add an init method
        ObjFunction* initFunc = newFunction();
        initFunc->name = copyString("init", 4);
        initFunc->arity = 0;
        ObjClosure* initClosure = newClosure(initFunc);
        
        // Store in both table and cache (as done in actual implementation)
        Value initValue = OBJ_VAL(initClosure);
        tableSet(&klass->methods, vm.initString, initValue);
        klass->initMethod = initClosure;  // Cache it
        
        classes[i] = klass;
    }
    
    clock_t creation_time = clock();
    double class_creation_seconds = (double)(creation_time - start) / CLOCKS_PER_SEC;
    printf("Class creation time: %.6f seconds\n", class_creation_seconds);
    
    // Benchmark instance creation with cached lookups
    clock_t instance_start = clock();
    
    for (int i = 0; i < NUM_CLASSES; i++) {
        ObjClass* klass = classes[i];
        for (int j = 0; j < INSTANCES_PER_CLASS; j++) {
            // Create instance
            ObjInstance* instance = newInstance(klass);
            
            // Access cached init method (optimized way)
            ObjClosure* cached = klass->initMethod;
            (void)cached;  // Use it to prevent optimization
        }
    }
    
    clock_t instance_end = clock();
    double instance_seconds = (double)(instance_end - instance_start) / CLOCKS_PER_SEC;
    
    clock_t total_end = clock();
    double total_seconds = (double)(total_end - start) / CLOCKS_PER_SEC;
    
    printf("Instance creation + cached lookup time: %.6f seconds\n", instance_seconds);
    printf("Total time: %.6f seconds\n\n", total_seconds);
    
    free(classes);
}

int main(int argc, const char* argv[]) {
    // Initialize the VM
    initVM();
    
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║   Init Method Lookup Performance Benchmark                ║\n");
    printf("║   Comparing Hash Table vs Cached Method Lookup            ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n\n");
    
    // Run benchmarks
    benchmark_table_lookup();
    printf("─────────────────────────────────────────────────────────────\n\n");
    
    benchmark_cached_lookup();
    printf("─────────────────────────────────────────────────────────────\n\n");
    
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║   Summary:                                                ║\n");
    printf("║   The cached lookup should be significantly faster than   ║\n");
    printf("║   the hash table lookup, especially with many classes     ║\n");
    printf("║   and instance creations.                                 ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");
    
    freeVM();
    
    return 0;
}
