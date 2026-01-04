cmake_minimum_required(VERSION 3.14)

project(Phase_1)

add_executable(${PROJECT_NAME} Phase_1.cpp

                               src/LogManager.cpp
                               src/ConsoleSinkImpl.cpp
                               src/FileSinkImpl.cpp
                               src/LogMessage.cpp
                               
                               )

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inc)