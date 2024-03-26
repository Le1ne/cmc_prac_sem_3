#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>

enum 
{ 
    RADIX = 10,
    SHM_SIZE = 4 
};

int main(int argc, char **argv) {
    int nproc = strtol(argv[1], NULL, RADIX);
    int key = strtol(argv[2], NULL, RADIX);
    int maxval = strtol(argv[3], NULL, RADIX);

    int semid = semget(key, nproc + 1, IPC_CREAT | 0666);
    semctl(semid, 0, SETVAL, 1);

    int val;
    
    int shmid = shmget(key, sizeof(val) * SHM_SIZE, IPC_CREAT | 0666);
    int *mem = shmat(shmid, NULL, 0);

    mem[0] = 0;
    mem[1] = -1;

    struct sembuf sem_down;
    sem_down.sem_num = 0;
    sem_down.sem_op = -1;
    sem_down.sem_flg = 0;

    struct sembuf sem_up;
    sem_up.sem_num = 0;
    sem_up.sem_op = 1;
    sem_up.sem_flg = 0;

    for (int i = 0; i < nproc; ++i) {
        pid_t pid = fork();

        if (pid < 0) {
            return 1;
        } else if (!pid) {
            while (1) {
                sem_down.sem_num = i;
                semop(semid, &sem_down, 1);

                if (mem[0] > maxval) {
                    semctl(semid, 0, IPC_RMID, 0);
                    shmctl(shmid, IPC_RMID, 0);
                    _exit(0);
                }

                printf("%d %d %d\n", i + 1, mem[0], mem[1] + 1);
                fflush(stdout);

                mem[0]++;
                mem[1] = i;

                int part_num = mem[0] % nproc;
                int new_num = (((((part_num * part_num) % nproc) * part_num) % nproc) * part_num) % nproc;
                sem_up.sem_num = new_num;
                semop(semid, &sem_up, 1);
            }
        }
    }

    while (wait(NULL) > 0);

    return 0;
}
