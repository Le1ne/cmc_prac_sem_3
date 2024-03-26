#include <stdio.h>
#include <time.h>

int main(void) {
    int year;
    scanf("%d", &year);
    struct tm cur = {.tm_year = year - 1900, .tm_mday = 1, .tm_mon = 0, .tm_isdst = -1};

    int extra_celeb = 1;
    int cnt_celeb = 0;

    while (cur.tm_year == year - 1900) {
        if (((cur.tm_yday + 1) == extra_celeb) && ((cur.tm_wday == 0) || (cur.tm_wday == 6)) 
                && (extra_celeb <= 256)) {
            cnt_celeb++;
            extra_celeb *= 2;
        } else if (((cur.tm_yday + 1) == extra_celeb) && (extra_celeb <= 256)) {
            cnt_celeb++;
            extra_celeb *= 2;
        } else if ((cur.tm_wday == 0) || (cur.tm_wday == 6)) {
            cnt_celeb++;
        }

        cur.tm_mday++;
        mktime(&cur);
    }

    printf("%d\n", cnt_celeb);

    return 0;
}
