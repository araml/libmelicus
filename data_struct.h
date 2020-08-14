#include <string>

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
