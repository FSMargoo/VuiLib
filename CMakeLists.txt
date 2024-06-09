cmake_minimum_required(VERSION 3.28)
project(VUILib)

set(CMAKE_CXX_STANDARD 20)

add_executable(VUILib main.cpp
        include/base/test/vTest.h
        include/base/event/vEvent.h
        source/vtest/vTest.cpp
        source/vtest/vTest.cpp
        unitTest/base/base.event.h
        unitTest/base/base.event.h
        unitTest/base/base.event.cpp)

target_include_directories(VUILib PRIVATE ./include)
target_include_directories(VUILib PRIVATE ./)