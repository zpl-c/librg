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

    librg_log("spawning player at: %f %f %f\n",
        transform->position.x,
        transform->position.y,
        transform->position.z
    );

    librg_streamer_client_set(event->entity, client->peer);
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
        .mode           = librg_server_ev,
        .world_size     = zplm_vec2(5000.0f, 5000.0f),
        .max_connections = 1000,
    });

    librg_event_add(LIBRG_CONNECTION_REQUEST, on_connect_request);
    librg_event_add(LIBRG_CONNECTION_ACCEPT, on_connect_accepted);

    librg_network_start((librg_address_t) { .port = 27010 });

    for (isize i = 0; i < 10000; i++) {
        librg_entity_t enemy = librg_entity_create(0);
        librg_transform_t *transform = librg_fetch_transform(enemy);
        transform->position.x = (float)(2000 - rand() % 4000);
        transform->position.y = (float)(2000 - rand() % 4000);
    }

    while (true) {
        librg_tick();
        zpl_sleep_ms(1);
    }

    librg_network_stop();
    librg_free();

    return 0;
}
