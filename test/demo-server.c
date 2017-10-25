#define LIBRG_DEBUG
#define LIBRG_IMPLEMENTATION
#include <librg.h>
#include "demo-defines.h"

void on_connect_request(librg_event_t *event) {
    if (librg_data_ru32(event->data) != 42) {
        return librg_event_reject(event);
    }
}

void on_connect_accepted(librg_event_t *event) {
    librg_log("on_connect_accepted\n");

    librg_transform_t *transform = librg_fetch_transform(event->ctx, event->entity);
    librg_client_t    *client    = librg_fetch_client(event->ctx, event->entity);

    hero_t *hero = librg_attach_hero(event->ctx, event->entity, NULL);
    hero->cur_hp = 100;

    librg_log("spawning player %u at: %f %f %f\n",
        event->entity,
        transform->position.x,
        transform->position.y,
        transform->position.z
    );

    librg_entity_control_set(event->ctx, event->entity, client->peer);
}

void on_spawn_npc(librg_message_t *msg) {
    librg_transform_t tr;
    librg_data_rptr(msg->data, &tr, sizeof(librg_transform_t));

    librg_entity_t npc = librg_entity_create(msg->ctx, 1);
    *librg_fetch_transform(msg->ctx, npc) = tr;

    // librg_entity_control_remove(event->ctx, librg_entity_get(msg->peer));
}

void on_entity_create_forplayer(librg_event_t *event) {
    switch (librg_entity_type(event->ctx, event->entity)) {
        case DEMO_TYPE_PLAYER:
        case DEMO_TYPE_NPC: {
            hero_t* hero = librg_fetch_hero(event->ctx, event->entity);
            librg_data_wptr(event->data, hero, sizeof(*hero));
        } break;
    }
}

void entity_think_cb(librg_ctx_t *ctx, librg_entity_t node) {
    if (librg_entity_type(ctx, node) == DEMO_TYPE_NPC) {
        hero_t *hero = librg_fetch_hero(ctx, node);
        librg_transform_t *tran = librg_fetch_transform(ctx, node);

        if (hero->walk_time == 0) {
            hero->walk_time = 1000;
            hero->accel.x += (rand() % 3 - 1.0) / 10.0;
            hero->accel.y += (rand() % 3 - 1.0) / 10.0;

            hero->accel.x = (hero->accel.x > -1.0) ? ((hero->accel.x < 1.0) ? hero->accel.x : 1.0) : -1.0;
            hero->accel.y = (hero->accel.y > -1.0) ? ((hero->accel.y < 1.0) ? hero->accel.y : 1.0) : -1.0;
        }
        else {
            zplm_vec3_t curpos = tran->position;

            curpos.x += hero->accel.x;
            curpos.y += hero->accel.y;

            if (curpos.x < 0 || curpos.x >= 5000) {
                curpos.x += hero->accel.x * -2;
                hero->accel.x *= -1;
            }

            if (curpos.y < 0 || curpos.y >= 5000) {
                curpos.y += hero->accel.y * -2;
                hero->accel.y *= -1;
            }
#define PP(x) x*x
            if (zplm_vec3_mag2(hero->accel) > PP(0.3)) {
               tran->position = curpos;
            }
#undef PP
            hero->walk_time -= 32.0f;

            if (hero->walk_time < 0) {
                hero->walk_time = 0;
            }
        }
    }
}

void ai_think(librg_ctx_t *ctx) {
    librg_filter_t filter = { component_hero };
    librg_entity_each(ctx, filter, entity_think_cb);
}

void on_component_register(librg_ctx_t *ctx) {
    librg_component_register(ctx, component_hero, sizeof(hero_t));
}

void measure(void *userptr) {
	librg_ctx_t *ctx = (librg_ctx_t *)userptr;

	if (!ctx || !ctx->network.host) return;

	static u32 lastdl = 0;
	static u32 lastup = 0;

	f32 dl = (ctx->network.host->totalReceivedData - lastdl) * 8.0f / (1000.0f * 1000); // mbps
	f32 up = (ctx->network.host->totalSentData - lastup) * 8.0f / (1000.0f * 1000); // mbps

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

	//librg_option_set(LIBRG_MAX_THREADS_PER_UPDATE, 8);

    librg_ctx_t ctx     = {0};
    ctx.mode            = LIBRG_MODE_SERVER;
    ctx.tick_delay      = 32;
    ctx.world_size      = zplm_vec3(5000.0f, 5000.0f, 0.f);
    ctx.max_connections = 128;
    ctx.max_entities    = 15000,

    librg_init(&ctx, on_component_register);

    librg_event_add(&ctx, LIBRG_CONNECTION_REQUEST, on_connect_request);
    librg_event_add(&ctx, LIBRG_CONNECTION_ACCEPT, on_connect_accepted);
    librg_event_add(&ctx, LIBRG_ENTITY_CREATE, on_entity_create_forplayer);

    //librg_network_add(42, on_spawn_npc);

    librg_network_start(&ctx, (librg_address_t) { .port = 27010 });

#if 0
    for (int i = 0; i < 15; ++i)
    librg_fetch_transform(librg_entity_create(0))->position.x = i * 20;
#endif

#if 1
    for (isize i = 0; i < 10000; i++) {
        librg_entity_t enemy = librg_entity_create(&ctx, DEMO_TYPE_NPC);
        librg_transform_t *transform = librg_fetch_transform(&ctx, enemy);
        transform->position.x = (float)(2000 - rand() % 4000);
        transform->position.y = (float)(2000 - rand() % 4000);

        hero_t hero_ = {0};
        hero_.max_hp = 100;
        hero_.cur_hp = 40;

        hero_.accel.x = (rand() % 3 - 1.0);
        hero_.accel.y = (rand() % 3 - 1.0);

        hero_t *hero = librg_attach_hero(&ctx, enemy, &hero_);
    }
#endif

	zpl_timer_t *tick_timer = zpl_timer_add(ctx.timers);
	tick_timer->user_data = (void *)&ctx; /* provide ctx as a argument to timer */
	zpl_timer_set(tick_timer, 1000 * 1000, -1, measure);
	zpl_timer_start(tick_timer, 1000);

    while (true) {
        librg_tick(&ctx);
        ai_think(&ctx);
        zpl_sleep_ms(1);
    }

    librg_network_stop(&ctx);
    librg_free(&ctx);

    return 0;
}
