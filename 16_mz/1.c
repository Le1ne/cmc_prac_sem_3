#include <stdio.h>
#include <stdlib.h>

enum { RADIX = 10 };

int main(int argc, char **argv) {
    int cache_size = strtol(argv[2], NULL, RADIX);
    int block_size = strtol(argv[3], NULL, RADIX);

    int blocks_amount = cache_size / block_size;
    int *blocks = calloc(blocks_amount, sizeof(*blocks));

    for (int i = 0; i < blocks_amount; ++i) {
        blocks[i] = -1;
    }

    char rw, di;
    unsigned addr;
    int size, value;

    int misses = 0;

    while (scanf("%c%c%x%u%d\n", &rw, &di, &addr, &size, &value) != EOF) {
        int block_num = addr / block_size;

        if (rw == 'W') {
            blocks[block_num % blocks_amount] = block_num;
        } else if (rw == 'R') {
            if (blocks[block_num % blocks_amount] != block_num) {
                if (blocks[block_num % blocks_amount] != - 1) {
                    misses++;
                }
            }

            blocks[block_num % blocks_amount] = block_num;
        }
    }

    printf("%d\n", misses);
    free(blocks);

    return 0;
}
