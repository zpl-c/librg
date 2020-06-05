#define LIBRG_IMPL
#include "librg.h"

int main() {
    librg_world *world = librg_world_create();

    /* create our world configuration */
    librg_config_chunksize_set(world, 16, 16, 16);
    librg_config_chunkamount_set(world, 9, 9, 9);
    librg_config_chunkoffset_set(world, LIBRG_OFFSET_MID, LIBRG_OFFSET_MID, LIBRG_OFFSET_MID);

    /* track an existing game entity and set it's params */
    librg_entity_track(world, 1);
    librg_entity_owner_set(world, 1, 1);
    librg_entity_chunk_set(world, 1, 1);
    librg_entity_radius_set(world, 1, 1);

    /* fetch entities via query */
    int64_t entities[64] = {0};
    size_t entity_amount = 64;
    librg_world_query(world, 1, entities, &entity_amount);

    /* write owner's point of view to a buffer */
    char buffer[256] = {0};
    size_t buffer_legnth = 256;
    librg_world_write(world, 1, buffer, &buffer_legnth, NULL);
    printf("written a buffer of length %d\n", (int)buffer_legnth);

    librg_world_destroy(world);
    return 0;
}
