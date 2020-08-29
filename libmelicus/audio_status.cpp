#include <string.h>
#include <cstdio>
#include <audio_status.h>
#include <audio_status.hpp>

#define COPY_TO(x, y) x = (char *)malloc(y.size() + 1); \
                      strcpy(x, y.c_str());

error_status mel_error = OK;

// TODO: If caller wants to handler their own loggin whatever with err status
extern "C" char *get_error_status() {
    return NULL;
}

extern "C" void melicus_perror() {
    printf("Error: ");
    if (mel_error == OK)
        puts("OK");
    if (mel_error == SERVICE_DOWN)
        puts("SERVICE DOWN");
    if (mel_error == NO_SONG_PLAYING)
        puts("NO_SONG_PLAYING");
}

static void convert_status_to_c(audio_status *dst, melicus::audio_status &src) {
    COPY_TO(dst->file_name, src.file_name);
    COPY_TO(dst->title, src.title);
    COPY_TO(dst->artist, src.artist);
    COPY_TO(dst->album, src.album);
    COPY_TO(dst->date, src.date);
    COPY_TO(dst->genre, src.genre);
    COPY_TO(dst->comment, src.comment);
    COPY_TO(dst->track, src.track);
    dst->duration = src.duration;
    dst->elapsed_time = src.elapsed_time;
}

extern "C" audio_status* melicus_create_status() {
    return (audio_status *)malloc(sizeof(audio_status));
}

extern "C" int get_cmus_status(audio_status *dst) {
    if (!dst) {
        mel_error = NULL_STATUS;
        return 1;
    }
    auto [error, s] = melicus::get_cmus_status();
    if (error == OK) {
        convert_status_to_c(dst, s);
        return 0;
    } else {
        mel_error = error;
        return 1;
    }
}

#ifdef MPD_SUPPORT
extern "C" audio_status *get_mpd_status() {
    melicus::audio_status s = melicus::get_mpd_status();
    return convert_status_to_c(s);
}
#endif

extern "C" void audio_status_free(audio_status *st) {
    free(st->file_name);
    free(st->title);
    free(st->artist);
    free(st->album);
    free(st->date);
    free(st->genre);
    free(st->comment);
    free(st->track);
    free(st);
}
