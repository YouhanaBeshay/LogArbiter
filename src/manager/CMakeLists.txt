cmake_minimum_required(VERSION 3.14)
project(logarbiter_manager)

add_library(${PROJECT_NAME} STATIC
    LogManager.cpp
    LogManagerBuilder.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/../../inc/manager
)


target_link_libraries(${PROJECT_NAME} PUBLIC
    logarbiter_sinks
    logarbiter_common
)
