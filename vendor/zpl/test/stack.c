

#define ZPL_IMPLEMENTATION
#include "zpl.h"

int
main(void) {
    
    // NOTE(ZaKlaus): Create stack memory
    zpl_stack_memory_t stack = {0};
    zpl_stack_memory_init(&stack, zpl_heap_allocator(), 2);
    
    zpl_allocator_t stack_alloc = zpl_stack_allocator(&stack);
    
    // NOTE(ZaKlaus): Create array container
    zpl_array_t(u32) arr;
    zpl_array_init(arr, stack_alloc);
    
    // NOTE(ZaKlaus): Push 5 elements
    for (i32 i = 0; i < 5; ++i) {
        zpl_array_append(arr, i*2);
    }
    
    // NOTE(ZaKlaus): List them
    zpl_printf("Before removal:\n");
    for (i32 i = 0; i < 5; ++i) {
        zpl_printf("Value: %lld\n", arr[i]);
    }
    
    // NOTE(ZaKlaus): Pop 2 values
    zpl_array_pop(arr);
    zpl_array_pop(arr);

    // NOTE(ZaKlaus): List them again
    zpl_printf("\nAfter removal:\n");
    for (i32 i = 0; i < 3; ++i) {
        zpl_printf("Value: %lld\n", arr[i]);
    }
    
    // NOTE(ZaKlaus): Clear the array out
    zpl_array_clear(arr);
    
    // NOTE(ZaKlaus): Release used resources
    zpl_stack_memory_free(&stack);
    return 0;
}
