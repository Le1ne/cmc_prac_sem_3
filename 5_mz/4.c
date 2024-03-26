#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <limits.h>

int cmp(const void *a, const void *b) {
    char **cur_a = (char**)a;
    char **cur_b = (char**)b;
    
    return strcmp(*cur_a, *cur_b);
}

int main(int argc, char **argv) {
    struct stat buf;
    char **file_arr = calloc(argc, sizeof(*file_arr));

    int total_files = 0;

    for (int i = 1; i < argc; ++i) {
        if (stat(argv[i], &buf) < 0) {
            continue;
        }
   
        int check_file = 0;

        for (int j = 0; j < total_files; ++j) {
            struct stat buf_check;

            if (stat(file_arr[j], &buf_check) < 0) {
                continue;
            }

            if ((buf_check.st_dev == buf.st_dev) && (buf_check.st_ino == buf.st_ino)) {
                check_file = 1;
                
                if (strcmp(file_arr[j], argv[i]) < 0) {
                    file_arr[j] = argv[i];
                }

                break;
            }
        }

        if (!check_file) {
            file_arr[total_files] = argv[i];
            total_files++;
        }
    }

    qsort(file_arr, total_files, sizeof(*file_arr), cmp);

    for (int i = 0; i < total_files; ++i) {
        printf("%s\n", file_arr[i]);
    }
        
    return 0;
}
