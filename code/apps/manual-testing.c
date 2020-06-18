/* this file is usually used to manually test some stuff */

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
// #define LIBRG_WORLDWRITE_MAXQUERY 360
#include "librg.h"

// TODO: add librg_enet code
// TODO: add behavior support

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
    printf("version %d\n", librg_version());

    librg_world *world = librg_world_create();
    assert(librg_world_valid(world));

    librg_config_chunksize_set(world, 16, 16, 16);
    librg_config_chunkamount_set(world, 16, 16, 16);
    librg_config_chunkoffset_set(world, LIBRG_OFFSET_MID, LIBRG_OFFSET_MID, LIBRG_OFFSET_MID);

    librg_event_set(world, LIBRG_WRITE_CREATE, _parent_create);
    librg_event_set(world, LIBRG_READ_CREATE, _child_create);

    const int myId = 24;
    const int observerRadius = 1;
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

    #define RESSIZE 4096
    int64_t results[RESSIZE] = {0};

    #define BUFSIZE 10000
    char buffer[BUFSIZE] = {0};

    f64 tstart = zpl_time_now();
    size_t amount = RESSIZE;
    librg_world_query(world, 1, results, &amount);
    zpl_printf("query found %d results of %d in (%.3f ms)\n", amount, totalEnts, zpl_time_now() - tstart);
    // for (int i=0; i<amount; i++) zpl_printf("result #%d: %lld\n", i, results[i]);

    zpl_printf("> encoding...\n");

    tstart = zpl_time_now();

    size_t buffer_size = 10000;
    int32_t result = librg_world_write(world, 1, buffer, &buffer_size, NULL);

    if (result > 0) {
        printf("AAA, you didnt have enough space to write stuff in your buffer mister\n");
    }

    zpl_printf("written %zu bytes in (%.3f ms)\n", buffer_size, zpl_time_now() - tstart);

    librg_world *w2 = librg_world_create();

    librg_event_set(w2, LIBRG_WRITE_CREATE, _parent_create);
    librg_event_set(w2, LIBRG_READ_CREATE, _child_create);

    tstart = zpl_time_now();
    int r = librg_world_read(w2, 1, buffer, buffer_size, NULL);
    zpl_printf("read %zu bytes, result: %d, entities: %d in (%.3f ms)\n", buffer_size, r, librg_entity_count(w2), zpl_time_now() - tstart);

    librg_entity_untrack(world, myId);
    librg_world_destroy(world);
    librg_world_destroy(w2);
    return 0;
}
