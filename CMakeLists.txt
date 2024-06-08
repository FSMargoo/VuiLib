cmake_minimum_required(VERSION 3.28)
project(VUILib)

set(CMAKE_CXX_STANDARD 20)

add_executable(VUILib main.cpp
        include/base/test/vException.h
        include/base/event/vEvent.h)

target_include_directories(VUILib PRIVATE ./include)