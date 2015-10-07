#include "common.h"

struct timeval nowtime() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return  tv;
}
