#ifndef AUDIO_STATUS_H
#define AUDIO_STATUS_H

#include <error_enum.h>

typedef struct {
    char *file_name;
    char *title;
    char *artist;
    char *album;
    char *date;
    char *genre;
    char *comment;
    char *track;
    int duration;
    int elapsed_time;
} audio_status;

typedef enum {
    MPV_SUPPORT = 1,
    MPD_SUPPORT = 2,
} lib_options;

#ifdef __cplusplus
    extern "C" int melicus_init(lib_options opts);
    extern "C" audio_status* melicus_create_status();
    extern "C" int get_cmus_status(audio_status *dst);
    extern "C" void audio_status_free(audio_status *dst);
    extern "C" char *get_error_status();
    extern "C" void melicus_perror();
    extern "C" int get_mpd_status(audio_status *dst);
#else
    int melicus_init(lib_options opts);
    audio_status* melicus_create_status();
    int get_cmus_status(audio_status *dst);
    void audio_status_free(audio_status *st);
    char *get_error_status();
    void melicus_perror();
    int get_mpd_status(audio_status *dst);
#endif

#endif // AUDIO_STATUS
