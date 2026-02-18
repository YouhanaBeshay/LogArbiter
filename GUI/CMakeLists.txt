cmake_minimum_required(VERSION 3.16)
project(LogGUI VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt specific settings
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Quick QuickControls2)

# Source files
set(SOURCES
    main.cpp
    src/AppController.cpp
    src/LogModel.cpp
    src/LogFilterProxyModel.cpp
)

set(HEADERS
    src/AppController.hpp
    src/LogModel.hpp
    src/LogFilterProxyModel.hpp
)

# Create executable with resources
add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS}
    resources.qrc
)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Link Qt libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Quick
    Qt6::QuickControls2
)

# Install
install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)