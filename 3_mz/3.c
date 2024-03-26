#include <stdio.h>
#include <stdlib.h>
#include <math.h>

enum 
{ 
    DENUM = 100,
    PRECISION = 10000 
};

int main(int argc, char *argv[]) {
    char *end;
    double value = strtod(argv[1], &end);

    for (int i = 2; i < argc; ++i) {
        double changing = strtod(argv[i], &end);
        value = value * (1 + changing / DENUM);
        value = round(value * PRECISION) / PRECISION;
    }

    printf("%.4lf\n", value);

    return 0;
}
