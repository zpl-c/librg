#define ZPL_IMPLEMENTATION
#include <zpl.h>

zpl_global u8 counter = 0;

ZPL_TIMER_CB(test1_cb) {
    printf("Hello, Sailor!\n");

    if (counter == 10) {
        printf("The second timer is done, press ^C to terminate the process.\n");
    }
}

ZPL_TIMER_CB(test2_cb) {
    printf("This has been called %d/10 times!\n", ++counter);
}

#define TO_MS 1000

int main(void) {

    zpl_timer_pool timers;
    zpl_array_init(timers, zpl_heap_allocator());

    zpl_timer_t *t1 = zpl_timer_add(timers);
    zpl_timer_set(t1, TO_MS * 4000, -1, test1_cb);
    zpl_timer_start(t1, 0);

    zpl_timer_t *t2 = zpl_timer_add(timers);
    zpl_timer_set(t2, TO_MS * 1000, 10, test2_cb);
    zpl_timer_start(t2, TO_MS * 2000);

    zpl_timer_t *t3 = zpl_timer_add(timers);
    zpl_timer_set(t3, TO_MS * 1000, 10, test2_cb);
    zpl_timer_start(t3, TO_MS * 2000);
    zpl_timer_stop(t3); // NOTE(ZaKlaus): This won't be fired!

    while (1) {
        zpl_timer_update(timers);
    }

    zpl_array_free(timers);

    return 0;
}
