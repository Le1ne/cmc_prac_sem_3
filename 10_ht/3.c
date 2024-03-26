#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/mman.h>
#include <errno.h>

enum { RADIX = 10 };

int main(int argc, char **argv) {
    int fd = open(argv[1], O_CREAT | O_TRUNC | O_RDWR, 0600);
    
    if (fd < 0) {
        return 1;
    }
    
    int rows = strtol(argv[2], NULL, RADIX);
    int cols = strtol(argv[3], NULL, RADIX);

    int cur_num = 1;
    int length = rows * cols;
    int file_size = length * sizeof(cur_num);

    if (ftruncate(fd, file_size) < 0) {
        return 1;
    }

    int *output = mmap(NULL, file_size, PROT_WRITE, MAP_SHARED, fd, 0);

    if (output == MAP_FAILED) {
        return 1;
    }

    int cur_col = 0;
    int cur_row = 0;
    int empty_row = rows;
    int empty_col = cols;

    while (cur_num <= length) {
        if (cur_num <= length) {
            while (cur_col < empty_col) {
                output[cur_row * cols + cur_col] = cur_num;
                cur_col++;
                cur_num++;
            }

            cur_col--;
            cur_row++;

            if (cur_num <= length) {
                while (cur_row < empty_row) {
                    output[cur_row * cols + cur_col] = cur_num;
                    cur_row++;
                    cur_num++;
                }
                    
                cur_row--;
                cur_col--;
                empty_row--;
            }

            if (cur_num <= length) {
                while ((cols - empty_col) <= cur_col) {
                    output[cur_row * cols + cur_col] = cur_num;
                    cur_col--;
                    cur_num++;
                }

                cur_col++;
                cur_row--;
                empty_col--;

                if (cur_num <= length) {
                    while ((rows - empty_row) <= cur_row) {
                        output[cur_row * cols + cur_col] = cur_num;
                        cur_row--;
                        cur_num++;
                    }

                    cur_row++;
                    cur_col++;
                }
            }
        }
    }

    munmap(output, file_size);

    if (close(fd) < 0) {
        return 1;
    }

    return 0;
}
