cmake_minimum_required(VERSION 3.14)
project(logarbiter_sinks)

add_library(${PROJECT_NAME} STATIC
    ConsoleSinkImpl.cpp
    FileSinkImpl.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/../../inc/sinks
)

# for messages
target_link_libraries(${PROJECT_NAME} PUBLIC
    logarbiter_common
)
