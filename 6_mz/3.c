#include <stdio.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <unistd.h>

int main(void) {
    unsigned year;
    scanf("%u", &year);

    struct tm time = {.tm_year = year - 1900, .tm_mday = 1};
    mktime(&time);

    int cnt_thu = 0;
    int cur_mon = 0;

    while (time.tm_year == year - 1900) {
        if (time.tm_wday == 4) {
            cnt_thu++;

            if ((cnt_thu % 2 == 0) && (time.tm_mday % 3 != 0)) {
                printf("%d %d\n", time.tm_mon + 1, time.tm_mday);
            } 
        }

        time.tm_mday++;
        mktime(&time);

        if (time.tm_mon != cur_mon) {
            cnt_thu = 0;
            cur_mon++;
        }
    }    

    return 0;
}
