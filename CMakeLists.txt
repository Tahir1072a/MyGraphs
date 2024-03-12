cmake_minimum_required(VERSION 3.26)
project(Graphs C)

set(CMAKE_C_STANDARD 11)

add_executable(Graphs main.c
        adjMatrix.c
)
