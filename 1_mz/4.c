#include <stdio.h>
#include <stdlib.h>

int cmp(const void* a, const void* b) {
    int a_1 = *(int*)a;
    int b_1 = *(int*)b;

    if ((a_1 & 1) == 1 && (b_1 & 1) == 0) {
        return 1;
    } else if ((a_1 & 1) == 0 && (b_1 & 1) == 1) {
        return -1;
    } else {
        if (a_1 > b_1) {
            if ((a_1 & 1) == 0) {
                return 1;
            } else {
                return -1;
            }
        } else if (a_1 < b_1) {
            if ((a_1 & 1) == 0) {
                return -1;
            } else {
                return 1;
            }
        }
    }

    return 0;
}

void sort_even_odd(size_t count, int *data) {
    if (count <= 0) {
        return;
    }

    qsort(data, count, sizeof(*data), cmp);
}
