#define LIBRG_IMPL
// #define LIBRG_ENTITY_MAXCHUNKS 1
#include "librg.h"

#define MAX_ENTITY 1000
#define QUERY_ATTEMPTS 1000

int main() {
    librg_world *world = librg_world_create();

    /* create our world configuration */
    librg_config_chunksize_set(world, 16, 16, 0);
    librg_config_chunkamount_set(world, 64, 64, 0);
    librg_config_chunkoffset_set(world, LIBRG_OFFSET_BEG, LIBRG_OFFSET_BEG, LIBRG_OFFSET_BEG);

    zpl_f64 tstart = zpl_time_rel_ms();

    /* create set of testing entities */
    for (int i = 0; i < MAX_ENTITY; ++i) {
        int chx = rand() % 64;
        int chy = rand() % 64;

        librg_entity_track(world, i);
        librg_chunk chid = librg_chunk_from_chunkpos(world, chx, chy, 0);
        librg_entity_chunk_set(world, i, chid);
    }

    zpl_printf("[test] tracked %d entities in (%.3f ms)\n", librg_world_entities_tracked(world), zpl_time_rel_ms() - tstart);

    /* set owner to a single entity */
    int ownerid = 1;
    librg_entity_owner_set(world, 0, ownerid);

    /* fetch entities via query */
    int64_t entities[1000] = {0};
    size_t entity_amount = 1000;
    int query_radius = 16;

    tstart = zpl_time_rel();

    for (int i = 0; i < QUERY_ATTEMPTS; ++i) {
        size_t entity_limit = 1000;
        librg_world_query(world, ownerid, query_radius, entities, &entity_limit);
        entity_amount = entity_limit;
    }

    zpl_printf("[test] found %d entities in (%.3f ms)\n", entity_amount, zpl_time_rel_ms() - tstart);

    /* results */
    //
    // before switch to internal memory
    // [test] found 171 entities in (7800.685 ms)
    // [test] found 171 entities in (7789.673 ms)
    // [test] found 171 entities in (7804.661 ms)
    //
    // after switch to internal memory
    // [test] found 171 entities in (4308.592 ms)
    // [test] found 171 entities in (4234.625 ms)
    // [test] found 171 entities in (4221.636 ms)

    librg_world_destroy(world);
    return 0;
}
