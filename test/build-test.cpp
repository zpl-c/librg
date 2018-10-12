#define LIBRG_IMPLEMENTATION
#include <librg.h>

void on_connect_accepted(librg_event *event) {
    librg_log("someone connected to the server!\n");
}

int main() {
    // initialization
    librg_ctx ctx = {0};

    ctx.tick_delay   = 32;
    ctx.mode         = LIBRG_MODE_SERVER;
    ctx.world_size   = zpl_vec3f(5000.0f, 5000.0f, 0.0f);

    librg_init(&ctx);

    // adding event handlers
    librg_event_add(&ctx, LIBRG_CONNECTION_ACCEPT, on_connect_accepted);

    librg_event e = {0};
    librg_eventrigger(&ctx, 42, &e);

    // starting server
    librg_address address = {0}; address.port = 27010;
    librg_network_start(&ctx, address);

    // starting main loop (run 100 times for test)
    for (int i = 0; i < 100; ++i) {
        librg_tick(&ctx);
        zpl_sleep_ms(1);
    }

    // stopping network and freeing resources
    librg_network_stop(&ctx);
    librg_free(&ctx);
    return 0;
}
