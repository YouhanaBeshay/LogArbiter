cmake_minimum_required(VERSION 3.14)

project(LogArbiter)

add_executable(${PROJECT_NAME} 
                                App/main.cpp

                                Phase_01/src/LogManager.cpp
                                Phase_01/src/ConsoleSinkImpl.cpp
                                Phase_01/src/FileSinkImpl.cpp
                                Phase_01/src/LogMessage.cpp
                                
                                Phase_02/src/SafeFile.cpp
                                Phase_02/src/FileTelemetrySourceImpl.cpp
                                Phase_02/src/SafeSocket.cpp
                                Phase_02/src/SocketTelemetrySourceImpl.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE 
                                                    ${CMAKE_CURRENT_SOURCE_DIR}/Phase_01/inc
                                                    ${CMAKE_CURRENT_SOURCE_DIR}/Phase_02/inc)