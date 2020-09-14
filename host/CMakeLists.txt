cmake_minimum_required(VERSION 3.1)

project(TestHost LANGUAGES CXX)

find_package(Qt5 COMPONENTS Qml Quick REQUIRED)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/headers" "${CMAKE_SOURCE_DIR}/plugin/headers")

set(SOURCES
	sources/main.cpp
	sources/network_device.cpp
)

set(HEADERS
	headers/network_device.h
	../plugin/headers/network_interface.h

)

set(RESOURCES
	resources/test.qrc
)

add_executable(TestHost ${SOURCES} ${HEADERS} ${RESOURCES})

add_dependencies(TestHost WhiteboardPlugin)

target_link_libraries(TestHost Qt5::Quick)
