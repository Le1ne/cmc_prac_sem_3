#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/file.h>
#include <time.h>
#include <limits.h>
#include <errno.h>

int main(int argc, char **argv) {
    const char first_part_prog[] = 
        "#include <stdio.h>\n"
        "#include <stdlib.h>\n"
        "enum { MAX_LEN = 15 };\n"
        "int main(void) {\n"
        "const char reject[] = \"reject\";\n"
        "const char disqualify[] = \"disqualify\";\n"
        "const char summon[] = \"summon\";\n"
        "char s_num[MAX_LEN] = {0};\n"
        "while (scanf(\"%s\", s_num) == 1) {\n"
        "int x = strtol(s_num, NULL, 10);\n"
        "printf(\"%s\\n\", ";
    const char third_part_prog[] =
        ");\n"
        "}\n";
    const char forth_part_prog[] =
        "return 0;\n"
        "}\n";

    char tmp_dir[] = "/tmp";
    char *e_dir = getenv("XDG_RUNTIME_DIR");

    if (!e_dir) {
        e_dir = getenv("TMPDIR");
    }

    if (!e_dir) {
        e_dir = tmp_dir;
    }

    char file_c[PATH_MAX + 1] = {0};
    snprintf(file_c, sizeof(file_c), "%s/c_file_%d_%ld.c", e_dir, getpid(), time(NULL));

    char file_o[PATH_MAX + 1] = {0};
    snprintf(file_o, sizeof(file_o), "%s/prog_%d_%ld.o", e_dir, getpid(), time(NULL));

    int fd = open(file_c, O_CREAT | O_TRUNC | O_WRONLY, 0644);

    if (fd < 0) {
        fprintf(stderr, "'open' error\n");
        
        return 1;
    }

    FILE* fd_up = fdopen(fd, "w");
    
    if (fprintf(fd_up, "%s", first_part_prog) < 0) {
        fprintf(stderr, "'fprintf' error\n");

        return 1;
    }

    if (fprintf(fd_up, "%s", argv[1]) < 0) {
        fprintf(stderr, "'fprintf' error\n");

        return 1;
    }

    if (fprintf(fd_up, "%s", third_part_prog) < 0) {
        fprintf(stderr, "'fprintf' error\n");

        return 1;
    }

    if (fprintf(fd_up, "remove(\"%s\");\nremove(\"%s\");\n", file_c, file_o) < 0) {
        fprintf(stderr, "'fprintf' error\n");

        return 1;
    }

    if (fprintf(fd_up, "%s", forth_part_prog) < 0) {
        fprintf(stderr, "'fprintf' error\n");

        return 1;
    }

    fclose(fd_up);

    pid_t pid = fork();

    if (pid < 0) {
        fprintf(stderr, "'fork' error\n");

        return 1;
    } else if (!pid) {
        execlp("gcc", "gcc", file_c, "-o", file_o, NULL);
        fprintf(stderr, "'execlp' error\n");
        _exit(1);
    } else {
        int status;
        waitpid(pid, &status, 0);

        if (!WIFEXITED(status) || WEXITSTATUS(status)) {
            return 1;
        }

        execlp(file_o, file_o, NULL);
        fprintf(stderr, "'execlp' error\n");
        _exit(1);
    }

    remove(file_c);
    remove(file_o);

    return 0;
}
