cmake_minimum_required(VERSION 3.29)
project(Chip8_Emulator)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Chip8_Emulator
    main.cpp
    main.h)
