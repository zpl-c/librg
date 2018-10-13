#define LIBRG_DEBUG
#define LIBRG_IMPLEMENTATION
#include "librg.h"


int main() {
    // f64 foo[5] = { 5, 3, 6, 0, 1};
    // zpl_printf("librg_standard_deviation: %f\n", librg_standard_deviation(foo, 5));

    // zpl_sort(foo, 5, sizeof(f64), zpl_f64_cmp(0));

    // zpl_printf("%f\n", foo[0]);
    // zpl_printf("%f\n", foo[1]);
    // zpl_printf("%f\n", foo[2]);
    // zpl_printf("%f\n", foo[3]);
    // zpl_printf("%f\n", foo[4]);

    librg_data data;
    librg_data_init_size(&data, sizeof(u32));
    // librg_log("r: %d w: %d\n", librg_data_get_rpos(&data), librg_data_get_wpos(&data));

    librg_data_wu32(&data, 32);

    u32 a = librg_data_ru32(&data);
    // u32 b = librg_data_ru32(&data);

    // librg_log("%ld > %zu\n", librg_data_get_rpos(&data) + sizeof(u32), librg_data_capacity(&data));
    // librg_data_read_safe(u32, b, &data);
    // librg_data_read_safe(u32, c, &data);
    // librg_log("b = %d\n", b);

    // #undef _LIBRG_METHOD_NAME
    // #undef _LIBRG_METHOD_TYPE
    return 0;
}
