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

LIBRG_API int8_t librg_world_read(librg_world *, int64_t owner_id, LIBRG_IN char *buffer, size_t size, void *userdata);
LIBRG_API size_t librg_world_write(librg_world *, int64_t owner_id, LIBRG_OUT char *buffer, size_t buffer_limit, void *userdata);

LIBRG_API size_t librg_world_query(librg_world *, int64_t owner_id, LIBRG_OUT int64_t *entity_ids, size_t buffer_limit);
LIBRG_API size_t librg_world_fetch_all(librg_world *, LIBRG_OUT int64_t *entity_ids, size_t buffer_limit);
LIBRG_API size_t librg_world_fetch_chunk(librg_world *, librg_chunk, LIBRG_OUT int64_t *entity_ids, size_t buffer_limit);
LIBRG_API size_t librg_world_fetch_chunkarray(librg_world *, LIBRG_IN librg_chunk *chunks, size_t chunk_amount, LIBRG_OUT int64_t *entity_ids, size_t buffer_limit);
LIBRG_API size_t librg_world_fetch_owner(librg_world *, int64_t owner_id, LIBRG_OUT int64_t *entity_ids, size_t buffer_limit);
LIBRG_API size_t librg_world_fetch_ownerarray(librg_world *, LIBRG_IN int64_t *owner_ids, size_t owner_amount, LIBRG_OUT int64_t *entity_ids, size_t buffer_limit);

LIBRG_END_C_DECLS
