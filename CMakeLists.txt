cmake_minimum_required(VERSION 3.1)

project(Whiteboard)

option(SANITIZE "Use address sanitizer." FALSE)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -static-libgcc -static-libstdc++")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/Whiteboard")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

list(APPEND QML_DIRS ${CMAKE_SOURCE_DIR}/plugin/resources)
list(APPEND QML_DIRS ${CMAKE_BINARY_DIR}/bin)
list(APPEND QML_DIRS ${CMAKE_BINARY_DIR}/bin/Whiteboard)

set(QML_IMPORT_PATH ${QML_DIRS} CACHE STRING "" FORCE)

message(${QML_IMPORT_PATH})

set(QML_IMPORT_TRACE TRUE)

if(SANITIZE)
    message(STATUS "Building with AddressSanitizer...")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
endif(SANITIZE)

add_subdirectory(plugin)
add_subdirectory(host)
