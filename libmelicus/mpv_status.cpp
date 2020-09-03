#include <netdb.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <unistd.h>
#include <json.hpp>

//#include <error_enum.h>
//#include <audio_status.hpp>

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

void print_data(json &j1) {
    if (j1.count("data")) {
        std::cout << j1["data"].dump() << std::endl;
        std::cout << find_in_json(j1["data"], "title") << std::endl;
        std::cout << find_in_json(j1["data"], "artist") << std::endl;
        std::cout << find_in_json(j1["data"], "album") << std::endl;
        std::cout << find_in_json(j1["data"], "track") << std::endl;
        std::cout << find_in_json(j1["data"], "genre") << std::endl;
        std::cout << find_in_json(j1["data"], "comment") << std::endl;
        std::cout << find_in_json(j1["data"], "date") << std::endl;

        //std::cout << find_in_json(j1["data"], "title") << std::endl;

    }
}


int main() {
    struct sockaddr_un sock_addr;
    memset(&sock_addr, 0, sizeof(sock_addr));
    sock_addr.sun_family = AF_UNIX;
    strcpy(sock_addr.sun_path, mpv_path);

    int sock_fd = socket(sock_addr.sun_family, SOCK_STREAM, 0);
    if (sock_fd == -1) {
        perror("socket");
        return -1;
    }

    if (connect(sock_fd, (struct sockaddr *)&sock_addr, sizeof(sock_addr)) == - 1) {
        perror("Error connecting");
        return -1;
    }

    const char metadata[] = "{ \"command\": [\"get_property\", \"metadata\"] }\n";
    char buf[9000] = {0};
    send(sock_fd, metadata, sizeof(metadata), 0);
    int err = recv(sock_fd, buf, sizeof(buf), 0);
    if (err < 0) {
        perror("Error receiving");
    }

    printf("%s\n", buf);

    auto j1 = json::parse(buf);

    printf("\n\n\n");
    print_data(j1);
    //printf("\ndata: %s\n", j1["data"].get<std::string>().c_str());

    shutdown(sock_fd, SHUT_RD);
    close(sock_fd);
    return 0;
}
/*
namespace melicus {
    std::tuple<error_status, audio_status> mpv_status() {
        //s = linux_socket(mpv_path);
        return {OK, {}};
    }
}*/
