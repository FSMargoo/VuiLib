cmake_minimum_required(VERSION 3.28)
project(VUILib)

set(CMAKE_CXX_STANDARD 20)

add_executable(VUILib main.cpp
        include/base/test/vTest.h
        include/base/event/vEvent.h
        source/base/vtest/vTest.cpp
        source/base/vtest/vTest.cpp
        unitTest/base/base.event.h
        unitTest/base/base.event.h
        unitTest/base/base.event.cpp
        include/base/binding/vBindingType.h
        include/base/binding/vBindingType.h
        source/base/binding/vBindingType.cpp
        unitTest/base/base.binding.h
        unitTest/base/base.binding.cpp
        third/winToast/include/wintoastlib.h
        third/winToast/src/wintoastlib.cpp)

target_include_directories(VUILib PRIVATE ./include)
target_include_directories(VUILib PRIVATE ./)