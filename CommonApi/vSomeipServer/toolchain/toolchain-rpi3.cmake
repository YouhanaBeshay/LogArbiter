# rpi3-toolchain.cmake
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)


set(CROSS_COMPILER_PATH /home/youhana/x-tools/aarch64-rpi3-linux-gnu/bin)
set(CMAKE_C_COMPILER ${CROSS_COMPILER_PATH}/aarch64-rpi3-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER ${CROSS_COMPILER_PATH}/aarch64-rpi3-linux-gnu-g++)

# it cant find string without this for some reason
set(CMAKE_CXX_FLAGS "-include string" CACHE STRING "C++ flags" FORCE)

# Paths to cross-compiled libraries
set(BOOST_ROOT /home/youhana/ITI_assignments2/cpp_labs/vsomeip_for_rpi/boost-built)
set(BOOST_INCLUDEDIR /home/youhana/ITI_assignments2/cpp_labs/vsomeip_for_rpi/boost-built/include)
set(BOOST_LIBRARYDIR /home/youhana/ITI_assignments2/cpp_labs/vsomeip_for_rpi/boost-built/lib)
set(Boost_NO_SYSTEM_PATHS ON)

set(vsomeip3_DIR /home/youhana/ITI_assignments2/cpp_labs/vsomeip_for_rpi/vsomeip-built/lib/cmake/vsomeip3)

set(CommonAPI_DIR /home/youhana/ITI_assignments2/cpp_labs/commonapi_for_rpi/commonapi-core-built/lib/cmake/CommonAPI-3.2.4)
set(CommonAPI-SomeIP_DIR /home/youhana/ITI_assignments2/cpp_labs/commonapi_for_rpi/commonapi-someip-built/lib/cmake/CommonAPI-SomeIP-3.2.4)


# Search settings
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

