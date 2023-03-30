// file: source/entity.c

#ifdef LIBRG_EDITOR
#include <librg.h>
#include <zpl.h>
#endif

LIBRG_BEGIN_C_DECLS

// =======================================================================//
// !
// ! Basic entity manipulation
// !
// =======================================================================//

int8_t librg_entity_track(librg_world *world, int64_t entity_id) {
    LIBRG_ASSERT(world); if (!world) return LIBRG_WORLD_INVALID;
    librg_world_t *wld = (librg_world_t *)world;

    if (librg_entity_tracked(world, entity_id) == LIBRG_TRUE) {
        return LIBRG_ENTITY_ALREADY_TRACKED;
    }

    if (entity_id < 0 || entity_id > ZPL_I64_MAX) {
        return LIBRG_ENTITY_INVALID;
    }

    librg_entity_t _entity = {0};
    librg_table_ent_set(&wld->entity_map, entity_id, _entity);

    /* set defaults */
    librg_entity_chunk_set(world, entity_id, LIBRG_CHUNK_INVALID);
    librg_entity_owner_set(world, entity_id, LIBRG_OWNER_INVALID);

    return LIBRG_OK;
}

int8_t librg_entity_untrack(librg_world *world, int64_t entity_id) {
    LIBRG_ASSERT(world); if (!world) return LIBRG_WORLD_INVALID;
    librg_world_t *wld = (librg_world_t *)world;
    librg_entity_t *entity = librg_table_ent_get(&wld->entity_map, entity_id);

    if (!entity) {
        return LIBRG_ENTITY_UNTRACKED;
    }

    if (entity->flag_foreign == LIBRG_TRUE) {
        return LIBRG_ENTITY_FOREIGN;
    }

    /* cleanup owner snapshots */
    if (entity->owner_id != LIBRG_OWNER_INVALID) {
        size_t owned = 0;
        size_t total = zpl_array_count(wld->entity_map.entries);

        /* count already owned entities by this user */
        for (size_t i=0; i<total; i++) {
            if (librg_entity_owner_get(world, wld->entity_map.entries[i].key) == entity->owner_id)
                owned++;
        }

        librg_table_i64 *snapshot = librg_table_tbl_get(&wld->owner_map, entity->owner_id);

        /* free up our snapshot storage, if owner does not own other entities (except current one) */
        if (snapshot && owned <= 1) {
            librg_table_i64_destroy(snapshot);
            librg_table_tbl_remove(&wld->owner_map, entity->owner_id);
        }

        /* cleanup owner-entity pair */
        for (int i = 0; i < zpl_array_count(wld->owner_entity_pairs); ++i) {
            if (wld->owner_entity_pairs[i].entity_id == entity_id) {
                zpl_array_remove_at(wld->owner_entity_pairs, i);
                break;
            }
        }
    }

    /* cleanup owner visibility */
    if (entity->flag_visbility_owner_enabled) {
        entity->flag_visbility_owner_enabled = LIBRG_FALSE;
        librg_table_i8_destroy(&entity->owner_visibility_map);
    }

    librg_table_ent_remove(&wld->entity_map, entity_id);
    return LIBRG_OK;
}

int8_t librg_entity_tracked(librg_world *world, int64_t entity_id) {
    LIBRG_ASSERT(world); if (!world) return LIBRG_FALSE;
    librg_world_t *wld = (librg_world_t *)world;

    librg_entity_t *entity = librg_table_ent_get(&wld->entity_map, entity_id);
    return entity == NULL ? LIBRG_FALSE : LIBRG_TRUE;
}

int8_t librg_entity_foreign(librg_world *world, int64_t entity_id) {
    LIBRG_ASSERT(world); if (!world) return LIBRG_FALSE;
    librg_world_t *wld = (librg_world_t *)world;

    librg_entity_t *entity = librg_table_ent_get(&wld->entity_map, entity_id);
    if (entity == NULL) return LIBRG_FALSE;

    return entity->flag_foreign == LIBRG_TRUE;
}

