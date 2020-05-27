// file: header/world.h

#ifdef LIBRG_EDITOR
#include <librg.h>
#endif

LIBRG_BEGIN_C_DECLS

// =======================================================================//
// !
// ! World data/query methods
// !
// =======================================================================//

LIBRG_API int8_t librg_world_read(librg_ctx *, int64_t owner_id, LIBRG_IN char *buffer, size_t size, void *userdata);
LIBRG_API size_t librg_world_write(librg_ctx *, int64_t owner_id, LIBRG_OUT char *buffer, size_t buffer_limit, void *userdata);

LIBRG_API size_t librg_world_query(librg_ctx *, int64_t owner_id, LIBRG_OUT int64_t *entity_ids, size_t buffer_limit);
LIBRG_API size_t librg_world_fetch_all(librg_ctx *, LIBRG_OUT int64_t *entity_ids, size_t buffer_limit);
LIBRG_API size_t librg_world_fetch_chunk(librg_ctx *, librg_chunk, LIBRG_OUT int64_t *entity_ids, size_t buffer_limit);
LIBRG_API size_t librg_world_fetch_chunkarray(librg_ctx *, LIBRG_IN librg_chunk *chunks, size_t chunk_amount, LIBRG_OUT int64_t *entity_ids, size_t buffer_limit);
LIBRG_API size_t librg_world_fetch_owner(librg_ctx *, int64_t owner_id, LIBRG_OUT int64_t *entity_ids, size_t buffer_limit);
LIBRG_API size_t librg_world_fetch_ownerarray(librg_ctx *, LIBRG_IN int64_t *owner_ids, size_t owner_amount, LIBRG_OUT int64_t *entity_ids, size_t buffer_limit);

// =======================================================================//
// !
// ! Events
// !
// =======================================================================//

typedef enum librg_events {
    LIBRG_PARENT_CREATE,
    LIBRG_PARENT_UPDATE,
    LIBRG_PARENT_REMOVE,

    LIBRG_CHILD_CREATE,
    LIBRG_CHILD_UPDATE,
    LIBRG_CHILD_REMOVE,
} librg_event_id;

typedef struct {
    uint8_t     type;           /* type of the event that was called, might be useful in bindings */
    int64_t     owner_id;       /* id of the owner who this event is called for */
    int64_t     entity_id;      /* id of an entity which this event is called about */
    char      * buffer;         /* ptr to the buffer data */
    size_t      size;           /* depending on the event type, can show maximum amount of data you are able to write, or amount of data you can read */
    void      * userdata;       /* userpointer that is passed from librg_world_write/librg_world_read fns */
} librg_event;

typedef size_t (*librg_event_fn)(librg_ctx *, librg_event *);

LIBRG_API int8_t librg_event_set(librg_ctx *, librg_event_id, librg_event_fn);
LIBRG_API int8_t librg_event_remove(librg_ctx *, librg_event_id);

LIBRG_END_C_DECLS
