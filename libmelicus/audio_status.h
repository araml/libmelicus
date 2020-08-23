#ifndef AUDIO_STATUS_H
#define AUDIO_STATUS_H

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
    extern "C" audio_status* get_cmus_status();
    extern "C" void audio_status_free(audio_status *st);
#ifdef MPD_SUPPORT
    extern "C" audio_status* get_mpd_status();
#endif
#else
    audio_status* get_cmus_status();
    void audio_status_free(audio_status *st);
#ifdef MPD_SUPPORT
    audio_status* get_mpd_status();
#endif
#endif

#endif // AUDIO_STATUS
