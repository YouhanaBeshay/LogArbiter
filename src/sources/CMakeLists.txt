cmake_minimum_required(VERSION 3.14)
project(logarbiter_sources)

add_library(${PROJECT_NAME} STATIC
    FileTelemetrySourceImpl.cpp
    SocketTelemetrySourceImpl.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/../../inc/sources
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    logarbiter_io_raii
)