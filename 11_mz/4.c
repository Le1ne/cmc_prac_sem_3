#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main(void) {
    int n;
    scanf("%d", &n);
    
    pid_t pid;

    for (int i = 1; i <= n; ++i) {
        if (i == n) {
            printf("%d\n", i);
            
            break;
        } else {
            printf("%d ", i);
            fflush(stdout);
        }

        if ((pid = fork()) > 0) {
            wait(NULL);

            break;
        } else if (pid < 0) {
            return 1;
        }
    }

    return 0;
}