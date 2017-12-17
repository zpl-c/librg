#define LIBRG_IMPLEMENTATION
#define LIBRG_DEBUG
#include <librg.h>

void on_connect_request(librg_event_t *event) {
    librg_data_wu32(event->data, 42);
    librg_log("on_connect_request\n");
}

void on_connect_accepted(librg_event_t *event) {
    librg_log("on_connect_accepted\n");
    librg_log("my entity: %u\n", event->entity);
}

void on_connect_refused(librg_event_t *event) {
    librg_log("on_connect_refused\n");
}

void on_entity_create(librg_event_t *event) {
    // ...
}

void on_entity_update(librg_event_t *event) {
    // ...
}

int main() {
    char *test = "===============      CLIENT      =================\n" \
                 "==                                              ==\n" \
                 "==                 ¯\\_(ツ)_/¯                   ==\n" \
                 "==                                              ==\n" \
                 "==================================================\n";
    librg_log("%s\n\n", test);

    librg_ctx_t original     = {0};
    original.tick_delay      = 1000;
    original.mode            = LIBRG_MODE_CLIENT;
    original.world_size      = zplm_vec3(5000.0f, 5000.0f, 0.f);
    original.max_entities    = 60000;

    #define size 16
    librg_ctx_t *ctxs = zpl_malloc(size*sizeof(librg_ctx_t));

    for (int i = 0; i < size; ++i) {
        ctxs[i] = original;

        librg_init(&ctxs[i]);

        librg_event_add(&ctxs[i], LIBRG_CONNECTION_REQUEST, on_connect_request);
        librg_event_add(&ctxs[i], LIBRG_CONNECTION_ACCEPT, on_connect_accepted);
        librg_event_add(&ctxs[i], LIBRG_CONNECTION_REFUSE, on_connect_refused);

        librg_event_add(&ctxs[i], LIBRG_ENTITY_CREATE, on_entity_create);
        librg_event_add(&ctxs[i], LIBRG_ENTITY_UPDATE, on_entity_update);

        librg_network_start(&ctxs[i], (librg_address_t) { .host = "localhost", .port = 7777 });
    }

    while (true) {
        for (int i = 0; i < size; ++i) {
            librg_tick(&ctxs[i]);
        }

        zpl_sleep_ms(1);
    }

    return 0;
}
