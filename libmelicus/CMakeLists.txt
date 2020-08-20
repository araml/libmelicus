add_library(melicus SHARED
            mpd_get_status.cpp
            cmus_get_status.cpp)

include_directories("../libmelicus")
target_include_directories(melicus PUBLIC ${CMAKE_CURRENT_DIR})
