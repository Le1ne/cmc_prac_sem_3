#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

enum { BYTE_SIZE = 8 };

int main(int argc, char **argv) {
    if (argc < 2) {
        return 0;
    }
    
    int x;
    char buf;

    int fd = open(argv[1], O_RDWR);

    if (fd < 0) {
        return 0;
    }

    int file_size = lseek(fd, 0, SEEK_END) * BYTE_SIZE;

    while (scanf("%d", &x) == 1) {
        if (x > 0) {
            if (x <= file_size) {
                x--;
                
                lseek(fd, x / BYTE_SIZE, SEEK_SET); 
                read(fd, &buf, sizeof(buf));
                buf |= (1u << x % BYTE_SIZE);
                lseek(fd, x / BYTE_SIZE, SEEK_SET);
                write(fd, &buf, sizeof(buf));
            }
        } else if (x < 0) {
            if (x >= -file_size) {
                x = -x - 1;
                
                lseek(fd, x / BYTE_SIZE, SEEK_SET); 
                read(fd, &buf, sizeof(buf));
                buf &= ~(1u << x % BYTE_SIZE);
                lseek(fd, x / BYTE_SIZE, SEEK_SET);
                write(fd, &buf, sizeof(buf));
            }
        }
    }

    if (close(fd) < 0) {
        return 1;
    }

    return 0;
}
