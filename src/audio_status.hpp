#ifndef PLAYER_STATUS_H
#define PLAYER_STATUS_H

#include <string>
#include <iostream>

namespace audio_status {
    struct audio_status {
        std::string file_name;
        std::string title;
        std::string artist;
        std::string album;
        std::string date;
        std::string genre;
        std::string comment;
        std::string track;
        int duration; // In seconds.
        int elapsed_time;
    };

#ifdef MPD_SUPPORT
    audio_status get_mpd_status();
#endif
    audio_status get_cmus_status();
}

inline std::ostream& operator<<(std::ostream &os, const audio_status::audio_status &data) {
    os << "{\n";
    os << "  file_name: " << data.file_name << "\n";
    os << "  title: " << data.title << "\n";
    os << "  artist: " << data.artist << "\n";
    os << "  album: " << data.album << "\n";
    os << "  date: " << data.date << "\n";
    os << "  genre: " << data.genre << "\n";
    os << "  comment: " << data.comment << "\n";
    os << "  track: " << data.track << "\n";
    os << "  duration: " << data.duration << "\n";
    os << "  elapsed time: " << data.elapsed_time << "\n";
    os << "}";
    return os;
}

#endif // PLAYER_STATUS_H
