cmake_minimum_required(VERSION 3.10)
project(VUILib)

set(CMAKE_CXX_STANDARD 20)

SET(CMAKE_C_FLAGS_DEBUG "-D_DEBUG")

add_executable(UnitTest main.cpp
        toolchain/vcommit/third/include/cmdparser.hpp
        third/SimpleJson/sJSON.h
        third/SimpleJson/sJSON.cpp)
add_executable(VCommit toolchain/vcommit/main.cpp
        toolchain/vcommit/third/include/cmdparser.hpp
        third/SimpleJson/sJSON.h
        third/SimpleJson/sJSON.cpp)

add_library(VStaticLib STATIC include/base/test/vTest.h
        include/base/event/vEvent.h
        source/base/vtest/vTest.cpp
        unitTest/base/base.event.h
        unitTest/base/base.event.cpp
        include/base/binding/vBindingType.h
        source/base/binding/vBindingType.cpp
        unitTest/base/base.binding.h
        unitTest/base/base.binding.cpp
        third/winToast/include/wintoastlib.h
        third/winToast/src/wintoastlib.cpp)

target_include_directories(UnitTest PUBLIC ./include)
target_include_directories(UnitTest PUBLIC ./)

target_include_directories(VCommit PUBLIC ./toolchain/vcommit/third)
target_include_directories(VCommit PUBLIC ./)

target_include_directories(VStaticLib PUBLIC ./include)
target_include_directories(VStaticLib PUBLIC ./)

target_link_libraries(UnitTest VStaticLib)