// file: header/general.h

#ifdef LIBRG_EDITOR
#include <librg.h>
#endif

LIBRG_BEGIN_C_DECLS

// =======================================================================//
// !
// ! Context methods
// !
// =======================================================================//

LIBRG_API librg_world * librg_world_create();
LIBRG_API int8_t        librg_world_destroy(librg_world *);
LIBRG_API int8_t        librg_world_valid(librg_world *);
LIBRG_API int8_t        librg_world_userdata_set(librg_world *, void *data);
LIBRG_API void *        librg_world_userdata_get(librg_world *);

// =======================================================================//
// !
// ! Configuration methods
// !
// =======================================================================//

LIBRG_API int8_t librg_config_chunkamount_set(librg_world *, uint16_t x, uint16_t y, uint16_t z);
LIBRG_API int8_t librg_config_chunkamount_get(librg_world *, uint16_t *x, uint16_t *y, uint16_t *z);
LIBRG_API int8_t librg_config_chunksize_set(librg_world *, uint16_t x, uint16_t y, uint16_t z);
LIBRG_API int8_t librg_config_chunksize_get(librg_world *, uint16_t *x, uint16_t *y, uint16_t *z);
LIBRG_API int8_t librg_config_chunkoffset_set(librg_world *, int16_t x, int16_t y, int16_t z);
LIBRG_API int8_t librg_config_chunkoffset_get(librg_world *, int16_t *x, int16_t *y, int16_t *z);


// =======================================================================//
// !
// ! Events
// !
// =======================================================================//

typedef enum librg_events {
    LIBRG_WRITE_CREATE,
    LIBRG_WRITE_UPDATE,
    LIBRG_WRITE_REMOVE,

    LIBRG_READ_CREATE,
    LIBRG_READ_UPDATE,
    LIBRG_READ_REMOVE,

    LIBRG_ERROR_CREATE,
    LIBRG_ERROR_UPDATE,
    LIBRG_ERROR_REMOVE,
} librg_event_id;

typedef struct {
    uint8_t     type;           /* type of the event that was called, might be useful in bindings */
    int64_t     owner_id;       /* id of the owner who this event is called for */
    int64_t     entity_id;      /* id of an entity which this event is called about */
    char      * buffer;         /* ptr to the buffer data */
    size_t      size;           /* depending on the event type, can show maximum amount of data you are able to write, or amount of data you can read */
    void      * userdata;       /* userpointer that is passed from librg_world_write/librg_world_read fns */
} librg_event;

typedef int32_t (*librg_event_fn)(librg_world *, librg_event *);

LIBRG_API int8_t librg_event_set(librg_world *, librg_event_id, librg_event_fn);
LIBRG_API int8_t librg_event_remove(librg_world *, librg_event_id);

// =======================================================================//
// !
// ! Utility methods
// !
// =======================================================================//

LIBRG_ALWAYS_INLINE LIBRG_API librg_chunk librg_chunk_from_realpos(librg_world *, double x, double y, double z);
LIBRG_ALWAYS_INLINE LIBRG_API librg_chunk librg_chunk_from_chunkpos(librg_world *, int16_t chunk_x, int16_t chunk_y, int16_t chunk_z);
LIBRG_ALWAYS_INLINE LIBRG_API int8_t      librg_chunk_to_chunkpos(librg_world *, librg_chunk id, int16_t *chunk_x, int16_t *chunk_y, int16_t *chunk_z);

LIBRG_END_C_DECLS
