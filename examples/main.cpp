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
#ifdef MPD_SUPPORT
    melicus::audio_status s = melicus::get_mpd_status();
    std::cout << s << std::endl;
#endif
    return 0;
}
