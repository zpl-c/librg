#define LIBRG_IMPLEMENTATION
#include <librg.h>

int main() {
    char *test = "===============      CLIENT      =================\n" \
                 "==                                              ==\n" \
                 "==                 ¯\\_(ツ)_/¯                   ==\n" \
                 "==                                              ==\n" \
                 "==================================================\n";
    librg_log("%s\n\n", test);

    librg_init(librg_mode_client_ev, (librg_cfg_t){
        .tick_delay     = 32,
        .world_size     = zplm_vec2(5000.0f, 5000.0f),
        .entity_limit   = 2048,
    });

    while (true) {
        librg_tick();
    }

    librg_free();
    return 0;
}
