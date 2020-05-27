#include <assert.h>

#define LIBRG_IMPL
#include "librg.h"


/* usage */
#if 0
int main() {

    librg_config_chunksize_set(ctx, 16, 16, 16);
    librg_config_worldsize_set(ctx, 256, 256, 256);

    /* called by a librg_world_write, to encode the data for a child */
    librg_event_set(ctx, LIBRG_PARENT_CREATE, [](librg_event *e) {
        /* returning > 0 - amount of data we've written */
        /* return 0 - no data written */
        /* return < 0 - return an error, will be pased to the parent world_write call */
        return 0;
    });

    /* called by librg_world_read, to decode the data from a parent */
    librg_event_set(ctx, LIBRG_CHILD_CREATE, [](librg_event *e) {
        /* return 0 - ok */
        /* return < 0 - return an error, will be pased to the parent world_read call */
        return 0;
    });

    /* create a few simple entities */
    librg_entity_set(ctx, 13);
    librg_entity_type_set(ctx, 13, LIBRG_STATIC); /*static entity - no update events will ever be generated for it*/

    /* additionaly, set a refresh rate */
    librg_entity_set(ctx, 14);
    librg_entity_type_set(ctx, 15, LIBRG_DYNAMIC); /* dynamic entity - entity always will be kept updated (default value) */
    librg_entity_refresh_set(ctx, 14, LIBRG_LINEAR, 20.0f); /*amount of updates shown to a specific observer*/
                                                            /*will divided by 2, every 20.0f units depending how far they are*/
    /* add an observer entity */
    librg_entity_set(ctx, 15);
    librg_entity_owner_set(ctx, 15, 100, 1); /* set owner and, mark it as observable */
    librg_entity_chunkrange_set(ctx, 15, 5); /* chunk range sets a viewable range by our observer */
    /* owner id can be anything, a ID from a newtork connection, a random number, or a ptr to a instance of a class */

    /* set its position */
    librg_entity_chunk_set(ctx, 15, librg_chunk_from_realpos(ctx, 15.0f, 25.0f, 353.0f));

    /* write AoI POV to buffer */
    char buffer[1024] = {0};
    int res = librg_world_write(ctx, 100, buffer, 1024);
    /* res can be one of following: */
    /* > 0: success, and amount of actual bytes written */
    /* == 0: success, empty result, no data written, empty view */
    /* < 0: error with code: */
    /* LIBRG_ERROR_NO_OBSERVERS - owner does not have any observer entities */
    /* LIBRG_ERROR_BUFFER_TOO_SMALL - unable to continue writing since buffer is too small */

    {
        /* create our 2nd instance */
        librg_ctx *ctx2 = librg_context_create();

        librg_config_chunksize_set(ctx2, 16, 16, 16);
        librg_config_worldsize_set(ctx2, 256, 256, 256);

        /* rerad the world, and re-create our limited PoV local view of the world */
        int code = librg_world_read(ctx2, LIBRG_UNDEFINED_OWNER, buffer, res);

        /* modify some entity 15 internal data */

        memset(buffer, 0, sizeof(char) * 1024);
        res = librg_world_write(ctx2, LIBRG_UNDEFINED_OWNER, buffer, 1024);
    }


    /* back to main instance, read the incoming data */
    int code = librg_data_read(ctx, 100, buffer, res);

    return 0;
}
#endif
/* impl part*/

size_t _parent_create(librg_ctx *ctx, librg_event *event) {
    printf("_parent_create %p %d\n", ctx, event->type);
    return 0;
}

size_t _child_create(librg_ctx *ctx, librg_event *event) {
    printf("_child_create %p %d\n", ctx, event->type);
    return 0;
}

int main() {
    librg_ctx *ctx = librg_context_create();
    assert(librg_context_valid(ctx));

    librg_config_worldsize_set(ctx, 9, 9, 9);
    librg_config_chunksize_set(ctx, 16, 16, 16);
    librg_config_chunkoffset_set(ctx, LIBRG_OFFSET_MID, LIBRG_OFFSET_MID, LIBRG_OFFSET_MID);

    librg_event_set(ctx, LIBRG_PARENT_CREATE, _parent_create);
    librg_event_set(ctx, LIBRG_CHILD_CREATE, _child_create);

    const int myId = 24;

    librg_entity_track(ctx, myId);
    librg_entity_owner_set(ctx, myId, 423, 1);

    for (int i=0;i<100;i++) {
        librg_entity_track(ctx, i);
        librg_entity_chunk_set(ctx, i, i);
    }

    assert(librg_entity_tracked(ctx, myId) == LIBRG_TRUE);

    librg_entity_userdata_set(ctx, myId, (void *)124);
    assert(librg_entity_userdata_get(ctx, myId) == (void *)124);

    librg_entity_type_set(ctx, myId, LIBRG_ENTITY_STATIC);
    assert(librg_entity_type_get(ctx, myId) == LIBRG_ENTITY_STATIC);

    // librg_entity_chunk_set(ctx, myId, librg_chunk_from_realpos(ctx, 0.f, 0.f, 0.f));
    librg_entity_chunk_set(ctx, myId, 3);
    printf("entity chunk: %lld\n", librg_entity_chunk_get(ctx, myId));

    int64_t results[255] = {0};
    int amount = librg_world_query(ctx, 423, results, 255);
    printf("query found: %d results\n", amount);
    for (int i=0; i<amount; i++) printf("result #%d: %lld\n", i, results[i]);

    librg_entity_dimension_set(ctx, myId, 1);
    assert(librg_entity_dimension_get(ctx, myId) == 1);

    librg_entity_untrack(ctx, myId);
    assert(librg_entity_tracked(ctx, myId) == LIBRG_FALSE);

    librg_context_destroy(ctx);
    return 0;
}
