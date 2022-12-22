cmake_minimum_required(VERSION 3.16)
project(code)

set(CMAKE_CXX_STANDARD 14)

add_executable(code
        codes/main.cpp
        codes/include/tokenScanner.cpp
        codes/include/interpreter.cpp
        codes/include/error.cpp
        codes/include/strlib.cpp
        codes/include/writer.cpp
        )
