cmake_minimum_required(VERSION 3.9)
project(signals C)

set(CMAKE_C_STANDARD 11)

add_executable(signals main.c utils.c utils.h sigconfig.h)