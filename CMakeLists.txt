cmake_minimum_required(VERSION 3.3)
project(MGS_V_Patch)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

#Copy config files to build directory



set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} -std=gnu11)
set(SOURCE_FILES main.c readFile.c readFile.h CCS_CLogger.c CCS_CLogger.h common.c common.h
        patcher_utility.c patcher_utility.h strmap.c strmap.h)

add_executable(MGS_V_Patch ${SOURCE_FILES} ${CONFIG_FILES})
add_executable(ErrorCheck main.cpp)
add_executable(simple simple.c)

set(CONFIG_FILES MGSV_TPP_WideScreen_Patch.cfg)
add_custom_command(TARGET MGS_V_Patch POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${PROJECT_SOURCE_DIR}/MGSV_TPP_WideScreen_Patch.cfg"
        $<TARGET_FILE_DIR:MGS_V_Patch>)