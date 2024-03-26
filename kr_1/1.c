#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

enum { START_CNT = 3 };

int main(void) {
    long long total = 0;
    long long overflow_num = 0;
    long long cur;
    long long tmp;
    int not_zero = 0;
    int cnt = START_CNT;
    long long *overflow_nums = calloc(START_CNT, sizeof(overflow_num));
    int i = 0;

    while (scanf("%lld", &cur) == 1) {
        if (__builtin_add_overflow(total, cur, &tmp)) {
            if (i < cnt) {
                overflow_nums[i] = cur;
                i++;
            } else {
                overflow_nums = realloc(overflow_nums, 3 * cnt * sizeof(overflow_num));
                overflow_nums[i] = cur;
                i++;
            }
        } else {
            total += cur;
        }
    }

    do {
        not_zero = 0;

        for (int j = 0; j < i; j++) {
            if (overflow_nums[j] != 0) {
                not_zero = 1;
                    
                if (!__builtin_add_overflow(total, overflow_nums[j], &tmp)) {
                    total += overflow_nums[j];
                    overflow_nums[j] = 0;
                }
            }
        }
    } while (not_zero);

    printf("%lld\n", total);
    free(overflow_nums);

    return 0;
}
