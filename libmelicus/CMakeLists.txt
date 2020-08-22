add_library(melicus SHARED
            audio_status.cpp
            mpd_get_status.cpp
            cmus_get_status.cpp)

include_directories("../libmelicus")
target_include_directories(melicus PUBLIC ${CMAKE_CURRENT_DIR})


if (MPD_SUPPORT)
    target_link_libraries(melicus "-lmpdclient")
    add_definitions(-DMPD_SUPPORT)
endif()
