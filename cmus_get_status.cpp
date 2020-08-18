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

using namespace std::chrono;

void print_cmus_status() {
    std::string env_path = std::string(getenv("XDG_RUNTIME_DIR"));
    env_path += "/cmus-socket";
    std::cout << env_path << std::endl;

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

    while (true) {
        char status[] = "status\n";
        std::string buf;
        buf.resize(9000, 0);
        send(sock_fd, status, sizeof(status), 0);
        int err = recv(sock_fd, buf.data(), buf.size(), 0);
        if (err < 0)
            perror("Error receiving");

        std::cout << "[STATUS]" << std::endl;
        std::cout << buf << std::endl;
        break;
    }

    close(sock_fd);
}

#include <cassert>

static int handle_error(struct mpd_connection *c) {
    assert(mpd_connection_get_error(c) != MPD_ERROR_SUCCESS);

    fprintf(stderr, "%s\n", mpd_connection_get_error_message(c));
    mpd_connection_free(c);
    return EXIT_FAILURE;
}

#define ine(x) (x ? x : "")

int print_mpd_status() {
    mpd_connection *conn = mpd_connection_new(NULL, 0, 0);

    if (mpd_connection_get_error(conn) != MPD_ERROR_SUCCESS)
        return handle_error(conn);


    std::cout << mpd_connection_get_error(conn) << std::endl;

    const unsigned* tpl = mpd_connection_get_server_version(conn);
    for (int i = 0; i < 3; i++)
        std::cout << tpl[i] << " ";
    std::cout << std::endl;

    mpd_command_list_begin(conn, true);
    mpd_send_current_song(conn);
    mpd_command_list_end(conn);

    mpd_song *song = mpd_recv_song(conn);

    if (!song) {
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

    std::cout << sdata << std::endl;

    mpd_connection_free(conn);
    return 0;
}

int main() {
    print_mpd_status();

    return 0;
}
