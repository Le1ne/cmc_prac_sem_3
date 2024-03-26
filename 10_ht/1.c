#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/mman.h>

enum 
{ 
    RADIX = 16,
    FILE_SIZE = 0x10000,
    OFFSET = 9,
    MASK = 0x1ff
};

int main(int argc, char **argv) {
    int fd = open(argv[1], O_RDONLY);

    if (fd < 0) {
        return 1;
    }

    unsigned short phys_addr_t_page = strtol(argv[2], NULL, RADIX);
    struct stat info;

    if (fstat(fd, &info) < 0) {
        return 1;
    }

    if (info.st_size != FILE_SIZE) {
        return 1;
    }

    unsigned char *virt_memory = mmap(NULL, FILE_SIZE, PROT_READ, MAP_PRIVATE, fd, 0);

    if (virt_memory == MAP_FAILED) {
        return 1;
    }

    unsigned short virt_addr;
    unsigned short virt_page;
    unsigned short page_offset;
    unsigned short phys_addr;

    while (scanf("%hx", &virt_addr) == 1) {
        virt_page = virt_addr >> OFFSET;
        page_offset = virt_addr & MASK;
        phys_addr = *(unsigned short *) &virt_memory[phys_addr_t_page + 2 * virt_page] | page_offset;
        printf("%hu\n", *(unsigned short *) (virt_memory + phys_addr));
    }

    munmap(virt_memory, FILE_SIZE);

    if (close(fd) < 0) {
        return 1;
    }

    return 0;
}
