cmake_minimum_required(VERSION 3.10)
project(lr1)

set(CMAKE_C_STANDARD 11) 
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror=maybe-uninitialized")

add_executable(lab1 main.c)
add_executable(child child.c)
