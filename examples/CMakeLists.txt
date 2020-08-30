include_directories("../libmelicus")

add_executable(libmelicus_ex main.cpp)
add_executable(libmelicus_c_ex main.c)

target_link_libraries (libmelicus_ex LINK_PUBLIC melicus "-ldl")
target_link_libraries (libmelicus_c_ex LINK_PUBLIC melicus "-ldl")
