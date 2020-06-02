#include <assert.h>
// #include <stdlib.h>
// #include <stdio.h>

// void *myalloc(size_t size) {
//     void *ptr = malloc(size);
//     printf("allocating mem[%zd]: 0x%llx\n", size, (uint64_t)ptr);
//     return ptr;
// }

// void myfree(void *ptr) {
//     printf("freeing mem: 0x%llx\n", (uint64_t)ptr);
//     free(ptr);
//     return;
// }

// #define LIBRG_MEM_ALLOC(x) myalloc(x)
// #define LIBRG_MEM_FREE(x) myfree(x)

#define LIBRG_IMPL
#define LIBRG_DEBUG
#define LIBRG_WORLDWRITE_MAXQUERY 40000
#include "librg.h"

// TODO: add text info header part for librg.h
// TODO: add visibility features
// TODO: add behavior support

/* usage */
#if 0
int main() {

    librg_config_chunksize_set(world, 16, 16, 16);
    librg_config_worldsize_set(world, 256, 256, 256);

    /* called by a librg_world_write, to encode the data for a child */
    librg_event_set(world, LIBRG_PARENT_CREATE, [](librg_event *e) {
        /* returning > 0 - amount of data we've written */
        /* return 0 - no data written */
        /* return < 0 - return an error, will be pased to the parent world_write call */
        return 0;
    });

    /* called by librg_world_read, to decode the data from a parent */
    librg_event_set(world, LIBRG_CHILD_CREATE, [](librg_event *e) {
        /* return 0 - ok */
        /* return < 0 - return an error, will be pased to the parent world_read call */
        return 0;
    });

    /* create a few simple entities */
    librg_entity_set(world, 13);
    librg_entity_type_set(world, 13, LIBRG_STATIC); /*static entity - no update events will ever be generated for it*/

    /* additionaly, set a refresh rate */
    librg_entity_set(world, 14);
    librg_entity_type_set(world, 15, LIBRG_DYNAMIC); /* dynamic entity - entity always will be kept updated (default value) */
    librg_entity_refresh_set(world, 14, LIBRG_LINEAR, 20.0f); /*amount of updates shown to a specific observer*/
                                                            /*will divided by 2, every 20.0f units depending how far they are*/
    /* add an observer entity */
    librg_entity_set(world, 15);
    librg_entity_owner_set(world, 15, 100, 1); /* set owner and, mark it as observable */
    librg_entity_chunkrange_set(world, 15, 5); /* chunk range sets a viewable range by our observer */
    /* owner id can be anything, a ID from a newtork connection, a random number, or a ptr to a instance of a class */

    /* set its position */
    librg_entity_chunk_set(world, 15, librg_chunk_from_realpos(world, 15.0f, 25.0f, 353.0f));

    /* write AoI POV to buffer */
    char buffer[1024] = {0};
    int res = librg_world_write(world, 100, buffer, 1024);
    /* res can be one of following: */
    /* > 0: success, and amount of actual bytes written */
    /* == 0: success, empty result, no data written, empty view */
    /* < 0: error with code: */
    /* LIBRG_ERROR_NO_OBSERVERS - owner does not have any observer entities */
    /* LIBRG_ERROR_BUFFER_TOO_SMALL - unable to continue writing since buffer is too small */

    {
        /* create our 2nd instance */
        librg_world *world2 = librg_world_create();

        librg_config_chunksize_set(world2, 16, 16, 16);
        librg_config_worldsize_set(world2, 256, 256, 256);

        /* rerad the world, and re-create our limited PoV local view of the world */
        int code = librg_world_read(world2, LIBRG_UNDEFINED_OWNER, buffer, res);

        /* modify some entity 15 internal data */

        memset(buffer, 0, sizeof(char) * 1024);
        res = librg_world_write(world2, LIBRG_UNDEFINED_OWNER, buffer, 1024);
    }


    /* back to main instance, read the incoming data */
    int code = librg_data_read(world, 100, buffer, res);

    return 0;
}
#endif
/* impl part*/

int32_t _parent_create(librg_world *world, librg_event *event) {
    zpl_unused(world);
    zpl_unused(event);
    // printf("_parent_create %p %d\n", world, event->type);
    return 0;
}

int32_t _child_create(librg_world *world, librg_event *event) {
    zpl_unused(world);
    zpl_unused(event);
    // printf("_child_create %p %d\n", world, event->type);
    return 0;
}

int main() {
    librg_world *world = librg_world_create();
    assert(librg_world_valid(world));

    librg_config_chunksize_set(world, 16, 16, 16);
    librg_config_chunkamount_set(world, 16, 16, 16);
    librg_config_chunkoffset_set(world, LIBRG_OFFSET_MID, LIBRG_OFFSET_MID, LIBRG_OFFSET_MID);

    librg_event_set(world, LIBRG_WRITE_CREATE, _parent_create);
    librg_event_set(world, LIBRG_READ_CREATE, _child_create);

    const int myId = 24;
    const int observerRadius = 3;
    const librg_chunk chunkId = librg_chunk_from_chunkpos(world, 0, 0, 0);

    librg_entity_track(world, myId);
    zpl_printf("setting chunk to: %lld\n", chunkId);
    librg_entity_chunk_set(world, myId, chunkId);
    librg_entity_owner_set(world, myId, 1);
    librg_entity_radius_set(world, myId, observerRadius);

    const int totalEnts = 40000;
    for (int i=0;i<totalEnts;i++) {
        if (librg_entity_track(world, i) == LIBRG_OK) {
            librg_entity_chunk_set(world, i, librg_chunk_from_chunkpos(world, -3+(i%6), -2+(i%4), -1+(i%2)));
        }
    }

    zpl_printf("> querying...\n");

    #define RESSIZE 65655
    int64_t results[RESSIZE] = {0};

    #define BUFSIZE 2000000
    char buffer[BUFSIZE] = {0};

    f64 tstart = zpl_time_now();
    int amount = librg_world_query(world, 1, results, RESSIZE);
    zpl_printf("query found %d results of %d in (%.3f ms)\n", amount, totalEnts, zpl_time_now() - tstart);
    // for (int i=0; i<amount; i++) zpl_printf("result #%d: %lld\n", i, results[i]);

    zpl_printf("> encoding...\n");

    tstart = zpl_time_now();
    size_t actual = librg_world_write(world, 1, buffer, BUFSIZE, NULL);
    zpl_printf("written %zu bytes in (%.3f ms)\n", actual, zpl_time_now() - tstart);

    librg_world *w2 = librg_world_create();

    librg_event_set(w2, LIBRG_WRITE_CREATE, _parent_create);
    librg_event_set(w2, LIBRG_READ_CREATE, _child_create);

    tstart = zpl_time_now();
    int r = librg_world_read(w2, 1, buffer, actual, NULL);
    zpl_printf("read %zu bytes, result: %d, entities: %d in (%.3f ms)\n", actual, r, librg_entity_count(w2), zpl_time_now() - tstart);

    librg_entity_untrack(world, myId);
    librg_world_destroy(world);
    librg_world_destroy(w2);
    return 0;
}
