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

LIBRG_API uint32_t      librg_version();
LIBRG_API librg_world * librg_world_create();
LIBRG_API int8_t        librg_world_destroy(librg_world *world);
LIBRG_API int8_t        librg_world_valid(librg_world *world);
LIBRG_API int8_t        librg_world_userdata_set(librg_world *world, void *data);
LIBRG_API void *        librg_world_userdata_get(librg_world *world);

// =======================================================================//
// !
// ! Configuration methods
// !
// =======================================================================//

LIBRG_API int8_t librg_config_chunkamount_set(librg_world *world, uint16_t x, uint16_t y, uint16_t z);
LIBRG_API int8_t librg_config_chunkamount_get(librg_world *world, uint16_t *x, uint16_t *y, uint16_t *z);
LIBRG_API int8_t librg_config_chunksize_set(librg_world *world, uint16_t x, uint16_t y, uint16_t z);
LIBRG_API int8_t librg_config_chunksize_get(librg_world *world, uint16_t *x, uint16_t *y, uint16_t *z);
LIBRG_API int8_t librg_config_chunkoffset_set(librg_world *world, int16_t x, int16_t y, int16_t z);
LIBRG_API int8_t librg_config_chunkoffset_get(librg_world *world, int16_t *x, int16_t *y, int16_t *z);

// =======================================================================//
// !
// ! Events
// !
// =======================================================================//

LIBRG_API int8_t            librg_event_set(librg_world *world, librg_event_type, librg_event_fn);
LIBRG_API int8_t            librg_event_remove(librg_world *world, librg_event_type);

LIBRG_API librg_event_type  librg_event_type_get(librg_world *world, librg_event *event);
LIBRG_API int64_t           librg_event_owner_get(librg_world *world, librg_event *event);
LIBRG_API int64_t           librg_event_entity_get(librg_world *world, librg_event *event);
LIBRG_API char *            librg_event_buffer_get(librg_world *world, librg_event *event);
LIBRG_API int32_t           librg_event_size_get(librg_world *world, librg_event *event);
LIBRG_API void *            librg_event_userdata_get(librg_world *world, librg_event *event);

// =======================================================================//
// !
// ! Utility methods
// !
// =======================================================================//

LIBRG_API librg_chunk librg_chunk_from_realpos(librg_world *world, double x, double y, double z);
LIBRG_API librg_chunk librg_chunk_from_chunkpos(librg_world *world, int16_t chunk_x, int16_t chunk_y, int16_t chunk_z);
LIBRG_API int8_t      librg_chunk_to_chunkpos(librg_world *world, librg_chunk id, int16_t *chunk_x, int16_t *chunk_y, int16_t *chunk_z);

LIBRG_END_C_DECLS
