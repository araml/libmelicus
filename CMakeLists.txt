cmake_minimum_required(VERSION 3.0)

project(libmelicus)

enable_testing()

option(MPD_SUPPORT "Enables mpd interface support (libmpdclient must be available")

set(CMAKE_CXX_FLAGS "-ggdb -std=c++2a -Wall -Werror -Wextra -Wsign-conversion  -ldl")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
   add_compile_options (-fdiagnostics-color=always)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
   add_compile_options (-fcolor-diagnostics)
endif ()

set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE "/usr/bin/iwyu")

add_subdirectory(examples)
add_subdirectory(libmelicus)

if (BUILD_TESTS)
    macro(add_custom_test name file)
        string(REGEX REPLACE "(^.*/|\\.[^.]*$)" "" bare_name ${file})
        add_executable(${name} ${file} ${ARGN})
        include_directories(libmelicus)
        add_test(NAME ${name} COMMAND ${name}
                 WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
        target_link_libraries(${name} ${LIBRARIES} "-lpthread")
    endmacro()

    add_custom_test(test_task tests/test_task.cpp "")
endif()

