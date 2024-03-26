#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int parse_rwx_permissions(const char *str) {
    if (str == NULL) {
        return -1;
    }

    unsigned int permissions = 0;

    const char full_rights[] = "rwxrwxrwx";
    int len_full = sizeof(full_rights) - 1;
    int len = strlen(str);

    if (len_full != len) {
        return -1;
    }

    for (int i = 0; i < len; i++) {
        permissions <<= 1;

        if (str[i] == full_rights[i]) {
            permissions |= 1;
        } else if (str[i] != '-') {
            return -1;
        }
    }

    return permissions;
}
