#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <limits.h>

enum { CHAR_SIZE = 1 };

ssize_t getcwd2(int fd, char *buf, size_t size) {
    if (buf == NULL) {
        return -1;
    }
    
    DIR *start = opendir(".");
    
    if (start == NULL) {
        return -1;
    }
    
    int fd_ret = dirfd(start);
    
    if (fchdir(fd) == -1) {
        return -1;
    }

    struct stat prev;
    struct stat root;

    if ((fstat(fd, &prev) < 0) || (lstat("/", &root) < 0)) {
        fchdir(fd_ret);
        closedir(start);

        return -1;
    }

    if ((prev.st_ino == root.st_ino) && (prev.st_dev == root.st_dev)) {
        if (size > 1) {
            buf[0] = '/';
            buf[1] = '\0';            
        } else {
            buf[0] = '\0';
        }

        fchdir(fd_ret);
        closedir(start);

        return 1;
    }

    char *res = calloc(PATH_MAX, CHAR_SIZE);
    char *tmp = calloc(PATH_MAX, CHAR_SIZE);
    char *full_name = calloc(PATH_MAX, CHAR_SIZE);

    while (!((prev.st_ino == root.st_ino) && (prev.st_dev == root.st_dev))) {
        if (chdir("..") == -1) {
            fchdir(fd_ret);
            closedir(start);
            free(res);
            free(tmp);
            free(full_name);
            
            return -1;
        }

        DIR *d = opendir(".");
    
        if (d == NULL) {
            fchdir(fd_ret);
            closedir(start);
            free(res);
            free(tmp);
            free(full_name);
            
            return -1;
        }
    
        struct dirent *dd = NULL;
        struct stat cur;
    
        if (lstat(".", &cur) < 0) {
            fchdir(fd_ret);
            closedir(start);
            free(res);
            free(tmp);
            free(full_name);
            
            return -1;
        }

        while ((dd = readdir(d)) != NULL) {
            struct stat buf;
            
            snprintf(full_name, PATH_MAX + 1, "./%s", dd->d_name);

            if (lstat(full_name, &buf) < 0) {
                fchdir(fd_ret);
                closedir(start);
                free(res);
                free(tmp);
                free(full_name);
                
                return -1;
            }

            if ((buf.st_ino == prev.st_ino) && (buf.st_dev == prev.st_dev)) {
                snprintf(tmp, PATH_MAX + 1, "/%s%s", dd->d_name, res);
                prev = cur;

                break;
            }
        }

        if (closedir(d) != 0) {
            fchdir(fd_ret);
            closedir(start);
            free(res);
            free(tmp);
            free(full_name);
            
            return -1;
        }

        snprintf(res, PATH_MAX + 1, "%s", tmp);
    }

    ssize_t len = strlen(res);

    if (size == 0) {   
        fchdir(fd_ret);
        closedir(start);
        free(res);
        free(tmp);
        free(full_name);
    } else if (size > 0) {
        snprintf(buf, size, "%s", res);
    }

    if (fchdir(fd_ret) == -1) {
        free(res);
        free(tmp);
        free(full_name);
        
        return -1;
    }

    if (closedir(start) < 0) {
        free(res);
        free(tmp);
        free(full_name);
        
        return -1;
    }

    free(res);
    free(tmp);
    free(full_name);
    
    return len;
}
