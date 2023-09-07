cmake_minimum_required (VERSION 3.10.2)
include(GNUInstallDirs)

set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

project (TimeStatistics)

message("system_name : ${CMAKE_SYSTEM_NAME}")

set(LIB_NAME time_tracker)

set(SRC_FILES time_ut.cpp time_ut.h time_statistics.cpp time_statistics.h safe_map.h safe_queue.h statistics_time_tracker.h statistics_time_tracker.cpp)

# aux_source_directory("." TIME_TRACKER_LIB_SRC)

# 生成静态库
add_library(${LIB_NAME} STATIC "${SRC_FILES}")

# 将源代码添加到此项目的可执行文件。
add_executable (TimeStatistics main.cpp time_ut.cpp time_ut.h time_statistics.cpp time_statistics.h safe_map.h safe_queue.h statistics_time_tracker.h statistics_time_tracker.cpp)

