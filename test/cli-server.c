#define LIBRG_IMPLEMENTATION
#define LIBRG_DEBUG
#include <librg.h>

void on_connect_request(librg_event_t *event) {
    u32 secret = librg_data_ru32(event->data);

    if (secret != 42) {
        librg_event_reject(event);
    }
}

void on_connect_accepted(librg_event_t *event) {
    librg_log("on_connect_accepted\n");
    librg_entity_t *blob = event->entity;

    blob->position.x = (float)(2000 - rand() % 4000);
    blob->position.y = (float)(2000 - rand() % 4000);

    librg_log("spawning player %u at: %f %f %f\n",
        event->entity->id,
        blob->position.x,
        blob->position.y,
        blob->position.z
    );
}

void on_connect_refused(librg_event_t *event) {
    librg_log("on_connect_refused\n");
}

void on_entity_create(librg_event_t *event) {

}

void on_entity_update(librg_event_t *event) {

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

    librg_ctx_t ctx     = {0};

    ctx.tick_delay      = 1000;
    ctx.mode            = LIBRG_MODE_SERVER;
    ctx.world_size      = zplm_vec3(5000.0f, 5000.0f, 0.f);
    ctx.min_branch_size = zplm_vec3(-1, -1, -1);
    ctx.max_entities    = 60000;
    ctx.max_connections = 1200;

    librg_init(&ctx);

    librg_event_add(&ctx, LIBRG_CONNECTION_REQUEST, on_connect_request);
    librg_event_add(&ctx, LIBRG_CONNECTION_ACCEPT, on_connect_accepted);
    librg_event_add(&ctx, LIBRG_CONNECTION_REFUSE, on_connect_refused);

    librg_event_add(&ctx, LIBRG_ENTITY_CREATE, on_entity_create);
    librg_event_add(&ctx, LIBRG_ENTITY_UPDATE, on_entity_update);

    librg_network_start(&ctx, (librg_address_t) { .host = "localhost", .port = 7777 });

    for (isize i = 0; i < 10000; i++) {
        librg_entity_t *enemy = librg_entity_create(&ctx, 0);

        //librg_attach_foo(&ctx, enemy, NULL);
        enemy->position.x = (float)(2000 - rand() % 4000);
        enemy->position.y = (float)(2000 - rand() % 4000);
    }

    zpl_timer_t *tick_timer = zpl_timer_add(ctx.timers);
    tick_timer->user_data = (void *)&ctx; /* provide ctx as a argument to timer */
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
