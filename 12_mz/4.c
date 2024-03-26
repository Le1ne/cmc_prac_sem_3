#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/file.h>
#include <time.h>
#include <limits.h>

int main(int argc, char **argv) {
    const char first_part_script[] = 
        "#!/usr/bin/python3\n"
        "import sys\n"
        "from os import remove\n"
        "sys.set_int_max_str_digits(10 ** 6)\n"
        "print(";
    const char third_part_script[] = 
        "1)\n"
        "remove(__file__)";
    
    char tmp_dir[] = "/tmp";
    char *e_dir = getenv("XDG_RUNTIME_DIR");

    if (!e_dir) {
        e_dir = getenv("TMPDIR");
    }

    if (!e_dir) {
        e_dir = tmp_dir;
    }

    char script_path[PATH_MAX + 1] = {0};
    snprintf(script_path, sizeof(script_path), "%s/script_%d_%ld.py", e_dir, getpid(), time(NULL));

    int fd = open(script_path, O_CREAT | O_TRUNC | O_WRONLY, 0770);

    if (fd < 0) {
        return 1;
    }
    
    if (write(fd, first_part_script, sizeof(first_part_script) - 1) != sizeof(first_part_script) - 1) {
        return 1;
    }
    
    for (int i = 1; i < argc; ++i) {
        int len = strlen(argv[i]);
        
        if (write(fd, argv[i], len) != len) {
            return 1;
        }

        if (write(fd, " * ", 3) != 3) {
            return 1;
        }
    }

    if (write(fd, third_part_script, sizeof(third_part_script) - 1) != sizeof(third_part_script) - 1) {
        return 1;
    }
    
    if (close(fd) < 0) {
        return 1;
    } 
    
    execlp(script_path, script_path, NULL);

    return 1;
}
