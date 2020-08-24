#include <netdb.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <thread>
#include <chrono>
#include <arpa/inet.h>

#include <audio_status.hpp>
#include <vector>
#include <sstream>

using namespace std::chrono;

static inline std::vector<std::string> split_string(std::string line) {
    std::stringstream ss(line);
    std::string word;
    std::vector<std::string> words;
    while (std::getline(ss, word, '\n')) {
        words.emplace_back(word);
    }

    return words;
}

#include <algorithm>

template <typename T, typename Predicate>
std::vector<T> filter(const std::vector<T> &v, Predicate p) {
    std::vector<T> filtered;
    std::copy_if(v.begin(), v.end(), std::back_inserter(filtered), p);
    return filtered;
}

std::string find_in(const std::vector<std::string> &v, std::string tag) {
    for (auto &s : v) {
        size_t pos = s.find(tag);
        if (pos != std::string::npos) {
            return s.substr(tag.size(), s.size());
        }
    }

    return "";
}

static bool no_song_playing(std::vector<std::string> &v) {
    if (find_in(v, "tag title ").empty() && find_in(v, "tag artist ").empty() &&
        find_in(v, "tag album ").empty()) {
        return true;
    }
    return false;
}

static int str_to_int(const std::string &s) {
    if (s.empty())
        return 0;

    try {
        return stoi(s);
    } catch (...) {
        return 0;
    }
}

namespace melicus {
    std::tuple<error_status, audio_status> get_cmus_status() {
        std::string env_path = std::string(getenv("XDG_RUNTIME_DIR"));
        env_path += "/cmus-socket";

        struct sockaddr_un sock_addr;
        memset(&sock_addr, 0, sizeof(sock_addr));
        sock_addr.sun_family = AF_UNIX;
        strcpy(sock_addr.sun_path, env_path.c_str());

        int sock_fd = socket(sock_addr.sun_family, SOCK_STREAM, 0);
        if (sock_fd == -1) {
            perror("socket");
            return {SERVICE_DOWN, {}};
        }

        if (connect(sock_fd, (struct sockaddr *)&sock_addr,
                    sizeof(sock_addr)) == -1) {
            perror("Couldn't connect");
            return {SERVICE_DOWN, {}};
        }

        error_status err_stat = error_status::OK;
        char status[] = "status\n";
        std::string buf;
        buf.resize(9000, 0);
        send(sock_fd, status, sizeof(status), 0);
        int err = recv(sock_fd, buf.data(), buf.size(), 0);
        if (err < 0) {
            perror("Error receiving");
            err = SERVICE_DOWN;
        }
        auto vv = split_string(buf);

        vv = filter(vv, [](const std::string &s) {
                            return !(s.find("set") != std::string::npos);
                    });


        audio_status song_data;

        if (no_song_playing(vv)) {
            err_stat = NO_SONG_PLAYING;
        }

        song_data.title = find_in(vv, "tag title ");
        song_data.artist = find_in(vv, "tag artist ");
        song_data.album = find_in(vv, "tag album ");
        song_data.date = find_in(vv, "tag date ");
        song_data.genre = find_in(vv, "tag genre ");
        song_data.comment = find_in(vv, "tag comment");
        song_data.file_name = find_in(vv, "file ");
        song_data.track = find_in(vv, "tag tracknumber ");
        song_data.duration = str_to_int(find_in(vv, "duration"));
        song_data.elapsed_time = str_to_int(find_in(vv, "position"));

        close(sock_fd);
        return std::make_tuple(err_stat, song_data);
    }
}
