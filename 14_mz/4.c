#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>

volatile sig_atomic_t flag;

enum 
{
    RADIX = 10,
    FIRST_PID = 1,
    SECOND_PID = 2
};

void handler(int signal) {
    flag = 1;
}

int main(int argc, char **argv) {
    char *end = NULL;
    errno = 0;
    long n = strtol(argv[1], &end, RADIX);

    if (errno || *end || end == argv[1] || (int) n != n) {
        return 1;
    }

    int fds[2];
    pipe(fds);

    FILE *fd_in = fdopen(fds[0], "r");
    FILE *fd_out = fdopen(fds[1], "w");

    sigset_t sig_1;
    sigemptyset(&sig_1);
    sigaddset(&sig_1, SIGUSR1);
    sigprocmask(SIG_BLOCK, &sig_1, NULL);

    sigaction(SIGUSR1, &(struct sigaction) {.sa_handler = handler, .sa_flags = SA_RESTART}, NULL);

    pid_t pid_1 = fork();

    if (pid_1 < 0) {
        fclose(fd_in);
        fclose(fd_out);

        return 1;
    } else if (!pid_1) {
        sigset_t sig_2;
        sigemptyset(&sig_2);

        pid_t other_pid;
        fscanf(fd_in, "%d", &other_pid);

        while (1) {
            while (!flag) {
                sigsuspend(&sig_2);
            }

            flag = 0;
            int cur;

            fscanf(fd_in, "%d", &cur);

            if (cur < n) {
                printf("%d %d\n", FIRST_PID, cur);
                fflush(stdout);
            }

            cur++;

            fprintf(fd_out, "%d\n", cur);
            fflush(fd_out);
            kill(other_pid, SIGUSR1);

            if (cur >= n) {
                fclose(fd_in);
                fclose(fd_out);
                _exit(0);
            }
        }
    }

    pid_t pid_2 = fork();

    if (pid_2 < 0) {
        return 1;
    } else if (!pid_2) {
        sigset_t sig_2;
        sigemptyset(&sig_2);

        while (1) {
            while (!flag) {
                sigsuspend(&sig_2);
            }

            flag = 0;
            int cur;

            fscanf(fd_in, "%d", &cur);

            if (cur < n) {
                printf("%d %d\n", SECOND_PID, cur);
                cur++;
                fflush(stdout);
            }

            fprintf(fd_out, "%d\n", cur);
            fflush(fd_out);
            kill(pid_1, SIGUSR1);

            if (cur >= n) {
                fclose(fd_in);
                fclose(fd_out);
                _exit(0);
            }
        }
    }

    fprintf(fd_out, "%d\n", pid_2);
    fprintf(fd_out, "%d\n", 1);
    kill(pid_1, SIGUSR1);
    
    fclose(fd_in);
    fclose(fd_out);
    
    while (wait(NULL) > 0);

    printf("Done\n");
    fflush(stdout);

    return 0;    
}
