cmake_minimum_required(VERSION 3.10)

# Project name
project(CAN_interface)

set(CMAKE_CXX_STANDARD 17)

add_executable(
    ${PROJECT_NAME}
    src/main.cpp
    src/CanMessage.cpp
    src/RawCanDevice.cpp
    src/IsoTpCanDevice.cpp
    src/CanDevice.cpp
    src/CanDeviceFactory.cpp
)

target_link_libraries(
    ${PROJECT_NAME}
    socketcan
)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    include
)