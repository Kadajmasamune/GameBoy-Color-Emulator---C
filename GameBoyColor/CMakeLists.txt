cmake_minimum_required(VERSION 3.31)
project(GameBoyColor C)

set(CMAKE_C_STANDARD 23)

add_executable(GameBoyColor src/include/main.c
        src/include/CART_HEADER.h
        src/CART_HEADER.c
        src/include/Common.h)
