#include <stdio.h>

enum 
{
    MY_INT_MAX = ~0u >> 1,
    MY_INT_MIN = ~(MY_INT_MAX)
};

int satsum(int v1, int v2) {
    int sum = 0;

    if (v1 > 0) {
        if (v2 > MY_INT_MAX - v1) {
            return MY_INT_MAX;
        }
    } else if (v2 > 0) {
        if (v1 > MY_INT_MAX - v2) {
            return MY_INT_MAX;
        }
    } else if (v1 < 0) {
        if (v2 < MY_INT_MIN - v1) {
            return MY_INT_MIN;
        }
    } else if (v2 < 0) {
        if (v1 < MY_INT_MIN - v2) {
            return MY_INT_MIN;
        }
    }

    sum = v1 + v2;

    return sum;
}
