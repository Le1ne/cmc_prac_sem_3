#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>

enum { RADIX = 10 };

typedef struct Message 
{
    long type;
    long long value1;
    long long value2;
} Message;

int main(int argc, char **argv) {
    int key = strtol(argv[1], NULL, RADIX);
    int n = strtol(argv[2], NULL, RADIX);
    long long value1 = strtoll(argv[3], NULL, RADIX);
    long long value2 = strtoll(argv[4], NULL, RADIX);
    long long max_val = strtoll(argv[5], NULL, RADIX);

    int msqid = msgget(key, IPC_CREAT | 0666);
    pid_t *pids = calloc(n + 1, sizeof(*pids));

    Message msg;

    for (int i = 0; i < n; ++i) {
        pid_t pid = fork();
        pids[i] = pid;

        if (pid < 0) {
            msgctl(msqid, IPC_RMID, NULL);
            free(pids);

            return 1;
        } else if (!pid) {
            while (msgrcv(msqid, &msg, sizeof(msg) - sizeof(msg.type), i + 1, 0) >= 0) {
                long long value3 = msg.value1 + msg.value2;
                printf("%d %lld\n", i, value3);
                fflush(stdout);

                if (value3 > abs(max_val)) {
                    msgctl(msqid, IPC_RMID, NULL);
                    _exit(0);
                }

                msg.value1 = msg.value2;
                msg.value2 = value3;
                msg.type = value3 % n + 1;
                msgsnd(msqid, &msg, sizeof(msg) - sizeof(msg.type), 0);
            }

            _exit(0);
        }
    }

    msg.type = 1;
    msg.value1 = value1;
    msg.value2 = value2;
    msgsnd(msqid, &msg, sizeof(msg) - sizeof(msg.type), 0);

    while (wait(NULL) > 0);

    free(pids);

    return 0;
}
