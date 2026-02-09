cmake_minimum_required(VERSION 3.13)
project(vSomeipServer_GpuUsage)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

# Generated source files
set(GEN_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../generated/src-gen/v1/logarbiter/gpu)

set(GEN_COMMON_SRCS
    ${GEN_SRC_DIR}/GpuUsageDataSomeIPDeployment.cpp
)

set(GEN_PROXY_SRCS
    ${GEN_SRC_DIR}/GpuUsageDataSomeIPProxy.cpp
)

set(GEN_STUB_SRCS
    ${GEN_SRC_DIR}/GpuUsageDataSomeIPStubAdapter.cpp
)


find_package(CommonAPI 3.2.0 REQUIRED)
find_package(CommonAPI-SomeIP 3.2.0 REQUIRED)
find_package(vsomeip3 3.4.10 REQUIRED)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../generated/src-gen
    ${CMAKE_CURRENT_SOURCE_DIR}/../generated/src-gen/v1/logarbiter/gpu
    ${CMAKE_CURRENT_SOURCE_DIR}/inc

)



# Server executable
add_executable(${PROJECT_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/GpuUsageDataStubImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/vSomeipServer.cpp
    ${GEN_COMMON_SRCS}
    ${GEN_STUB_SRCS}
    ${GEN_PROXY_SRCS}
)

target_link_libraries(${PROJECT_NAME}
    CommonAPI
    CommonAPI-SomeIP
    vsomeip3
)