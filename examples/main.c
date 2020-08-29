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

int main() {
    audio_status *st = melicus_create_status();
    if (get_cmus_status(st)) {
        printf("Error getting status\n");
        melicus_perror();
    } else {
        print_example(st);
        audio_status_free(st);
    }
    st = NULL;
#ifdef MPD_SUPPORT
    st = get_mpd_status();
    print_example(st);
    audio_status_free(st);
#endif
}
