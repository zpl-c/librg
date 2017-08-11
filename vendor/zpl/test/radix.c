

#define ZPL_IMPLEMENTATION
#include "zpl.h"

int
main(void) {
   
    // NOTE(ZaKlaus): Define a storage for our numbers and the temporary buffer.
    u32 nums[4096*8] = {0};
    u32 temp[4096*8] = {0};
    
    // NOTE(ZaKlaus): Initialize random seed.
    // and generate random values.
    zpl_random_t r; zpl_random_init(&r);
    for (int i = 0; i < 4096*8; ++i) {
        nums[i] = zpl_random_gen_u32_unique(&r);
    }
    
    // NOTE(ZaKlaus): Print these values out
    zpl_printf("First 30 numbers...\nBefore: ");
    for (int i = 0; i < 90; ++i) {
        zpl_printf("%lld, ", nums[i]);
    }
    
    // NOTE(ZaKlaus): Use radix sort on our numbers and print them out again.
    zpl_radix_sort_u32(nums, temp, 4096*8);
    
    zpl_printf("\n\nAfter: ");
    for (int i = 0; i < 90; ++i) {
        zpl_printf("%lld, ", nums[i]);
    }    
    
    return 0;
}