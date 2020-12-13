cmake_minimum_required(VERSION 3.17)
project(data_base C)

set(CMAKE_C_STANDARD 11)

add_executable(data_base main.c hash_table.c hash_table.h quick_sort.c quick_sort.h)