#include <stdio.h>
#include <stdlib.h>

enum 
{ 
    FIRST_NUM = 1,
    LOWER_BORDER = 1,
    UPPER_BORDER = 2000
};

int main(void) {
    int n;
    
    if (scanf("%d", &n) != 1) {
        return 1;
    }

    if (n <= LOWER_BORDER || n >= UPPER_BORDER) {
        return 1;
    }

    int *dividers = calloc(n, sizeof(*dividers));

    if (dividers == NULL) {
        return 1;
    } 

    for (int i = 1; i < n; ++i) {
        for (int j = 1; j < n; ++j) {
            if ((i * j) % n == FIRST_NUM) {
                dividers[i] = j;
                break;
            }
        }
    }    

    for (int i = 0; i < n; ++i) {
        for (int j = 1; j < n; ++j) {
            printf("%d ", (dividers[j] * i) % n);
        }
        
        putchar('\n');
    }

    free(dividers);

    return 0;
}
