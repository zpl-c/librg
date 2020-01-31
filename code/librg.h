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
typedef int32_t librg_client;
typedef int32_t librg_entity;

enum librg_errors {
    LIBRG_INVALID_ENTITY    = -1,
    LIBRG_INVALID_CLIENT    = -2,
    LIBRG_INVALID_CONTEXT   = -3,
    LIBRG_INVALID_CHUNK     = -4,
};

typedef enum librg_events {
    LIBRG_HANDSHAKE_INIT,
    LIBRG_HANDSHAKE_REQUEST,
    LIBRG_HANDSHAKE_ACCEPT,
    LIBRG_HANDSHAKE_REFUSE,

    LIBRG_CONNECTION_CONNECT,
    LIBRG_CONNECTION_DISCONNECT,

    LIBRG_ENTITY_CREATE,
    LIBRG_ENTITY_UPDATE,
    LIBRG_ENTITY_REMOVE,

    LIBRG_ENTITY_CONTROL_ADD,
    LIBRG_ENTITY_CONTROL_REMOVE,
    LIBRG_ENTITY_CONTROL_UPDATE,

    LIBRG_EVENT_CUSTOM,
} librg_event_id;

typedef struct {
    librg_ctx   *ctx;
    void        *data;
    size_t       size;
    uint32_t     flags;
    librg_client client;
    librg_entity entity;
} librg_event;

typedef void (librg_event_callback)(librg_event *event);

LIBRG_API librg_ctx *   librg_context_create();
LIBRG_API int           librg_context_destroy(librg_ctx *);
LIBRG_API int           librg_context_valid(librg_ctx *);
LIBRG_API int           librg_context_userdata_set(librg_ctx *, void *data);
LIBRG_API void *        librg_context_userdata_get(librg_ctx *);

LIBRG_API int           librg_config_chunksize_set(librg_ctx *, uint16_t x, uint16_t y, uint16_t z);
LIBRG_API int           librg_config_chunksize_get(librg_ctx *, uint16_t *x, uint16_t *y, uint16_t *z);
LIBRG_API int           librg_config_worldsize_set(librg_ctx *, uint16_t x, uint16_t y, uint16_t z);
LIBRG_API int           librg_config_worldsize_get(librg_ctx *, uint16_t *x, uint16_t *y, uint16_t *z);
LIBRG_API int           librg_config_tickdelay_set(librg_ctx *, float time_ms);
LIBRG_API float         librg_config_tickdelay_get(librg_ctx *);
LIBRG_API int           librg_config_entitylimit_set(librg_ctx *, size_t limit);
LIBRG_API size_t        librg_config_entitylimit_get(librg_ctx *);

LIBRG_API int           librg_server_start(librg_ctx *, const char *hostname, uint16_t port);
LIBRG_API int           librg_server_stop(librg_ctx *);
LIBRG_API int           librg_server_tick(librg_ctx *);
LIBRG_API int           librg_server_status(librg_ctx *);
LIBRG_API int           librg_server_disconnect(librg_ctx *, librg_client);
LIBRG_API int           librg_server_userdata_set(librg_ctx *, librg_client, void *);
LIBRG_API void *        librg_server_userdata_get(librg_ctx *, librg_client);
LIBRG_API int           librg_server_send_single(librg_ctx *, librg_client, void *data, size_t data_size);
LIBRG_API int           librg_server_send_multiple(librg_ctx *, librg_client *, size_t client_amount, void *data, size_t data_size);
LIBRG_API int           librg_server_send_all(librg_ctx *, void *data, size_t data_size);
LIBRG_API int           librg_server_send_allexcept(librg_ctx *, librg_client *, size_t client_amount, void *data, size_t data_size);
LIBRG_API int           librg_server_send_chunks(librg_ctx *, librg_chunk *, size_t chunk_amount, void *data, size_t data_size);
LIBRG_API int           librg_server_send_chunksexcept(librg_ctx *, librg_chunk *, size_t chunk_amount, librg_client *, size_t client_amount, void *data, size_t data_size);

LIBRG_API int           librg_client_connect(librg_ctx *, const char *hostname, uint16_t port);
LIBRG_API int           librg_client_disconnect(librg_ctx *);
LIBRG_API int           librg_client_tick(librg_ctx *);
LIBRG_API int           librg_client_status(librg_ctx *);
LIBRG_API int           librg_client_send(librg_ctx *, void *data, size_t data_size);

LIBRG_API int           librg_event_set(librg_ctx *, librg_event_id, librg_event_callback);
LIBRG_API int           librg_event_remove(librg_ctx *, librg_event_id);
LIBRG_API int           librg_event_reject(librg_ctx *, librg_event *);

LIBRG_API librg_entity  librg_entity_create(librg_ctx *);
LIBRG_API int           librg_entity_destroy(librg_ctx *, librg_entity);
LIBRG_API int           librg_entity_valid(librg_ctx *, librg_entity);
LIBRG_API int           librg_entity_userdata_set(librg_ctx *, librg_entity, void *data);
LIBRG_API void *        librg_entity_userdata_get(librg_ctx *, librg_entity);

LIBRG_API size_t        librg_entity_query(librg_ctx *, librg_entity, librg_entity **results, size_t buffer_limit);
LIBRG_API size_t        librg_entity_fetch_all(librg_ctx *, librg_entity **results, size_t buffer_limit);
LIBRG_API size_t        librg_entity_fetch_chunk(librg_ctx *, librg_chunk, librg_entity **results, size_t buffer_limit);
LIBRG_API size_t        librg_entity_fetch_chunkarray(librg_ctx *, librg_chunk *, size_t chunk_amount, librg_entity **results, size_t buffer_limit);

LIBRG_API int           librg_entity_chunkrange_set(librg_ctx *, librg_entity, int8_t range);
LIBRG_API int8_t        librg_entity_chunkrange_get(librg_ctx *, librg_entity);
LIBRG_API int           librg_entity_virtualworld_set(librg_ctx *, librg_entity, int32_t world);
LIBRG_API int32_t       librg_entity_virtualworld_get(librg_ctx *, librg_entity);
LIBRG_API int           librg_entity_chunkarray_set(librg_ctx *, librg_entity, librg_chunk *, size_t chunk_amount);
LIBRG_API size_t        librg_entity_chunkarray_get(librg_ctx *, librg_entity, librg_chunk **results);
LIBRG_API int           librg_entity_chunk_set(librg_ctx *, librg_entity, librg_chunk);
LIBRG_API librg_chunk   librg_entity_chunk_get(librg_ctx *, librg_entity);
LIBRG_API int           librg_entity_controller_set(librg_ctx *, librg_entity, librg_client);
LIBRG_API librg_client  librg_entity_controller_get(librg_ctx *, librg_entity);

LIBRG_API librg_entity  librg_entity_from_client(librg_ctx *, librg_client);
LIBRG_API librg_client  librg_client_from_entity(librg_ctx *, librg_entity);
LIBRG_API librg_chunk   librg_chunk_from_realpos(librg_ctx *, float x, float y, float z);
LIBRG_API librg_chunk   librg_chunk_from_chunkpos(librg_ctx *, int16_t chunk_x, int16_t chunk_y, int16_t chunk_z);


// typedef int (librg_transport_sendfn)()
// int librg_transport_send_set(librg_transport_sendfn);

#endif // LIBRG_H
