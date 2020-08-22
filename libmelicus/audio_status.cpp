#include <string.h>
#include <audio_status.h>
#include <audio_status.hpp>

#define COPY_TO(x, y) x = (char *)malloc(y.size() + 1); \
                      strcpy(x, y.c_str());

static audio_status *convert_status_to_c(melicus::audio_status &st) {
    audio_status *rs = (audio_status *)malloc(sizeof(audio_status));
    COPY_TO(rs->file_name, st.file_name);
    COPY_TO(rs->title, st.title);
    COPY_TO(rs->artist, st.artist);
    COPY_TO(rs->album, st.album);
    COPY_TO(rs->date, st.date);
    COPY_TO(rs->genre, st.genre);
    COPY_TO(rs->comment, st.comment);
    COPY_TO(rs->track, st.track);
    rs->duration = st.duration;
    rs->elapsed_time = st.elapsed_time;

    return rs;
}

extern "C" audio_status *get_cmus_status() {
    melicus::audio_status s = melicus::get_cmus_status();
    return convert_status_to_c(s);
}

extern "C" audio_status *get_mpd_status() {
    melicus::audio_status s = melicus::get_mpd_status();
    return convert_status_to_c(s);
}
