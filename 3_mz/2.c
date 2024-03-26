#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

enum { RADIX = 10 };

int main(int argc, char* argv[]) {
    long long pos_sum = 0;
    long long neg_sum = 0;

    for (int i = 1; i < argc; ++i) {
        char *end = NULL;
        errno = 0;
        long cur = strtol(argv[i], &end, RADIX);

        if (errno || *end || end == argv[i] || (int) cur != cur) {
            return 1;
        }

        if (cur >= 0) {
            pos_sum += cur;
        } else {
            neg_sum += cur;
        }
    }

    printf("%lld\n%lld\n", pos_sum, neg_sum);

    return 0;
}
