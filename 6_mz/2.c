#include <stdio.h>

char *shift_back(char *ptr) {
    ptr--;
        
    while (*ptr != '/') {
        ptr--;
    }

    return ptr;
}

void normalize_path(char *buf) {
    if (buf == NULL) {
        return;
    }

    if (buf[0] == '/' && buf[1] == '\0') {
        return;
    }

    char *write_p = buf;
    char *cur = buf;
    int pos = -1;
    int rewrite = 0;

    while (*cur != '\0') {
        rewrite = 0;
        
        if ((*cur == '/') && (*(cur + 1) == '.') && (*(cur + 2) == '.')
                && ((*(cur + 3) == '/') || (*(cur + 3) == '\0'))) {
            cur += 3;
            
            if (write_p != buf) {
                write_p = shift_back(write_p);
            }

            pos = write_p - buf;

            if (pos < 0) {
                pos = 0;
            }
            
            rewrite = 1;
        } else if ((*cur == '/') && (*(cur + 1) == '.')
                && ((*(cur + 2) == '/') || (*(cur + 2) == '\0'))) {
            cur += 2;
            pos = write_p - buf;
            
            if (pos < 0) {
                pos = 0;
            }

            rewrite = 1;
        } else if ((*cur == '/') && (*(cur + 1) == '\0')) {
            *write_p = '\0';
        } else {
            *write_p++ = *cur++;
        }

        if (rewrite) {
            while (*cur != '\0') {
                *write_p++ = *cur++;
            }

            *write_p = '\0';
            write_p = buf + pos;
            cur = buf + pos;

            continue;
        }
    }

    if (pos == 0) {
        *buf = '/';
        *(buf + 1) = '\0';
    }
}
