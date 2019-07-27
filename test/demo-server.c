#define LIBRG_DEBUG
#define LIBRG_IMPLEMENTATION
#include <librg.h>

#define DEMO_SERVER

enum {
    DEMO_SPAWN_BLOCK = LIBRG_EVENT_LAST,
};

enum {
    DEMO_TYPE_PLAYER,
    DEMO_TYPE_NPC,
};

// enum {
//     component_hero = librg_component_last,
// };

typedef struct hero {

    struct {
        zpl_vec3 accel;
        f32 walk_time;
        f32 cooldown;
        i32 max_hp;
        i32 cur_hp;
    } stream;

#ifdef DEMO_CLIENT
    // interpolation
    f32 delta;
    zpl_vec3 curr_pos, last_pos, target_pos;
#endif

    librg_entity *follower1;
    librg_entity *follower2;
    librg_entity *follower3;
} hero;

// generate methods for components
// librg_component(hero, component_hero, hero);


void on_connect_request(librg_event *event) {
    u32 secret = librg_data_ru32(event->data);

    // librg_log(" ==== secret is %d\n", secret);
    if (secret != 42) {
        librg_event_reject(event);
    }
}

void on_connect_accepted(librg_event *event) {
    librg_log("on_connect_accepted\n");

    librg_log("spawning player %u at: %f %f %f\n",
        event->entity->id,
        event->entity->position.x,
        event->entity->position.y,
        event->entity->position.z
    );

    event->entity->stream_range = 250.0f;

    hero hero_ = {0};
    hero_.stream.max_hp = 100;
    hero_.stream.cur_hp = 40;

    hero_.stream.accel.x = (rand() % 3 - 1.0);
    hero_.stream.accel.y = (rand() % 3 - 1.0);

    hero *subhero1 = zpl_malloc(sizeof(hero_));
    *subhero1 = hero_;

    hero_.follower1 = librg_entity_create(event->ctx, DEMO_TYPE_PLAYER);
    hero_.follower1->user_data = subhero1;

    hero *subhero2 = zpl_malloc(sizeof(hero_));
    *subhero2 = hero_;

    hero_.follower2 = librg_entity_create(event->ctx, DEMO_TYPE_PLAYER);
    hero_.follower2->user_data = subhero2;

    hero *subhero3 = zpl_malloc(sizeof(hero_));
    *subhero3 = hero_;

    hero_.follower3 = librg_entity_create(event->ctx, DEMO_TYPE_PLAYER);
    hero_.follower3->user_data = subhero3;

    event->entity->user_data = zpl_malloc(sizeof(hero_));
    *(hero *)event->entity->user_data = hero_;

    librg_entity_world_set(event->ctx, hero_.follower1->id, 5);

    // librg_entity_visibility_set_for(event->ctx, event->entity->id, hero_.follower1->id, LIBRG_ALWAYS_INVISIBLE);
    // librg_entity_visibility_set_for(event->ctx, event->entity->id, hero_.follower2->id, LIBRG_ALWAYS_INVISIBLE);
    // librg_entity_visibility_set_for(event->ctx, event->entity->id, hero_.follower3->id, LIBRG_ALWAYS_INVISIBLE);

    librg_entity_visibility_set(event->ctx, hero_.follower1->id, LIBRG_ALWAYS_VISIBLE);
    librg_entity_visibility_set(event->ctx, hero_.follower2->id, LIBRG_ALWAYS_VISIBLE);
    librg_entity_visibility_set(event->ctx, hero_.follower3->id, LIBRG_ALWAYS_VISIBLE);

    librg_entity_control_set(event->ctx, event->entity->id, event->entity->client_peer);
}

void on_client_entity_update(librg_event *event) {
    librg_entity *follower1 = ((hero *)event->entity->user_data)->follower1;
    librg_entity *follower2 = ((hero *)event->entity->user_data)->follower2;
    librg_entity *follower3 = ((hero *)event->entity->user_data)->follower3;

    follower1->position.x = event->entity->position.x + 15;
    follower1->position.y = event->entity->position.y + 30;
    follower2->position.x = event->entity->position.x - 15;
    follower2->position.y = event->entity->position.y - 100;
    follower3->position.x = event->entity->position.x - 0;
    follower3->position.y = event->entity->position.y + 100;
}

