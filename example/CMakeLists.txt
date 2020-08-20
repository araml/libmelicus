include_directories("../libmelicus")

add_executable(libmelicus_ex
               main.cpp)

target_link_libraries (libmelicus_ex LINK_PUBLIC melicus)
