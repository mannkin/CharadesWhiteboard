cmake_minimum_required(VERSION 3.1)

project(WhiteboardPlugin LANGUAGES CXX)

find_package(Qt5 COMPONENTS Qml Quick REQUIRED)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/headers" )

set(SOURCES
	sources/whiteboard_plugin.cpp
	sources/whiteboard_logic.cpp
	sources/whiteboard_input_backend.cpp
	sources/whiteboard_quick_canvas.cpp
	sources/network_event.cpp
	sources/input_event.cpp
	sources/local_drawing_client.cpp
	sources/local_drawing_server.cpp
	sources/virtual_drawing_client.cpp
	sources/virtual_drawing_server.cpp
	sources/whiteboard_logic.cpp
	sources/toolbox.cpp
	sources/shape_editor.cpp
	sources/shape_style.cpp
	sources/shape_position.cpp
	sources/bounding_box.cpp
	sources/shape.cpp
	sources/line.cpp
	sources/dash_line.cpp
	sources/straight_line.cpp
	sources/rectangle.cpp
	sources/text.cpp
	sources/cactus.cpp
	sources/arrow.cpp
	sources/ellipse.cpp
)

set(HEADERS
	headers/whiteboard_plugin.h
	headers/whiteboard_logic.h
	headers/whiteboard_input_backend.h
	headers/whiteboard_quick_canvas.h
	headers/network_event.h
	headers/input_event.h
	headers/local_drawing_server.h
	headers/local_drawing_client.h
	headers/virtual_drawing_client.h
	headers/virtual_drawing_server.h
	headers/drawing_client.h
	headers/drawing_server.h
	headers/network_interface.h
	headers/toolbox.h
	headers/shape_editor.h
	headers/shape_style.h
	headers/shape_position.h
	headers/bounding_box.h
	headers/shape.h
	headers/line.h
	headers/dash_line.h
	headers/straight_line.h
	headers/arrow.h
	headers/ellipse.h
	headers/text.h
	headers/cactus.h
	headers/rectangle.h
)

set(RESOURCES
	resources/qmldir
	resources/plugin.qrc
)

add_library(
	${PROJECT_NAME}
	SHARED

	${SOURCES}
	${HEADERS}
	${RESOURCES}
)

target_link_libraries(${PROJECT_NAME} Qt5::Quick Qt5::Qml)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/resources/qmldir ${CMAKE_BINARY_DIR}/bin/Whiteboard/qmldir COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/resources/qmldir ${CMAKE_BINARY_DIR}/bin/qmldir COPYONLY)

