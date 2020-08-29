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

#ifdef __cplusplus
    extern "C" audio_status* melicus_create_status();
    extern "C" int get_cmus_status(audio_status *dst);
    extern "C" void audio_status_free(audio_status *dst);
    extern "C" char *get_error_status();
    extern "C" void melicus_perror();
#ifdef MPD_SUPPORT
    extern "C" audio_status* get_mpd_status();
#endif
#else
    audio_status* melicus_create_status();
    int get_cmus_status(audio_status *dst);
    void audio_status_free(audio_status *st);
    char *get_error_status();
    void melicus_perror();
#ifdef MPD_SUPPORT
    audio_status* get_mpd_status();
#endif
#endif

#endif // AUDIO_STATUS
