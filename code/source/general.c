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
// ! Context methods
// !
// =======================================================================//

uint32_t librg_version() {
    return LIBRG_VERSION;
}

librg_world *librg_world_create() {
    librg_world_t *wld = LIBRG_MEM_ALLOC(sizeof(librg_world_t));
    zpl_memset(wld, 0, sizeof(librg_world_t));

    /* setup initials */
    wld->valid = LIBRG_TRUE;
    wld->allocator = librg_alloc_wrap();

    /* setup defaults */
    librg_config_chunksize_set((librg_world *)wld, 16, 16, 16);
    librg_config_chunkamount_set((librg_world *)wld, 256, 256, 256);
    librg_config_chunkoffset_set((librg_world *)wld, LIBRG_OFFSET_MID, LIBRG_OFFSET_MID, LIBRG_OFFSET_MID);

    /* initialize internal structs */
    librg_table_ent_init(&wld->entity_map, wld->allocator);
    librg_table_tbl_init(&wld->owner_map, wld->allocator);
    zpl_random_init(&wld->random);

    return (librg_world *)wld;
}

int8_t librg_world_destroy(librg_world *world) {
    LIBRG_ASSERT(world); if (!world) return LIBRG_WORLD_INVALID;
    librg_world_t *wld = (librg_world_t *)world;

    {/* free up entities */
        for (int i = 0; i < zpl_array_count(wld->entity_map.entries); ++i) {
            librg_entity_t *entity = &wld->entity_map.entries[i].value;

            if (entity->flag_visbility_owner_enabled) {
                entity->flag_visbility_owner_enabled = LIBRG_FALSE;
                librg_table_i8_destroy(&entity->owner_visibility_map);
            }
        }

        librg_table_ent_destroy(&wld->entity_map);
    }

    {/* free up owners */
        for (int i = 0; i < zpl_array_count(wld->owner_map.entries); ++i)
            librg_table_i64_destroy(&wld->owner_map.entries[i].value);

        librg_table_tbl_destroy(&wld->owner_map);
    }

    /* mark it invalid */
    wld->valid = LIBRG_FALSE;

    LIBRG_MEM_FREE(world);
    return LIBRG_OK;
}

int8_t librg_world_valid(librg_world *world) {
    if (!world) return LIBRG_FALSE;
    librg_world_t *wld = (librg_world_t *)world;
    return wld->valid;
}

int8_t librg_world_userdata_set(librg_world *world, void *data) {
    LIBRG_ASSERT(world); if (!world) return LIBRG_WORLD_INVALID;
    librg_world_t *wld = (librg_world_t *)world;
    wld->userdata = data;
    return LIBRG_OK;
}

void *librg_world_userdata_get(librg_world *world) {
    LIBRG_ASSERT(world); if (!world) return NULL;
    librg_world_t *wld = (librg_world_t *)world;
    return wld->userdata;
}

// =======================================================================//
// !
// ! Runtime configuration
// !
// =======================================================================//

int8_t librg_config_chunkamount_set(librg_world *world, uint16_t x, uint16_t y, uint16_t z) {
    LIBRG_ASSERT(world); if (!world) return LIBRG_WORLD_INVALID;
    librg_world_t *wld = (librg_world_t *)world;
    wld->worldsize.x = x == 0 ? 1 : x;
    wld->worldsize.y = y == 0 ? 1 : y;
    wld->worldsize.z = z == 0 ? 1 : z;
    return LIBRG_OK;
}

int8_t librg_config_chunkamount_get(librg_world *world, uint16_t *x, uint16_t *y, uint16_t *z) {
    LIBRG_ASSERT(world); if (!world) return LIBRG_WORLD_INVALID;
    librg_world_t *wld = (librg_world_t *)world;
    *x = wld->worldsize.x;
    *y = wld->worldsize.y;
    *z = wld->worldsize.z;
    return LIBRG_OK;
}

int8_t librg_config_chunksize_set(librg_world *world, uint16_t x, uint16_t y, uint16_t z) {
    LIBRG_ASSERT(world); if (!world) return LIBRG_WORLD_INVALID;
    librg_world_t *wld = (librg_world_t *)world;
    wld->chunksize.x = x;
    wld->chunksize.y = y;
    wld->chunksize.z = z;
    return LIBRG_OK;
}

int8_t librg_config_chunksize_get(librg_world *world, uint16_t *x, uint16_t *y, uint16_t *z) {
    LIBRG_ASSERT(world); if (!world) return LIBRG_WORLD_INVALID;
    librg_world_t *wld = (librg_world_t *)world;
    *x = wld->chunksize.x;
    *y = wld->chunksize.y;
    *z = wld->chunksize.z;
    return LIBRG_OK;
}

int8_t librg_config_chunkoffset_set(librg_world *world, int16_t x, int16_t y, int16_t z) {
    LIBRG_ASSERT(world); if (!world) return LIBRG_WORLD_INVALID;
    librg_world_t *wld = (librg_world_t *)world;
    wld->chunkoffset.x = x;
    wld->chunkoffset.y = y;
    wld->chunkoffset.z = z;
    return LIBRG_OK;
}

int8_t librg_config_chunkoffset_get(librg_world *world, int16_t *x, int16_t *y, int16_t *z) {
    LIBRG_ASSERT(world); if (!world) return LIBRG_WORLD_INVALID;
    librg_world_t *wld = (librg_world_t *)world;
    *x = wld->chunkoffset.x;
    *y = wld->chunkoffset.y;
    *z = wld->chunkoffset.z;
    return LIBRG_OK;
}

