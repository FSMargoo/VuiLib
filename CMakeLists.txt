cmake_minimum_required(VERSION 3.23)
project(vuilib_cmake)

set(CMAKE_CXX_STANDARD 11)

add_definitions(-DUNICODE -D_UNICODE)

aux_source_directory(src/core/object DIR_SRCS)
aux_source_directory(src/core/render DIR_SRCS)
aux_source_directory(src/core/signal DIR_SRCS)
aux_source_directory(src/core/uibasic DIR_SRCS)
aux_source_directory(src/core/widget DIR_SRCS)
aux_source_directory(src/core/control DIR_SRCS)
aux_source_directory(src/core/animation DIR_SRCS)
aux_source_directory(src/core/visual DIR_SRCS)
aux_source_directory(src/core/ DIR_SRCS)

aux_source_directory(include/core/object DIR_SRCS)
aux_source_directory(include/core/render DIR_SRCS)
aux_source_directory(include/core/signal DIR_SRCS)
aux_source_directory(include/core/uibasic DIR_SRCS)
aux_source_directory(include/core/widget DIR_SRCS)
aux_source_directory(include/core/animation DIR_SRCS)
aux_source_directory(include/core/visual DIR_SRCS)
aux_source_directory(include/core/ DIR_SRCS)
aux_source_directory(include/core/control DIR_SRCS)

aux_source_directory(include/vml VML_DIR_SRC)
aux_source_directory(src/vml VML_DIR_SRC)

aux_source_directory(include/kits VKITS_DIR_SRC)
aux_source_directory(src/kits VKITS_DIR_SRC)

aux_source_directory(include/vss VSS_DIR_SRC)
aux_source_directory(src/vss VSS_DIR_SRC)

aux_source_directory(include/win32base VWIN32_SRC)
aux_source_directory(src/win32base VWIN32_SRC)

add_library(vuilib STATIC ${DIR_SRCS})
add_library(vmllib STATIC ${VML_DIR_SRC})
add_library(vkitlib STATIC ${VKITS_DIR_SRC})
add_library(vsslib STATIC ${VSS_DIR_SRC})
add_library(vwin32base STATIC ${VWIN32_SRC})

add_executable(vuilib_cmake main.cpp)
target_link_libraries(vuilib_cmake vuilib)
target_link_libraries(vuilib_cmake vmllib)
target_link_libraries(vuilib_cmake vkitlib)
target_link_libraries(vuilib_cmake vsslib)
target_link_libraries(vuilib_cmake vwin32base)
