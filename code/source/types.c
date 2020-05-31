// file: source/types.c

#ifdef LIBRG_EDITOR
#include <librg.h>
#include <zpl.h>
#endif

LIBRG_BEGIN_C_DECLS

// =======================================================================//
// !
// ! Macro based helpers, and functions available for redefintion
// !
// =======================================================================//

/* allows to define a custom allocator */
#ifndef LIBRG_MEM_ALLOC
#define LIBRG_MEM_ALLOC(size) zpl_malloc(size)
#endif

/* allows to define a custom de-allocator */
#ifndef LIBRG_MEM_FREE
#define LIBRG_MEM_FREE(ptr) zpl_mfree(ptr)
#endif

/* allows to define a custom assert handler */
#ifndef LIBRG_ASSERT
#if defined(_DEBUG) || defined(LIBRG_DEBUG)
#define LIBRG_ASSERT(x) ZPL_ASSERT(x)
#else
#define LIBRG_ASSERT(x)
#endif
#endif

#ifndef LIBRG_MIN
#define LIBRG_MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

#ifndef LIBRG_MAX
#define LIBRG_MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

// =======================================================================//
// !
// ! Compile-time static configuration settings
// !
// =======================================================================//


/* defines how many max chunks an entity */
/* can be located in simultaneously */
#ifndef LIBRG_ENTITY_MAXCHUNKS
#define LIBRG_ENTITY_MAXCHUNKS 8
#endif

/* defines how many max entity ids could be used  */
/* inside of the librg_world_write call */
#ifndef LIBRG_WORLDWRITE_MAXQUERY
#define LIBRG_WORLDWRITE_MAXQUERY 4096
#endif

// =======================================================================//
// !
// ! Internal data structures
// !
// =======================================================================//

ZPL_TABLE(static inline, librg_table_i64, librg_table_i64_, int64_t);
ZPL_TABLE(static inline, librg_table_tbl, librg_table_tbl_, librg_table_i64);

typedef struct {
    uint8_t type : 4;
    uint8_t flag_owner_updated : 1;
    uint8_t flag_foreign : 1;
    uint8_t flag_unused_2 : 1;
    uint8_t flag_unused_3 : 1;

    int8_t observed_radius;
    uint16_t ownership_token;

    int32_t dimension;
    int64_t owner_id;

    librg_chunk chunks[LIBRG_ENTITY_MAXCHUNKS];

    void *userdata;
} librg_entity_t;

ZPL_TABLE(static inline, librg_table_ent, librg_table_ent_, librg_entity_t);

typedef struct {
    uint8_t valid;
    zpl_allocator allocator;
    zpl_random random;

    struct { uint16_t x, y, z; } worldsize;
    struct { uint16_t x, y, z; } chunksize;
    struct { int16_t x, y, z; } chunkoffset;

    librg_event_fn handlers[LIBRG_ERROR_REMOVE];
    librg_table_ent entity_map;
    librg_table_tbl owner_map;

    void *userdata;
} librg_world_t;

LIBRG_END_C_DECLS
