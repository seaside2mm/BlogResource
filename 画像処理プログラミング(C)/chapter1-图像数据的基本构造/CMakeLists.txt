cmake_minimum_required(VERSION 3.9)
project(chapter1 C)

set(CMAKE_C_STANDARD 99)

add_executable(chapter1 main.c image.h bmp.h bmpUtil.h)