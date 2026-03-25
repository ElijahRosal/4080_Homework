cmake_minimum_required(VERSION 4.1)
project(C)

set(CMAKE_CXX_STANDARD 20)

add_executable(C common.h
        main.c
        common.h
        chunk.h
        chunk.c
        memory.h
        memory.c
        debug.h
        debug.c
        value.h
        value.c
        vm.h
        vm.c)
