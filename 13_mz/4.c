#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void kill_proc(pid_t *pids, int cur) {
    int status;
    
    for (int i = 1; i < cur; ++i) {
        kill(pids[i], SIGKILL);
        waitpid(pids[i], &status, 0);
    }
}

int main(int argc, char **argv) {
    pid_t *pids = calloc(argc, sizeof(*pids));

    if (fclose(stderr) < 0) {
        return 1;
    }

    int fd_in = 0;

    for (int i = 1; i < argc; ++i) {
        int fds[2];
        
        if (pipe(fds) < 0) {
            kill_proc(pids, i);
            free(pids);
            _exit(1);
        }

        pids[i] = fork();

        if (pids[i] < 0) {
            kill_proc(pids, i);
            close(fds[0]);
            close(fds[1]);
            free(pids);
            _exit(1);
        } else if (!pids[i]) {
            close(fds[0]);

            if (i == 1 && argc != 2) {
                dup2(fds[1], 1);
            } else if (i == (argc - 1)) {
                if (fd_in != 0) {
                    dup2(fd_in, 0);
                }
            } else {
                dup2(fds[1], 1);
                
                if (fd_in != 0) {
                    dup2(fd_in, 0);
                }
            }

            if (fd_in != 0) {
                close(fd_in);
            }

            execlp(argv[i], argv[i], NULL);
            _exit(1);
        } else {
            close(fds[1]);
            
            if (fd_in != 0) {
                close(fd_in);
            }

            fd_in = fds[0];
        }
    }

    close(fd_in);
    free(pids);

    while (wait(NULL) > 0);

    return 0;
}
