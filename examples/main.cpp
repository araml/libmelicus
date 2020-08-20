#include <iostream>
#include <audio_status.hpp>

using namespace melicus;

int main() {
    melicus::audio_status c = get_cmus_status();
    std::cout << c << std::endl;
#ifdef MPD_SUPPORT
    melicus::audio_status s = get_mpd_status();
    std::cout << s << std::endl;
#endif
    return 0;
}
