#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>

int main(int argc, char *argv[]) {
    unsigned long long value = 0;

    for (int i = 1; i < argc; ++i) {
        struct stat buf;

        if (lstat(argv[i], &buf) < 0) {
            continue;
        }

        if (S_ISREG(buf.st_mode) && !(S_ISLNK(buf.st_mode))) {
            if (buf.st_nlink == 1 && (buf.st_size % 1024 == 0)) {
                value += buf.st_size;
            }
        }
    }

    printf("%llu\n", value);

    return 0;
}
