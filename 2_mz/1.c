#include <stdio.h>

enum { MAX_LEN = 129 };

int main(void) {
    char s1[MAX_LEN], s2[MAX_LEN], s3[MAX_LEN]; 
    scanf("%128s", s1);
    scanf("%128s", s2);
    scanf("%128s", s3);

    printf("[Host:%s,Login:%s,Password:%s]\n", s1, s2, s3);

    return 0;
}
