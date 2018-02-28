#define LIBRG_DEBUG
#define LIBRG_IMPLEMENTATION
#include "librg.h"

int main() {
    f64 foo[5] = { 5, 3, 6, 0, 1};
    zpl_printf("librg_standard_deviation: %f\n", librg_standard_deviation(foo, 5));

    zpl_sort(foo, 5, sizeof(f64), zpl_f64_cmp(0));

    zpl_printf("%f\n", foo[0]);
    zpl_printf("%f\n", foo[1]);
    zpl_printf("%f\n", foo[2]);
    zpl_printf("%f\n", foo[3]);
    zpl_printf("%f\n", foo[4]);

    return 0;
}
