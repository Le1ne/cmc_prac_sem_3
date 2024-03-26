#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <math.h>

volatile int mode = 0;

int find_prime(int ceil, int last_prime_num) {
    for (int i = 2; i < sqrt(ceil); ++i) {
        if (ceil % i == 0) {
            return last_prime_num;
        }
    }

    return ceil;
}

void handler(int sig) {
    if (sig == SIGINT) {
        mode = 1;
    }
    
    if (sig == SIGTERM) {
        _exit(0);
    }
}

int main(void) {
    sigaction(SIGINT, &(struct sigaction) {.sa_handler = handler, .sa_flags = SA_RESTART}, NULL);
    sigaction(SIGTERM, &(struct sigaction) {.sa_handler = handler, .sa_flags = SA_RESTART}, NULL);

    printf("%d\n", getpid());
    fflush(stdout);

    int low, high;
    scanf("%d%d", &low, &high);
    int last_prime_num = 0;
    int cnt = 0;

    for (int i = low; i < high; ++i) {
        if (mode) {
            cnt++;
            
            if (cnt < 4) {
                printf("%d\n", last_prime_num);
                fflush(stdout);
                mode = 0;
            } else {
                fflush(stdout);
                
                return 0;
            }
        }

        last_prime_num = find_prime(i, last_prime_num);
    }

    signal(SIGINT, SIG_IGN);
    printf("-1\n");
    fflush(stdout);

    return 0;
}
