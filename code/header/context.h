// file: header/context.h

#ifdef LIBRG_EDITOR
#include <librg.h>
#endif

LIBRG_BEGIN_C_DECLS

// =======================================================================//
// !
// ! Context methods
// !
// =======================================================================//

LIBRG_API librg_ctx *   librg_context_create();
LIBRG_API int8_t        librg_context_destroy(librg_ctx *);
LIBRG_API int8_t        librg_context_valid(librg_ctx *);
LIBRG_API int8_t        librg_context_userdata_set(librg_ctx *, void *data);
LIBRG_API void *        librg_context_userdata_get(librg_ctx *);

// =======================================================================//
// !
// ! Configuration methods
// !
// =======================================================================//

LIBRG_API int8_t librg_config_worldsize_set(librg_ctx *, uint16_t x, uint16_t y, uint16_t z);
LIBRG_API int8_t librg_config_worldsize_get(librg_ctx *, uint16_t *x, uint16_t *y, uint16_t *z);
LIBRG_API int8_t librg_config_chunksize_set(librg_ctx *, uint16_t x, uint16_t y, uint16_t z);
LIBRG_API int8_t librg_config_chunksize_get(librg_ctx *, uint16_t *x, uint16_t *y, uint16_t *z);
LIBRG_API int8_t librg_config_chunkoffset_set(librg_ctx *, int16_t x, int16_t y, int16_t z);
LIBRG_API int8_t librg_config_chunkoffset_get(librg_ctx *, int16_t *x, int16_t *y, int16_t *z);

// =======================================================================//
// !
// ! Utility methods
// !
// =======================================================================//

LIBRG_ALWAYS_INLINE LIBRG_API librg_chunk librg_chunk_from_realpos(librg_ctx *, double x, double y, double z);
LIBRG_ALWAYS_INLINE LIBRG_API librg_chunk librg_chunk_from_chunkpos(librg_ctx *, int16_t chunk_x, int16_t chunk_y, int16_t chunk_z);

LIBRG_END_C_DECLS
