cmake_minimum_required(VERSION 2.8)

project(LifeSearchLeap)

option(DEVELOP "enable develop mode (use clang++ if exists)" false)

exec_program(arch OUTPUT_VARIABLE ARCH)
if (ARCH STREQUAL x86_64)
    link_directories(${PROJECT_SOURCE_DIR}/lib/x64/)
else()
    link_directories(${PROJECT_SOURCE_DIR}/lib/x86/)
endif()

if (DEVELOP)
    exec_program(which ARGS clang++ OUTPUT_VARIABLE CLANG_PATH)
    if (NOT CLANG_PATH STREQUAL "")
        message("== Set C++ Compiler to clang++")
        set(CMAKE_CXX_COMPILER ${CLANG_PATH})
    endif()
endif()

set(CMAKE_CXX_FLAGS -std=c++11)

find_package(PkgConfig)
pkg_check_modules(DBUS REQUIRED dbus-glib-1)

file(GLOB SRCS src/*.cpp)

add_executable(${CMAKE_PROJECT_NAME} ${SRCS})

include_directories(${PROJECT_SOURCE_DIR}/include ${DBUS_INCLUDE_DIRS})
target_link_libraries(${CMAKE_PROJECT_NAME} ${DBUS_LIBRARIES} Leap)
