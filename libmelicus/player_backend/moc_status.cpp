#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <iostream>

#include <../fork.h>
#include <../audio_status.hpp>

/* State: PAUSE
File:
Title:
Artist:
SongTitle:
Album:
TotalTime:
TimeLeft:
TotalSec:
CurrentTime:
CurrentSec:
Bitrate: 128kbps
AvgBitrate: 128kbps
Rate: 44kHz
*/

/*
static std::string find_tag(std::vector<std::string> &info, std::string tag) {
    std::string tag_info{""};
    for (auto &v : info) {
        if (v.size() < tag.size())
            continue;
        auto [p1, p2] = std::mismatch(tag.begin(), tag.end(), v.begin());
        if (p1 != tag.end()) // found tag
            continue;
        while (p2 != v.end()) {
            tag_info.push_back(*p2);
            p2++;
        }
    }

    return tag_info;

*/
std::string find_tag(const std::vector<std::string> &v, std::string tag) {
    for (auto &s : v) {
        size_t pos = s.find(tag);
        if (pos != std::string::npos) {
            return s.substr(tag.size(), s.size());
        }
    }

    return "";
}

static int str_to_int(const std::string &s) {
    if (s.empty())
        return -1;

    try {
        return stoi(s);
    } catch (...) {
        return -1;
    }
}


// TODO: STOP MUSIC CASE (State: STOP)
// TODO: MOCP OFFLINE (FATAL_ERROR: The server is not running!)
namespace melicus {
    std::tuple<error_status, audio_status> moc_status() {

        const char bin[] = "/usr/bin/mocp";
        const char *args[3] = {"mocp", "--info", NULL};
        std::vector<std::string> info = fork(bin, args);

        audio_status status;

        status.file_name = find_tag(info, "File: ");
        status.title = find_tag(info, "Title: ");
        status.artist = find_tag(info, "Artist: ");
        status.album = find_tag(info, "Album: ");
        status.duration = str_to_int(find_tag(info, "TotalSec: "));
        status.elapsed_time = str_to_int(find_tag(info, "CurrentSec: "));
        /* NOTE: no apparent way to get date/genre/comment/track number with mocp */
        /* TODO: Inspect source to see if we can by talking to the server directly */


        return {OK, status};
    }
}