void on_entity_create_forplayer(librg_event *event) {
     switch (event->entity->type) {
         case DEMO_TYPE_PLAYER:
         case DEMO_TYPE_NPC: {
            hero *hero_var = (hero *)event->entity->user_data;
            librg_data_wptr(event->data, event->entity->user_data, sizeof(hero_var->stream));
         } break;
     }
}

void on_entity_update_forplayer(librg_event *event) {
}


void ai_think(librg_ctx *ctx) {
    for (int i = 0; i < ctx->max_entities; i++)
    {
        if (!librg_entity_valid(ctx, i)) continue;
        librg_entity *entity = librg_entity_fetch(ctx, i);
        if (entity->type == DEMO_TYPE_NPC) {

            hero *hero = entity->user_data;

            if (hero->stream.walk_time == 0) {
                hero->stream.walk_time = 1000;
                hero->stream.accel.x += (rand() % 3 - 1.0) / 10.0;
                hero->stream.accel.y += (rand() % 3 - 1.0) / 10.0;

                hero->stream.accel.x = (hero->stream.accel.x > -1.0) ? ((hero->stream.accel.x < 1.0) ? hero->stream.accel.x : 1.0) : -1.0;
                hero->stream.accel.y = (hero->stream.accel.y > -1.0) ? ((hero->stream.accel.y < 1.0) ? hero->stream.accel.y : 1.0) : -1.0;
            }
            else {
                zpl_vec3 curpos = entity->position;

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
                if (zpl_vec3_mag2(hero->stream.accel) > PP(0.3)) {
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
    librg_ctx *ctx = (librg_ctx *)userptr;

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

    librg_ctx ctx     = {0};
    ctx.mode            = LIBRG_MODE_SERVER;
    ctx.tick_delay      = 16.66666666666 * 4;
    // ctx.tick_delay      = 1000;
    ctx.world_size      = zpl_vec3f(50000.0f, 50000.0f, 0.f);
    ctx.max_connections = 128;
    ctx.max_entities    = 16000,

    librg_init(&ctx);

    librg_event_add(&ctx, LIBRG_CONNECTION_REQUEST, on_connect_request);
    librg_event_add(&ctx, LIBRG_CONNECTION_ACCEPT, on_connect_accepted);
    librg_event_add(&ctx, LIBRG_ENTITY_CREATE, on_entity_create_forplayer);
    librg_event_add(&ctx, LIBRG_ENTITY_UPDATE, on_entity_update_forplayer);
    librg_event_add(&ctx, LIBRG_CLIENT_STREAMER_UPDATE, on_client_entity_update);

    librg_network_start(&ctx, (librg_address) { .port = 7777 });

    f64 s = zpl_time_now();

#if 1
    for (isize i = 0; i < 10000; i++) {
        librg_entity *enemy = librg_entity_create(&ctx, DEMO_TYPE_NPC);

        enemy->position.x = (float)(2000 - rand() % 4000);
        enemy->position.y = (float)(2000 - rand() % 4000);

        hero hero_ = {0};
        hero_.stream.max_hp = 100;
        hero_.stream.cur_hp = 40;

        hero_.stream.accel.x = (rand() % 3 - 1.0);
        hero_.stream.accel.y = (rand() % 3 - 1.0);

        enemy->user_data = zpl_malloc(sizeof(hero_));
        *(hero *)enemy->user_data = hero_;
    }
#endif

    librg_log("took %f\n", zpl_time_now() - s);

    zpl_timer *tick_timer = zpl_timer_add(ctx.timers);
    tick_timer->user_data = (void *)&ctx; /* provide ctx as a argument to timer */
    zpl_timer_set(tick_timer, 1.0f, -1, measure);
    zpl_timer_start(tick_timer, 0.01f);

    while (true) {
        librg_tick(&ctx);
        ai_think(&ctx);
        zpl_sleep_ms(1);
    }

    librg_network_stop(&ctx);
    librg_free(&ctx);

    return 0;
}
