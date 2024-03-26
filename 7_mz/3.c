#include <dirent.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <sys/types.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv) {
    DIR *first_dir = opendir(argv[1]);
    
    if (first_dir < 0) {
        return 1;
    }
    
    DIR *second_dir = opendir(argv[2]);

    if (second_dir < 0) {
        return 1;
    }

    struct dirent *first_dd;
    struct dirent *second_dd;
    char first_name[PATH_MAX + 1];
    char second_name[PATH_MAX + 1];
    unsigned long long total_size = 0;

    while ((first_dd = readdir(first_dir)) != NULL) {
        snprintf(first_name, PATH_MAX, "%s/%s", argv[1], first_dd->d_name);
        struct stat first_buf;

        if (lstat(first_name, &first_buf) < 0) {
            return 1;
        }

        if (S_ISREG(first_buf.st_mode) && !S_ISLNK(first_buf.st_mode) && (access(first_name, W_OK) != -1)) {
            while ((second_dd = readdir(second_dir)) != NULL) {
                snprintf(second_name, PATH_MAX, "%s/%s", argv[2], second_dd->d_name);
                struct stat second_buf;

                if (stat(second_name, &second_buf) < 0) {
                    return 1;
                }

                if ((strcmp(first_dd->d_name, second_dd->d_name) == 0) && (first_buf.st_dev == second_buf.st_dev) && 
                        (first_buf.st_ino == second_buf.st_ino)) {
                    total_size += first_buf.st_size;
                }
            }
            
            seekdir(second_dir, 0);
        }
    }

    printf("%llu\n", total_size);

    return 0;
}
