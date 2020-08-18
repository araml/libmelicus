#ifndef SONG_METADATA_H
#define SONG_METADATA_H

#include <string>
#include <iostream>

struct song_metadata {
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

std::ostream& operator<<(std::ostream &os, const song_metadata &data) {
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

#endif // SONG_METADATA_H
