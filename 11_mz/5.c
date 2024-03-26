#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main(void) {
    pid_t pid;
    pid_t start_pid = getpid();
    setbuf(stdin, 0);
    int status;

    while (1) {
        int cur;

        if (scanf("%d", &cur) != 1) {
            _exit(0);
        }

        pid = fork();

        if (pid == -1) {
            _exit(1);
        } 
        
        if (pid) {
            wait(&status);

            if (!WIFEXITED(status) || WEXITSTATUS(status)) {
                if (getpid() == start_pid) {
                    printf("-1\n");
                    fflush(stdout);
                
                    _exit(0);
                } else {
                    _exit(1);
                }
            } else {
                printf("%d\n", cur);
                fflush(stdout);

                _exit(0);
            }
        }
    }

    return 0;
}