int8_t librg_entity_owned(librg_world *world, int64_t entity_id) {
    LIBRG_ASSERT(world); if (!world) return LIBRG_FALSE;
    librg_world_t *wld = (librg_world_t *)world;

    librg_entity_t *entity = librg_table_ent_get(&wld->entity_map, entity_id);
    if (entity == NULL) return LIBRG_FALSE;

    return entity->owner_id != LIBRG_OWNER_INVALID;
}

int32_t librg_entity_count(librg_world *world) {
    LIBRG_ASSERT(world); if (!world) return LIBRG_WORLD_INVALID;
    librg_world_t *wld = (librg_world_t *)world;

    return (int32_t)zpl_array_count(wld->entity_map.entries);
}

// =======================================================================//
// !
// ! Main entity data methods
// !
// =======================================================================//

int8_t librg_entity_chunk_set(librg_world *world, int64_t entity_id, librg_chunk chunk) {
    LIBRG_ASSERT(world); if (!world) return LIBRG_WORLD_INVALID;
    librg_world_t *wld = (librg_world_t *)world;

    librg_entity_t *entity = librg_table_ent_get(&wld->entity_map, entity_id);
    if (entity == NULL) return LIBRG_ENTITY_UNTRACKED;

    for (int i = 0; i < LIBRG_ENTITY_MAXCHUNKS; ++i) entity->chunks[i] = LIBRG_CHUNK_INVALID;
    entity->chunks[0] = chunk;

    return LIBRG_OK;
}

librg_chunk librg_entity_chunk_get(librg_world *world, int64_t entity_id) {
    LIBRG_ASSERT(world); if (!world) return LIBRG_WORLD_INVALID;
    librg_world_t *wld = (librg_world_t *)world;

    librg_entity_t *entity = librg_table_ent_get(&wld->entity_map, entity_id);
    if (entity == NULL) return LIBRG_ENTITY_UNTRACKED;

    return entity->chunks[0];
}

int8_t librg_entity_owner_set(librg_world *world, int64_t entity_id, int64_t owner_id) {
    LIBRG_ASSERT(world); if (!world) return LIBRG_WORLD_INVALID;
    librg_world_t *wld = (librg_world_t *)world;

    librg_entity_t *entity = librg_table_ent_get(&wld->entity_map, entity_id);
    if (entity == NULL) return LIBRG_ENTITY_UNTRACKED;

    if (entity->flag_foreign == LIBRG_TRUE) {
        return LIBRG_ENTITY_FOREIGN;
    }

    /* update owner-entity pairing */
    if (owner_id != LIBRG_OWNER_INVALID) {
        bool ownership_pair_found = false;
        for (int i = 0; i < zpl_array_count(wld->owner_entity_pairs) && !ownership_pair_found; ++i) {
            if (wld->owner_entity_pairs[i].entity_id == entity_id) {
                ownership_pair_found = true;

                /* update owner if we found the entity */
                if (wld->owner_entity_pairs[i].owner_id != owner_id) {
                    wld->owner_entity_pairs[i].owner_id = owner_id;
                }
            }
        }
        if (!ownership_pair_found) {
            librg_owner_entity_pair_t pair = { owner_id, entity_id };
            zpl_array_append(wld->owner_entity_pairs, pair);
        }
    } else {
        if (entity->owner_id != LIBRG_OWNER_INVALID) {
            /* cleanup owner-entity pair */
            for (int i = 0; i < zpl_array_count(wld->owner_entity_pairs); ++i) {
                if (wld->owner_entity_pairs[i].entity_id == entity_id) {
                    zpl_array_remove_at(wld->owner_entity_pairs, i);
                    break;
                }
            }
        }
    }

    entity->owner_id = owner_id;
    entity->flag_owner_updated = LIBRG_TRUE;

    if (entity->owner_id != LIBRG_OWNER_INVALID) {
        /* set new token, and make sure to prevent collisions */
        uint16_t newtoken = 0;
        do { newtoken = (uint16_t)(zpl_random_gen_u32(&wld->random) % ZPL_U16_MAX); }
        while (newtoken == 0 || newtoken == entity->ownership_token);
        entity->ownership_token = newtoken;

        /* fetch or create a new subtable */
        librg_table_i64 *snapshot = librg_table_tbl_get(&wld->owner_map, owner_id);

        if (!snapshot) {
            librg_table_i64 _i64 = {0};
            librg_table_tbl_set(&wld->owner_map, owner_id, _i64);
            snapshot = librg_table_tbl_get(&wld->owner_map, owner_id);
            librg_table_i64_init(snapshot, wld->allocator);
        }
    } else {
        entity->ownership_token = 0;
    }

    return LIBRG_OK;
}

