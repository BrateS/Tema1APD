cmake_minimum_required(VERSION 3.9)
project(Tema1APD)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)
include_directories(in)
include_directories(orig_image)
include_directories(out_ref)

add_executable(Tema1APD
        in/lenna_bw.pgm
        in/lenna_color.pnm
        orig_image/1000.jpg
        orig_image/lenna.jpg
        orig_image/lenna.png
        out_ref/1000.pnm
        out_ref/2lenna_bw.pgm
        out_ref/2lenna_color.pnm
        out_ref/3lenna_bw.pgm
        out_ref/3lenna_color.pnm
        out_ref/8lenna_bw.pgm
        out_ref/8lenna_color.pnm
        homework1.c
        homework1.h
        main1.c
        Makefile)
