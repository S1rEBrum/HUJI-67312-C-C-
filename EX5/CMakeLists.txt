cmake_minimum_required(VERSION 3.27)
project(CPP_HW5)

set(CMAKE_CXX_STANDARD 14)

add_executable(CPP_HW5 main.cpp
        Movie.cpp
        Movie.h
        print_main.cpp
        RecommendationSystemLoader.cpp
        RecommendationSystemLoader.h
        User.cpp
        User.h
        UsersLoader.cpp
        UsersLoader.h
        RecommendationSystem.cpp
        RecommendationSystem.h)
