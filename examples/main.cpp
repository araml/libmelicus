#include <iostream>
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
    auto [error, st] = melicus::get_cmus_status();
    print_err(error);
    std::cout << st << std::endl;

    melicus::init_mpd();

    auto [error2, st2] = melicus::get_mpd_status();

    std::cout << st2 << std::endl;
    return 0;
}
