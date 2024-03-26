#include <stdio.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/wait.h>
#include <signal.h>
#include <limits.h>

enum { MASK = 0x80 };

int cur_sig;
pid_t pid, pid1, pid2;

void handler(int signal) {
    cur_sig = signal;

    if (pid2 != 0) {
        kill(pid2, SIGALRM);
    }
}

int main(int argc, char **argv) {
    struct sigaction main_sig;
    main_sig.sa_flags = SA_RESTART;
    main_sig.sa_handler = handler;
    sigaction(SIGUSR1, &main_sig, NULL);
    sigaction(SIGUSR2, &main_sig, NULL);
    sigaction(SIGIO, &main_sig, NULL);
    sigaction(SIGALRM, &main_sig, NULL);
    
    sigset_t sig_1, sig_2;
    
    pid = getpid();
    pid1 = fork();
    
    if (pid1 < 0) {
        return 1;
    } else if (!pid1) {
        sigemptyset(&sig_1);
        sigaddset(&sig_1, SIGUSR1);
        sigaddset(&sig_1, SIGUSR2);
        sigaddset(&sig_1, SIGALRM);
        sigprocmask(SIG_BLOCK, &sig_1, &sig_2);

        int cnt_bits = 0;
        char buf = 0;

        while (1) {
            sigsuspend(&sig_2);

            if (cur_sig == SIGUSR1) {
                buf <<= 1;
                cnt_bits++;
            } else if (cur_sig == SIGUSR2) {
                buf = (buf << 1) | 1;
                cnt_bits++;
            } else if (cur_sig == SIGIO) {
                _exit(0);
            }

            if (cnt_bits == 8) {
                putchar(buf);
                fflush(stdout);
                cnt_bits = 0;
                buf = 0;
            }
        
            kill(pid, SIGALRM);
        }
    }

    pid2 = fork();

    if (pid2 < 0) {
        return 1;
    } else if (!pid2) {
        sigemptyset(&sig_1);
        sigaddset(&sig_1, SIGALRM);
        sigprocmask(SIG_BLOCK, &sig_1, &sig_2);
        
        int fd = open(argv[1], O_RDONLY);

        if (fd < 0) {
            return 1;
        }
        
        char buf;

        while (read(fd, &buf, sizeof(buf)) == sizeof(buf)) {
            for (int i = 0; i < CHAR_BIT; ++i) {
                if ((buf & MASK) != 0) {
                    kill(pid1, SIGUSR2);
                } else {
                    kill(pid1, SIGUSR1);
                }

                buf <<= 1;
                sigsuspend(&sig_2);
                cur_sig = 0;
            }
        }

        if (close(fd) < 0) {
            return 1;
        }

        kill(pid1, SIGIO);
        _exit(0);
    }

    while (wait(NULL) > 0);

    return 0;
}
