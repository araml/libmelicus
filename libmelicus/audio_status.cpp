#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <error_enum.h>
#include <audio_status.h>
#include <audio_status.hpp>

#define COPY_TO(x, y)                 \
    if (x)                            \
        free(x);                      \
    x = (char *)malloc(y.size() + 1); \
    strcpy(x, y.c_str());

error_status mel_error = OK;

// TODO: If caller wants to handler their own loggin whatever with err status
extern "C" char *get_error_status() {
    return (char *) err_strs[mel_error];
}

std::string last_error;

extern "C" void melicus_perror() {
    printf("Error: %s\n", err_strs[mel_error]);
}

extern "C" int melicus_init(lib_options opts) {
    if (opts & MPD_SUPPORT) {
        int err = melicus::mpd_init();
        if (err) {
            mel_error = CANT_LOAD_MPD_DLL;
            return 1;
        }
    }

    return 0;
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
    audio_status *as = (audio_status *)malloc(sizeof(audio_status));
    as->file_name = NULL;
    as->title = NULL;
    as->artist = NULL;
    as->album = NULL;
    as->date = NULL;
    as->genre = NULL;
    as->comment = NULL;
    as->track = NULL;
    as->duration = as->elapsed_time = 0;
    return as;
}

extern "C" int cmus_status(audio_status *dst) {
    if (!dst) {
        mel_error = NULL_STATUS;
        return 1;
    }
    auto [error, s] = melicus::cmus_status();
    if (error == OK) {
        convert_status_to_c(dst, s);
        return 0;
    } else {
        mel_error = error;
        return 1;
    }
}

extern "C" int mpd_status(audio_status *dst) {
    auto [error, s] = melicus::mpd_status();
    if (error == OK) {
        convert_status_to_c(dst, s);
        return 0;
    } else {
        mel_error = error;
        return 1;
    }
}

extern "C" int mpv_status(audio_status *dst) {
    auto [error, s] = melicus::mpv_status();
    if (error == OK) {
        convert_status_to_c(dst, s);
        return 0;
    } else {
        mel_error = error;
        return 1;
    }
}

extern "C" int moc_status(audio_status *dst) {
    auto [error, s] = melicus::moc_status();
    if (error == OK) {
        convert_status_to_c(dst, s);
        return 0;
    } else {
        mel_error = error;
        return 1;
    }
}

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

extern "C" void melicus_close() {
    melicus::mpd_close();
}
