cmake_minimum_required(VERSION 3.27)
project(Ex1 C)

set(CMAKE_C_STANDARD 99)

add_executable(Ex1 main.c
        cipher.c
        cipher.h
        tests.h
        tests.c)
