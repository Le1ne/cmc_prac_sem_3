#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/file.h>

int main(int argc, char **argv) {
    int fds[2];
    pipe(fds);

    pid_t pid_1 = fork();

    if (pid_1 < 0) {
        close(fds[0]);
        close(fds[1]);
        _exit(1);
    } else if (!pid_1) {
        close(fds[0]);
        
        if (dup2(fds[1], 1) < 0) {
            _exit(1);
        }

        close(fds[1]);
        int fd = open(argv[2], O_RDONLY);
        
        if (fd < 0) {
            _exit(1);
        }
        
        if (dup2(fd, 0) < 0) {
            _exit(1);
        }

        close(fd);
        execlp(argv[1], argv[1], NULL);
        _exit(1);
    }

    pid_t pid_2 = fork();

    if (pid_2 < 0) {
        close(fds[0]);
        close(fds[1]);
        _exit(1);
    } else if (!pid_2) {
        close(fds[1]);
        dup2(fds[0], 0);
        close(fds[0]);
        execlp(argv[3], argv[3], NULL);
        _exit(1);
    }

    close(fds[0]);
    close(fds[1]);

    while (wait(NULL) > 0);

    pid_t pid_3 = fork();

    if (pid_3 < 0) {
        _exit(1);
    } else if (!pid_3) {
        execlp(argv[4], argv[4], NULL);
        _exit(1);
    }

    wait(NULL);

    return 0;
}