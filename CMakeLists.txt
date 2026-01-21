cmake_minimum_required(VERSION 3.14)

project(LogArbiter)

set(appName "main" CACHE STRING "App file name")

add_executable(${PROJECT_NAME} App/${appName}.cpp)

add_subdirectory(src/common)
add_subdirectory(src/io_raii)
add_subdirectory(src/sinks)
add_subdirectory(src/sources)
add_subdirectory(src/manager)

# as formatter & magic_enum is only a .hpp so no need to make it a library 
target_include_directories(LogArbiter PRIVATE 
    ${CMAKE_CURRENT_LIST_DIR}/external_lib/magic_enum
)

target_include_directories(LogArbiter PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/inc/formatter
)


target_link_libraries(LogArbiter PRIVATE
    logarbiter_common
    logarbiter_sinks
    logarbiter_sources
    logarbiter_manager
)
