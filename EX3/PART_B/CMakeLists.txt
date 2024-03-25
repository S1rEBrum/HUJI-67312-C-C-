cmake_minimum_required(VERSION 3.27)
project(C_Homework_3B C)

set(CMAKE_C_STANDARD 99)

add_executable(C_Homework_3B
        tweets_generator.c
        linked_list.h
        linked_list.c
        markov_chain.h
        markov_chain.c
)
