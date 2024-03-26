#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

enum { RADIX = 10 };

int main(int argc, char **argv) {
    long long total = 0;      

    for (int i = 1; i < argc; ++i) {
        int multiple_flag = 0;
        int add_flag = 0;

        if ((argv[i][strlen(argv[i]) - 1] != '+') && (argv[i][strlen(argv[i]) - 1] != '-')) {
            continue;
        }

        if (argv[i][strlen(argv[i]) - 1] == '+') {
            add_flag = 1;
            argv[i][strlen(argv[i]) - 1] = '\0';
        } else if(argv[i][strlen(argv[i]) - 1] == '-') {
            argv[i][strlen(argv[i]) - 1] = '\0';
        }

        if (argv[i][strlen(argv[i]) - 1] == 'k') {
            multiple_flag = 1;
            argv[i][strlen(argv[i]) - 1] = '\0';
        }
        
        errno = 0;
        char *end = NULL; 
        long cur = strtol(argv[i], &end, RADIX);

        if (errno || *end || end == argv[i] || (int) cur != cur) {
            continue;
        }

        if (multiple_flag) {
            if (__builtin_mul_overflow(cur, 1000, &cur)) {
                continue;
            }
            
            if (add_flag) {
                total += cur; 
            } else {
                total -= cur;
            }
        } else {
            if (add_flag) {
                total += cur;
            } else {
                total -= cur;
            }
        }
    }
    
    printf("%lld\n", total);

    return 0;
}