// =======================================================================//
// !
// ! Events
// !
// =======================================================================//

int8_t librg_event_set(librg_world *world, librg_event_type id, librg_event_fn handler) {
    LIBRG_ASSERT(world); if (!world) return LIBRG_WORLD_INVALID;
    librg_world_t *wld = (librg_world_t *)world;

    if (wld->handlers[id]) {
        wld->handlers[id] = handler;
        return LIBRG_HANDLER_REPLACED;
    }

    wld->handlers[id] = handler;
    return LIBRG_OK;
}

int8_t librg_event_remove(librg_world *world, librg_event_type id) {
    LIBRG_ASSERT(world); if (!world) return LIBRG_WORLD_INVALID;
    librg_world_t *wld = (librg_world_t *)world;

    if (!wld->handlers[id]) {
        return LIBRG_HANDLER_EMPTY;
    }

    wld->handlers[id] = NULL;
    return LIBRG_OK;
}

librg_event_type librg_event_type_get(librg_world *world, librg_event *event) {
    LIBRG_ASSERT(event); if (!event) return LIBRG_EVENT_INVALID;
    zpl_unused(world);
    librg_event_t *e = (librg_event_t*)event;
    return e->type;
}

int64_t librg_event_owner_get(librg_world *world, librg_event *event) {
    LIBRG_ASSERT(event); if (!event) return LIBRG_EVENT_INVALID;
    zpl_unused(world);
    librg_event_t *e = (librg_event_t*)event;
    return e->owner_id;
}

int64_t librg_event_entity_get(librg_world *world, librg_event *event) {
    LIBRG_ASSERT(event); if (!event) return LIBRG_EVENT_INVALID;
    zpl_unused(world);
    librg_event_t *e = (librg_event_t*)event;
    return e->entity_id;
}

char * librg_event_buffer_get(librg_world *world, librg_event *event) {
    LIBRG_ASSERT(event); if (!event) return NULL;
    zpl_unused(world);
    librg_event_t *e = (librg_event_t*)event;
    return e->buffer;
}

int32_t librg_event_size_get(librg_world *world, librg_event *event) {
    LIBRG_ASSERT(event); if (!event) return LIBRG_EVENT_INVALID;
    zpl_unused(world);
    librg_event_t *e = (librg_event_t*)event;
    return e->size;
}

void * librg_event_userdata_get(librg_world *world, librg_event *event) {
    LIBRG_ASSERT(event); if (!event) return NULL;
    zpl_unused(world);
    librg_event_t *e = (librg_event_t*)event;
    return e->userdata;
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

librg_chunk librg_chunk_from_chunkpos(librg_world *world, int16_t chunk_x, int16_t chunk_y, int16_t chunk_z) {
    LIBRG_ASSERT(world); if (!world) return LIBRG_WORLD_INVALID;
    librg_world_t *wld = (librg_world_t *)world;

    int16_t chx = librg_util_chunkoffset_line(chunk_x, wld->chunkoffset.x, wld->worldsize.x);
    int16_t chy = librg_util_chunkoffset_line(chunk_y, wld->chunkoffset.y, wld->worldsize.y);
    int16_t chz = librg_util_chunkoffset_line(chunk_z, wld->chunkoffset.z, wld->worldsize.z);

    librg_chunk id = (chz * wld->worldsize.y * wld->worldsize.z) + (chy * wld->worldsize.y) + (chx);

    if (id < 0 || id > (wld->worldsize.x * wld->worldsize.y * wld->worldsize.z)) {
        return LIBRG_CHUNK_INVALID;
    }

    return id;
}

int8_t librg_chunk_to_chunkpos(librg_world *world, librg_chunk id, int16_t *chunk_x, int16_t *chunk_y, int16_t *chunk_z) {
    LIBRG_ASSERT(world); if (!world) return LIBRG_WORLD_INVALID;
    librg_world_t *wld = (librg_world_t *)world;

    if (id < 0 || id > (wld->worldsize.x * wld->worldsize.y * wld->worldsize.z)) {
        return LIBRG_CHUNK_INVALID;
    }

    int16_t z = (int16_t)(id / (wld->worldsize.z * wld->worldsize.y));
    int16_t r1 = (int16_t)(id % (wld->worldsize.z * wld->worldsize.y));
    int16_t y = r1 / wld->worldsize.y;
    int16_t x = r1 % wld->worldsize.y;

    *chunk_x = x - librg_util_chunkoffset_line(0, wld->chunkoffset.x, wld->worldsize.x);
    *chunk_y = y - librg_util_chunkoffset_line(0, wld->chunkoffset.y, wld->worldsize.y);
    *chunk_z = z - librg_util_chunkoffset_line(0, wld->chunkoffset.z, wld->worldsize.z);

    return LIBRG_OK;
}

librg_chunk librg_chunk_from_realpos(librg_world *world, double x, double y, double z) {
    LIBRG_ASSERT(world); if (!world) return LIBRG_WORLD_INVALID;
    librg_world_t *wld = (librg_world_t *)world;
    return librg_chunk_from_chunkpos(world, (int16_t)(x/wld->chunksize.x), (int16_t)(y/wld->chunksize.y), (int16_t)(z/wld->chunksize.z));
}

LIBRG_END_C_DECLS
