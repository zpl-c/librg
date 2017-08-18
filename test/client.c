#define LIBRG_IMPLEMENTATION
#define LIBRG_DEBUG
#include <librg.h>

typedef struct { i32 a; } librg_component(foo);

void on_connect_request(librg_event_t *event) {
    zpl_bs_write_u32(event->data, 42);
    librg_log("on_connect_request\n");
}

void on_connect_accepted(librg_event_t *event) {
    librg_log("on_connect_accepted\n");
}

void on_connect_refused(librg_event_t *event) {
    librg_log("on_connect_refused\n");
}

// // client
// void damage_car(librg_entity_t entity) {
//     librg_log("client: damanging the car\n");
//     librg_send(21, librg_lambda(data), { zpl_bs_write_u32(data, entity.id); });
// }

// void onvehcielcreate(librg_message_t *msg) {
//     u32 guid = zpl_bs_read_u32(msg->data);

//     librg_entity_t entity = librg_entity_create_shared(guid);
//     librg_attach_foo(entity, (foo_t) { 123 });

//     librg_log("server created vehicle %lu\n", entity.id);

//     damage_car(entity);
// }

// void on_damage_finished(librg_message_t *msg) {
//     u32 guid = zpl_bs_read_u32(msg->data);
//     librg_entity_t entity = librg_entity_get(guid);

//     foo_t *foo = librg_fetch_foo(entity);

//     ZPL_ASSERT(foo && foo->a == 123);

//     librg_log("damaged car finished\n");
// }


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

    // librg_network_add(20, onvehcielcreate);
    // librg_network_add(22, on_damage_finished);

    librg_network_start((librg_address_t) { .host = "localhost", .port = 27010 });

    while (true) {
        librg_tick();
        zpl_sleep_ms(1);
    }

    librg_network_stop();
    librg_free();
    return 0;
}
