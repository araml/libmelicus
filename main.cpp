#include <iostream>
#include <audio_status.hpp>

using namespace audio_status;

int main() {
    audio_status::audio_status c = get_cmus_status();
    std::cout << c << std::endl;
#ifdef MPD_SUPPORT
    audio_status::audio_status s = get_mpd_status();
    std::cout << s << std::endl;
#endif
    return 0;
}
