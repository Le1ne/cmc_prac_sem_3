#include <stdio.h>
#include <stdlib.h>

enum 
{
    CMD_1 = 1,
    CMD_2 = 2,
    LOWER_BORDER = 1,
    UPPER_BORDER_N = 10000,
    UPPER_BORDER_M = 50000
};

int main(void) {
    int n, m;
    scanf("%d%d", &n, &m);

    if (n < LOWER_BORDER || n > UPPER_BORDER_N) {
        return 1;
    } else if (m < LOWER_BORDER || m > UPPER_BORDER_M) {
        return 1;
    }

    int *snow = calloc(n, sizeof(*snow));

    if (snow == NULL) {
        return 1;
    }

    int cmd, left, right, s;

    for (int i = 0; i < m; ++i) {
        scanf("%d", &cmd);
        
        if (cmd == CMD_1) {
            scanf("%d %d %d", &left, &right, &s);

            for (int j = left; j < right; ++j) {
                snow[j] += s;

                if (snow[j] < 0) {
                    snow[j] = 0;
                }
            }
        } else if (cmd == CMD_2) {
            scanf("%d %d", &left, &right);
            
            int res = 0;

            for (int j = left; j < right; ++j) {
                res += snow[j];
            }

            printf("%d\n", res);
        }
    }

    free(snow);

    return 0;
}
