cmake_minimum_required(VERSION 3.14)
project(logarbiter_sinks)

add_library(${PROJECT_NAME} STATIC
    ConsoleSinkImpl.cpp
    FileSinkImpl.cpp
    LogSinkFactory.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/../../inc/sinks
)

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_LIST_DIR}/../../external_lib/magic_enum
)

# for messages
target_link_libraries(${PROJECT_NAME} PUBLIC
    logarbiter_common
)
