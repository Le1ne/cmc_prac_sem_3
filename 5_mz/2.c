#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

enum { RADIX = 8 };

int main(int argc, char* argv[]) {
    for (int i = 1; i < argc; ++i) {
        char full_rights[] = "rwxrwxrwx";
        
        errno = 0;
        char *end = NULL;       
        long rights = strtol(argv[i], &end, RADIX);
        
        if (errno || *end || end == argv[i] || (int) rights != rights) {
            return 1;
        }

        long check = 1;

        for (int j = strlen(full_rights) - 1; j >= 0; j--) {
            if ((rights & check) == 0) {
                full_rights[j] = '-';
            } 

            check <<= 1;
        }

        printf("%s\n", full_rights);
    }

    return 0;
}
