#include <stdio.h>

int main(void) {
    unsigned cur_num = 0;
    unsigned max_align = 0;
    unsigned total_size = 0;

    while (scanf("%d", &cur_num) != EOF) {
        unsigned mask = 1;

        if (cur_num == 0) {
            continue;
        }

        while (cur_num % mask == 0) {
            mask <<= 1;
        }

        mask >>= 1;

        if (mask > max_align) {
            max_align = mask;
        }

        if (total_size % mask != 0) {
            int align = mask - total_size % mask;
            total_size += cur_num + align;
        } else {
            total_size += cur_num;
        }
    }

    if (total_size == 0) {
        printf("1 1\n");
    } else if (total_size % max_align != 0) {
        int align = max_align - total_size % max_align;
        total_size += align;
        printf("%u %u\n", total_size, max_align);
    } else {
        printf("%u %u\n", total_size, max_align);
    }

    return 0;
}
