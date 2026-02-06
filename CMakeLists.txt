cmake_minimum_required(VERSION 3.14)

project(LogArbiter)

set(appName "main" CACHE STRING "App file name")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

# Generated source files
set(GEN_SRC_DIR ${CMAKE_CURRENT_LIST_DIR}/CommonApi/generated/src-gen/v1/logarbiter/gpu)

set(GEN_COMMON_SRCS
    ${GEN_SRC_DIR}/GpuUsageDataSomeIPDeployment.cpp
)

set(GEN_PROXY_SRCS
    ${GEN_SRC_DIR}/GpuUsageDataSomeIPProxy.cpp
)

find_package(CommonAPI 3.2.0 REQUIRED)
find_package(CommonAPI-SomeIP 3.2.0 REQUIRED)
find_package(vsomeip3 3.4.10 REQUIRED)
find_package(Boost 1.55 COMPONENTS system thread log REQUIRED)

add_executable(${PROJECT_NAME} App/${appName}.cpp
    ${GEN_COMMON_SRCS}
    ${GEN_PROXY_SRCS})

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


target_link_libraries(LogArbiter 
    logarbiter_common
    logarbiter_sinks
    logarbiter_sources
    logarbiter_manager
)
