#define LIBRG_IMPLEMENTATION
#define LIBRG_DEBUG
#include <librg.h>

void on_connect_request(librg_event_t *event) {
    zpl_bs_write_u32(event->bs, 42);
    librg_log("on_connect_request\n");
}

void on_connect_accepted(librg_event_t *event) {
    librg_log("on_connect_accepted\n");
}

void on_connect_refused(librg_event_t *event) {
    librg_log("on_connect_refused\n");
}

void on_customdata(librg_message_t *msg) {
    u64 aaa = zpl_bs_read_u64(msg->data);

    librg_log("server sent to everyone %llu", aaa);
}

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

    librg_event_add(LIBRG_CONNECTION_REQUEST, on_connect_request);
    librg_event_add(LIBRG_CONNECTION_ACCEPT, on_connect_accepted);
    librg_event_add(LIBRG_CONNECTION_REFUSE, on_connect_refused);

    librg_network_add(158, on_customdata);

    librg_network_start((librg_address_t) { .host = "localhost", .port = 27010 });

    while (true) {
        librg_tick();
    }

    librg_network_stop();
    librg_free();
    return 0;
}
