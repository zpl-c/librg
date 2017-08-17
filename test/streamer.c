#define LIBRG_IMPLEMENTATION
#define LIBRG_DEBUG
#include <librg.h>

int main() {

    librg_init((librg_cfg_t) {
        .tick_delay     = 32,
        .mode           = librg_server_ev,
        .world_size     = zplm_vec2(5000.0f, 5000.0f),
        .entity_limit   = 2048,
    });



    librg_entity_t entity = librg_entity_create();

    librg_fetch_transform(entity)->position = zplm_vec3(30, 20, 0);
    librg_fetch_streamable(entity)->range = 1000;

    {
        zpl_array_t(librg_entity_t) queue = librg_streamer_query(entity);
        librg_assert(zpl_buffer_count(queue) == 0);
        zpl_array_free(queue);
    }

    // should be able to return exactly 1 entity
    {
        zplc_clear(&librg__streamer);

        librg_entity_t friendly = librg_entity_create();
        librg_fetch_transform(friendly)->position = zplm_vec3(30, 20, 10);

        librg__streamer_update();

        zpl_array_t(librg_entity_t) queue = librg_streamer_query(entity);
        librg_assert(zpl_buffer_count(queue) == 2);
        zpl_array_free(queue);

        librg_entity_destroy(friendly);
    }

    // should be able to return exactly 666 entities
    {
        zplc_clear(&librg__streamer);

        for (int i = 0; i < 666; i++) {
            librg_entity_t enemy = librg_entity_create();
            librg_fetch_transform(enemy)->position = zplm_vec3(i, 20, 10);
        }

        librg__streamer_update();

        zpl_array_t(librg_entity_t) queue = librg_streamer_query(entity);
        librg_log("amount: %tu\n", zpl_buffer_count(queue));
        librg_assert(zpl_buffer_count(queue) == 668);
        zpl_array_free(queue);
    }

    // librg::streamer::clear();

    // it("should be able to return less than 32k entities", [entity](vald_t validate) {
    //     auto newEntity = librg::entities->create();
    //     newEntity.assign<librg::streamable_t>(hmm_vec3{ 30000, 30000, 30000 });
    //     newEntity.assign<librg::transform_t>();
    //     for (int i = 0; i < 48000; i++) {
    //         auto enemy = librg::entities->create();
    //         enemy.assign<librg::streamable_t>(hmm_vec3{ 300, 300, 300 });
    //         auto ft = librg::transform_t();
    //         ft.position = hmm_vec3{ (float)i,30.f,10 };
    //         enemy.assign<librg::transform_t>(ft);

    //         librg::streamer::insert(enemy);
    //     }
    //     auto queue = librg::streamer::query(newEntity);
    //     validate(queue.size() <= 32000);
    // });

    // librg::streamer::clear();

    // it("should be able to blacklist 1 entity globally", [entity](vald_t validate) {
    //     auto badEntity = librg::entities->create();
    //     badEntity.assign<librg::streamable_t>(hmm_vec3{ 100, 100, 100 });
    //     badEntity.assign<librg::transform_t>();
    //     librg::streamer::set_visible(badEntity, false);

    //     auto goodEntity = librg::entities->create();
    //     goodEntity.assign<librg::streamable_t>(hmm_vec3{ 100, 100, 100 });
    //     goodEntity.assign<librg::transform_t>();

    //     librg::streamer::insert(badEntity);
    //     librg::streamer::insert(goodEntity);

    //     auto queue = librg::streamer::query(entity);
    //     validate(queue.size() == 1);
    // });

    // librg::streamer::clear();

    // it("should be able to ignore 1 entity for target entity", [entity](vald_t validate) {
    //     auto badEntity = librg::entities->create();
    //     badEntity.assign<librg::streamable_t>(hmm_vec3{ 100, 100, 100 });
    //     badEntity.assign<librg::transform_t>();
    //     //librg::streamer::set_visible(badEntity, false);

    //     auto goodEntity = librg::entities->create();
    //     goodEntity.assign<librg::streamable_t>(hmm_vec3{ 100, 100, 100 });
    //     goodEntity.assign<librg::transform_t>();

    //     auto targetEntity = librg::entities->create();
    //     targetEntity.assign<librg::streamable_t>(hmm_vec3{ 100, 100, 100 });
    //     targetEntity.assign<librg::transform_t>();

    //     librg::streamer::insert(badEntity);
    //     librg::streamer::insert(goodEntity);
    //     librg::streamer::insert(targetEntity);

    //     librg::streamer::set_visible_for(targetEntity, badEntity, false);

    //     auto queue = librg::streamer::query(targetEntity);
    //     auto biggerQueue = librg::streamer::query(goodEntity);
    //     validate(queue.size() == 1 && biggerQueue.size() == 2);
    // });

    // while (true) {
    //     librg_tick();
    // }

    librg_free();
    return 0;
}
