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
#else
    audio_status* get_cmus_status();
#endif


#endif // AUDIO_STATUS
