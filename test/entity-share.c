#define LIBRG_IMPLEMENTATION
#include <librg.h>

void each(librg_entity_t entity) {
    librg_log("iterating on: %u\n", entity);
}

int main() {

    librg_init((librg_config_t) {
        .tick_delay     = 32,
        .mode           = LIBRG_MODE_CLIENT,
        .world_size     = zplm_vec2(5000.0f, 5000.0f),
        .max_entities   = 16048,
    });

    if (librg_is_server()) {
        for (int i = 0; i < 1000; ++i) {
            librg_log("creating entity: %u\n", librg_entity_create(0));
        }

        librg__entity_destroy(5);
        librg__entity_destroy(3);
        librg__entity_destroy(8);

        librg_log("creating entity: %u\n", librg_entity_create(0));
        librg_log("creating entity: %u\n", librg_entity_create(0));
        librg_log("creating entity: %u\n", librg_entity_create(0));

        // librg_log("creating entity: %u\n", librg_entity_create(0));
        // librg_log("creating entity: %u\n", librg_entity_create(0));
        // librg_log("creating entity: %u\n", librg_entity_create(0));
    }
    else {
        for (int i = 0; i < 10000; ++i) {
            librg_log("creating entity: %u\n", librg_entity_create_shared(i, 0));
        }

        librg__entity_destroy(5);
        librg__entity_destroy(3);
        librg__entity_destroy(8);

        librg_log("creating entity: %u\n", librg_entity_create(0));
        librg_log("creating entity: %u\n", librg_entity_create(0));
        librg_log("creating entity: %u\n", librg_entity_create(0));

    }

    librg_entity_filter_t filter = {
        librg_index_transform(),
    };

    librg_entity_each(filter, each);

    // for (int j = 0; j < 5; ++j) {
    //     librg_log("---------\nround: %d\n---------\n", j);
    //     for (int i = 0; i < 10; ++i) librg_entity_create(i, 0);
    //     for (int i = 0; i < 10; ++i) librg_entity_destroy(i);
    // }

    librg_free();
    return 0;
}
