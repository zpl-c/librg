// file: source/context.c

#ifdef LIBRG_EDITOR
#include <librg.h>
#include <zpl.h>
#endif

LIBRG_BEGIN_C_DECLS

// =======================================================================//
// !
// ! Custom internal zpl alloc proc hanlders
// !
// =======================================================================//

LIBRG_PRIVATE zpl_allocator librg_alloc_wrap();

ZPL_ALLOCATOR_PROC(librg_allocator_proc) {
    void *ptr = NULL;

    zpl_unused(allocator_data);
    zpl_unused(old_size);
    zpl_unused(flags);

    switch (type) {
        case ZPL_ALLOCATION_ALLOC:  { ptr = LIBRG_MEM_ALLOC(size); } break;
        case ZPL_ALLOCATION_FREE:   { LIBRG_MEM_FREE(old_memory); } break;
        case ZPL_ALLOCATION_RESIZE: { ptr = zpl_default_resize_align(librg_alloc_wrap(), old_memory, old_size, size, alignment); } break;
        case ZPL_ALLOCATION_FREE_ALL: break;
    }

    return ptr;
}

LIBRG_PRIVATE zpl_allocator librg_alloc_wrap() {
    zpl_allocator a = {0};
    a.proc = librg_allocator_proc;
    a.data = NULL;
    return a;
}

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

    librg_event_fn handlers[LIBRG_CHILD_REMOVE+1];
    librg_table_entity entity_map;

    void *userdata;
} librg_ctx_t;

// =======================================================================//
// !
// ! Context methods
// !
// =======================================================================//

librg_ctx *librg_context_create() {
    librg_ctx_t *ictx = LIBRG_MEM_ALLOC(sizeof(librg_ctx_t));
    zpl_memset(ictx, 0, sizeof(librg_ctx_t));

    /* setup initials */
    ictx->valid = LIBRG_TRUE;
    ictx->allocator = librg_alloc_wrap();

    /* setup defaults */
    librg_config_chunksize_set((librg_ctx *)ictx, 16, 16, 16);
    librg_config_worldsize_set((librg_ctx *)ictx, 256, 256, 256);
    librg_config_chunkoffset_set((librg_ctx *)ictx, LIBRG_OFFSET_MID, LIBRG_OFFSET_MID, LIBRG_OFFSET_MID);

    /* initialize internal structs */
    librg_table_entity_init(&ictx->entity_map, ictx->allocator);

    return (librg_ctx *)ictx;
}

int8_t librg_context_destroy(librg_ctx *ctx) {
    LIBRG_ASSERT(ctx); if (!ctx) return LIBRG_CONTEXT_INVALID;
    librg_ctx_t *ictx = (librg_ctx_t *)ctx;

    /* free up internal structs */
    librg_table_entity_destroy(&ictx->entity_map);

    /* mark it invalid */
    ictx->valid = LIBRG_FALSE;

    LIBRG_MEM_FREE(ctx);
    return LIBRG_OK;
}

int8_t librg_context_valid(librg_ctx *ctx) {
    if (!ctx) return LIBRG_FALSE;
    librg_ctx_t *ictx = (librg_ctx_t *)ctx;
    return ictx->valid;
}

int8_t librg_context_userdata_set(librg_ctx *ctx, void *data) {
    LIBRG_ASSERT(ctx); if (!ctx) return LIBRG_CONTEXT_INVALID;
    librg_ctx_t *ictx = (librg_ctx_t *)ctx;
    ictx->userdata = data;
    return LIBRG_OK;
}

void *librg_context_userdata_get(librg_ctx *ctx) {
    LIBRG_ASSERT(ctx); if (!ctx) return NULL;
    librg_ctx_t *ictx = (librg_ctx_t *)ctx;
    return ictx->userdata;
}

// =======================================================================//
// !
// ! Runtime configuration
// !
// =======================================================================//

int8_t librg_config_worldsize_set(librg_ctx *ctx, uint16_t x, uint16_t y, uint16_t z) {
    LIBRG_ASSERT(ctx); if (!ctx) return LIBRG_CONTEXT_INVALID;
    librg_ctx_t *ictx = (librg_ctx_t *)ctx;
    ictx->worldsize.x = x == 0 ? 1 : x;
    ictx->worldsize.y = y == 0 ? 1 : y;
    ictx->worldsize.z = z == 0 ? 1 : z;
    return LIBRG_OK;
}

