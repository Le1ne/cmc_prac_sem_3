#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <errno.h>

enum { RADIX = 10 };

int num_mod(int a, int b) {
    int res = a % b;

    if (res < 0) {
        res += b;
    }

    return res;
}

int main(int argc, char **argv) {
    char *end = NULL;
    errno = 0;
    long count = strtol(argv[1], &end, RADIX);

    if (errno || *end || end == argv[1] || (int) count != count) {
        return 1;
    }

    setbuf(stdin, NULL);

    int semid = semget(IPC_PRIVATE, count, IPC_CREAT | 0666);
    semctl(semid, 0, SETVAL, 1);

    for (int i = 0; i < count; ++i) {
        pid_t pid = fork();

        if (pid < 0) {
            return 1;
        } else if (!pid) {
            struct sembuf sem;

            while (1) {
                sem.sem_num = i;
                sem.sem_op = -1;
                sem.sem_flg = 0;
                semop(semid, &sem, 1);

                int cur;

                if (scanf("%d", &cur) == EOF) {
                    break;
                }

                printf("%d %d\n", i, cur);
                fflush(stdout);

                sem.sem_num = num_mod(cur, count);
                sem.sem_op = 1;
                sem.sem_flg = 0;
                semop(semid, &sem, 1);
            }

            semctl(semid, 0, IPC_RMID, 0);
            _exit(0);
        }
    }

    while (wait(NULL) > 0);

    return 0;
}
