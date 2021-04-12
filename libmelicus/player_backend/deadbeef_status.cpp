#include <stdio.h>     
#include <stdlib.h>    
#include <string.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/socket.h>

#include <tuple>       
#include <string>      
#include <iostream>    

#include <error_enum.h>
#include <audio_status.hpp>

int try_connect_deadbeef(const char *path, size_t path_size) { 

    struct sockaddr_un sock_addr;
    memset(&sock_addr, 0, sizeof(sock_addr));
    sock_addr.sun_family = AF_UNIX;
    memcpy(sock_addr.sun_path, path, path_size);

    int sockfd = socket(sock_addr.sun_family, SOCK_STREAM, 0);
    if (sockfd == -1) { 
        perror("socket");
        return -1; // TODO(aram): maybe SETUP_ERR code?
    }

    if (connect(sockfd, 
                (struct sockaddr *)&sock_addr, sizeof(sock_addr)) == -1) { 
        perror("Connect error");
        return -1;
    }

    return sockfd;
}

std::string deadbeef_nowplaying(std::string fmt) { 
    // First we try connecting to the default socket path
    // (The one you would get if you compile deadbeef yourself)
    std::string socket_dir = std::string(getenv("XDG_RUNTIME_DIR")) + 
                             "/deadbeef/socket";
    int sockfd = try_connect_deadbeef(socket_dir.c_str(), socket_dir.size());
    if (sockfd != -1)    
        goto now_playing;

    // Use the snap path :/
    socket_dir = std::string(getenv("HOME")) +
                 "/snap/deadbeef-vs/5/.config/deadbeef/socket";
    std::cout << "Socket dir " << socket_dir;
    sockfd = try_connect_deadbeef(socket_dir.c_str(), socket_dir.size());
    if (sockfd != -1)
        goto now_playing;

    return "";

now_playing:

    std::string null_term(1, '\0');
    std::string deadbeef_cmd = "--nowplaying" + null_term + fmt;
    send(sockfd, deadbeef_cmd.data(), deadbeef_cmd.size(), 0);
    shutdown(sockfd, SHUT_WR);
    
    char buf[4096];

    int err = recv(sockfd, buf, sizeof(buf), 0);

    if (err < 0)
        perror("Error getting data");
   
    shutdown(sockfd, SHUT_RD);
    close(sockfd);
    
    return std::string(buf);
}

namespace melicus { 
    status deadbeef_status() { 
        audio_status song_info;
        song_info.title = deadbeef_nowplaying("%t");
        song_info.album = deadbeef_nowplaying("%b");
        song_info.artist = deadbeef_nowplaying("%a");
        song_info.date = deadbeef_nowplaying("%y");
        song_info.genre = deadbeef_nowplaying("%g");
        song_info.comment = deadbeef_nowplaying("%c");
        song_info.file_name = deadbeef_nowplaying("%F");
        song_info.track = deadbeef_nowplaying("%t");
        
        std::cout << deadbeef_nowplaying("%l") << " " << deadbeef_nowplaying("%e") << std::endl;
        //song_info.duration = std::stoi(deadbeef_nowplaying("%l"));
        //song_info.elapsed_time = std::stoi(deadbeef_nowplaying("%e"));

        return std::make_tuple(error_status::OK, song_info);
    }
}
