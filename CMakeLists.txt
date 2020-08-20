cmake_minimum_required(VERSION 3.0)

project(libmelicus)

set(CMAKE_CXX_FLAGS "-ggdb -std=c++2a -Wall -Werror -Wextra -Wsign-conversion")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
   add_compile_options (-fdiagnostics-color=always)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
   add_compile_options (-fcolor-diagnostics)
endif ()

macro(add_files name)
    file(GLOB ${name} src/${name}/*.h src/${name}/*.cpp src/${name}/*.c)
    include_directories(src/${name})
endmacro()

include_directories(src)

add_executable(libmelicus_ex
               src/main.cpp
               src/mpd_get_status.cpp
               src/cmus_get_status.cpp)
#set_source_files_properties(ncurses_mock/ncurses.c PROPERTIES LANGUAGE CXX )

