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

typedef enum {
    MPV_SUPPORT = 1,
    MPD_SUPPORT = 2,
} lib_options;

#ifdef __cplusplus
extern "C" {
#endif
    int cmus_status(audio_status *dst);
    int mpd_status(audio_status *dst);
    int mpv_status(audio_status *dst);
    int moc_status(audio_status *dst);

    void audio_status_free(audio_status *dst);

    int melicus_init(lib_options opts);
    audio_status* melicus_create_status();
    char *melicus_error_status();
    void melicus_perror();
    void melicus_close();

#ifdef __cplusplus
}
#endif

#endif // AUDIO_STATUS