int64_t librg_entity_owner_get(librg_world *world, int64_t entity_id) {
    LIBRG_ASSERT(world); if (!world) return LIBRG_WORLD_INVALID;
    librg_world_t *wld = (librg_world_t *)world;

    librg_entity_t *entity = librg_table_ent_get(&wld->entity_map, entity_id);
    if (entity == NULL) return LIBRG_ENTITY_UNTRACKED;

    return entity->owner_id;
}

LIBRG_DEPRECATED(7.0) int8_t librg_entity_radius_set(librg_world *world, int64_t entity_id, int8_t observed_chunk_radius) {
    zpl_unused(world);
    zpl_unused(entity_id);
    zpl_unused(observed_chunk_radius);
    return -1;
}

LIBRG_DEPRECATED(7.0) int8_t librg_entity_radius_get(librg_world *world, int64_t entity_id) {
    zpl_unused(world);
    zpl_unused(entity_id);
    return -1;
}

int8_t librg_entity_dimension_set(librg_world *world, int64_t entity_id, int32_t dimension) {
    LIBRG_ASSERT(world); if (!world) return LIBRG_WORLD_INVALID;
    librg_world_t *wld = (librg_world_t *)world;

    librg_entity_t *entity = librg_table_ent_get(&wld->entity_map, entity_id);
    if (entity == NULL) return LIBRG_ENTITY_UNTRACKED;

    entity->dimension = dimension;
    return LIBRG_OK;
}

int32_t librg_entity_dimension_get(librg_world *world, int64_t entity_id) {
    LIBRG_ASSERT(world); if (!world) return LIBRG_WORLD_INVALID;
    librg_world_t *wld = (librg_world_t *)world;

    librg_entity_t *entity = librg_table_ent_get(&wld->entity_map, entity_id);
    if (entity == NULL) return LIBRG_ENTITY_UNTRACKED;

    return entity->dimension;
}

int8_t librg_entity_userdata_set(librg_world *world, int64_t entity_id, void *data) {
    LIBRG_ASSERT(world); if (!world) return LIBRG_WORLD_INVALID;
    librg_world_t *wld = (librg_world_t *)world;

    librg_entity_t *entity = librg_table_ent_get(&wld->entity_map, entity_id);
    if (entity == NULL) return LIBRG_ENTITY_UNTRACKED;

    entity->userdata = data;
    return LIBRG_OK;
}

void *librg_entity_userdata_get(librg_world *world, int64_t entity_id) {
    LIBRG_ASSERT(world); if (!world) return NULL;
    librg_world_t *wld = (librg_world_t *)world;

    librg_entity_t *entity = librg_table_ent_get(&wld->entity_map, entity_id);
    if (entity == NULL) return NULL;

    return entity->userdata;
}

int8_t librg_entity_chunkarray_set(librg_world *world, int64_t entity_id, const librg_chunk *values, size_t chunk_amount) {
    LIBRG_ASSERT(world); if (!world) return LIBRG_WORLD_INVALID;
    librg_world_t *wld = (librg_world_t *)world;

    librg_entity_t *entity = librg_table_ent_get(&wld->entity_map, entity_id);
    if (entity == NULL) return LIBRG_ENTITY_UNTRACKED;

    LIBRG_ASSERT(chunk_amount > 0 && chunk_amount < LIBRG_ENTITY_MAXCHUNKS);

    for (int i = 0; i < LIBRG_ENTITY_MAXCHUNKS; ++i) entity->chunks[i] = LIBRG_CHUNK_INVALID;
    zpl_memcopy(entity->chunks, values, sizeof(librg_chunk) * LIBRG_MIN(chunk_amount, LIBRG_ENTITY_MAXCHUNKS));

    return LIBRG_OK;

}

