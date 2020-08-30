add_library(melicus SHARED
            audio_status.cpp
            mpd_get_status.cpp
            cmus_get_status.cpp
            spotify_get_status.cpp)

include_directories("../libmelicus")
include_directories("../deps/json/single_include/nlohmann")
target_include_directories(melicus PUBLIC ${CMAKE_CURRENT_DIR})

