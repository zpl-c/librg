#define LIBRG_DEBUG
#define LIBRG_IMPLEMENTATION
#define LIBRG_LIMITER_IMPLEMENTATION
#include <librg.h>
#include <librg_limiter.h>

#define DEMO_SERVER
#include "demo-defines.h"

void on_connect_request(librg_event_t *event) {
    if (librg_data_ru32(event->data) != 42) {
        librg_event_reject(event);
    }
}

void on_connect_accepted(librg_event_t *event) {
    librg_log("on_connect_accepted\n");

    librg_log("spawning player %u at: %f %f %f\n",
        event->entity->id,
        event->entity->position.x,
        event->entity->position.y,
        event->entity->position.z
    );

    librg_entity_control_set(event->ctx, event->entity->id, event->entity->client_peer);
}

void on_entity_create_forplayer(librg_event_t *event) {
     switch (event->entity->type) {
         case DEMO_TYPE_PLAYER:
             break;
         case DEMO_TYPE_NPC: {
            hero_t *hero = (hero_t *)event->entity->user_data;
            librg_data_wptr(event->data, event->entity->user_data, sizeof(hero->stream));
         } break;
     }
}

void on_entity_update_forplayer(librg_event_t *event) {
    // ..
}


void ai_think(librg_ctx_t *ctx) {
    for (int i = 0; i < ctx->max_entities; i++)
    {
        if (!librg_entity_valid(ctx, i)) continue;
        librg_entity_t *entity = librg_entity_fetch(ctx, i);
        if (entity->type == DEMO_TYPE_NPC) {

            hero_t *hero = entity->user_data;

            if (hero->stream.walk_time == 0) {
                hero->stream.walk_time = 1000;
                hero->stream.accel.x += (rand() % 3 - 1.0) / 10.0;
                hero->stream.accel.y += (rand() % 3 - 1.0) / 10.0;

                hero->stream.accel.x = (hero->stream.accel.x > -1.0) ? ((hero->stream.accel.x < 1.0) ? hero->stream.accel.x : 1.0) : -1.0;
                hero->stream.accel.y = (hero->stream.accel.y > -1.0) ? ((hero->stream.accel.y < 1.0) ? hero->stream.accel.y : 1.0) : -1.0;
            }
            else {
                zplm_vec3_t curpos = entity->position;

                curpos.x += hero->stream.accel.x;
                curpos.y += hero->stream.accel.y;

                if (curpos.x < 0 || curpos.x >= 5000) {
                    curpos.x += hero->stream.accel.x * -2;
                    hero->stream.accel.x *= -1;
                }

                if (curpos.y < 0 || curpos.y >= 5000) {
                    curpos.y += hero->stream.accel.y * -2;
                    hero->stream.accel.y *= -1;
                }
#define PP(x) x*x
                if (zplm_vec3_mag2(hero->stream.accel) > PP(0.3)) {
                    entity->position = curpos;
                }
#undef PP
                hero->stream.walk_time -= 32.0f;

                if (hero->stream.walk_time < 0) {
                    hero->stream.walk_time = 0;
                }
            }
        }
    }
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

    librg_ctx_t ctx     = {0};
    ctx.mode            = LIBRG_MODE_SERVER;
    ctx.tick_delay      = 64;
    ctx.world_size      = zplm_vec3(5000.0f, 5000.0f, 0.f);
    ctx.max_connections = 128;
    ctx.max_entities    = 2000,

    librg_init(&ctx);

    librg_event_add(&ctx, LIBRG_CONNECTION_REQUEST, on_connect_request);
    librg_event_add(&ctx, LIBRG_CONNECTION_ACCEPT, on_connect_accepted);
    librg_event_add(&ctx, LIBRG_ENTITY_CREATE, on_entity_create_forplayer);
    librg_event_add(&ctx, LIBRG_ENTITY_UPDATE, on_entity_update_forplayer);

    librg_network_start(&ctx, (librg_address_t) { .port = 7777 });

#if 1
    for (isize i = 0; i < 1200; i++) {
        librg_entity_id enemy = librg_entity_create(&ctx, DEMO_TYPE_NPC);
        librg_entity_t *blob = librg_entity_fetch(&ctx, enemy);

        blob->position.x = (float)(2000 - rand() % 4000);
        blob->position.y = (float)(2000 - rand() % 4000);

        hero_t hero_ = {0};
        hero_.stream.max_hp = 100;
        hero_.stream.cur_hp = 40;

        hero_.stream.accel.x = (rand() % 3 - 1.0);
        hero_.stream.accel.y = (rand() % 3 - 1.0);

        blob->user_data = zpl_malloc(sizeof(hero_));
        *(hero_t *)blob->user_data = hero_;
        librg_limiter_init(&((hero_t *)blob->user_data)->stream.limiter);
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
