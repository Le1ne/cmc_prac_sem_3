#include <unistd.h>
#include <fcntl.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <string.h>
#include <limits.h>
#include <stdio.h>

enum { BUF_SIZE = 4096 };

int copy_file(const char *scrpath, const char *dstpath) {
    struct stat in_stat;
    
    if (stat(scrpath, &in_stat) < 0 || S_ISDIR(in_stat.st_mode)) {
        return -1;
    }

    int fd_in = open(scrpath, O_RDONLY);

    if (fd_in < 0) {
        return -1;
    }

    struct stat res_stat;
    int fd_res;
    int flag = 0;
    unsigned short mask = 0x1ff;
    unsigned short permissions = in_stat.st_mode & mask;

    if (stat(dstpath, &res_stat) < 0) {
        fd_res = open(dstpath, O_CREAT | O_WRONLY | O_TRUNC, permissions);
        
        flag = 1;
    } else {
        if ((in_stat.st_dev == res_stat.st_dev) && (in_stat.st_ino == res_stat.st_ino)) {
            return 0;
        }
    }

    if (!S_ISDIR(res_stat.st_mode) && !flag) {
        fd_res = open(dstpath, O_CREAT | O_WRONLY | O_TRUNC, permissions);
    
        if (fd_res < 0) {
            return -1;
        }
    } else if (S_ISDIR(res_stat.st_mode) && !flag) {
        char path_name[PATH_MAX + 1];
        int len_src = strlen(scrpath) - 1;

        while ((len_src != 0) && (scrpath[len_src] != '/')) {
            len_src--;
        }

        snprintf(path_name, PATH_MAX + 1, "%s/%s", dstpath, (scrpath + len_src));
        fd_res = open(path_name, O_CREAT | O_WRONLY | O_TRUNC, permissions);

        if (fd_res < 0) {
            return -1;
        }
    }

    char buf[BUF_SIZE];
    int total_bytes;

    while ((total_bytes = read(fd_in, buf, BUF_SIZE)) > 0) {
        int cur_bytes = 0;
        int cur_pos = 0;

        while (total_bytes > 0) {
            if ((cur_bytes = write(fd_res, buf + cur_pos, total_bytes)) < 0) {
                return -1;
            }
            
            cur_pos += cur_bytes;
            total_bytes -= cur_bytes;
        }
    }

    if ((close(fd_in) < 0) || (close(fd_res) < 0)) {
        return -1;
    }

    return 0;
}
