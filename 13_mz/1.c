#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

int main(void) {
    struct tm res_time;
    int fds[2];
    pipe(fds);
    pid_t pid = fork();

    if (pid < 0) {
        return 1;
    } else if (!pid) {
        pid = fork();

        if (pid < 0) {
            _exit(1);
        } else if (!pid) {
            pid = fork();

            if (pid < 0) {
                return 1;
            } else if (!pid) {
                time_t cur_time = time(NULL);

                if (close(fds[0]) < 0) {
                    _exit(1);
                }

                write(fds[1], &cur_time, sizeof(cur_time));
                write(fds[1], &cur_time, sizeof(cur_time));
                write(fds[1], &cur_time, sizeof(cur_time));

                if (close(fds[1]) < 0) {
                    _exit(1);
                }
                
                _exit(0);
            }

            wait(NULL);

            if (close(fds[1]) < 0) {
                _exit(1);
            }

            time_t cur_time;
            read(fds[0], &cur_time, sizeof(cur_time));
            localtime_r(&cur_time, &res_time);
            printf("D:%.02d\n", res_time.tm_mday);
            fflush(stdout);
            
            if (close(fds[0]) < 0) {
                _exit(1);
            }

            _exit(0);
        }

        wait(NULL);

        if (close(fds[1]) < 0) {
            _exit(1);
        }

        time_t cur_time;
        read(fds[0], &cur_time, sizeof(cur_time));
        localtime_r(&cur_time, &res_time);
        printf("M:%.02d\n", res_time.tm_mon + 1);
        fflush(stdout);

        if (close(fds[0]) < 0) {
            _exit(1);
        }

        _exit(0);
    }

    wait(NULL);

    if (close(fds[1]) < 0) {
        return 1;
    } 

    time_t cur_time;
    read(fds[0], &cur_time, sizeof(cur_time));
    localtime_r(&cur_time, &res_time);
    printf("Y:%d\n", res_time.tm_year + 1900);
    fflush(stdout);

    if (close(fds[0]) < 0) {
        _exit(1);
    }

    return 0;
}
