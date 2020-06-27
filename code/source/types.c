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
#define LIBRG_WORLDWRITE_MAXQUERY 16384
#endif

/* validate that value is less than maximum allowed */
#if LIBRG_WORLDWRITE_MAXQUERY > ZPL_U16_MAX
#error "LIBRG_WORLDWRITE_MAXQUERY must have value less than 65535"
#endif

// =======================================================================//
// !
// ! Internal data structures
// !
// =======================================================================//

ZPL_TABLE(static inline, librg_table_i8, librg_table_i8_, int8_t);
ZPL_TABLE(static inline, librg_table_i64, librg_table_i64_, int64_t);
ZPL_TABLE(static inline, librg_table_tbl, librg_table_tbl_, librg_table_i64);

enum  {
    LIBRG_WRITE_OWNER = (LIBRG_ERROR_REMOVE+1),
    LIBRG_READ_OWNER,
    LIBRG_ERROR_OWNER,
    LIBRG_PACKAGING_TOTAL,
};

typedef struct librg_entity_t {
    uint8_t type : 2;
    uint8_t visibility_global : 2;
    uint8_t flag_owner_updated : 1;
    uint8_t flag_foreign : 1;
    uint8_t flag_visbility_owner_enabled : 1;
    uint8_t flag_unused2 : 1;

    int8_t observed_radius;
    uint16_t ownership_token;

    int32_t dimension;
    int64_t owner_id;

    librg_chunk chunks[LIBRG_ENTITY_MAXCHUNKS];
    librg_table_i8 owner_visibility_map;

    void *userdata;
} librg_entity_t;

ZPL_TABLE(static inline, librg_table_ent, librg_table_ent_, librg_entity_t);

typedef struct librg_event_t {
    uint8_t     type;           /* type of the event that was called, might be useful in bindings */
    int64_t     owner_id;       /* id of the owner who this event is called for */
    int64_t     entity_id;      /* id of an entity which this event is called about */
    char      * buffer;         /* ptr to the buffer data */
    size_t      size;           /* depending on the event type, can show maximum amount of data you are able to write, or amount of data you can read */
    void      * userdata;       /* userpointer that is passed from librg_world_write/librg_world_read fns */
} librg_event_t;

typedef struct librg_world_t {
    uint8_t valid;
    zpl_allocator allocator;
    zpl_random random;

    struct { uint16_t x, y, z; } worldsize;
    struct { uint16_t x, y, z; } chunksize;
    struct { int16_t x, y, z; } chunkoffset;

    librg_event_fn handlers[LIBRG_PACKAGING_TOTAL];
    librg_table_ent entity_map;
    librg_table_tbl owner_map;

    void *userdata;
} librg_world_t;

/* unsued */

// #define LIBRG_OBSERVE_ALL               (-0x01)
// #define LIBRG_OBSERVE_NONE              (+0x00)

// typedef enum librg_entity_refreshing {
//     LIBRG_CONSTANT,     /* int argument, update entity every Nth tick */
//     LIBRG_LINEAR,       /* float argument, linearly decrease update interval based on distance step (argument) */
//     LIBRG_QUADRATIC,    /* float argument, cubicly decrease update inverval based on distance step (argument) */
// } librg_entity_refreshing;

// typedef enum {
//     LIBRG_ENTITY_DYNAMIC,
//     LIBRG_ENTITY_STATIC,
// } librg_behavior_type;

// typedef enum {
//     LIBRG_BEHAVIOR_TYPE,
//     LIBRG_BEHAVIOR_STEPBACK,
// } librg_behavior;

LIBRG_END_C_DECLS