int8_t librg_config_worldsize_get(librg_ctx *ctx, uint16_t *x, uint16_t *y, uint16_t *z) {
    LIBRG_ASSERT(ctx); if (!ctx) return LIBRG_CONTEXT_INVALID;
    librg_ctx_t *ictx = (librg_ctx_t *)ctx;
    *x = ictx->worldsize.x;
    *y = ictx->worldsize.y;
    *z = ictx->worldsize.z;
    return LIBRG_OK;
}

int8_t librg_config_chunksize_set(librg_ctx *ctx, uint16_t x, uint16_t y, uint16_t z) {
    LIBRG_ASSERT(ctx); if (!ctx) return LIBRG_CONTEXT_INVALID;
    librg_ctx_t *ictx = (librg_ctx_t *)ctx;
    ictx->chunksize.x = x;
    ictx->chunksize.y = y;
    ictx->chunksize.z = z;
    return LIBRG_OK;
}

int8_t librg_config_chunksize_get(librg_ctx *ctx, uint16_t *x, uint16_t *y, uint16_t *z) {
    LIBRG_ASSERT(ctx); if (!ctx) return LIBRG_CONTEXT_INVALID;
    librg_ctx_t *ictx = (librg_ctx_t *)ctx;
    *x = ictx->chunksize.x;
    *y = ictx->chunksize.y;
    *z = ictx->chunksize.z;
    return LIBRG_OK;
}

int8_t librg_config_chunkoffset_set(librg_ctx *ctx, int16_t x, int16_t y, int16_t z) {
    LIBRG_ASSERT(ctx); if (!ctx) return LIBRG_CONTEXT_INVALID;
    librg_ctx_t *ictx = (librg_ctx_t *)ctx;
    ictx->chunkoffset.x = x;
    ictx->chunkoffset.y = y;
    ictx->chunkoffset.z = z;
    return LIBRG_OK;
}

int8_t librg_config_chunkoffset_get(librg_ctx *ctx, int16_t *x, int16_t *y, int16_t *z) {
    LIBRG_ASSERT(ctx); if (!ctx) return LIBRG_CONTEXT_INVALID;
    librg_ctx_t *ictx = (librg_ctx_t *)ctx;
    *x = ictx->chunkoffset.x;
    *y = ictx->chunkoffset.y;
    *z = ictx->chunkoffset.z;
    return LIBRG_OK;
}

// =======================================================================//
// !
// ! Utitilites
// !
// =======================================================================//

LIBRG_ALWAYS_INLINE int16_t librg_util_chunkoffset_line(int16_t v, int16_t off, int16_t size) {
    int16_t o = 0;
    if (off == LIBRG_OFFSET_BEG) o = 0;
    if (off == LIBRG_OFFSET_MID) o = (size/2);
    if (off == LIBRG_OFFSET_END) o = (size-1);
    return v + o;
}

librg_chunk librg_chunk_from_chunkpos(librg_ctx *ctx, int16_t chunk_x, int16_t chunk_y, int16_t chunk_z) {
    LIBRG_ASSERT(ctx); if (!ctx) return LIBRG_CONTEXT_INVALID;
    librg_ctx_t *ictx = (librg_ctx_t *)ctx;

    int16_t chx = librg_util_chunkoffset_line(chunk_x, ictx->chunkoffset.x, ictx->worldsize.x);
    int16_t chy = librg_util_chunkoffset_line(chunk_y, ictx->chunkoffset.y, ictx->worldsize.y);
    int16_t chz = librg_util_chunkoffset_line(chunk_z, ictx->chunkoffset.z, ictx->worldsize.z);

    librg_chunk id = (chz * ictx->worldsize.y * ictx->worldsize.z) + (chy * ictx->worldsize.y) + (chx);

    if (id > (ictx->worldsize.x * ictx->worldsize.y * ictx->worldsize.z)) {
        return LIBRG_CHUNK_INVALID;
    }

    return id;
}

librg_chunk librg_chunk_from_realpos(librg_ctx *ctx, double x, double y, double z) {
    LIBRG_ASSERT(ctx); if (!ctx) return LIBRG_CONTEXT_INVALID;
    librg_ctx_t *ictx = (librg_ctx_t *)ctx;
    return librg_chunk_from_chunkpos(ctx, (int16_t)(x/ictx->chunksize.x), (int16_t)(y/ictx->chunksize.y), (int16_t)(z/ictx->chunksize.z));
}

LIBRG_END_C_DECLS
