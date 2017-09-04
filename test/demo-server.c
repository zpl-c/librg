#define LIBRG_DEBUG
#define LIBRG_IMPLEMENTATION
#include <librg.h>

void on_connect_request(librg_event_t *event) {
    if (librg_data_ru32(&event->data) != 42) {
        return librg_event_reject(event);
    }
}

void on_connect_accepted(librg_event_t *event) {
    librg_log("on_connect_accepted\n");

    librg_transform_t *transform = librg_fetch_transform(event->entity);
    librg_client_t *client = librg_fetch_client(event->entity);

    // transform->position.x = (float)(2000 - rand() % 4000);
    // transform->position.y = (float)(2000 - rand() % 4000);
    // transform->position.x = 200;
    // transform->position.y = 200;

    librg_log("spawning player %u at: %f %f %f\n",
        event->entity,
        transform->position.x,
        transform->position.y,
        transform->position.z
    );

    librg_streamer_client_set(event->entity, client->peer);
}

void on_spawn_npc(librg_message_t *msg) {
    librg_transform_t tr;
    librg_data_rptr(&msg->data, &tr, sizeof(librg_transform_t));

    librg_entity_t npc = librg_entity_create(1);
    librg_attach_transform(npc, tr);

    // librg_streamer_client_remove(librg_get_client_entity(msg->peer));
}

int main() {
    char *test = "===============      SERVER      =================\n" \
                 "==                                              ==\n" \
                 "==                 ¯\\_(ツ)_/¯                   ==\n" \
                 "==                                              ==\n" \
                 "==================================================\n";
    librg_log("%s\n\n", test);

    librg_init((librg_config_t) {
        .tick_delay     = 32,
        .mode           = LIBRG_MODE_SERVER,
        .world_size     = zplm_vec2(5000.0f, 5000.0f),
        .max_connections = 1000,
    });

    librg_event_add(LIBRG_CONNECTION_REQUEST, on_connect_request);
    librg_event_add(LIBRG_CONNECTION_ACCEPT, on_connect_accepted);

    librg_network_add(42, on_spawn_npc);

    librg_network_start((librg_address_t) { .port = 27010 });

#if 0
    for (int i = 0; i < 15; ++i)
    librg_fetch_transform(librg_entity_create(0))->position.x = i * 20;
#endif

#if 1
    for (isize i = 0; i < 10000; i++) {
        librg_entity_t enemy = librg_entity_create(2);
        librg_transform_t *transform = librg_fetch_transform(enemy);
        transform->position.x = (float)(2000 - rand() % 4000);
        transform->position.y = (float)(2000 - rand() % 4000);
    }
#endif

    while (true) {
        librg_tick();
        zpl_sleep_ms(1);
    }

    librg_network_stop();
    librg_free();

    return 0;
}
