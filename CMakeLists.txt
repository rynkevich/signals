cmake_minimum_required(VERSION 3.9)
project(signals C)

set(CMAKE_C_STANDARD 11)

add_custom_command(
        OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/signal_to_str_cases.inc
        COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/signame_gen.sh
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
)

add_executable(signals main.c utils.c utils.h sigconfig.h signame.c signame.h signal_to_str_cases.inc)