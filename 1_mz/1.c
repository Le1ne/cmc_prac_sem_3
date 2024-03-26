#include <stdio.h>

int main(void) {
    double x, y;

    if (scanf("%lf%lf", &x, &y) != 2) {
        return 1;
    }

    const double LEFT_BORDER = 2;
    const double RIGHT_BORDER = 5;
    const double LOWER_BORDER = 1;
    const double UPPER_BORDER = 7;
    const double RIGHT_ANGLE_OFFSET = -2;

    int res = (y >= x + RIGHT_ANGLE_OFFSET) 
                && y >= LOWER_BORDER && y <= UPPER_BORDER && x >= LEFT_BORDER && x <= RIGHT_BORDER;

    printf("%d\n", res);

    return 0;
}
