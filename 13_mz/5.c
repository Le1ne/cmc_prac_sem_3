#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

enum 
{ 
    RADIX = 10,
    FIRST_PID = 1,
    SECOND_PID = 2 
};

int main(int argc, char **argv) {
    char *end = NULL;
    errno = 0;
    long n = strtol(argv[1], &end, RADIX);

    if (errno || *end || end == argv[1] || (int) n != n) {
        return 1;
    }

    int fds_1[2];
    pipe(fds_1);
    int fds_2[2];
    pipe(fds_2);

    FILE *fd_write = fdopen(fds_1[1], "w");
    fprintf(fd_write, "1\n");
    fflush(fd_write);

    pid_t pid_1 = fork();

    if (pid_1 < 0) {
        return 1;
    } else if (!pid_1) {
        if (close(fds_1[1]) < 0) {
            _exit(1);
        }

        if (close(fds_2[0]) < 0) {
            _exit(1);
        }

        FILE* fd_in = fdopen(fds_1[0], "r");
        FILE* fd_out = fdopen(fds_2[1], "w");
        int cnt;

        while (fscanf(fd_in, "%d", &cnt) == 1) {
            if (cnt < n) {
                printf("%d %d\n", FIRST_PID, cnt);
                fflush(stdout);
                fprintf(fd_out, "%d ", cnt + 1);
                fflush(fd_out);
            } else {
                fclose(fd_in);
                fclose(fd_out);
                _exit(0);
            }
        }

        fclose(fd_in);
        fclose(fd_out);
        _exit(0);
    }

    pid_t pid_2 = fork();

    if (pid_2 < 0) {
        return 1;
    } else if (!pid_2) {
        if (close(fds_1[0]) < 0) {
            _exit(1);
        }

        if (close(fds_2[1]) < 0) {
            _exit(1);
        }

        FILE* fd_in = fdopen(fds_2[0], "r");
        FILE* fd_out = fdopen(fds_1[1], "w");
        int cnt;

        while (fscanf(fd_in, "%d", &cnt) == 1) {
            if (cnt < n) {
                printf("%d %d\n", SECOND_PID, cnt);
                fflush(stdout);
                fprintf(fd_out, "%d ", cnt + 1);
                fflush(fd_out);
            } else {
                fclose(fd_in);
                fclose(fd_out);
                _exit(0);
            }
        }

        fclose(fd_in);
        fclose(fd_out);
        _exit(0);
    }

    if (close(fds_1[0]) < 0) {
        return 1;
    }

    if (close(fds_1[1]) < 0) {
        return 1;
    }

    if (close(fds_2[0]) < 0) {
        return 1;
    }

    if (close(fds_2[1]) < 0) {
        return 1;
    }

    while (wait(NULL) > 0);
    
    printf("Done\n");
    fflush(stdout);

    return 0;
}
