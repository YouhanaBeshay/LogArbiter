cmake_minimum_required(VERSION 3.14)

project(LogArbiter)

add_executable(${PROJECT_NAME} App/main.cpp)

add_subdirectory(src/common)
add_subdirectory(src/io_raii)
add_subdirectory(src/sinks)
add_subdirectory(src/sources)
add_subdirectory(src/manager)

target_link_libraries(LogArbiter PRIVATE
    logarbiter_common
    logarbiter_sinks
    logarbiter_sources
    logarbiter_manager
)
