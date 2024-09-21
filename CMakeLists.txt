cmake_minimum_required(VERSION 3.10)
project(lr1 C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror=maybe-uninitialized")

include_directories(include)

add_library(${CMAKE_PROJECT_NAME}_lib 
              src/deleteVowels.c)

add_executable(${CMAKE_PROJECT_NAME}_exe main.c)

target_link_libraries(${CMAKE_PROJECT_NAME}_exe PRIVATE ${CMAKE_PROJECT_NAME}_lib)
