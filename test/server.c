#define LIBRG_IMPLEMENTATION
#define LIBRG_DEBUG
#include <librg.h>

typedef struct { bool a; } librg_component(foo);

void on_connect_request(librg_event_t *event) {
    u32 secret = zpl_bs_read_u32(event->bs);

    librg_log("user sent number: %u\n", secret);

    if (secret != 42) {
        return librg_event_reject(event);
    }
}

void on_connect_accepted(librg_event_t *event) {
    librg_log("on_connect_accepted\n");
    librg_send_all(158, { zpl_bs_write_u64(data, 228); });
}

void on_connect_refused(librg_event_t *event) {
    librg_log("on_connect_refused\n");
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

    librg_event_add(LIBRG_CONNECTION_REQUEST, on_connect_request);
    librg_event_add(LIBRG_CONNECTION_ACCEPT, on_connect_accepted);
    librg_event_add(LIBRG_CONNECTION_REFUSE, on_connect_refused);
    librg_network_start((librg_address_t) { .host = "localhost", .port = 27010 });

    while (true) {
        librg_tick();
    }

    librg_network_stop();
    librg_free();
    return 0;
}
