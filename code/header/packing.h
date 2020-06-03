// file: header/packing.h

#ifdef LIBRG_EDITOR
#include <librg.h>
#endif

LIBRG_BEGIN_C_DECLS

// =======================================================================//
// !
// ! World data (de)packing methods
// !
// =======================================================================//

LIBRG_API int32_t librg_world_read(librg_world *world, int64_t owner_id, const char *buffer, size_t size, void *userdata);
LIBRG_API int32_t librg_world_write(librg_world *world, int64_t owner_id, LIBRG_OUT char *buffer, LIBRG_INOUT size_t *size, void *userdata);

LIBRG_END_C_DECLS
