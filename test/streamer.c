#define LIBRG_IMPLEMENTATION
#define LIBRG_DEBUG
#include <librg.h>

int main() {

    librg_init((librg_config_t) {
        .tick_delay     = 32,
        .mode           = LIBRG_MODE_SERVER,
        .world_size     = zplm_vec2(5000.0f, 5000.0f),
        .entity_limit   = 128000,
    });



    librg_entity_t entity = librg_entity_create(0);

    librg_fetch_transform(entity)->position = zplm_vec3(30, 20, 0);
    librg_fetch_streamable(entity)->range = 1000;

    {
        zpl_array_t(librg_entity_t) queue = librg_streamer_query(entity);
        librg_assert(zpl_array_count(queue) == 0);
        zpl_array_free(queue);
    }

    zplc_clear(&librg__streamer);

    // should be able to return exactly 1 entity
    {
        librg_entity_t friendly = librg_entity_create(0);
        librg_fetch_transform(friendly)->position = zplm_vec3(30, 20, 10);

        librg__streamer_update();

        zpl_array_t(librg_entity_t) queue = librg_streamer_query(entity);
        librg_assert(zpl_array_count(queue) == 2);
        zpl_array_free(queue);

        librg_entity_destroy(friendly);
    }

    librg__entity_execute_destroy();
    zplc_clear(&librg__streamer);

    // should be able to return exactly 666 entities
    {
        for (int i = 0; i < 666; i++) {
            librg_entity_t enemy = librg_entity_create(0);
            librg_fetch_transform(enemy)->position = zplm_vec3(i, 20, 10);
        }

        librg__streamer_update();

        zpl_array_t(librg_entity_t) queue = librg_streamer_query(entity);
        librg_assert(zpl_array_count(queue) == 667);
        zpl_array_free(queue);

        for (int i = 1; i <= 666; i++) {
            librg_entity_destroy(librg_entity_get(i));
        }
    }

    librg__entity_execute_destroy();
    zplc_clear(&librg__streamer);

    // should be able to return less than 32k entities
    {
        for (int i = 0; i < 48000; i++) {
            librg_entity_t enemy = librg_entity_create(0);
            librg_fetch_transform(enemy)->position = zplm_vec3((float)i, 20, 10);
        }

        librg__streamer_update();

        zpl_array_t(librg_entity_t) queue = librg_streamer_query(entity);
        librg_assert(zpl_array_count(queue) <= 32000);
        zpl_array_free(queue);

        for (int i = 1; i <= 48000; i++) {
            librg_entity_destroy(librg_entity_get(i));
        }
    }

    librg__entity_execute_destroy();
    zplc_clear(&librg__streamer);

    // should be able to blacklist 1 entity globally
    {
        librg_entity_t badentity = librg_entity_create(0);
        librg_fetch_transform(badentity)->position = zplm_vec3(30, 20, 10);
        librg__streamer_update();

        librg_streamer_set_visible(badentity, false);
        {
            zpl_array_t(librg_entity_t) queue = librg_streamer_query(entity);
            librg_assert(zpl_array_count(queue) == 1);
            zpl_array_free(queue);
        }

        librg_streamer_set_visible(badentity, true);
        {
            zpl_array_t(librg_entity_t) queue = librg_streamer_query(entity);
            librg_assert(zpl_array_count(queue) == 2);
            zpl_array_free(queue);
        }

        librg_entity_destroy(badentity);
    }

    librg__entity_execute_destroy();
    zplc_clear(&librg__streamer);

    // should be able to ignore 1 entity for target entity
    {
        librg_entity_t badentity = librg_entity_create(0);
        librg_entity_t tarentity = librg_entity_create(0);
        librg__streamer_update();

        librg_streamer_set_visible_for(badentity, tarentity, false);
        {
            zpl_array_t(librg_entity_t) queue = librg_streamer_query(tarentity);
            zpl_array_t(librg_entity_t) bigge = librg_streamer_query(entity);

            librg_assert(zpl_array_count(queue) == 2 && zpl_array_count(bigge) == 3);

            zpl_array_free(queue);
            zpl_array_free(bigge);
        }

        librg_entity_destroy(badentity);
        librg_entity_destroy(tarentity);
    }

    librg_free();
    return 0;
}