int8_t librg_entity_chunkarray_get(librg_world *world, int64_t entity_id, librg_chunk *results, size_t *chunk_amount) {
    LIBRG_ASSERT(world); if (!world) return LIBRG_WORLD_INVALID;
    librg_world_t *wld = (librg_world_t *)world;

    librg_entity_t *entity = librg_table_ent_get(&wld->entity_map, entity_id);
    if (entity == NULL) return LIBRG_ENTITY_UNTRACKED;

    LIBRG_ASSERT(results);
    size_t count = 0;
    size_t buffer_limit = *chunk_amount;

    for (size_t i = 0; i < LIBRG_MIN(buffer_limit, LIBRG_ENTITY_MAXCHUNKS); ++i) {
        if (entity->chunks[i] != LIBRG_CHUNK_INVALID) {
            results[count++] = entity->chunks[i];
        }
    }

    *chunk_amount = count;
    return (int8_t)(LIBRG_ENTITY_MAXCHUNKS - buffer_limit);
}


int8_t librg_entity_visibility_global_set(librg_world *world, int64_t entity_id, librg_visibility value) {
    LIBRG_ASSERT(world); if (!world) return LIBRG_WORLD_INVALID;
    librg_world_t *wld = (librg_world_t *)world;

    librg_entity_t *entity = librg_table_ent_get(&wld->entity_map, entity_id);
    if (entity == NULL) return LIBRG_ENTITY_UNTRACKED;

    entity->visibility_global = value;

    return LIBRG_OK;
}

int8_t librg_entity_visibility_global_get(librg_world *world, int64_t entity_id) {
    LIBRG_ASSERT(world); if (!world) return LIBRG_WORLD_INVALID;
    librg_world_t *wld = (librg_world_t *)world;

    librg_entity_t *entity = librg_table_ent_get(&wld->entity_map, entity_id);
    if (entity == NULL) return LIBRG_ENTITY_UNTRACKED;

    return entity->visibility_global;
}

int8_t librg_entity_visibility_owner_set(librg_world *world, int64_t entity_id, int64_t owner_id, librg_visibility value) {
    LIBRG_ASSERT(world); if (!world) return LIBRG_WORLD_INVALID;
    librg_world_t *wld = (librg_world_t *)world;

    librg_entity_t *entity = librg_table_ent_get(&wld->entity_map, entity_id);
    if (entity == NULL) return LIBRG_ENTITY_UNTRACKED;

    if (!entity->flag_visbility_owner_enabled) {
        entity->flag_visbility_owner_enabled = LIBRG_TRUE;
        librg_table_i8_init(&entity->owner_visibility_map, wld->allocator);
    }

    librg_table_i8_set(&entity->owner_visibility_map, owner_id, value);

    return LIBRG_OK;
}

int8_t librg_entity_visibility_owner_get(librg_world *world, int64_t entity_id, int64_t owner_id) {
    LIBRG_ASSERT(world); if (!world) return LIBRG_WORLD_INVALID;
    librg_world_t *wld = (librg_world_t *)world;

    librg_entity_t *entity = librg_table_ent_get(&wld->entity_map, entity_id);
    if (entity == NULL) return LIBRG_ENTITY_UNTRACKED;

    if (!entity->flag_visbility_owner_enabled)
        return LIBRG_VISIBLITY_DEFAULT;

    int8_t *value = librg_table_i8_get(&entity->owner_visibility_map, owner_id);
    return (value ? *value : LIBRG_VISIBLITY_DEFAULT);
}

LIBRG_END_C_DECLS
