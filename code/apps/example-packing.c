#define LIBRG_IMPL
#include "librg.h"

int32_t mywrite_create(librg_world *w, librg_event *e) {
    char *buffer = librg_event_buffer_get(w, e);
    const char *str = "Hello world!";

    /* make sure we have enough space */
    if ((int32_t)strlen(str) > librg_event_size_get(w, e))
        return LIBRG_WRITE_REJECT;

    /* feel free to replace memcpy with any serialization library */
    /* like: protobuf, flatbuf, msgpack, cap'n'proto, etc */
    memcpy(buffer, str, strlen(str));

    int64_t owner_id = librg_event_owner_get(w, e);
    int64_t entity_id = librg_event_entity_get(w, e);
    printf("entity: %d was created for owner: %d\n", (int)entity_id, (int)owner_id);

    return strlen(str);
}

int32_t mywrite_update(librg_world *w, librg_event *e) {
    int32_t my_secret_number = 42;
    char *buffer = librg_event_buffer_get(w, e);
    memcpy(buffer, &my_secret_number, sizeof(int32_t));
    return sizeof(int32_t);
}

int32_t mywrite_remove(librg_world *w, librg_event *e) {
    int64_t owner_id = librg_event_owner_get(w, e);
    int64_t entity_id = librg_event_entity_get(w, e);
    printf("entity: %d was removed for owner: %d\n", (int)entity_id, (int)owner_id);
    return 0;
}

int main() {
    #define entity_amt 4
    const int entities[entity_amt] = {
        32, 34, 55, 67
    };

    librg_world *world = librg_world_create();

    /* set our event handlers */
    librg_event_set(world, LIBRG_WRITE_CREATE, mywrite_create);
    librg_event_set(world, LIBRG_WRITE_UPDATE, mywrite_update);
    librg_event_set(world, LIBRG_WRITE_REMOVE, mywrite_remove);

    /* track all of our world entities, put each in its own chunk */
    for (int i = 0; i < entity_amt; ++i) {
        librg_entity_track(world, entities[i]);
        librg_entity_chunk_set(world, entities[i], i);
    }

    /* and track a special entity for our owner */
    const int owner = 42;
    const int owner_entity = 777;
    librg_entity_track(world, owner_entity);
    librg_entity_chunk_set(world, owner_entity, 1);
    librg_entity_owner_set(world, owner_entity, owner);
    librg_entity_radius_set(world, owner_entity, 1);

    /* since we've placed our entities in chunks 0,1,2,3 */
    /* and our owner entity is in chunk 1, with visibility radius of 1 */
    /* it means it should see only entities in chunks 0,1,2 */
    /* (3 other entities plus itself from 5 total) */

    /* write owner viewport to the buffer */
    char buffer[256] = {0}; size_t total_size = 256;
    librg_world_write(world, owner, buffer, &total_size, NULL);

    /* now our buffer contains packed world data */
    /* specific for that given owner id (42) */
    /* you can send this data over the network */
    /* store it in memory or save to a file */
    /* and later on, read the data using librg_world_read */

    {
        /* in this example we going to save buffer to a file */
        /* check out example-unpacking.c for reading demo */
        zpl_file f = {0};
        zpl_file_open_mode(&f, ZPL_FILE_MODE_WRITE, "build/example-data.buf");
        zpl_file_write(&f, buffer, total_size);
        zpl_file_close(&f);
    }

    /* cleanup */
    librg_world_destroy(world);
    return 0;
}
