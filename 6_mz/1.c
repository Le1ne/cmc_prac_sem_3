#include <stdio.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <dirent.h>
#include <limits.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char **argv) {
    DIR *d = opendir(argv[1]);

    if (d == NULL) {
        return 1;
    }

    struct stat buf;
    struct dirent *dd;
    char name[PATH_MAX + 1];
    unsigned long long cnt = 0;

    while ((dd = readdir(d)) != NULL) {
        snprintf(name, PATH_MAX + 1, "%s/%s", argv[1], dd->d_name);

        if (stat(name, &buf) < 0) {
            continue;
        }

        if (S_ISREG(buf.st_mode) && (!access(name, X_OK))) {
            if ((strlen(dd->d_name) >= 4) && !(strcmp(dd->d_name + strlen(dd->d_name) - 4, ".exe"))) {
                cnt++;
            }
        }
    }

    if (closedir(d) < 0) {
        return 1;
    }
    
    printf("%llu\n", cnt);

    return 0;
}
