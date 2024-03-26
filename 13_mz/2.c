#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
    int fds[2];
    pipe(fds);

    pid_t pid = fork();

    if (pid < 0) {
        return 1;
    } else if (!pid) {
        pid = fork();

        if (pid < 0) {
            return 1;
        } else if (!pid) {
            if (close(fds[1]) < 0) {
                _exit(1);
            }

            long long res = 0;
            int cur;

            while (read(fds[0], &cur, sizeof(cur)) == sizeof(cur)) {
                res += cur;
            }

            printf("%lld\n", res);
            fflush(stdout);
            _exit(0);
        }

        if (close(fds[0]) < 0) {
            _exit(1);
        }

        if (close(fds[1]) < 0) {
            _exit(1);
        }

        wait(NULL);
        _exit(0);
    } else {
        if (close(fds[0]) < 0) {
            return 1;
        }

        int num;

        while (scanf("%d", &num) == 1) {
            write(fds[1], &num, sizeof(num));
        }

        if (close(fds[1]) < 0) {
            return 1;
        }
    }

    wait(NULL);

    return 0;
}
