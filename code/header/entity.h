// file: header/entity.h

#ifdef LIBRG_EDITOR
#include <librg.h>
#endif

LIBRG_BEGIN_C_DECLS

typedef enum librg_entity_type {
    LIBRG_ENTITY_STATIC,
    LIBRG_ENTITY_DYNAMIC,
} librg_entity_type;

typedef enum librg_entity_refreshing {
    LIBRG_TICK,     /* int argument, update entity every Nth tick */
    LIBRG_LINEAR,   /* float argument, linearly decrease update interval based on distance step (argument) */
    LIBRG_CUBIC,    /* float argument, cubicly decrease update inverval based on distance step (argument) */
} librg_entity_refreshing;

// =======================================================================//
// !
// ! Basic entity manipulation
// !
// =======================================================================//

LIBRG_API int8_t librg_entity_track(librg_world *, int64_t entity_id);
LIBRG_API int8_t librg_entity_untrack(librg_world *, int64_t entity_id);
LIBRG_API int8_t librg_entity_tracked(librg_world *, int64_t entity_id);
LIBRG_API int8_t librg_entity_userdata_set(librg_world *, int64_t entity_id, void *data);
LIBRG_API void * librg_entity_userdata_get(librg_world *, int64_t entity_id);

// =======================================================================//
// !
// ! Simple entity data methods
// !
// =======================================================================//

LIBRG_API int8_t        librg_entity_type_set(librg_world *, int64_t entity_id, uint8_t type);
LIBRG_API int16_t       librg_entity_type_get(librg_world *, int64_t entity_id);
LIBRG_API int8_t        librg_entity_chunk_set(librg_world *, int64_t entity_id, librg_chunk);
LIBRG_API librg_chunk   librg_entity_chunk_get(librg_world *, int64_t entity_id);
LIBRG_API int8_t        librg_entity_owner_set(librg_world *, int64_t entity_id, int64_t owner_id, int8_t observed_chunk_radius);
LIBRG_API int64_t       librg_entity_owner_get(librg_world *, int64_t entity_id);
LIBRG_API int8_t        librg_entity_dimension_set(librg_world *, int64_t entity_id, int32_t dimension);
LIBRG_API int32_t       librg_entity_dimension_get(librg_world *, int64_t entity_id);

// =======================================================================//
// !
// ! Advanced entity data methods
// !
// =======================================================================//

LIBRG_API int8_t librg_entity_chunkarray_set(librg_world *, int64_t entity_id, librg_chunk *chunks, size_t chunk_amount);
LIBRG_API size_t librg_entity_chunkarray_get(librg_world *, int64_t entity_id, LIBRG_OUT librg_chunk *chunks, size_t buffer_limit);
LIBRG_API int8_t librg_entity_refresh_set(librg_world *, int64_t entity_id, int type, float value); // ??
LIBRG_API int8_t librg_entity_refresh_get(librg_world *, int64_t entity_id, int *type, float *value);

LIBRG_END_C_DECLS
