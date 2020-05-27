// file: source/world.c

#ifdef LIBRG_EDITOR
#include <librg.h>
#include <zpl.h>
#endif

LIBRG_BEGIN_C_DECLS

enum {
    LIBRG_OWNER_SET = LIBRG_READ_REMOVE+1,
    LIBRG_OWNER_REMOVE,
    LIBRG_OWNER_UPDATE,
};

// =======================================================================//
// !
// ! World data (de)serialization methods
// !
// =======================================================================//

// size_t librg_world_write(librg_world *world, int64_t owner_id, char *buffer, size_t buffer_limit, void *userdata) {
//     LIBRG_ASSERT(world); if (!world) return LIBRG_WORLD_INVALID;
//     librg_world_t *wld = (librg_world_t *)world;

//     int64_t results[LIBRG_WORLDWRITE_MAXQUERY] = {0};
//     size_t amount = librg_world_query(world, owner_id, results, LIBRG_WORLDWRITE_MAXQUERY);

//     for (int i = 0; i < amount; ++i) {
//         int64_t entity_id = results[i];
//     }

//     return 0;
// }

// int8_t librg_world_read(librg_world *, int64_t owner_id, char *buffer, size_t size, void *userdata) {

// }

// =======================================================================//
// !
// ! Simple general fetching methods
// !
// =======================================================================//

size_t librg_world_fetch_all(librg_world *world, int64_t *entity_ids, size_t buffer_limit) {
    LIBRG_ASSERT(world); if (!world) return LIBRG_WORLD_INVALID;
    librg_world_t *wld = (librg_world_t *)world;

    size_t count = 0;
    size_t total_count = zpl_array_count(wld->entity_map.entries);

    for (size_t i=0; i < LIBRG_MIN(buffer_limit, total_count); ++i) {
        entity_ids[count++] = wld->entity_map.entries[i].key;
    }

    return count;
}

size_t librg_world_fetch_chunk(librg_world *world, librg_chunk chunk, int64_t *entity_ids, size_t buffer_limit) {
    return librg_world_fetch_chunkarray(world, (librg_chunk[]){chunk}, 1, entity_ids, buffer_limit);
}

size_t librg_world_fetch_chunkarray(librg_world *world, librg_chunk *chunks, size_t chunk_amount, int64_t *entity_ids, size_t buffer_limit) {
    LIBRG_ASSERT(world); if (!world) return LIBRG_WORLD_INVALID;
    librg_world_t *wld = (librg_world_t *)world;

    size_t count = 0;
    size_t total_count = zpl_array_count(wld->entity_map.entries);

    for (size_t i=0; i < LIBRG_MIN(buffer_limit, total_count); ++i) {
        uint64_t entity_id = wld->entity_map.entries[i].key;
        librg_entity_t *entity = &wld->entity_map.entries[i].value;

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

    return count;
}

size_t librg_world_fetch_owner(librg_world *world, int64_t owner_id, int64_t *entity_ids, size_t buffer_limit) {
    return librg_world_fetch_ownerarray(world, (int64_t[]){owner_id}, 1, entity_ids, buffer_limit);
}

size_t librg_world_fetch_ownerarray(librg_world *world, int64_t *owner_ids, size_t owner_amount, int64_t *entity_ids, size_t buffer_limit) {
    LIBRG_ASSERT(world); if (!world) return LIBRG_WORLD_INVALID;
    librg_world_t *wld = (librg_world_t *)world;

    size_t count = 0;
    size_t total_count = zpl_array_count(wld->entity_map.entries);

    for (size_t i=0; i < LIBRG_MIN(buffer_limit, total_count); ++i) {
        uint64_t entity_id = wld->entity_map.entries[i].key;
        librg_entity_t *entity = &wld->entity_map.entries[i].value;

        for (size_t k = 0; k < owner_amount; ++k) {
            int64_t owner_id = owner_ids[k];
            if (entity->owner_id == owner_id) entity_ids[count++] = entity_id;
        }
    }

    return count;
}

// =======================================================================//
// !
// ! Main owner entity query method
// !
// =======================================================================//

ZPL_TABLE(static inline, librg_table_dim, librg_table_dim_, librg_table_i64);

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

size_t librg_world_query(librg_world *world, int64_t owner_id, int64_t *entity_ids, size_t buffer_limit) {
    LIBRG_ASSERT(world); if (!world) return LIBRG_WORLD_INVALID;
    librg_world_t *wld = (librg_world_t *)world;

    size_t total_count = zpl_array_count(wld->entity_map.entries);

    librg_table_i64 results = {0};
    librg_table_dim dimensions = {0};

    librg_table_i64_init(&results, wld->allocator);
    librg_table_dim_init(&dimensions, wld->allocator);

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
        librg_table_i64 *dim_chunks = librg_table_dim_get(&dimensions, entity->dimension);

        if (!dim_chunks) {
            librg_table_i64 _chunks = {0};
            librg_table_dim_set(&dimensions, entity->dimension, _chunks);
            dim_chunks = librg_table_dim_get(&dimensions, entity->dimension);
            librg_table_i64_init(dim_chunks, wld->allocator);
        }

        /* add entity chunks to the total visible chunks */
        for (int k = 0; k < LIBRG_ENTITY_MAXCHUNKS; ++k) {
            if (entity->chunks[k] == LIBRG_CHUNK_INVALID) break;

            int16_t chx, chy, chz;
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
        librg_table_i64 *chunks = librg_table_dim_get(&dimensions, entity->dimension);

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

    librg_table_dim_destroy(&dimensions);
    librg_table_i64_destroy(&results);

    return LIBRG_MIN(buffer_limit, count);

}

LIBRG_END_C_DECLS
