cmake_minimum_required(VERSION 3.3)
project(MGS_V_Patch)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CC_FLAGS "${CMAKE_CC_FLAGS} -std=C99")

set(SOURCE_FILES main.c readFile.c readFile.h common.c common.h patcher_utility.c patcher_utility.h)

add_executable(MGS_V_Patch ${SOURCE_FILES})
add_executable(ErrorCheck main.cpp)
add_executable(simple simple.c)