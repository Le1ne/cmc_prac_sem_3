#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char **argv) {
    int res = 0;
    int status;

    for (int i = 1; i < argc; ++i) {
        if (argv[i][0] == 'p') {
            pid_t pid_1 = fork();

            if (pid_1 < 0) {
                _exit(1);
            } else if (!pid_1) {
                if (execlp(argv[i] + 1, argv[i] + 1, NULL) == -1) {
                    _exit(1);
                }
            }

            continue;
        } else if (argv[i][0] == 's') {
            while(wait(&status) > 0) {
                if (WIFEXITED(status) && !WEXITSTATUS(status)) {
                    res++;
                }
            }
            
            pid_t pid_2 = fork();

            if (pid_2 < 0) {
                _exit(1);
            } else if (!pid_2) {
                execlp(argv[i] + 1, argv[i] + 1, NULL);
                _exit(1);
            }
        }
    }

    while(wait(&status) > 0) {
        if (WIFEXITED(status) && !WEXITSTATUS(status)) {
            res++;
        }
    }

    printf("%d\n", res);
}
