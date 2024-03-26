#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>

enum { BASE = 3 };

int main(void) {
    long long res = 0;
    char sym;
    int overflow_check = 0;
    int ll_min_check = 0;
    int not_valid_sym = 0;

    while ((sym = getchar()) != EOF) {
        if (isspace(sym) > 0) {
            overflow_check = 0;
            
            continue;            
        }

        if (overflow_check) {
            continue;
        }

        overflow_check = 0;

        while ((sym != EOF) && (isspace(sym) == 0)) {
            if (sym != 'a' && sym != '1' && sym != '0') {
                not_valid_sym = 1;

                break;
            }
            
            if (__builtin_mul_overflow(res, BASE, &res)) {
                if (res == LLONG_MAX && sym == '1') {
                    printf("%lld\n", LLONG_MIN);
                    ll_min_check = 1;
                    
                    break;
                }
                
                overflow_check = 1;

                break;
            }

            if (sym == 'a') {
                if (__builtin_ssubll_overflow(res, 1, &res)) {
                    overflow_check = 1;

                    break;
                } 
            } else if (sym == '1') {
                if (__builtin_saddll_overflow(res, 1, &res)) {
                    overflow_check = 1;

                    break;
                }
            }

            sym = getchar();
        }

        if (!overflow_check) {
            if (!ll_min_check) {
                if (!not_valid_sym) {
                    printf("%lld\n", res);
                }
            } else {
                ll_min_check = 0;
            }

            if (!not_valid_sym) {
                res = 0;
            } else {
                not_valid_sym = 0;
            }
        } else {
            printf("18446744073709551616\n");
            res = 0;
        }
    }

    return 0;
}
