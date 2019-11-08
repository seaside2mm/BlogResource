cmake_minimum_required(VERSION 3.9)
project(ImageProcessingByC C)

set(CMAKE_C_STANDARD 99)

add_executable(ImageProcessingByC main.c chapter1/image.h chapter1/bmp.h chapter1/bmpUtil.h chapter2/effect.h chapter2/solve.h)