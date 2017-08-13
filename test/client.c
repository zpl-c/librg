#define LIBRG_IMPLEMENTATION
#define LIBRG_DEBUG
#include <librg.h>

int main() {
    char *test = "===============      CLIENT      =================\n" \
                 "==                                              ==\n" \
                 "==                 ¯\\_(ツ)_/¯                   ==\n" \
                 "==                                              ==\n" \
                 "==================================================\n";
    librg_log("%s\n\n", test);

    librg_init((librg_cfg_t) {
        .tick_delay     = 32,
        .mode           = librg_client_ev,
        .world_size     = zplm_vec2(5000.0f, 5000.0f),
        .entity_limit   = 2048,
    });

    librg_network_start(&(librg_address_t) { .host = "localhost", .port = 27010 });

    while (true) {
        librg_tick();
    }

    librg_network_stop();
    librg_free();
    return 0;
}
