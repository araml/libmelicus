#ifndef SONG_METADATA_CONVERT_H
#define SONG_METADATA_CONVERT_H

#include <song_metadata.h>

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
} player_status;

player_status * extern "C" convert_to_c_status(song_metadata &md);
extern "C" player_status* get_cmus_status();

#endif
