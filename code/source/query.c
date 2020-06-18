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
    return librg_world_fetch_chunkarray(world, (librg_chunk[]){chunk}, 1, entity_ids, entity_amount);
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
    return librg_world_fetch_ownerarray(world, (int64_t[]){owner_id}, 1, entity_ids, entity_amount);
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
    int radius2 = radius * radius;

    for (int z=-radius; z<=radius; z++) {
        for (int y=-radius; y<=radius; y++) {
            for (int x=-radius; x<=radius; x++) {
                if(x*x+y*y+z*z <= radius2) {
                    librg_chunk id = librg_chunk_from_chunkpos(w, cx+x, cy+y, cz+z);
                    if (id != LIBRG_CHUNK_INVALID) librg_table_i64_set(ch, id, 1);
                }
            }
        }
    }

    return;
}

int32_t librg_world_query(librg_world *world, int64_t owner_id, int64_t *entity_ids, size_t *entity_amount) {
    LIBRG_ASSERT(world); if (!world) return LIBRG_WORLD_INVALID;
    LIBRG_ASSERT(entity_amount); if (!entity_amount) return LIBRG_NULL_REFERENCE;
    librg_world_t *wld = (librg_world_t *)world;

    size_t buffer_limit = *entity_amount;
    size_t total_count = zpl_array_count(wld->entity_map.entries);

    librg_table_i64 results = {0};
    librg_table_tbl dimensions = {0};

    librg_table_i64_init(&results, wld->allocator);
    librg_table_tbl_init(&dimensions, wld->allocator);

    /* generate a map of visible chunks (only counting owned entities) */
    for (size_t i=0; i < total_count; ++i) {
        uint64_t entity_id = wld->entity_map.entries[i].key;
        librg_entity_t *entity = &wld->entity_map.entries[i].value;

        /* allways add self-owned entities */
        if (entity->owner_id == owner_id) {
            librg_table_i64_set(&results, entity_id, 1);
        }

        /* immidiately skip, if entity was not placed correctly */
        if (entity->chunks[0] == LIBRG_CHUNK_INVALID) continue;
        /* and skip, if used is not an owner of the entity */
        if (entity->owner_id != owner_id) continue;
        /* and skip if entity is not an observer */
        if (entity->observed_radius == 0) continue;

        /* fetch, or create chunk set in this dimension if does not exist */
        librg_table_i64 *dim_chunks = librg_table_tbl_get(&dimensions, entity->dimension);

        if (!dim_chunks) {
            librg_table_i64 _chunks = {0};
            librg_table_tbl_set(&dimensions, entity->dimension, _chunks);
            dim_chunks = librg_table_tbl_get(&dimensions, entity->dimension);
            librg_table_i64_init(dim_chunks, wld->allocator);
        }

        /* add entity chunks to the total visible chunks */
        for (int k = 0; k < LIBRG_ENTITY_MAXCHUNKS; ++k) {
            if (entity->chunks[k] == LIBRG_CHUNK_INVALID) break;

            int16_t chx=0, chy=0, chz=0;
            librg_chunk_to_chunkpos(world, entity->chunks[k], &chx, &chy, &chz);
            librg_util_chunkrange(world, dim_chunks, chx, chy, chz, entity->observed_radius);
        }
    }

    /* a slightly increased buffer_limit, that includes own entities */
    /* that allows us to prevent edge-cases where the code below will include our entities in the result as well */
    size_t owned_entities = zpl_array_count(results.entries);
    size_t buffer_limit_extended = buffer_limit + owned_entities;

    /* iterate on all entities, and check if they are inside of the interested chunks */
    for (size_t i=0; i < LIBRG_MIN(buffer_limit_extended, total_count); ++i) {
        uint64_t entity_id = wld->entity_map.entries[i].key;
        librg_entity_t *entity = &wld->entity_map.entries[i].value;
        librg_table_i64 *chunks = librg_table_tbl_get(&dimensions, entity->dimension);

        /* owner visiblity (personal)*/
        int8_t vis_owner = librg_entity_visibility_owner_get(world, entity_id, owner_id);
        if (vis_owner == LIBRG_VISIBLITY_NEVER) {
            continue; /* prevent from being included */
        }
        else if (vis_owner == LIBRG_VISIBLITY_ALWAYS) {
            librg_table_i64_set(&results, entity_id, 1); /* always included */
            continue;
        }

        /* global entity visiblity */
        int8_t vis_global = librg_entity_visibility_global_get(world, entity_id);
        if (vis_global == LIBRG_VISIBLITY_NEVER) {
            continue; /* prevent from being included */
        }
        else if (vis_global == LIBRG_VISIBLITY_ALWAYS) {
            librg_table_i64_set(&results, entity_id, 1); /* always included */
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
                    librg_table_i64_set(&results, entity_id, 1);
                    break;
                }
            }
        }
    }

    /* copy/transform results to a plain array */
    size_t count = zpl_array_count(results.entries);
    for (size_t i = 0; i < LIBRG_MIN(buffer_limit, count); ++i)
        entity_ids[i] = results.entries[i].key;

    /* free up temp data */
    for (int i = 0; i < zpl_array_count(dimensions.entries); ++i)
        librg_table_i64_destroy(&dimensions.entries[i].value);

    librg_table_tbl_destroy(&dimensions);
    librg_table_i64_destroy(&results);

    *entity_amount = LIBRG_MIN(buffer_limit, count);
    return LIBRG_MAX(0, (int32_t)(count - buffer_limit));
}

LIBRG_END_C_DECLS
