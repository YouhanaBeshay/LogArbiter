cmake_minimum_required(VERSION 3.14)
project(logarbiter_io_raii)

add_library(${PROJECT_NAME} STATIC
    SafeFile.cpp
    SafeSocket.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/../../inc/io_raii
)
