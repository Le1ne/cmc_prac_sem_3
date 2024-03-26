#include <unistd.h>
#include <sys/wait.h>

int func(char *prog) {
    int status;
    pid_t pid = fork();

    if (pid < 0) {
        return 0;
    } else if (pid == 0) {
        if (execlp(prog, prog, NULL) == -1) {
            _exit(1);
        }
    }

    waitpid(pid, &status, 0);
        
    return WIFEXITED(status) && (WEXITSTATUS(status) == 0);
}

int main(int argc, char **argv) {
    return !((func(argv[1]) || func(argv[2])) && func(argv[3]));
}

