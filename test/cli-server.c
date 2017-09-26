#define LIBRG_IMPLEMENTATION
#define LIBRG_DEBUG
#include <librg.h>

enum {
    TYPE_VEHICLE = 242,
};

typedef struct {
    zplm_vec3_t a;
    zplm_vec3_t b;
    zplm_vec3_t c;
    zplm_vec3_t d;
    zplm_vec3_t e;
    zplm_vec3_t f;
} foo_t;

enum {
    component_foo = librg_component_last,
};

librg_component(foo, component_foo, foo_t);

void on_connect_request(librg_event_t *event) {
    u32 secret = librg_data_ru32(event->data);

    if (secret != 42) {
        return librg_event_reject(event);
    }
}

void on_connect_accepted(librg_event_t *event) {
    librg_log("on_connect_accepted\n");
    librg_attach_foo(event->ctx, event->entity, NULL);

    librg_transform_t *transform = librg_fetch_transform(event->ctx, event->entity);
    transform->position.x = (float)(2000 - rand() % 4000);
    transform->position.y = (float)(2000 - rand() % 4000);

    librg_log("spawning player at: %f %f %f\n",
        transform->position.x,
        transform->position.y,
        transform->position.z
    );

}

void on_connect_refused(librg_event_t *event) {
    librg_log("on_connect_refused\n");
}

void on_entity_create(librg_event_t *event) {
    foo_t *foo = librg_fetch_foo(event->ctx, event->entity);
    if (foo)
    librg_data_wptr(event->data, foo, sizeof(foo_t));
}

void on_entity_update(librg_event_t *event) {
    librg_data_wf32(event->data, librg_fetch_foo(event->ctx, event->entity)->a.x);
}

void on_components_register(librg_ctx_t *ctx) {
    librg_component_register(ctx, component_foo, sizeof(foo_t));
}

void custom_handler(librg_message_t *msg) {

}

int main() {
    char *test = "===============      SERVER      =================\n" \
                 "==                                              ==\n" \
                 "==                 ¯\\_(ツ)_/¯                   ==\n" \
                 "==                                              ==\n" \
                 "==================================================\n";
    librg_log("%s\n\n", test);


    librg_ctx_t ctx     = {0};
    ctx.tick_delay      = 1000;
    ctx.mode            = LIBRG_MODE_SERVER;
    ctx.world_size      = zplm_vec3(5000.0f, 5000.0f, 0.f);
    ctx.max_entities    = 12000;
    ctx.max_connections = 1500;

    librg_init(&ctx, on_components_register);

    librg_event_add(&ctx, LIBRG_CONNECTION_REQUEST, on_connect_request);
    librg_event_add(&ctx, LIBRG_CONNECTION_ACCEPT, on_connect_accepted);
    librg_event_add(&ctx, LIBRG_CONNECTION_REFUSE, on_connect_refused);

    librg_event_add(&ctx, LIBRG_ENTITY_CREATE, on_entity_create);
    librg_event_add(&ctx, LIBRG_ENTITY_UPDATE, on_entity_update);

    librg_network_add(&ctx, 42, custom_handler);

    librg_network_start(&ctx, (librg_address_t) { .host = "localhost", .port = 7777 });

    for (isize i = 0; i < 10000; i++) {
        librg_entity_t enemy = librg_entity_create(&ctx, 0);
        librg_transform_t *transform = librg_fetch_transform(&ctx, enemy);
        librg_attach_foo(&ctx, enemy, NULL);
        transform->position.x = (float)(2000 - rand() % 4000);
        transform->position.y = (float)(2000 - rand() % 4000);
    }

    while (true) {
        librg_tick(&ctx);
        librg_log("librg_update: took %f ms.      \r", ctx.last_update);

        zpl_sleep_ms(100);
    }

    librg_network_stop(&ctx);
    librg_free(&ctx);

    return 0;
}
