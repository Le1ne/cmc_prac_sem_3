#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/wait.h>

enum 
{ 
    STR_AMOUNT = 3, 
    MAX_LEN = 9,
    RADIX = 10    
};

int main(void) {
    pid_t pid;
    int status;
    setbuf(stdin, 0);

    for (int i = 1; i <= STR_AMOUNT; ++i) {
        pid = fork();

        if (!pid) {
            char buf[MAX_LEN];

            if (fgets(buf, MAX_LEN, stdin) == NULL) {
                return 1;
            }

            int num = strtol(buf, NULL, RADIX);
            printf("%d %d\n", i, num * num);
            fflush(stdout);

            break;
        }
    }

    while (wait(&status) != -1);

    return 0;
}