#include <iostream>
#include <error_enum.h>
#include <audio_status.hpp>

using namespace melicus;

static void print_err(error_status &err) {
    if (err == OK)
        std::cout << "OK" << std::endl;
    if (err == SERVICE_DOWN)
        std::cout << "SERVICE DOWN" << std::endl;
    if (err == NO_SONG_PLAYING)
        std::cout << "NO_SONG_PLAYING" << std::endl;
}

int main() {
    auto [error, st] = melicus::cmus_status();
    print_err(error);
    std::cout << "cmus " << std::endl << st << std::endl;

    melicus::mpd_init();

    auto [error2, st2] = melicus::mpd_status();

    std::cout << "mpd " << std::endl << st2 << std::endl;

    auto [error3, st3] = melicus::mpv_status();
    std::cout << "mpv" << std::endl << st3 << std::endl;

    auto [error4, st4] = melicus::moc_status();
    std::cout << "moc" << std::endl << st4 << std::endl;
    return 0;
}
