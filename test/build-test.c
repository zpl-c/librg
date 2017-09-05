#define LIBRG_IMPLEMENTATION
#include <librg.h>

void on_connect_accepted(librg_event_t *event) {
    librg_log("someone connected to the server!\n");
}

int main() {
    // initialization
    librg_config_t config = {0};

    config.tick_delay   = 32;
    config.mode         = LIBRG_MODE_SERVER;
    config.world_size   = zplm_vec2(5000.0f, 5000.0f);

    librg_init(config);

    // adding event handlers
    librg_event_add(LIBRG_CONNECTION_ACCEPT, on_connect_accepted);

    // starting server
    librg_address_t address = {0}; address.port = 27010;
    librg_network_start(address);

    // starting main loop (run 100 times for test)
    for (int i = 0; i < 100; ++i) {
        librg_tick();
        zpl_sleep_ms(1);
    }

    // stopping network and freeing resources
    librg_network_stop();
    librg_free();
    return 0;
}
