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

using namespace std::chrono;

int main() {
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
        //std::this_thread::sleep_for(1s);
    }

    close(sock_fd);
    return 0;
}
