#include <iostream>
#include <cmus_get_status.h>
#include <mpd_get_status.h>

int main() {
    song_metadata c = get_cmus_status();
    std::cout << c << std::endl;

    song_metadata s = get_mpd_status();
    std::cout << s << std::endl;

    return 0;
}
