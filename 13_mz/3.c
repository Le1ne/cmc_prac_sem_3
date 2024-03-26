#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/file.h>

int main(int argc, char **argv) {
    int fds[2];
    pipe(fds);

    pid_t pid = fork();

    if (pid < 0) {
        return 1;
    } else if (!pid) {
        if (close(fds[0]) < 0) {
            _exit(1);
        }

        if (dup2(fds[1], 1) < 0) {
            _exit(1);
        }

        if (close(fds[1]) < 0) {
            _exit(1);
        }

        pid = fork();

        if (pid < 0) {
            _exit(1);
        } else if (!pid) {
            int file_1 = open(argv[4], O_RDONLY);
            
            if (file_1 < 0) {
                _exit(1);
            }

            if (dup2(file_1, 0) < 0) {
                _exit(1);
            }

            if (close(file_1) < 0) {
                _exit(1);
            }

            execlp(argv[1], argv[1], NULL);
            _exit(1);
        }

        int status;
        wait(&status);

        if (!WIFEXITED(status) || WEXITSTATUS(status)) {
            _exit(0);
        }

        pid = fork();

        if (pid < 0) {
            _exit(1);
        } else if (!pid) {
            execlp(argv[2], argv[2], NULL);
            _exit(1);
        }

        wait(NULL);
        _exit(0);
    }

    pid = fork();

    if (pid < 0) {
        return 1;
    } else if (!pid) {
        int file_2 = open(argv[5], O_CREAT | O_WRONLY | O_APPEND, 0700);
        
        if (file_2 < 0) {
            _exit(1);
        }

        if (dup2(file_2, 1) < 0) {
            _exit(1);
        }

        if (close(file_2) < 0) {
            _exit(1);
        }

        if (close(fds[1]) < 0) {
            _exit(1);
        }

        if (dup2(fds[0], 0) < 0) {
            _exit(1);
        }

        if (close(fds[0]) < 0) {
            _exit(1);
        }

        execlp(argv[3], argv[3], NULL);
        _exit(1);
    }

    if (close(fds[0]) < 0) {
        return 1;
    }

    if (close(fds[1]) < 0) {
        return 1;
    }

    while (wait(NULL) > 0);

    return 0;
}
