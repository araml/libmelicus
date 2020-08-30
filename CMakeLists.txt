cmake_minimum_required(VERSION 3.0)

project(libmelicus)

option(MPD_SUPPORT "Enables mpd interface support (libmpdclient must be available")

set(CMAKE_CXX_FLAGS "-ggdb -std=c++2a -Wall -Werror -Wextra -Wsign-conversion  -ldl")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
   add_compile_options (-fdiagnostics-color=always)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
   add_compile_options (-fcolor-diagnostics)
endif ()

add_subdirectory(examples)
add_subdirectory(libmelicus)

