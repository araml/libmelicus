#ifndef AUDIO_STATUS_HXX
#define AUDIO_STATUS_HXX

#include <error_enum.h>

#include <iostream>
#include <string>
#include <tuple>

namespace melicus {
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

    // TODO(aram): maybe change this to a struct?..
    using status = std::tuple<error_status, audio_status>;

    int mpd_init();
    void mpd_close();
    status mpd_status();
    status cmus_status();
    status mpv_status();
    status moc_status();
}

inline std::ostream& operator<<(std::ostream &os, const melicus::audio_status &data) {
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

#endif // AUDIO_STATUS_HXX
