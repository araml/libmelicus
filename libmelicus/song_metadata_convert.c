#include <audio_status.h>
#include <string.h>

#define COPY_TO(x, y) x = (char *)malloc(y.size() + 1); \
                      strcpy(x, y.c_str());

audio_status *convert_status_to_c(audio_status &md) {
    audio_status *rs = (audio_status *)malloc(sizeof(audio_status));
    COPY_TO(rs->file_name, md.file_name);
    COPY_TO(rs->title, md.title);
    COPY_TO(rs->artist, md.artist);
    COPY_TO(rs->album, md.album);
    COPY_TO(rs->date, md.date);
    COPY_TO(rs->genre, md.genre);
    COPY_TO(rs->comment, md.comment);
    COPY_TO(rs->track, md.track);
    rs->duration = md.duration;
    rs->elapsed_time = md.elapsed_time;

    return rs;
}

audio_status *get_cmus_status() {

}
