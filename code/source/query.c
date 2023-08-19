// file: source/world.c

#ifdef LIBRG_EDITOR
#include <librg.h>
#include <zpl.h>
#endif

LIBRG_BEGIN_C_DECLS

// =======================================================================//
// !
// ! Simple general fetching methods
// !
// =======================================================================//

int32_t librg_world_fetch_all(librg_world *world, int64_t *entity_ids, size_t *entity_amount) {
    LIBRG_ASSERT(world); if (!world) return LIBRG_WORLD_INVALID;
    LIBRG_ASSERT(entity_amount); if (!entity_amount) return LIBRG_NULL_REFERENCE;
    librg_world_t *wld = (librg_world_t *)world;

    size_t count = 0;
    size_t buffer_limit = *entity_amount;
    size_t total_count = zpl_array_count(wld->entity_map.entries);

    for (size_t i=0; i < LIBRG_MIN(buffer_limit, total_count); ++i) {
        entity_ids[count++] = wld->entity_map.entries[i].key;
    }

    *entity_amount = count;
    return LIBRG_MAX(0, (int32_t)(total_count - buffer_limit));
}

int32_t librg_world_fetch_chunk(librg_world *world, librg_chunk chunk, int64_t *entity_ids, size_t *entity_amount) {
    librg_chunk chunks[1]; chunks[0] = chunk;
    return librg_world_fetch_chunkarray(world, chunks, 1, entity_ids, entity_amount);
}

int32_t librg_world_fetch_chunkarray(librg_world *world, const librg_chunk *chunks, size_t chunk_amount, int64_t *entity_ids, size_t *entity_amount) {
    LIBRG_ASSERT(world); if (!world) return LIBRG_WORLD_INVALID;
    LIBRG_ASSERT(entity_amount); if (!entity_amount) return LIBRG_NULL_REFERENCE;
    librg_world_t *wld = (librg_world_t *)world;

    size_t count = 0;
    size_t iterated = 0;
    size_t buffer_limit = *entity_amount;
    size_t total_count = zpl_array_count(wld->entity_map.entries);

    for (size_t i=0; i < LIBRG_MIN(buffer_limit, total_count); ++i) {
        uint64_t entity_id = wld->entity_map.entries[i].key;
        librg_entity_t *entity = &wld->entity_map.entries[i].value;
        iterated++;

        for (size_t k = 0; k < chunk_amount; ++k) {
            librg_chunk chunk = chunks[k];

            for (size_t j=0; j < LIBRG_ENTITY_MAXCHUNKS; ++j) {
                if (entity->chunks[j] == chunk) {
                    entity_ids[count++] = entity_id;
                    break;
                }

                /* immidiately exit if chunk is invalid (the rest will also be invalid) */
                if (entity->chunks[j] == LIBRG_CHUNK_INVALID) {
                    break;
                }
            }
        }
    }

    *entity_amount = count;
    return LIBRG_MAX(0, (int32_t)(total_count - iterated));
}

int32_t librg_world_fetch_owner(librg_world *world, int64_t owner_id, int64_t *entity_ids, size_t *entity_amount) {
    int64_t owner_ids[1]; owner_ids[0] = owner_id;
    return librg_world_fetch_ownerarray(world, owner_ids, 1, entity_ids, entity_amount);
}

int32_t librg_world_fetch_ownerarray(librg_world *world, const int64_t *owner_ids, size_t owner_amount, int64_t *entity_ids, size_t *entity_amount) {
    LIBRG_ASSERT(world); if (!world) return LIBRG_WORLD_INVALID;
    LIBRG_ASSERT(entity_amount); if (!entity_amount) return LIBRG_NULL_REFERENCE;
    librg_world_t *wld = (librg_world_t *)world;

    size_t count = 0;
    size_t iterated = 0;
    size_t buffer_limit = *entity_amount;
    size_t total_count = zpl_array_count(wld->entity_map.entries);

    for (size_t i=0; i < LIBRG_MIN(buffer_limit, total_count); ++i) {
        uint64_t entity_id = wld->entity_map.entries[i].key;
        librg_entity_t *entity = &wld->entity_map.entries[i].value;
        iterated++;

        for (size_t k = 0; k < owner_amount; ++k) {
            int64_t owner_id = owner_ids[k];
            if (entity->owner_id == owner_id) entity_ids[count++] = entity_id;
        }
    }

    *entity_amount = count;
    return LIBRG_MAX(0, (int32_t)(total_count - iterated));
}

// =======================================================================//
// !
// ! Main owner entity query method
// !
// =======================================================================//

static LIBRG_ALWAYS_INLINE void librg_util_chunkrange(librg_world *w, librg_table_i64 *ch, int cx, int cy, int cz, int8_t radius) {
    /* precalculate the radius power 2 for quicker distance check */
    int radius2 = radius * radius;

    /* create a "bubble" by cutting off chunks outside of radius using distance checks */
    for (int z=-radius; z<=radius; z++) {
        for (int y=-radius; y<=radius; y++) {
            for (int x=-radius; x<=radius; x++) {
                if (x*x+y*y+z*z <= radius2) {
                    librg_chunk id = librg_chunk_from_chunkpos(w, cx+x, cy+y, cz+z);
                    if (id != LIBRG_CHUNK_INVALID) librg_table_i64_set(ch, id, 1);
                }
            }
        }
    }

    return;
}

