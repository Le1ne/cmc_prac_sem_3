#include <stdio.h>
#include <stdlib.h>

enum 
{
    MULTIP = 1103515245,
    INC = 12345,
    MOD = 1u << 31
};

typedef struct RandomOperations RandomOperations;
typedef struct RandomGenerator RandomGenerator;

struct RandomGenerator {
    unsigned num;
    const RandomOperations *ops;
};

static int next(RandomGenerator *rr) {
    rr->num = (rr->num * MULTIP + INC) % MOD;

    return rr->num;
}

static void destroy(RandomGenerator *rr) {
    free(rr);
}

struct RandomOperations {
    int (*next) (RandomGenerator *rr);
    void (*destroy) (RandomGenerator *rr);
};

static const RandomOperations Operations = {.next = next, .destroy = destroy};

RandomGenerator *random_create(int seed) {
    RandomGenerator *rr = calloc(1, sizeof(*rr));

    if (!rr) {
        return NULL;
    }

    rr->num = seed;
    rr->ops = &Operations;

    return rr;
}
