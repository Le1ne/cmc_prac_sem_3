#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>

enum { RADIX = 10 };

void print_files(char *dir_name, int max_size, int depth, int len) {
    if (depth > 4) {
        return;
    }

    DIR *d = opendir(dir_name);
    
    if (d == NULL) {
        return;
    }

    struct dirent *dd = NULL;

    while ((dd = readdir(d)) != NULL) {
        if ((strcmp(dd->d_name, ".") == 0) || (strcmp(dd->d_name, "..") == 0)) {
            continue;
        }
        
        char path[PATH_MAX + 1];
        snprintf(path, PATH_MAX + 1, "%s/%s", dir_name, dd->d_name);
        
        struct stat buf;

        if (lstat(path, &buf) < 0) {
            continue;
        }

        if (S_ISDIR(buf.st_mode)) {
            print_files(path, max_size, depth + 1, len);
        } else if (S_ISREG(buf.st_mode) && (buf.st_size <= max_size) && (!access(path, R_OK))) {
            printf("%s\n", path + len);
        }
    }

    if (closedir(d) != 0) {
        return;
    }
}

int main(int argc, char **argv) {
    char *end = NULL;
    errno = 0;
    long max_size = strtol(argv[2], &end, RADIX);

    if (errno || *end || end == argv[2] || (int) max_size != max_size) {
        return 1;
    }

    int len = strlen(argv[1]) + 1;
    print_files(argv[1], max_size, 1, len);

    return 0;
}
