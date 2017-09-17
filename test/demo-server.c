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

    librg_transform_t *transform = librg_fetch_transform(event->entity);
    librg_client_t *client = librg_fetch_client(event->entity);
    hero_t hero_ = {0};
    hero_.max_hp = hero_.cur_hp = 100;

    hero_t *hero = librg_attach_hero(event->entity, hero_);

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
    librg_data_rptr(msg->data, &tr, sizeof(librg_transform_t));

    librg_entity_t npc = librg_entity_create(1);
    librg_attach_transform(npc, tr);

    // librg_streamer_client_remove(librg_get_client_entity(msg->peer));
}

void on_entity_create_forplayer(librg_event_t *event) {
    switch (librg_entity_type(event->entity)) {
        case DEMO_TYPE_PLAYER: 
        case DEMO_TYPE_NPC: {
            hero_t* hero = librg_fetch_hero(event->entity);

            librg_data_wptr(event->data, hero, sizeof(*hero));
        } break;
    }
}

void entity_think_cb(librg_entity_t node) {
    if (librg_entity_type(node) == DEMO_TYPE_NPC) {
        hero_t *hero = librg_fetch_hero(node);
        librg_transform_t *tran = librg_fetch_transform(node);

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

void ai_think() {
    librg_entity_filter_t filter = { librg_index_hero() };
    librg_entity_each(filter, entity_think_cb);
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
        .max_entities    = 16000,
    });

    librg_event_add(LIBRG_CONNECTION_REQUEST, on_connect_request);
    librg_event_add(LIBRG_CONNECTION_ACCEPT, on_connect_accepted);
    librg_event_add(LIBRG_ENTITY_CREATE, on_entity_create_forplayer); 

    //librg_network_add(42, on_spawn_npc);

    librg_network_start((librg_address_t) { .port = 27010 });

#if 0
    for (int i = 0; i < 15; ++i)
    librg_fetch_transform(librg_entity_create(0))->position.x = i * 20;
#endif

#if 1
    for (isize i = 0; i < 10000; i++) {
        librg_entity_t enemy = librg_entity_create(DEMO_TYPE_NPC);
        librg_transform_t *transform = librg_fetch_transform(enemy);
        transform->position.x = (float)(2000 - rand() % 4000);
        transform->position.y = (float)(2000 - rand() % 4000);

        hero_t hero_ = {0};
        hero_.max_hp = 100;
        hero_.cur_hp = 40;

        hero_.accel.x = (rand() % 3 - 1.0);
        hero_.accel.y = (rand() % 3 - 1.0);

        hero_t *hero = librg_attach_hero(enemy, hero_);
    }
#endif

    while (true) {
        librg_tick();
        ai_think();
        zpl_sleep_ms(32);
    }

    librg_network_stop();
    librg_free();

    return 0;
}
