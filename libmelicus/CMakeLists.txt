add_library(melicus SHARED
            audio_status.cpp
            player_backend/mpd_status.cpp
            player_backend/cmus_status.cpp
            player_backend/mpv_status.cpp
            player_backend/moc_status.cpp)

include_directories(player_backend)
include_directories("../libmelicus")
include_directories("../deps/json/single_include/nlohmann")
target_include_directories(melicus PUBLIC ${CMAKE_CURRENT_DIR})

