cmake_minimum_required(VERSION 3.27)
project(Homework4)

set(CMAKE_CXX_STANDARD 14)

add_executable(Homework4 main.cpp
        Matrix.cpp
        Matrix.h
        Activation.cpp
        Activation.h
        Dense.cpp
        Dense.h
        MlpNetwork.cpp
        MlpNetwork.h
)
