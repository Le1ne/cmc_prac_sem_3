#include <signal.h>
#include <stdio.h>
#include <unistd.h>

volatile int cnt = 0;
void(*prev)(int);

void handler(int sig) {
    signal(SIGHUP, handler);

    if (cnt < 5) {
        printf("%d\n", cnt);
        fflush(stdout);
        cnt++;
    } else {
        _exit(0);
    }
}

int main(void) {
    prev = signal(SIGHUP, handler);
    printf("%d\n", getpid());
    fflush(stdout);

    while (1) {
        pause();
    }

    return 0;
}
