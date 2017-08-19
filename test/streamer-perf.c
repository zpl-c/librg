#define LIBRG_IMPLEMENTATION
#define LIBRG_DEBUG
#include <librg.h>

#define measure(TITLE, CODE) do { \
        u64 start  = zpl_utc_time_now(); CODE; \
        f32 result = (zpl_utc_time_now() - start) / 1000.0; \
        librg_log("%s: took %f ms.\n", TITLE, result); \
    } while(0)

int main() {

    librg_init((librg_cfg_t) {
        .tick_delay     = 32,
        .mode           = librg_server_ev,
        .world_size     = zplm_vec2(5000.0f, 5000.0f),
        .entity_limit   = 128000,
    });

    librg_entity_t entity = librg_entity_create();
    librg_fetch_streamable(entity)->range = 250;

    // insertion and deletion of 48k entities
    srand(0xDEADBEEF);

    measure("creating 48k entities", {
        for (int i = 0; i < 48000; i++) {
            librg_entity_t enemy = librg_entity_create();
            librg_transform_t *transform = librg_fetch_transform(enemy);
            transform->position.x = (float)(2000 - rand() % 4000);
            transform->position.y = (float)(2000 - rand() % 4000);
        }
    });

    measure("creating insertions for 48k entities", {
        librg__streamer_update();
    });

    // clean up old entities
    for (int i = 1; i <= 48000; i++)
        librg_entity_destroy(librg_entity_get(i));
    librg__entity_execute_destroy();

    // fill up new entities
    for (int i = 0; i < 48000; i++) {
        librg_entity_t enemy = librg_entity_create();
        librg_fetch_transform(enemy)->position = zplm_vec3((float)i, (float)i, 10);
    }

    librg__streamer_update();

    // start = zpl_utc_time_now();
    // zpl_array_t(librg_entity_t) res = librg_streamer_query(entity);
    // result = (zpl_utc_time_now() - start) / 1000.0;
    // librg_log("query of 48k entities with 250 units query range: took %f ms.\n", result);

    librg_free();
    return 0;
}
