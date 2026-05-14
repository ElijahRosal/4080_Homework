# Elijah Rosal - CS4080 - Homework 15, Chapter 30 Question 2
# 5.13.2026
#
# Added build targets for small-string optimization benchmarks.

cmake_minimum_required(VERSION 3.16)
project(Question_2 C)

set(CMAKE_C_STANDARD 11)

set(CLOX_CORE_SOURCES
        chunk.c
        memory.c
        debug.c
        value.c
        vm.c
        compiler.c
        scanner.c
        object.c
        table.c)

add_executable(C
        main.c
        ${CLOX_CORE_SOURCES})

add_executable(bench_small_vs_heap
        bench_small_vs_heap.c
        ${CLOX_CORE_SOURCES})

add_executable(bench_small_concat
        bench_small_concat.c
        ${CLOX_CORE_SOURCES})
