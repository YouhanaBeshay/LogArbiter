cmake_minimum_required(VERSION 3.14)

project(logarbiter_common)

add_library(${PROJECT_NAME} STATIC LogMessage.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../../inc/common)
