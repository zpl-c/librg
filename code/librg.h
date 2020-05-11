#ifndef LIBRG_H
#define LIBRG_H

#include <stdint.h>
#include <stddef.h>

#define LIBRG_VERSION_MAJOR 6
#define LIBRG_VERSION_MINOR 0
#define LIBRG_VERSION_PATCH 0
#define LIBRG_VERSION_PRE ""

#define LIBRG_API

typedef void librg_ctx;
typedef int32_t librg_chunk;
typedef int32_t librg_owner;

#ifndef librg_entity_id
typedef librg_entity_id int32_t;
#endif

enum librg_errors {
    LIBRG_INVALID_ENTITY    = -1,
    LIBRG_INVALID_CLIENT    = -2,
    LIBRG_INVALID_CONTEXT   = -3,
    LIBRG_INVALID_CHUNK     = -4,
};

typedef enum librg_events {
    LIBRG_PARENT_CREATE,
    LIBRG_PARENT_UPDATE,
    LIBRG_PARENT_REMOVE,

    LIBRG_CHILD_CREATE,
    LIBRG_CHILD_UPDATE,
    LIBRG_CHILD_REMOVE,
} librg_event_id;

typedef enum librg_entity_type {
    LIBRG_STATIC,
    LIBRG_DYNAMIC,
};

typedef enum librg_entity_refreshing {
    LIBRG_TICK, /* int argument, update entity every Nth tick */
    LIBRG_LINEAR, /* float argument, linearly decrease update interval based on distance step (argument) */
    LIBRG_CUBIC, /* float argument, cubicly decrease update inverval based on distance step (argument) */
};

typedef struct {
    librg_ctx   *ctx;
    void        *data;
    size_t       size;
    uint32_t     flags;
    librg_owner client;
    librg_entity_id entity;
} librg_event;

typedef int (librg_event_callback)(librg_event *event);

LIBRG_API librg_ctx *   librg_context_create();
LIBRG_API int           librg_context_destroy(librg_ctx *);
LIBRG_API int           librg_context_valid(librg_ctx *);
LIBRG_API int           librg_context_userdata_set(librg_ctx *, void *data);
LIBRG_API void *        librg_context_userdata_get(librg_ctx *);

LIBRG_API int           librg_config_chunksize_set(librg_ctx *, uint16_t x, uint16_t y, uint16_t z);
LIBRG_API int           librg_config_chunksize_get(librg_ctx *, uint16_t *x, uint16_t *y, uint16_t *z);
LIBRG_API int           librg_config_worldsize_set(librg_ctx *, uint16_t x, uint16_t y, uint16_t z);
LIBRG_API int           librg_config_worldsize_get(librg_ctx *, uint16_t *x, uint16_t *y, uint16_t *z);
LIBRG_API int           librg_config_entitylimit_set(librg_ctx *, size_t limit);
LIBRG_API size_t        librg_config_entitylimit_get(librg_ctx *);

LIBRG_API int           librg_world_write(librg_ctx *, librg_owner owner, char *buffer, size_t limit);
LIBRG_API int           librg_world_read(librg_ctx *, librg_owner owner, char *buffer, size_t size);

LIBRG_API int           librg_event_add(librg_ctx *, librg_event_id, librg_event_callback);
LIBRG_API int           librg_event_remove(librg_ctx *, librg_event_id);
LIBRG_API int           librg_event_reject(librg_ctx *, librg_event *);

LIBRG_API int           librg_entity_add(librg_ctx *, librg_entity_id);
LIBRG_API int           librg_entity_remove(librg_ctx *, librg_entity_id);
LIBRG_API int           librg_entity_valid(librg_ctx *, librg_entity_id);
LIBRG_API int           librg_entity_userdata_set(librg_ctx *, librg_entity_id, void *data);
LIBRG_API void *        librg_entity_userdata_get(librg_ctx *, librg_entity_id);

LIBRG_API size_t        librg_entity_query(librg_ctx *, librg_entity_id, librg_entity **results, size_t buffer_limit);
LIBRG_API size_t        librg_entity_fetch_all(librg_ctx *, librg_entity **results, size_t buffer_limit);
LIBRG_API size_t        librg_entity_fetch_chunk(librg_ctx *, librg_chunk, librg_entity **results, size_t buffer_limit);
LIBRG_API size_t        librg_entity_fetch_chunkarray(librg_ctx *, librg_chunk *, size_t chunk_amount, librg_entity **results, size_t buffer_limit);

LIBRG_API int           librg_entity_type_set(librg_ctx *, librg_entity_id, int type);
LIBRG_API int           librg_entity_type_get(librg_ctx *, librg_entity_id);
LIBRG_API int           librg_entity_chunk_set(librg_ctx *, librg_entity_id, librg_chunk);
LIBRG_API librg_chunk   librg_entity_chunk_get(librg_ctx *, librg_entity_id);
LIBRG_API int           librg_entity_chunkarray_set(librg_ctx *, librg_entity_id, librg_chunk *, size_t chunk_amount);
LIBRG_API size_t        librg_entity_chunkarray_get(librg_ctx *, librg_entity_id, librg_chunk **results);
LIBRG_API int           librg_entity_owner_set(librg_ctx *, librg_entity_id, librg_owner, int observer);
LIBRG_API librg_owner   librg_entity_owner_get(librg_ctx *, librg_entity_id);
LIBRG_API int           librg_entity_chunkrange_set(librg_ctx *, librg_entity_id, int8_t range);
LIBRG_API int8_t        librg_entity_chunkrange_get(librg_ctx *, librg_entity_id);
LIBRG_API int           librg_entity_refresh_set(librg_ctx *, librg_entity_id, int type, float value);
LIBRG_API int           librg_entity_refresh_get(librg_ctx *, librg_entity_id, int *type, float *value);
LIBRG_API int           librg_entity_dimension_set(librg_ctx *, librg_entity_id, int32_t dimension);
LIBRG_API int32_t       librg_entity_dimension_get(librg_ctx *, librg_entity_id);

