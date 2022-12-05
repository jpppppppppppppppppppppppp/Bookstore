cmake_minimum_required(VERSION 3.16)
project(code)

set(CMAKE_CXX_STANDARD 14)

add_executable(code
        code/main.cpp
        code/include/tokenScanner.cpp
        code/include/interpreter.cpp
        code/include/error.cpp
        code/include/strlib.cpp
        )
