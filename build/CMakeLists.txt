cmake_minimum_required( VERSION 2.8 )
SET( EXECUTABLE_OUTPUT_PATH "../passion/bin" )
SET( LIBRARY_OUTPUT_PATH "../passion/bin/bin" )
include( "${CMAKE_CURRENT_SOURCE_DIR}/lua.cmake" )

project(SOIL)
	FILE( GLOB_RECURSE SOIL_SRCS ../extlibs/SOIL/*.c )
	include_directories( "../extlibs/SOIL" )
	add_library( SOIL SHARED ${SOIL_SRCS} )
project(input)
	FILE( GLOB_RECURSE input_SRCS ../src/Passion/Input/*.cpp )
	include_directories( "../include" "../extlibs/headers" )
	add_library( input SHARED ${input_SRCS} )
	set_target_properties( input PROPERTIES PREFIX "" )
project(render)
	FILE( GLOB_RECURSE render_SRCS ../src/Passion/Render/*.cpp )
	include_directories( "../include" "../extlibs/headers" )
	link_directories( "${CMAKE_CURRENT_SOURCE_DIR}/../extlibs/lib" )
	add_library( render SHARED ${render_SRCS} )
	set_target_properties( render PROPERTIES PREFIX "" )
	target_link_libraries( render GL SOIL )
	add_dependencies( render SOIL )
project(network)
	FILE( GLOB_RECURSE network_SRCS ../src/Passion/Network/*.cpp )
	include_directories( "../include" )
	add_library( network SHARED ${network_SRCS} )
	set_target_properties( network PROPERTIES PREFIX "" )
project(scripting)
	FILE( GLOB_RECURSE scripting_SRCS ../src/Passion/Scripting/*.cpp )
	include_directories( "../include" "../extlibs/headers" )
	link_directories( "${CMAKE_CURRENT_SOURCE_DIR}/../extlibs/lib" )
	add_library( scripting SHARED ${scripting_SRCS} ${lua_SRCS} )
	set_target_properties( scripting PROPERTIES PREFIX "" )
	target_link_libraries( scripting dl lua )
project(Passion)
	FILE( GLOB_RECURSE passion_SRCS ../passion/src/*.cpp )
	include_directories( "../include" "../extlibs/headers" )
	link_directories( "${CMAKE_CURRENT_SOURCE_DIR}/../lib/"
	"${CMAKE_CURRENT_SOURCE_DIR}/../extlibs/lib" )
	add_executable( passion ${passion_SRCS} ../src/Passion/Render/GLee/GLee.c )
	target_link_libraries( passion input GL GLU SOIL lua render network dl scripting )
