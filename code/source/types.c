// file: source/types.c

#ifdef LIBRG_EDITOR
#include <librg.h>
#include <zpl.h>
#endif

LIBRG_BEGIN_C_DECLS

// =======================================================================//
// !
// ! Implementation compile-time configuration
// !
// =======================================================================//

#define LIBRG_ASSERT(x)

/* allows to define a custom allocator */
#ifndef LIBRG_MEM_ALLOC
#define LIBRG_MEM_ALLOC(size) zpl_malloc(size)
#endif

/* allows to define a custom de-allocator */
#ifndef LIBRG_MEM_FREE
#define LIBRG_MEM_FREE(ptr) zpl_mfree(ptr)
#endif

/* defines how many max chunks an entity */
/* can be located in simultaneously */
#ifndef LIBRG_ENTITY_MAXCHUNKS
#define LIBRG_ENTITY_MAXCHUNKS 8
#endif

/* defines how many max chunks an owner */
/* can see in a single query */
#ifndef LIBRG_QUERY_MAXCHUNKS
#define LIBRG_QUERY_MAXCHUNKS 256
#endif


// =======================================================================//
// !
// ! Internal data structures
// !
// =======================================================================//

typedef struct {
    uint8_t type;
    int8_t observed_radius;
    int32_t dimension;
    int64_t owner_id;

    librg_chunk chunks[LIBRG_ENTITY_MAXCHUNKS];

    void *userdata;
} librg_entity_t;

ZPL_TABLE(static inline, librg_table_entity, librg_table_entity_, librg_entity_t);
ZPL_TABLE(static inline, librg_table_i64, librg_table_i64_, int64_t);

typedef struct {
    uint8_t valid;
    zpl_allocator allocator;

    struct { uint16_t x, y, z; } worldsize;
    struct { uint16_t x, y, z; } chunksize;
    struct { int16_t x, y, z; } chunkoffset;

    librg_event_fn handlers[LIBRG_READ_REMOVE+1];
    librg_table_entity entity_map;

    void *userdata;
} librg_world_t;

LIBRG_END_C_DECLS
