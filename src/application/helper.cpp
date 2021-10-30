#include <stdint.h>
#include <stddef.h>
#include "helper.h"

int32_t constrain_num(int32_t num, int32_t min, int32_t max) {
    if (num < min) {
        return min;
    } else if (num > max) {
        return max;
    } else {
        return num;
    }
}

int16_t constrain_num(int16_t num, int16_t min, int16_t max) {
    if (num < min) {
        return min;
    } else if (num > max) {
        return max;
    } else {
        return num;
    }
}

