#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int n;
    scanf("%d", &n);
    int *fat = calloc(n, sizeof(*fat));

    for (int i = 2; i < n - 2; ++i) {
        scanf("%d", &fat[i]);
    }

    int num;

    while (scanf("%d", &num) != EOF) {
        int cnt = 0;
        
        while (num != 0 || num != (n - 1)) {
            if (num == 0 || num == 1) {
                break;
            }

            if (num == (n - 2) || num == (n - 1)) {
                break;
            }

            if (fat[num] == 0) {
                break;
            }

            cnt++;
            num = fat[num];
        }

        printf("%d\n", cnt);
    }

    return 0;
}
