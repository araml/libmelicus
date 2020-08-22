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
    audio_status *st = get_cmus_status();
    print_example(st);
#ifdef MPD_SUPPORT
    st = get_mpd_status();
    print_example(st);
#endif
}
