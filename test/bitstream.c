#define LIBRG_IMPLEMENTATION
#define LIBRG_DEBUG
#include <librg.h>


typedef struct {
    zplm_vec3_t a;
    zplm_vec3_t b;
    zplm_vec3_t c;
    zplm_vec3_t d;
    zplm_vec3_t e;
    zplm_vec3_t f;
} foo_t;

// typedef struct temp {
//     zpl_allocator_t allocator;

//     usize capacity;
//     usize read_pos;
//     usize write_pos;

//     void *data;
// } temp_t;


void aaaaa(librg_event_t *e, foo_t foo) {
    librg_data_wptr(e->data, &foo, sizeof(foo_t));
}

int main() {

    // librg_init((librg_config_t) {
    //     .tick_delay     = 32,
    //     .mode           = LIBRG_MODE_SERVER,
    //     .world_size     = zplm_vec2(5000.0f, 5000.0f),
    // });

    librg_data_t data;
    librg_data_init(&data);

    librg_data_wu32(&data, 15);
    librg_data_wu32(&data, 42);
    librg_data_wu32(&data, 23);

    librg_log("%u, %u\n",
        librg_data_ru32(&data),
        librg_data_ru32(&data)
    );

    librg_log("current wpos: %zu\n", librg_data_get_wpos(&data));
    librg_data_reset(&data);
    librg_log("current wpos: %zu\n", librg_data_get_wpos(&data));

    foo_t foo = { 0 };

    for (int i = 0; i < 10000; ++i) {
        librg_event_t e = {0};
        e.data = &data;
        aaaaa(&e, foo);
        librg_data_wptr(&data, &foo, sizeof(foo_t));
    }
    librg_log("current wpos: %zu\n", librg_data_get_wpos(&data));

    // librg_data_rptr()

    librg_data_free(&data);

    // librg_free();
    return 0;
}
