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


#include <mpd/client.h>
#include <mpd/status.h>
#include <mpd/entity.h>
#include <mpd/search.h>
#include <mpd/tag.h>
#include <mpd/message.h>

#include <mpd/client.h>
#include <mpd/status.h>
#include <mpd/entity.h>
#include <mpd/search.h>
#include <mpd/tag.h>
#include <mpd/message.h>

#include <song_metadata.h>
#include <vector>
#include <sstream>

using namespace std::chrono;

inline std::vector<std::string> split_string(std::string line) {
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

song_metadata get_cmus_status() {
    std::string env_path = std::string(getenv("XDG_RUNTIME_DIR"));
    env_path += "/cmus-socket";

    struct sockaddr_un sock_addr;
    memset(&sock_addr, 0, sizeof(sock_addr));
    sock_addr.sun_family = AF_UNIX;
    strcpy(sock_addr.sun_path, env_path.c_str());

    int sock_fd = socket(sock_addr.sun_family, SOCK_STREAM, 0);
    if (sock_fd == -1) {
        perror("socket");
        exit(1);
    }

    if (connect(sock_fd, (struct sockaddr *)&sock_addr, sizeof(sock_addr)) == -1) {
        perror("Couldn't connect");
        exit(1);
    }

    char status[] = "status\n";
    std::string buf;
    buf.resize(9000, 0);
    send(sock_fd, status, sizeof(status), 0);
    int err = recv(sock_fd, buf.data(), buf.size(), 0);
    if (err < 0)
        perror("Error receiving");

    auto vv = split_string(buf);

    vv = filter(vv, [](const std::string &s) {
                        return !(s.find("set") != std::string::npos);
                });

    song_metadata song_data;
    song_data.title = find_in(vv, "tag title ");
    song_data.artist = find_in(vv, "tag artist ");
    song_data.album = find_in(vv, "tag album ");
    song_data.date = find_in(vv, "tag date ");
    song_data.genre = find_in(vv, "tag genre ");
    song_data.comment = find_in(vv, "tag comment");
    song_data.file_name = find_in(vv, "file ");
    song_data.track = find_in(vv, "tag tracknumber ");
    song_data.duration = stoi(find_in(vv, "duration"));
    song_data.elapsed_time = stoi(find_in(vv, "position"));

    close(sock_fd);
    return song_data;
}

#include <cassert>

static int handle_error(struct mpd_connection *c) {
    assert(mpd_connection_get_error(c) != MPD_ERROR_SUCCESS);

    fprintf(stderr, "%s\n", mpd_connection_get_error_message(c));
    mpd_connection_free(c);
    return EXIT_FAILURE;
}

// mpd can return a NULL if the tag doesn't exists so we return an empty str
#define ine(x) (x ? x : "")

song_metadata get_mpd_status() {
    mpd_connection *conn = mpd_connection_new(NULL, 0, 0);

    if (mpd_connection_get_error(conn) != MPD_ERROR_SUCCESS)
        handle_error(conn);

    mpd_command_list_begin(conn, true);
    mpd_send_status(conn);
    mpd_send_current_song(conn);
    mpd_command_list_end(conn);

    mpd_status *status = mpd_recv_status(conn);
    if (status)
        mpd_response_next(conn);

    mpd_song *song = mpd_recv_song(conn);

    if (!status || !song) {
        std::cout << "Error getting status " << std::endl;
    }

    song_metadata sdata;
    sdata.title = std::string(ine(mpd_song_get_tag(song, MPD_TAG_TITLE, 0)));
    sdata.artist = std::string(ine(mpd_song_get_tag(song, MPD_TAG_ARTIST, 0)));
    sdata.album = std::string(ine(mpd_song_get_tag(song, MPD_TAG_ALBUM, 0)));
    sdata.date = std::string(ine(mpd_song_get_tag(song, MPD_TAG_DATE, 0)));
    sdata.genre = std::string(ine(mpd_song_get_tag(song, MPD_TAG_GENRE, 0)));
    sdata.comment = std::string(ine(mpd_song_get_tag(song, MPD_TAG_COMMENT, 0)));
    sdata.file_name = std::string(ine(mpd_song_get_uri(song)));
    sdata.track = ine(mpd_song_get_tag(song, MPD_TAG_TRACK, 0));
    sdata.duration = mpd_song_get_duration(song);
    sdata.elapsed_time = mpd_status_get_elapsed_time(status);


    mpd_song_free(song);
    mpd_status_free(status);
    mpd_connection_free(conn);
    return sdata;
}

int main() {
    song_metadata c = get_cmus_status();
    std::cout << c << std::endl;

    song_metadata s = get_mpd_status();
    std::cout << s << std::endl;

    return 0;
}
