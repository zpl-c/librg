#define ZPL_IMPLEMENTATION
#include <zpl.h>

void easy_print(zpl_buffer_t(i64) nums, isize cnt) {
    zpl_printf("Numbers:\n");
    for (isize i = 0; i < cnt; ++i) {
        zpl_when (&nums[i], i64*, number) {
            zpl_printf("%d, ", *number);
        }
    }
    zpl_printf("\n");
}

int main(void) {
    
    zpl_random_t rng = {0};
    zpl_random_init(&rng);
    
    // NOTE(ZaKlaus): Initialize our buffer
    zpl_buffer_t(i64) numbers = 0;
    zpl_buffer_init(numbers, zpl_heap_allocator(), zpl_size_of(i64)*20);
    
    for (isize i = 0; i < 20; ++i) {
        numbers[i] = zpl_random_range_i64(&rng, 0, 2);
    }
    
    easy_print(numbers, 20);
    
    // NOTE(ZaKlaus): Perform checks
    for (isize i = 0; i < 20; ++i) {
        if (i == 19) continue;
        zpl_when (&numbers[i], i64*, number) {
            *number = *number + *(number+1);;
        }
    }
    
    easy_print(numbers, 20);
    
    return 0;
}
