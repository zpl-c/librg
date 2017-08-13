#define LIBRG_IMPLEMENTATION
#include <librg.h>

void on_connect(librg_event_t evt) {

}

int main() {
    char *test = "===============      SERVER      =================\n" \
                 "==                                              ==\n" \
                 "==                 ¯\\_(ツ)_/¯                   ==\n" \
                 "==                                              ==\n" \
                 "==================================================\n";
    librg_log("%s\n\n", test);

    librg_init((librg_cfg_t) {
        .tick_delay     = 32,
        .mode           = librg_server_ev,
        .world_size     = zplm_vec2(5000.0f, 5000.0f),
        .entity_limit   = 2048,
    });


    librg_event_add(LIBRG_CONNECTION_REQUEST, on_connect);

    librg_network_start((librg_address_t) { .host = "localhost", .port = 27010 });
    librg_network_stop();

    // librg_client_connect("localhost", 27010);
    // librg_client_disconnect();

    // librg_connect();

    while (true) {
        librg_tick();
    }

    // librg_stop();

    librg_free();
    return 0;
}
