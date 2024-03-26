#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/file.h>
#include <limits.h>

int main(int argc, char **argv) {
    int fd = open(argv[1], O_RDWR);

    if (fd < 0) {
        return 1;
    }

    long long cur;

    if (read(fd, &cur, sizeof(cur)) != sizeof(cur)) {
        return 0;
    }

    long long min = cur;
    int pos = 0;

    while (read(fd, &cur, sizeof(cur)) == sizeof(cur)) {
        if (cur < min) {
            min = cur;
            pos = lseek(fd, 0, SEEK_CUR) - sizeof(cur);
        }
    }

    if (min > LLONG_MIN) {
        lseek(fd, pos, SEEK_SET);
        min = -min;
        write(fd, &min, sizeof(min));
    }

    if (close(fd) < 0) {
        return 1;
    }

    return 0;
}
