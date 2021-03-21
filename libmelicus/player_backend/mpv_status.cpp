#include <string>
#include <vector>
#include <tuple>
#include <unistd.h>
#include <json.hpp>
#include <stdio.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <initializer_list>

#include <error_enum.h>
#include <audio_status.hpp>
#include <cstring>

const char mpv_path[] = "/tmp/mpvsocket";

using json = nlohmann::json;

std::string find_in_json(json &j, const std::string &tag) {
    if (j.count(tag)) {
        return j[tag];
    }

    return "";
}

/* Commands

// duration
echo '{ "command": ["get_property", "duration"] }' | socat - /tmp/mpvsocket | python3 -m json.tool
{
    "data": 226.612245,
    "request_id": 0,
    "error": "success"
}

// played time
echo '{ "command": ["get_property", "playback-time"] }' | socat - /tmp/mpvsocket | python3 -m json.tool
{
    "data": 178.682808,
    "request_id": 0,
    "error": "success"
}

// file name
echo '{ "command": ["get_property", "filename"] }' | socat - /tmp/mpvsocket | python3 -m json.tool
{
    "data": "filename.mp3",
    "request_id": 0,
    "error": "success"
}

*/

/*
void print_data(json &j1) {
    if (j1.count("data")) {
        std::cout << j1["data"].dump() << std::endl;
        std::cout << "[mpv]" << std::endl;
        std::cout << "title " << find_in_json(j1["data"], "title") << std::endl;
        std::cout << "artist "<< find_in_json(j1["data"], "artist") << std::endl;
        std::cout << "album " << find_in_json(j1["data"], "album") << std::endl;
        std::cout << "track " << find_in_json(j1["data"], "track") << std::endl;
        std::cout << "genre "<< find_in_json(j1["data"], "genre") << std::endl;
        std::cout << "comment " << find_in_json(j1["data"], "comment") << std::endl;
        std::cout << "date " << find_in_json(j1["data"], "date") << std::endl;
    }
}*/

static int send_recv(int sock_fd, char *buf, size_t buf_size, std::string tag) {
    json msg;
    msg["command"] = {"get_property", tag};
    memset(buf, 0, buf_size);
    send(sock_fd, msg.dump().c_str(), msg.dump().length(), 0);
    send(sock_fd, "\n", 1, 0); // we send the newline
    int err = recv(sock_fd, buf, buf_size, 0);

    if (err < 0) {
        perror("Error receiving");
        return 1;
    }
    return 0;
}

namespace melicus {
    std::tuple<error_status, audio_status> mpv_status() {
        struct sockaddr_un sock_addr;
        memset(&sock_addr, 0, sizeof(sock_addr));
        sock_addr.sun_family = AF_UNIX;
        strcpy(sock_addr.sun_path, mpv_path);

        int sock_fd = socket(sock_addr.sun_family, SOCK_STREAM, 0);
        if (sock_fd == -1) {
            perror("socket");
            return {CONN_ERROR, {}};
        }

        if (connect(sock_fd, (struct sockaddr *)&sock_addr, sizeof(sock_addr)) == - 1) {
            perror("Error connecting");
            return {CONN_ERROR, {}};
        }

        char buf[9000] = {0};
        send_recv(sock_fd, buf, sizeof(buf), "metadata");

        auto j1 = json::parse(buf);

        audio_status st;

        if (j1.count("data")) {
            st.title = find_in_json(j1["data"], "title");
            st.artist = find_in_json(j1["data"], "artist");
            st.album = find_in_json(j1["data"], "album");
            st.track = find_in_json(j1["data"], "track");
            st.genre = find_in_json(j1["data"], "genre");
            st.comment = find_in_json(j1["data"], "comment");
            st.date = find_in_json(j1["data"], "date");
        }

        send_recv(sock_fd, buf, sizeof(buf), "duration");

        j1 = json::parse(buf);
        if (j1.count("data")) {
            st.elapsed_time = j1["data"];
        }

        send_recv(sock_fd, buf, sizeof(buf), "playback-time");

        j1 = json::parse(buf);
        if (j1.count("data")) {
            st.duration = j1["data"];
        }

        send_recv(sock_fd, buf, sizeof(buf), "filename");
        j1 = json::parse(buf);
        if (j1.count("data")) {
            st.file_name = j1["data"];
        }

        shutdown(sock_fd, SHUT_RD);
        close(sock_fd);
        return {OK, st};
    }
}
/*
namespace melicus {
    std::tuple<error_status, audio_status> mpv_status() {
        //s = linux_socket(mpv_path);
        return {OK, {}};
    }
}*/
