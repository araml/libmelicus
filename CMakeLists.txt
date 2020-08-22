cmake_minimum_required(VERSION 3.0)

project(libmelicus)

option(MPD_SUPPORT "Enables mpd interface support (libmpdclient must be available")

set(CMAKE_CXX_FLAGS "-ggdb -std=c++2a -Wall -Werror -Wextra -Wsign-conversion")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
   add_compile_options (-fdiagnostics-color=always)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
   add_compile_options (-fcolor-diagnostics)
endif ()

if (MPD_SUPPORT)
    add_definitions(-DMPD_SUPPORT)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lmpdclient")
endif()

add_subdirectory(examples)
add_subdirectory(libmelicus)

