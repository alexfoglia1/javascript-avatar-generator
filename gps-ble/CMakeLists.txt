cmake_minimum_required(VERSION 3.10)
project(gps-ble CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)


add_executable(
gps-ble
${CMAKE_SOURCE_DIR}/gps.h
${CMAKE_SOURCE_DIR}/gps.cpp
${CMAKE_SOURCE_DIR}/gps_server.h
${CMAKE_SOURCE_DIR}/gps_server.cpp

${CMAKE_SOURCE_DIR}/main.cpp
)

find_package(Qt5 COMPONENTS Core SerialPort Network WebSockets REQUIRED)

target_link_libraries(gps-ble ${SDL2_LIBRARIES} Qt5::Core Qt5::SerialPort Qt5::Network Qt5::WebSockets)