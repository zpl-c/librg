#define LIBRG_IMPLEMENTATION
#include <librg.h>

typedef struct my_ev_1_t {
    i32 value;
} my_ev_1_t;

enum {
    my_event_1,
    my_event_2,
    my_event_3,
};

void my_cb_1(librg_evt_t evt) {
    my_ev_1_t *event = cast(my_ev_1_t*)evt;
    librg_log("the value is: %d\n", event->value);
}

int main() {
    librg_init((librg_config_t) {
        .tick_delay     = 32,
        .mode           = LIBRG_MODE_SERVER,
        .world_size     = zplm_vec2(5000.0f, 5000.0f),
        .entity_limit   = 2048,
    });


    librg_event_add(my_event_1, my_cb_1);

    my_ev_1_t data = { 15 };
    librg_event_trigger(my_event_1, &data);
    librg_event_trigger(my_event_1, &data);
    data.value = 16;
    librg_event_trigger(my_event_1, &data);


    // while (true) {
    //     librg_tick();
    // }

    librg_free();
    return 0;
}
