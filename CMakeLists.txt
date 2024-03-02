cmake_minimum_required(VERSION 3.26)
project(VUILib_Fluent)

set(CMAKE_CXX_STANDARD 23)

include_directories(./include)

add_executable(VUILib_Fluent main.cpp
        include/base/vbase.h
        include/base/vbase.h
        include/base/event/vevent.h
        include/base/event/vevent.h
        include/base/object/vobject.h
        include/base/LINQ/vlinq.h
        include/base/LINQ/vlinq.h)
