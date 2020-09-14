add_library(melicus SHARED
            audio_status.cpp
            mpd_status.cpp
            cmus_status.cpp
            mpv_status.cpp)

include_directories("../libmelicus")
include_directories("../deps/json/single_include/nlohmann")
target_include_directories(melicus PUBLIC ${CMAKE_CURRENT_DIR})

