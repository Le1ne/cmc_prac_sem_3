#include <unistd.h>
#include <fcntl.h>
#include <sys/file.h>
#include <errno.h>
#include <stdlib.h>

enum 
{ 
    RADIX = 10,
    MASK = 1, 
    BYTE_SIZE = 8 
};

int main(int argc, char **argv) {
    int f_in = open(argv[1], O_RDONLY);
    
    if (f_in < 0) {
        return 1;
    }

    int f_res = creat(argv[2], 0777);

    if (f_res < 0) {
        return 1;
    }

    errno = 0;
    char *end = NULL;
    long mod = strtol(argv[3], &end, RADIX);

    if (errno || *end || end == argv[3] || (int) mod != mod) {
        return 1;
    }

    unsigned char cur;
    unsigned long long sum = 0;
    unsigned long long cnt = 0;
    int num_out = 0; 

    while (read(f_in, &cur, sizeof(cur)) > 0) {
        for (int i = 0; i < BYTE_SIZE; ++i) {
            cnt += 1;
            sum += cnt * cnt;
            
            if ((cur & MASK) == 1) {
                num_out = sum % mod;
                write(f_res, &num_out, sizeof(num_out));
            }

            cur >>= 1;
        }
    }

    if (close(f_in) < 0) {
        return 1;
    }

    if (close(f_res) < 0) {
        return 1;
    }

    return 0;
}