int32_t librg_world_query(librg_world *world, int64_t owner_id, uint8_t chunk_radius, int64_t *entity_ids, size_t *entity_amount) {
    LIBRG_ASSERT(world); if (!world) return LIBRG_WORLD_INVALID;
    LIBRG_ASSERT(entity_amount); if (!entity_amount) return LIBRG_NULL_REFERENCE;
    librg_world_t *wld = (librg_world_t *)world;

    size_t buffer_limit = *entity_amount;
    size_t total_count = zpl_array_count(wld->entity_map.entries);
    size_t result_amount = 0;

    /* mini helper for pushing entity */
    /* if it will overflow do not push, just increase counter for future statistics */
    #define librg_push_entity(entity_id) \
        { if (result_amount + 1 <= buffer_limit) entity_ids[result_amount++] = entity_id; else result_amount++; }

    /* generate a map of visible chunks (only counting owned entities) */
    for (int i = 0; i < zpl_array_count(wld->owner_entity_pairs); ++i) {
        if (wld->owner_entity_pairs[i].owner_id != owner_id) continue;

        uint64_t entity_id = wld->owner_entity_pairs[i].entity_id;
        librg_entity_t *entity = librg_table_ent_get(&wld->entity_map, entity_id);

        /* allways add self-owned entities */
        int8_t vis_owner = librg_entity_visibility_owner_get(world, entity_id, owner_id);
        if (vis_owner != LIBRG_VISIBLITY_NEVER) {
            /* prevent from being included */
            librg_push_entity(entity_id);
        }

        /* immidiately skip, if entity was not placed correctly */
        if (entity->chunks[0] == LIBRG_CHUNK_INVALID) continue;
        /* and skip, if used is not an owner of the entity */
        if (entity->owner_id != owner_id) continue;

        /* fetch, or create chunk set in this dimension if does not exist */
        librg_table_i64 *dim_chunks = librg_table_tbl_get(&wld->dimensions, entity->dimension);

        if (!dim_chunks) {
            librg_table_i64 _chunks = {0};
            librg_table_tbl_set(&wld->dimensions, entity->dimension, _chunks);
            dim_chunks = librg_table_tbl_get(&wld->dimensions, entity->dimension);
            librg_table_i64_init(dim_chunks, wld->allocator);
        }

        /* add entity chunks to the total visible chunks */
        for (int k = 0; k < LIBRG_ENTITY_MAXCHUNKS; ++k) {
            if (entity->chunks[k] == LIBRG_CHUNK_INVALID) break;

            int16_t chx=0, chy=0, chz=0;
            librg_chunk_to_chunkpos(world, entity->chunks[k], &chx, &chy, &chz);
            librg_util_chunkrange(world, dim_chunks, chx, chy, chz, chunk_radius);
        }
    }

    /* iterate on all entities, and check if they are inside of the interested chunks */
    for (size_t i=0; i < total_count; ++i) {
        uint64_t entity_id = wld->entity_map.entries[i].key;
        librg_entity_t *entity = &wld->entity_map.entries[i].value;
        librg_table_i64 *chunks = librg_table_tbl_get(&wld->dimensions, entity->dimension);

        if (entity->owner_id == owner_id) continue;

        /* owner visibility (personal)*/
        int8_t vis_owner = librg_entity_visibility_owner_get(world, entity_id, owner_id);
        if (vis_owner == LIBRG_VISIBLITY_NEVER) {
            continue; /* prevent from being included */
        }
        else if (vis_owner == LIBRG_VISIBLITY_ALWAYS) {
            librg_push_entity(entity_id);
            continue;
        }

        /* global entity visibility */
        int8_t vis_global = librg_entity_visibility_global_get(world, entity_id);
        if (vis_global == LIBRG_VISIBLITY_NEVER) {
            continue; /* prevent from being included */
        }
        else if (vis_global == LIBRG_VISIBLITY_ALWAYS) {
            librg_push_entity(entity_id);
            continue;
        }

        /* skip if there are no chunks in this dimension */
        if (!chunks) continue;
        size_t chunk_amount = zpl_array_count(chunks->entries);

        for (size_t k = 0; k < chunk_amount; ++k) {
            librg_chunk chunk = chunks->entries[k].key;

            for (size_t j=0; j < LIBRG_ENTITY_MAXCHUNKS; ++j) {
                /* immidiately exit if chunk is invalid (the rest will also be invalid) */
                if (entity->chunks[j] == LIBRG_CHUNK_INVALID) break;

                /* add entity and continue to the next one */
                if (entity->chunks[j] == chunk) {
                    librg_push_entity(entity_id);
                    break;
                }
            }
        }
    }

    /* free up temp data */
    for (int i = 0; i < zpl_array_count(wld->dimensions.entries); ++i)
        librg_table_i64_destroy(&wld->dimensions.entries[i].value);

    librg_table_tbl_clear(&wld->dimensions);

    #undef librg_push_entity

    *entity_amount = LIBRG_MIN(buffer_limit, result_amount);
    return LIBRG_MAX(0, (int32_t)(result_amount - buffer_limit));
}

LIBRG_END_C_DECLS