LIBRG_API librg_entity  librg_entity_from_client(librg_ctx *, librg_owner);
LIBRG_API librg_owner   librg_client_from_entity(librg_ctx *, librg_entity_id);
LIBRG_API librg_chunk   librg_chunk_from_realpos(librg_ctx *, double x, double y, double z);
LIBRG_API librg_chunk   librg_chunk_from_chunkpos(librg_ctx *, int16_t chunk_x, int16_t chunk_y, int16_t chunk_z);


/* usage */
int main() {
    librg_ctx *ctx = librg_context_create();

    librg_config_chunksize_set(ctx, 16, 16, 16);
    librg_config_worldsize_set(ctx, 256, 256, 256);

    /* called by a librg_world_write, to encode the data for a child */
    librg_event_add(ctx, LIBRG_PARENT_CREATE, [](librg_event *e) {
        /* returning > 0 - amount of data we've written */
        /* return 0 - no data written */
        /* return < 0 - return an error, will be pased to the parent world_write call */
        return 0;
    });

    /* called by librg_world_read, to decode the data from a parent */
    librg_event_add(ctx, LIBRG_CHILD_CREATE, [](librg_event *e) {
        /* return 0 - ok */
        /* return < 0 - return an error, will be pased to the parent world_read call */
        return 0;
    });

    /* create a few simple entities */
    librg_entity_add(ctx, 13);
    librg_entity_type_set(ctx, 13, LIBRG_STATIC); /*static entity - no update events will ever be generated for it*/

    /* additionaly, set a refresh rate */
    librg_entity_add(ctx, 14);
    librg_entity_type_set(ctx, 15, LIBRG_DYNAMIC); /* dynamic entity - entity always will be kept updated (default value) */
    librg_entity_refresh_set(ctx, 14, LIBRG_LINEAR, 20.0f); /*amount of updates shown to a specific observer*/
                                                            /*will divided by 2, every 20.0f units depending how far they are*/
    /* add an observer entity */
    librg_entity_add(ctx, 15);
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
/* impl part*/

enum {
    LIBRG_OWNER_ADD = LIBRG_CHILD_REMOVE+1,
    LIBRG_OWNER_REMOVE,
    LIBRG_OWNER_UPDATE,

    LIBRG_EVENT_CUSTOM,
};

#ifndef LIBRG_MEM_ALLOC
#define LIBRG_MEM_ALLOC(size) zpl_malloc(size)
#endif

#ifndef LIBRG_MEM_FREE
#define LIBRG_MEM_FREE(ptr) zpl_mfree(ptr)
#endif


/* net module */

    // LIBRG_HANDSHAKE_INIT,
    // LIBRG_HANDSHAKE_REQUEST,
    // LIBRG_HANDSHAKE_ACCEPT,
    // LIBRG_HANDSHAKE_REFUSE,

    // LIBRG_CONNECTION_CONNECT,
    // LIBRG_CONNECTION_DISCONNECT,

// LIBRG_API int           librg_config_tickdelay_set(librg_ctx *, float time_ms);
// LIBRG_API float         librg_config_tickdelay_get(librg_ctx *);

// LIBRG_API int           librg_server_start(librg_ctx *, const char *hostname, uint16_t port);
// LIBRG_API int           librg_server_stop(librg_ctx *);
// LIBRG_API int           librg_server_tick(librg_ctx *);
// LIBRG_API int           librg_server_status(librg_ctx *);
// LIBRG_API int           librg_server_disconnect(librg_ctx *, librg_client);
// LIBRG_API int           librg_server_userdata_set(librg_ctx *, librg_client, void *);
// LIBRG_API void *        librg_server_userdata_get(librg_ctx *, librg_client);
// LIBRG_API int           librg_server_send_single(librg_ctx *, librg_client, void *data, size_t data_size);
// LIBRG_API int           librg_server_send_multiple(librg_ctx *, librg_client *, size_t client_amount, void *data, size_t data_size);
// LIBRG_API int           librg_server_send_all(librg_ctx *, void *data, size_t data_size);
// LIBRG_API int           librg_server_send_allexcept(librg_ctx *, librg_client *, size_t client_amount, void *data, size_t data_size);
// LIBRG_API int           librg_server_send_chunks(librg_ctx *, librg_chunk *, size_t chunk_amount, void *data, size_t data_size);
// LIBRG_API int           librg_server_send_chunksexcept(librg_ctx *, librg_chunk *, size_t chunk_amount, librg_client *, size_t client_amount, void *data, size_t data_size);

// LIBRG_API int           librg_client_connect(librg_ctx *, const char *hostname, uint16_t port);
// LIBRG_API int           librg_client_disconnect(librg_ctx *);
// LIBRG_API int           librg_client_tick(librg_ctx *);
// LIBRG_API int           librg_client_status(librg_ctx *);
// LIBRG_API int           librg_client_send(librg_ctx *, void *data, size_t data_size);


// typedef int (librg_transport_sendfn)()
// int librg_transport_send_set(librg_transport_sendfn);

#endif // LIBRG_H
