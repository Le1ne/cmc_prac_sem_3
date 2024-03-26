#include <stdio.h>
#include <string.h>

enum 
{
    BIT_3_SHIFT = 3, 
    BIT_2_SHIFT = 2,
    MAX_NUM = 63, 
    NUM_OF_EXTRA_SYM = 2,
    NUM_SHIFT = '0' - 1, 
    LOWER_CASE_SHIFT = 1 + NUM_SHIFT + 'z' - 'a' + 1 + '9' - '0' + 1 + NUM_OF_EXTRA_SYM,
    UPPER_CASE_SHIFT = 1 + 'z' - 'a' + NUM_OF_EXTRA_SYM
};

int main(void) {
    int letter;
    char codes[] = "@0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ#";
    
    int bit_3_mask = (1 << BIT_3_SHIFT);
    int bit_2_mask = MAX_NUM ^ (1 << BIT_2_SHIFT);
    
    while ((letter = getchar()) != -1) {
        if (letter >= '0' && letter <= '9') {
            letter -= NUM_SHIFT;
        } else if (letter >= 'A' && letter <= 'Z') {
            letter -= UPPER_CASE_SHIFT;
        } else if (letter >= 'a' && letter <= 'z') {
            letter -= LOWER_CASE_SHIFT;
        } else {
            continue;
        } 
        
        letter ^= bit_3_mask;
        letter &= bit_2_mask;
        putchar(codes[letter]);
    }

    return 0;
}
