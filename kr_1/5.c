#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <errno.h>

enum { RADIX = 10 };

unsigned long long size_files(char *dir_name, int uid) {
    unsigned long long total_size = 0;
    DIR *d = opendir(dir_name);
    
    if (d == NULL) {
        return 0;
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
            total_size += size_files(path, uid);
        } else if (S_ISREG(buf.st_mode) && (buf.st_uid == uid)) {
            total_size += buf.st_size;
        }
    }

    if (closedir(d) != 0) {
        return 0;
    }

    return total_size;
}

int main(int argc, char **argv) {
    char *end = NULL;
    errno = 0;
    
    long uid = strtol(argv[2], &end, RADIX);

    if (errno || *end || end == argv[2] || (int) uid != uid) {
        return 1;
    }

    printf("%llu\n", size_files(argv[1], uid));

    return 0;
}
