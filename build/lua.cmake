cmake_minimum_required(VERSION 2.8)

FILE( GLOB_RECURSE lua_SRCS ../extlibs/lua-5.2/*.c )
LIST( REMOVE_ITEM lua_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/../extlibs/lua-5.2/luac.c" )