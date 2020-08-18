#ifndef SONG_METADATA_H
#define SONG_METADATA_H

#include <string>
#include <iostream>

struct song_metadata {
    int duration; // In seconds.
    int position;
    std::string file_name;
    std::string title;
    std::string artist;
    std::string album;
    std::string date;
    std::string genre;
    std::string comment;
    int track_number;
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
    os << "}";
    return os;
}

#endif // SONG_METADATA_H
