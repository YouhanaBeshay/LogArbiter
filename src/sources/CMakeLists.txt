cmake_minimum_required(VERSION 3.14)
project(logarbiter_sources)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

# Generated source files
set(GEN_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../CommonApi/generated/src-gen/v1/logarbiter/gpu)

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

add_library(${PROJECT_NAME} STATIC
    FileTelemetrySourceImpl.cpp
    SocketTelemetrySourceImpl.cpp
    SomeIPTelemetrySourceImpl.cpp
)

# add a **shared** library (as static will have problems)
add_library(commonapi_gen_client SHARED
    ${GEN_COMMON_SRCS}
    ${GEN_PROXY_SRCS})


target_include_directories(commonapi_gen_client PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/../../CommonApi/generated/src-gen
)



target_link_libraries(commonapi_gen_client PUBLIC
    CommonAPI
    CommonAPI-SomeIP
    )


target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/../../inc/sources
    ${CMAKE_CURRENT_LIST_DIR}/../../CommonApi/generated/src-gen
    ${COMMONAPI_INCLUDE_DIRS}
    ${COMMONAPI_SOMEIP_INCLUDE_DIRS}
    ${VSOMEIP_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIR}
)



target_link_libraries(${PROJECT_NAME} PUBLIC
    logarbiter_io_raii
    commonapi_gen_client
    vsomeip3
    ${Boost_LIBRARIES}
    pthread
)