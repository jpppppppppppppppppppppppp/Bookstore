cmake_minimum_required(VERSION 3.16)
project(Bookstore)

set(CMAKE_CXX_STANDARD 14)

include_directories(codes/include)

add_executable(Bookstore
        codes/include/error.cpp
        codes/include/error.hpp
        codes/include/interpreter.cpp
        codes/include/interpreter.h
        codes/include/strlib.cpp
        codes/include/strlib.hpp
        codes/include/tokenScanner.cpp
        codes/include/tokenScanner.hpp
        codes/include/user.h
        codes/include/writer.cpp
        codes/main.cpp)
