#define LIBRG_IMPLEMENTATION
#define LIBRG_DEBUG
#include <librg.h>

enum {
    TYPE_VEHICLE = 242,
};

typedef struct {
    zplm_vec3_t a;
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
    //librg_attach_foo(event->ctx, event->entity, NULL);

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
    //foo_t *foo = librg_fetch_foo(event->ctx, event->entity);
    //librg_data_wptr(event->data, foo, sizeof(foo_t));
}

void on_entity_update(librg_event_t *event) {
	librg_data_wf32(event->data, 42);// librg_fetch_foo(event->ctx, event->entity)->a.x);
}

void on_components_register(librg_ctx_t *ctx) {
    //librg_component_register(ctx, component_foo, sizeof(foo_t));
}

void custom_handler(librg_message_t *msg) {

}

void measure(void *userptr) {
    librg_ctx_t *ctx = (librg_ctx_t *)userptr;

    if (!ctx || !ctx->network.host) return;

    static u32 lastdl = 0;
    static u32 lastup = 0;

    f32 dl = (ctx->network.host->totalReceivedData - lastdl) * 8.0f / ( 1000.0f * 1000 ) ; // mbps
    f32 up = (ctx->network.host->totalSentData     - lastup) * 8.0f / ( 1000.0f * 1000 ) ; // mbps

    lastdl = ctx->network.host->totalReceivedData;
    lastup = ctx->network.host->totalSentData;

    librg_log("librg_update: took %f ms. Current used bandwidth D/U: (%f / %f) mbps. \r", ctx->last_update, dl, up);
}

int main() {
    char *test = "===============      SERVER      =================\n" \
                 "==                                              ==\n" \
                 "==                 ¯\\_(ツ)_/¯                   ==\n" \
                 "==                                              ==\n" \
                 "==================================================\n";
    librg_log("%s\n\n", test);

    librg_option_set(LIBRG_MAX_ENTITIES_PER_BRANCH, 4);
	librg_option_set(LIBRG_MAX_THREADS_PER_UPDATE, 8);

    librg_ctx_t ctx     = {0};
    ctx.tick_delay      = 1000;
    ctx.mode            = LIBRG_MODE_SERVER;
    ctx.world_size      = zplm_vec3(5000.0f, 5000.0f, 0.f);
    ctx.max_entities    = 2500;
    ctx.max_connections = 1200;

    librg_init(&ctx, on_components_register);

    librg_event_add(&ctx, LIBRG_CONNECTION_REQUEST, on_connect_request);
    librg_event_add(&ctx, LIBRG_CONNECTION_ACCEPT, on_connect_accepted);
    librg_event_add(&ctx, LIBRG_CONNECTION_REFUSE, on_connect_refused);

    librg_event_add(&ctx, LIBRG_ENTITY_CREATE, on_entity_create);
    librg_event_add(&ctx, LIBRG_ENTITY_UPDATE, on_entity_update);

    librg_network_add(&ctx, 42, custom_handler);

    librg_network_start(&ctx, (librg_address_t) { .host = "localhost", .port = 7777 });

    for (isize i = 0; i < 1000; i++) {
        librg_entity_t enemy = librg_entity_create(&ctx, 0);
        librg_transform_t *transform = librg_fetch_transform(&ctx, enemy);
        //librg_attach_foo(&ctx, enemy, NULL);
        transform->position.x = (float)(2000 - rand() % 4000);
        transform->position.y = (float)(2000 - rand() % 4000);
    }

    zpl_timer_t *tick_timer = zpl_timer_add(ctx.timers);
    tick_timer->userptr = (void *)&ctx; /* provide ctx as a argument to timer */
    zpl_timer_set(tick_timer, 1000 * 1000, -1, measure);
    zpl_timer_start(tick_timer, 1000);


    while (true) {
        librg_tick(&ctx);
        zpl_sleep_ms(1);
    }

    librg_network_stop(&ctx);
    librg_free(&ctx);

    return 0;
}
