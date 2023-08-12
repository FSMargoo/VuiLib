cmake_minimum_required(VERSION 3.26)
project(vlib)

set(CMAKE_CXX_STANDARD 23)

include_directories(inc)
include_directories(inc/kernel)
include_directories(inc/kernel/base)
include_directories(inc/kernel/mem)

add_executable(vlib
        inc/kernel/base/vbase.h
        inc/kernel/mem/vconstantpool.h
        inc/kernel/mem/vmemory.h
        inc/kernel/mem/vmemorypolicy.h
        inc/kernel/mem/vmemrbtree.h
        src/kernel/mem/vconstantpool.cpp
        src/kernel/mem/vmemory.cpp
        src/kernel/mem/vmemorypolicy.cpp
        src/kernel/mem/vmemrbtree.cpp
        tester/mem/mem-test.cpp
        tester/tester.cpp
        src/kernel/debug/vdebug.cpp
        inc/kernel/thread/vthread.h
        src/kernel/thread/vthread.cpp
        inc/kernel/container/vdeque.h
        src/kernel/container/vdeque.cpp
        inc/kernel/mem/vpointer.h
        src/kernel/mem/vpointer.cpp
        inc/kernel/container/viterator.h
        src/kernel/container/viterator.cpp
        inc/kernel/container/vtypeextractor.h
        inc/kernel/container/vinitlist.h
        src/kernel/container/vinitlist.cpp
        inc/kernel/container/vqueue.h
        src/kernel/container/vqueue.cpp
        inc/kernel/container/vstack.h
        src/kernel/container/vstack.cpp)

target_include_directories(vlib PRIVATE inc)
target_compile_options(vlib PRIVATE /W4 /WX)