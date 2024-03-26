#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/file.h>

int main(int argc, char **argv) {
    int status;

    pid_t pid = fork();

    if (pid < 0) {
        return 1;
    } else if (!pid) {
        int fd1 = open(argv[2], O_RDONLY);

        if (fd1 < 0) {
            _exit(42);
        }

        int fd2 = open(argv[3], O_WRONLY | O_CREAT | O_APPEND, 0660);

        if (fd2 < 0) {
            _exit(42);
        }

        int fd3 = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0660);

        if (fd3 < 0) {
            _exit(42);
        }
        
        if (dup2(fd1, 0) == -1) {
            _exit(42);
        }

        if (dup2(fd2, 1) == -1) {
            _exit(42);
        }
        
        if (dup2(fd3, 2) == -1) {
            _exit(42);
        }

        if (close(fd1) < 0) {
            _exit(42);
        }
        
        if (close(fd2) < 0) {
            _exit(42);
        }

        if (close(fd3) < 0) {
            _exit(42);
        }

        if (execlp(argv[1], argv[1], NULL) == -1) {
            _exit(42);
        }
    } else {
        wait(&status);
        printf("%d\n", status);
    }

    return 0;
}
