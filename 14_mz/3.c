#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

enum { RADIX = 10 };

int main(int argc, char **argv) {
    char *end = NULL;
    errno = 0;
    long n = strtol(argv[1], &end, RADIX);

    if (errno || *end || end == argv[1] || (int)n != n) {
        return 1;
    }

    int status;
    int total_proc = 0;

    for (int i = 0; i < n; ++i) {
        pid_t pid = fork();

        if (pid < 0) {
            return 1;
        } else if (!pid) {
            FILE *fd = fopen(argv[i + 2], "r");
            char buf[PATH_MAX + 1] = {0};
            fgets(buf, sizeof(buf), fd);
            buf[strlen(buf) - 1] = '\0';
            fclose(fd);
            execlp(buf, buf, NULL);
            _exit(1);
        }
    }

    while (wait(&status) > 0) {
        if (WIFEXITED(status) && (WEXITSTATUS(status) == 0)) {
            total_proc++;
        }
    }

    for (int i = n + 2; i < argc; ++i) {
        pid_t pid = fork();

        if (pid < 0) {
            return 1;
        } else if (!pid) {
            FILE *fd = fopen(argv[i], "r");
            char buf[PATH_MAX + 1] = {0};
            fgets(buf, sizeof(buf), fd);
            buf[strlen(buf) - 1] = '\0';
            fclose(fd);
            execlp(buf, buf, NULL);
            _exit(1);
        } else {
            while (wait(&status) > 0) {
                if (WIFEXITED(status) && (WEXITSTATUS(status) == 0)) {
                    total_proc++;
                }
            }
        }
    }

    printf("%d\n", total_proc);
    fflush(stdout);

    return 0;
}
