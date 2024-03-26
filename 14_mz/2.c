#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

volatile int mode = 1;

void handler(int sig) {
    if (sig == SIGINT) {
        mode = 1;
    }

    if (sig == SIGQUIT) {
        mode = 2;
    }
}

int main(void) {
    sigaction(SIGINT, &(struct sigaction) {.sa_handler = handler, .sa_flags = SA_RESTART}, NULL);
    sigaction(SIGQUIT, &(struct sigaction) {.sa_handler = handler, .sa_flags = SA_RESTART}, NULL);
    
    printf("%d\n", getpid());
    fflush(stdout);
    int cur = 0;
    int res = 0;

    while (scanf("%d", &cur) == 1) {
        if (mode == 1) {
            __builtin_sadd_overflow(res, cur, &res);
        } else if (mode == 2) {
            __builtin_smul_overflow(res, cur, &res);
        }

        printf("%d\n", res);
        fflush(stdout);
    }

    return 0;
}
