#include <dlfcn.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

enum { RADIX = 10 };

typedef struct Buf 
{
    char arguments[64];
} Buf;

int main(int argc, char *argv[]) {
    void *handle = dlopen(argv[1], RTLD_LAZY);

    if (handle == NULL) {
        return 1;
    }

    Buf input_args;
    int args_iter = 0;
    int type_iter = 1;

    for (int i = 4; i < argc; ++i) {
        if (argv[3][type_iter] == 'i') {
            char *end = NULL;
            errno = 0;
            long num = strtol(argv[i], &end, RADIX);

            if (errno || *end || end == argv[i] || (int)num != num) {
                return 1;
            }

            int i_num = num;
            memcpy(&(input_args.arguments[args_iter]), &i_num, sizeof(i_num));
            args_iter += sizeof(i_num);
        } else if (argv[3][type_iter] == 'd') {
            char *end = NULL;
            errno = 0;
            double d_num = strtod(argv[i], &end);

            if (errno || *end || end == argv[i]) {
                return 1;
            }

            memcpy(&(input_args.arguments[args_iter]), &d_num, sizeof(d_num));
            args_iter += sizeof(d_num);
        } else if (argv[3][type_iter] == 's') {
            char *str = argv[i];
            memcpy(&(input_args.arguments[args_iter]), &str, sizeof(argv[i]));
            args_iter += sizeof(argv[i]);
        }

        type_iter++;
    }

    void *func = dlsym(handle, argv[2]);

    if (func == NULL) {
        return 1;
    }

    if (argv[3][0] == 'v') {
        ((void (*)(Buf))(func))(input_args);
    } else if (argv[3][0] == 's') {
        printf("%s\n", ((char *(*)(Buf))(func))(input_args));
    } else if (argv[3][0] == 'i') {
        printf("%d\n", ((int(*)(Buf))(func))(input_args));
    } else if (argv[3][0] == 'd') {
        printf("%.10g\n", ((double(*)(Buf))(func))(input_args));
    }

    if (dlclose(handle) != 0) {
        return 1;
    }

    return 0;
}