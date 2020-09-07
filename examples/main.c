#include <audio_status.h>
#include <stdio.h>

static void print_example(audio_status *st) {
    printf("{\n");
    printf("  file name: %s\n", st->file_name);
    printf("  title: %s\n", st->title);
    printf("  artist: %s\n", st->artist);
    printf("  album: %s\n", st->album);
    printf("  date: %s\n", st->date);
    printf("  genre: %s\n", st->genre);
    printf("  comment: %s\n", st->comment);
    printf("  track: %s\n", st->track);
    printf("  duration: %d\n", st->duration);
    printf("  elapsed time: %d\n", st->elapsed_time);
    printf("}\n");

}

struct opts {
    char arr[50];
    int enabled;
};


int mpd_on = 0;

int main() {
    if (!melicus_init(MPD_SUPPORT))
        mpd_on = 1;

    audio_status *st = melicus_create_status();
    if (get_cmus_status(st)) {
        printf("Error getting status\n");
        melicus_perror();
    } else {
        print_example(st);
    }

    if (mpd_on && !get_mpd_status(st)) {
        print_example(st);
    }

    audio_status_free(st);

    melicus_close();
}
