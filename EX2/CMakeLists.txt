cmake_minimum_required(VERSION 3.27)
project(Ex2 C)

set(CMAKE_C_STANDARD 99)

add_executable(Ex2 main.c
        sort_bus_lines.c
        test_bus_lines.c
        sort_bus_lines.h
        test_bus_lines.h
)
