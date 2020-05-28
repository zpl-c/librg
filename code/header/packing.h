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

LIBRG_API int8_t librg_world_read(librg_world *, int64_t owner_id, const char *buffer, size_t size, void *userdata);
LIBRG_API size_t librg_world_write(librg_world *, int64_t owner_id, LIBRG_OUT char *buffer, size_t buffer_limit, void *userdata);

LIBRG_END_C_DECLS
