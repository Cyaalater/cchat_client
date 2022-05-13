cmake_minimum_required(VERSION 3.21)
project(CChat C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_executable(CChat src/main.c include/sockcom/sockcom.h include/sockcom/sockcom.c)
