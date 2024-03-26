#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/file.h>

enum 
{ 
    MASK_1 = 0xf,
    MASK_2 = 0xff,
    SHIFT_1 = 8,
    SHIFT_2 = 4,
    BUF = 4
};

int main(int argc, char *argv[]) {
    int f_res = open(argv[1], O_CREAT | O_WRONLY | O_TRUNC, 0600);
    
    if (f_res < 0) {
        return 1;
    }

    unsigned num;

    while (scanf("%u", &num) == 1) {
        char nums[4] = {};
        nums[3] = num & MASK_2;
        num >>= SHIFT_1;
        nums[2] = num & MASK_1;
        num >>= SHIFT_2;
        nums[1] = num & MASK_2;
        num >>= SHIFT_1;
        nums[0] = num & MASK_1;
        num >>= SHIFT_2;

        write(f_res, nums, sizeof(nums));
    }

    if (close(f_res) < 0) {
        return 1;
    }

    return 0;
}
