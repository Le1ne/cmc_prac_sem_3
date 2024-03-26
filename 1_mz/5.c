#include <stdio.h>

enum 
{
    SIZE = 10,
    SHIFT = 10, 
    LOWER_BORDER = 1,
    UPPER_BORDER = 9
};

int check_num(int cur_num, int i) {
    int remainders[SIZE];
    
    for (int j = 0; j < SIZE; ++j) {
        remainders[j] = 0;
    } 

    while (cur_num) {
        int rem = cur_num % SHIFT;
        remainders[rem] = 1;
        cur_num /= SHIFT;
    }

    if (remainders[i] == 0) {
        return 1;
    } else {
        return 0;
    }
}

void set_the_number(int max_size, int cur_size, int cur_num) {
    if (cur_size == max_size) {
        printf("%d\n", cur_num);
        
        return;
    }

    for (int i = 1; i < max_size + 1; ++i) {
        if (check_num(cur_num, i) == 1) {
            set_the_number(max_size, cur_size + 1, cur_num * SHIFT + i);
        }
    }
}

int main(void) {
    int n;
    
    if (scanf("%d", &n) != 1) {
        return 1;
    }

    if (n < LOWER_BORDER || n > UPPER_BORDER) {
        return 1;
    }

    for (int i = 1; i < n + 1; ++i) {
        set_the_number(n, 1, i);
    }

    return 0;
}
