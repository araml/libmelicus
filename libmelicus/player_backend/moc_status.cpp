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

#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sys/types.h>
#include <vector>
#include <string>
#include <iostream>

static bool get_line(int fd, std::string *s) {
    while (true) {
        char c;
        int status = read(fd, &c, 1);

        if (status == 0 || status == -1)
            return false;

        if (c == '\n')
            return true;
        else
            s->push_back(c);

    }
}

std::vector<std::string> get_moc_status() {
    int pipe_fd[2]; // Read from 0 write to 1
    int pipe_err[2]; // Pipe to read stderr
    pipe(pipe_fd);
    pipe(pipe_err);

    int pid = fork();

    if (pid == 0) {
        close(pipe_fd[0]);
        close(pipe_err[0]);
        dup2(pipe_fd[1], STDOUT_FILENO);
        dup2(pipe_err[1], STDERR_FILENO);
        char bin[] = "/usr/bin/mocp";
        char *args[3] = {"mocp", "--info", NULL};
        execvp(bin, args);
        return {};
    } else {
        close(pipe_fd[1]);
        close(pipe_err[1]);

        std::vector<std::string> info;
        while (true) {
            std::string line;
            if (!get_line(pipe_fd[0], &line))
                break;
            info.emplace_back(line);
        }

        waitpid(pid, NULL, 0);
        close(pipe_fd[0]);
        close(pipe_err[0]);
        usleep(10000);
        return info;
    }
}

int main() {
    auto v = get_moc_status();

    for (auto &vv : v) {
        std::cout << vv << std::endl;
    }
}
