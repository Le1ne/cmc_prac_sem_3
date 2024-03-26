#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int fd = open(argv[1], O_RDWR);

    if (fd < 0) {
        return 1;
    }

    double prev;

    if (read(fd, &prev, sizeof(prev)) < 0) {
        return 1;
    }

    int cnt = 1;
    char *end = NULL;
    long n = strtol(argv[2], &end, 10);
    double cur;

    while ((read(fd, &cur, sizeof(cur)) == sizeof(cur)) && cnt < n) {
        int size_cur = sizeof(cur);
        lseek(fd, -size_cur, SEEK_CUR);
        prev = cur - prev;
        write(fd, &prev, sizeof(prev));
        cnt++;
    }

    if (close(fd) < 0) {
        return 1;
    }

    return 0;
}
