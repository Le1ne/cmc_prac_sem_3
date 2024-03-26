#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/file.h>
#include <stdint.h>

struct Node 
{
    int32_t key;
    int32_t left_idx;
    int32_t right_idx;
};

void print_tree(int fd, int32_t offset) {
    lseek(fd, offset, SEEK_SET);
    struct Node *cur = calloc(1, 12);

    if (read(fd, cur, sizeof(*cur)) != sizeof(*cur)) {
        return;
    }

    int32_t left_idx = cur->left_idx;
    int32_t right_idx = cur->right_idx;

    if (right_idx != 0) {
        print_tree(fd, right_idx * sizeof(*cur));
    }

    printf("%d\n", cur->key);
    
    if (left_idx != 0) {
        print_tree(fd, left_idx * sizeof(*cur));
    }
}

int main(int argc, char **argv) {
    int fd = open(argv[1], O_RDONLY);

    if (fd < 0) {
        return 1;
    }

    print_tree(fd, 0);

    if (close(fd) < 0) {
        return 1;
    }

    return 0;
}
