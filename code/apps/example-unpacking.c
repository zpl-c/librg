#define LIBRG_IMPL
#include "librg.h"
#include <assert.h>

int32_t myread_create(librg_world *w, librg_event *e) {
    int64_t owner_id = librg_event_owner_get(w, e);
    int64_t entity_id = librg_event_entity_get(w, e);
    printf("entity: %d was created for owner: %d\n", (int)entity_id, (int)owner_id);

    char buffer[64] = {0};
    memcpy(buffer, librg_event_buffer_get(w, e), librg_event_size_get(w, e));
    printf("out special message: %s\n", buffer);

    return 0;
}

int32_t myread_update(librg_world *w, librg_event *e) {
    int32_t my_secret_number = -1;
    char *buffer = librg_event_buffer_get(w, e);
    memcpy(&my_secret_number, buffer, sizeof(int32_t));

    /* check our secret number */
    assert(my_secret_number == 42);
    return 0;
}

int32_t myread_remove(librg_world *w, librg_event *e) {
    int64_t owner_id = librg_event_owner_get(w, e);
    int64_t entity_id = librg_event_entity_get(w, e);
    printf("entity: %d was removed for owner: %d\n", (int)entity_id, (int)owner_id);
    return 0;
}

int main() {
    librg_world *world = librg_world_create();

    /* set our event handlers */
    librg_event_set(world, LIBRG_READ_CREATE, myread_create);
    librg_event_set(world, LIBRG_READ_UPDATE, myread_update);
    librg_event_set(world, LIBRG_READ_REMOVE, myread_remove);

    /* out data buffer */
    char buffer[256] = {0};
    size_t total_size = 0;

    {
        /* in this example we going to load existing buffer from a file */
        /* check out example-packing.c for writing demo */
        zpl_file f = {0};
        zpl_file_open_mode(&f, ZPL_FILE_MODE_READ, "build/example-data.buf");
        total_size = (size_t)zpl_file_size(&f);
        zpl_file_read(&f, buffer, total_size);
        zpl_file_close(&f);
    }

    /* owner id doesnot have to much original owner id from example-packing.c */
    /* think of it as a owner translation, if you encoded data for owner 42 */
    /* the one who reads on this side will be the one who was supposed to read it */
    /* even if his local owner id will be different */
    /* but for simplicity sake, we will keep it here 42 as well */

    const int owner = 42;
    librg_world_read(world, owner, buffer, total_size, NULL);

    /* we should expect to have 4 entities created */
    /* 3 other entities within his view radius */
    /* 1 entity is owner own entity */
    /* you can check original entity ids in example-packing.c */

    assert(librg_entity_tracked(world, 32));
    assert(librg_entity_tracked(world, 34));
    assert(librg_entity_tracked(world, 55));

    assert(librg_entity_tracked(world, 777));
    assert(librg_entity_owner_get(world, 777) == owner);

    /* cleanup */
    librg_world_destroy(world);
    return 0;
}
