cmake_minimum_required(VERSION 3.20)
project(prongs)
set(CMAKE_CXX_STANDARD 17)
add_executable(prong src/main.cpp
        src/tokenization.h
        src/tokenization.cpp
)