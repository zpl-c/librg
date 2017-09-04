#define LIBRG_IMPLEMENTATION
#define LIBRG_DEBUG
#include <librg.h>

typedef struct { i32 foo; f64 bar; } zap_t;

int main() {

    librg_init((librg_config_t) {
        .tick_delay     = 32,
        .mode           = LIBRG_MODE_SERVER,
        .world_size     = zplm_vec2(5000.0f, 5000.0f),
        .entity_limit   = 128000,
    });

    librg_data_t data;
    librg_data_init(&data);

    librg_data_wu32(&data, 15);
    librg_data_wu32(&data, 42);
    librg_data_wu32(&data, 23);

    librg_log("%u, %u\n",
        librg_data_ru32(&data),
        librg_data_ru32(&data)
    );

    zap_t zap = { -2542, 0.2322222222222244 };

    librg_data_wptr(&data, &zap, sizeof(zap));
    // librg_data_rptr()

    librg_data_free(&data);

    librg_free();
    return 0;
}
