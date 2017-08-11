#define ZPL_IMPLEMENTATION
#include <zpl.h>

#define ZPLEV_IMPLEMENTATION
#include <zpl_event.h>

typedef struct {
    u64 a,b,c;
} test_e1_t;

enum {
    test_e1_ev,
};

ZPLEV(test_e1_v1) {
    ZPLEV_CAST(test_e1_t, data);

    zpl_printf("a: %lu b: %lu c: %lu\n", data->a, data->b, data->c);
};

ZPLEV(test_e1_v2) {
    ZPLEV_CAST(test_e1_t, data);

    zpl_printf("a -- %lu b -- %lu c -- %lu\n", data->a, data->b, data->c);
};
ZPLEV(test_e1_v3) {
    ZPLEV_CAST(test_e1_t, data);

    zpl_printf("a> %lu b> %lu c> %lu\n", data->a, data->b, data->c);
};

int
main(void) {

    zplev_pool events;
    zplev_init(&events, zpl_heap_allocator());

    zplev_add(&events, test_e1_ev, test_e1_v1); // 0
    zplev_add(&events, test_e1_ev, test_e1_v2); // 1
    zplev_add(&events, test_e1_ev, test_e1_v3); // 2


    test_e1_t cb_data = {1, 2, 3};
    zplev_trigger(&events, test_e1_ev, &cb_data);

    zplev_remove(&events, test_e1_ev, 1);
    zpl_printf("After deletion of ID 1\n");

    // NOTE(ZaKlaus): Prints 0,2
    zplev_trigger(&events, test_e1_ev, &cb_data);

    zplev_destroy(&events);
    return 0;
}
