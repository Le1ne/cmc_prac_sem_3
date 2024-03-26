#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

enum 
{ 
    RADIX = 10,
    LIMIT = 12 
};

struct Elem
{
    struct Elem *next;
    char *str;
};

struct Elem *dup_elem(struct Elem *head) {
    if (!head) {
        return NULL;
    }

    struct Elem *cur = head; 
    struct Elem *prev = head;

    while (cur) {
        char *end = NULL;
        errno = 0;

        long cur_num = strtol(cur->str, &end, RADIX);

        if (!errno && !(*end) && end != cur->str && (int) cur_num == cur_num) {
            int res;

            if (!__builtin_sadd_overflow(cur_num, 1, &res)) {
                struct Elem *new_elem = malloc(sizeof(struct Elem));
                new_elem->str = malloc(LIMIT);
                snprintf(new_elem->str, LIMIT, "%ld", cur_num + 1);
                
                new_elem->next = cur;
                
                if (cur == head) {    
                    head = new_elem;
                } else {
                    new_elem->next = cur;
                    prev->next = new_elem;
                }
            }
        }

        prev = cur;
        cur = cur->next;
    }

    return head;
}
