#include <stdio.h>
#include <time.h>

int main(void) {
    int year;
    scanf("%d", &year);

    int moon_period = 29 * 24 * 60 * 60 + 12 * 60 * 60 + 44 * 60;
    struct tm example = {.tm_year = 2021 - 1900, .tm_mon = 4, 
            .tm_mday = 26, .tm_hour = 11, .tm_min = 14, .tm_isdst = -1};
    timegm(&example);
    
    int cnt_mon = 0;

    if (year >= 2021) {
        while (example.tm_year != year - 1900) {
            example.tm_sec += moon_period;
            timegm(&example);
        }

        while (example.tm_yday <= 255) {
            example.tm_sec += moon_period;
            timegm(&example);
        }
    } else {
        while (example.tm_year != year - 1900) {
            example.tm_sec -= moon_period;
            timegm(&example);
        }
        
        while (example.tm_yday > 255) {
            example.tm_sec -= moon_period;
            timegm(&example);
        }

        example.tm_sec += moon_period;
        example.tm_yday++;
        timegm(&example);
    }

    while (cnt_mon < 4) {
        example.tm_mday++;
        timegm(&example);

        if (example.tm_wday == 1) {
            cnt_mon++;
        }
    }

    if (((example.tm_mon + 1) < 10) && (example.tm_mday < 10)) {
        printf("%d-0%d-0%d\n", example.tm_year + 1900, example.tm_mon + 1, example.tm_mday);
    } else if (example.tm_mday < 10) {
        printf("%d-%d-0%d\n", example.tm_year + 1900, example.tm_mon + 1, example.tm_mday);
    } else {
        printf("%d-%d-%d\n", example.tm_year + 1900, example.tm_mon + 1, example.tm_mday);
    }

    return 0;
}
