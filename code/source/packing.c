// file: source/packing.c

#ifdef LIBRG_EDITOR
#include <librg.h>
#include <zpl.h>
#endif

LIBRG_BEGIN_C_DECLS

// =======================================================================//
// !
// ! Primitives
// !
// =======================================================================//

LIBRG_PRAGMA(pack(push, 1));
typedef struct {
    uint64_t id;
    uint16_t token;
    uint16_t size;
} librg_segval_t;

typedef struct {
    uint8_t  type;
    uint8_t  unused_;
    uint16_t amount;
    uint32_t size;
} librg_segment_t;
LIBRG_PRAGMA(pack(pop));

LIBRG_STATIC_ASSERT(sizeof(librg_segval_t) == 12, "packed librg_segval_t should have a valid size");
LIBRG_STATIC_ASSERT(sizeof(librg_segment_t) == 8, "packed librg_segment_t should have a valid size");

// =======================================================================//
// !
// ! World data packing method
// !
// =======================================================================//

int32_t librg_world_write(librg_world *world, int64_t owner_id, char *buffer, size_t *size, void *userdata) {
    LIBRG_ASSERT(world); if (!world) return LIBRG_WORLD_INVALID;
    librg_world_t *wld = (librg_world_t *)world;
    librg_table_i64 *last_snapshot = librg_table_tbl_get(&wld->owner_map, owner_id);

    /* no snapshot - means we are asking an invalid owner */
    if (!last_snapshot) {
        *size = 0;
        return LIBRG_OWNER_INVALID;
    }

    /* get old, and preapre new snapshot handlers */
    librg_table_i64 next_snapshot = {0};
    librg_table_i64_init(&next_snapshot, wld->allocator);

    int64_t *results = LIBRG_MEM_ALLOC(LIBRG_WORLDWRITE_MAXQUERY * sizeof(int64_t));
    size_t total_amount = LIBRG_WORLDWRITE_MAXQUERY;
    librg_world_query(world, owner_id, results, &total_amount);

    size_t total_written = 0;
    librg_event_t evt = {0};

    #define sz_total (total_written + sizeof(librg_segment_t))
    #define sz_value (sz_total + value_written + sizeof(librg_segval_t))

    uint8_t action_id = LIBRG_WRITE_CREATE;
    size_t buffer_limit = *size;
    size_t insufficient_size = 0;

librg_lbl_ww:

    /* create and update */
    if (sz_total < buffer_limit) {
        librg_segment_t *seg = (librg_segment_t*)(buffer+total_written);
        char *segend = (buffer + sz_total);

        uint16_t amount = 0;
        size_t value_written = 0;
        size_t iterations = total_amount;

        int64_t entity_id = LIBRG_ENTITY_INVALID;
        int32_t condition = LIBRG_TRUE;
        librg_entity_t *entity_blob = NULL;

        /* for deletions we are iterating something else */
        if (action_id == LIBRG_WRITE_REMOVE) {
            iterations = zpl_array_count(last_snapshot->entries);
        }

        for (size_t i = 0; i < iterations; ++i) {
            int16_t action_rejected = LIBRG_TRUE;
            int32_t data_size = 0;

            /* preparation */
            if (action_id == LIBRG_WRITE_CREATE) {
                entity_id   = results[i]; /* it did not exist && not foreign */
                condition   = librg_table_i64_get(last_snapshot, entity_id) == NULL
                    && librg_entity_foreign(world, entity_id) != LIBRG_TRUE;
            }
            else if (action_id == LIBRG_WRITE_UPDATE) {
                entity_id   = results[i];  /* it did exist */
                entity_blob = librg_table_ent_get(&wld->entity_map, entity_id);
                condition   = librg_table_i64_get(last_snapshot, entity_id) != NULL || librg_entity_foreign(world, entity_id) == LIBRG_TRUE;

                /* mark entity as still alive, to prevent it from being removed */
                librg_table_i64_set(last_snapshot, entity_id, 2);
            }
            else if (action_id == LIBRG_WRITE_REMOVE) {
                entity_id   = last_snapshot->entries[i].key; /* it was not marked as updated && and not foreign */
                condition   = last_snapshot->entries[i].value != 2
                    && librg_entity_foreign(world, entity_id) != LIBRG_TRUE;
            }
            else if (action_id == LIBRG_WRITE_OWNER) {
                entity_id   = results[i]; /* if we are the owner and we havent yet notified reader about that */
                entity_blob = librg_table_ent_get(&wld->entity_map, entity_id);
                condition   = entity_blob
                    && entity_blob->owner_id == owner_id
                    && entity_blob->flag_owner_updated
                    && librg_table_i64_get(&next_snapshot, entity_id);
            }

            /* data write */
            if (condition && sz_value < buffer_limit) {
                librg_segval_t *val = (librg_segval_t*)(segend + value_written);
                char *valend = (segend + value_written + sizeof(librg_segval_t));

                /* fill in event */
                evt.entity_id = entity_id;
                evt.type = action_id;
                evt.size = buffer_limit - sz_value;
                evt.buffer = valend;
                evt.owner_id = owner_id;
                evt.userdata = userdata;

                /* call event handlers */
                if (wld->handlers[action_id]) {
                    data_size = (int32_t)wld->handlers[action_id](world, (librg_event*)&evt);
                }

                /* if user returned < 0, we consider that event rejected */
                if (data_size >= 0) {
                    /* fill in segval */
                    val->id = entity_id;
                    val->size = data_size;

                    if (action_id == LIBRG_WRITE_OWNER) {
                        val->token = entity_blob->ownership_token;
                    }
                    else if (action_id == LIBRG_WRITE_UPDATE && entity_blob->flag_foreign) {
                        val->token = entity_blob->ownership_token;
                    } else {
                        val->token = 0;
                    }

                    /* increase the total size written */
                    value_written += sizeof(librg_segval_t) + val->size;
                    action_rejected = LIBRG_FALSE;
                    amount++;
                }
            }

            /* accumulate insufficient buffer size */
            if (condition && sz_value >= buffer_limit) {
                insufficient_size += (sz_value - buffer_limit);
            }

            /* finaliztion */
            if (action_id == LIBRG_WRITE_CREATE && !action_rejected) {
                /* mark entity as created, so it can start updating */
                librg_table_i64_set(&next_snapshot, entity_id, 1);
            }
            else if (action_id == LIBRG_WRITE_UPDATE && condition) {
                /* consider entitry updated, without regards was it written or not */
                librg_table_i64_set(&next_snapshot, entity_id, 1);
            }
            else if (action_id == LIBRG_WRITE_REMOVE && condition && action_rejected) {
                /* consider entity alive, till we are able to send it */
                librg_table_i64_set(&next_snapshot, entity_id, 1);
            }
            else if (action_id == LIBRG_WRITE_OWNER && condition) {
                /* mark reader as notified */
                entity_blob->flag_owner_updated = LIBRG_FALSE;
            }
        }

        if (amount > 0) {
            seg->type = action_id;
            seg->size = (uint32_t)value_written;
            seg->amount = amount;

            total_written += sizeof(librg_segment_t) + seg->size;
        }
    } else {
        insufficient_size += sz_total - buffer_limit;
    }

    /* iterate it again till all tasks are finished */
    switch (action_id) {
        case LIBRG_WRITE_CREATE: action_id = LIBRG_WRITE_UPDATE; goto librg_lbl_ww;
        case LIBRG_WRITE_UPDATE: action_id = LIBRG_WRITE_REMOVE; goto librg_lbl_ww;
        case LIBRG_WRITE_REMOVE: action_id = LIBRG_WRITE_OWNER; goto librg_lbl_ww;
    }

    /* swap snapshot tables */
    librg_table_i64_destroy(last_snapshot);
    librg_table_tbl_set(&wld->owner_map, owner_id, next_snapshot);
    LIBRG_MEM_FREE(results);

    /* write our total size */
    *size = total_written;

    #undef sz_total
    #undef sz_value

    /* if we didnt have enough space, value will be > 0 */
    return insufficient_size;
}

// =======================================================================//
// !
// ! World data unpacking method
// !
// =======================================================================//

int32_t librg_world_read(librg_world *world, int64_t owner_id, const char *buffer, size_t size, void *userdata) {
    LIBRG_ASSERT(world); if (!world) return LIBRG_WORLD_INVALID;
    librg_world_t *wld = (librg_world_t *)world;

    librg_event_t evt = {0};
    size_t total_read = 0;

    #define sz_segment (total_read + sizeof(librg_segment_t))
    #define sz_segval (sz_segment + segment_read + sizeof(librg_segval_t))

    while ((size-total_read) > sizeof(librg_segment_t)) {
        librg_segment_t *seg = (librg_segment_t*)(buffer+total_read);
        size_t segment_read = 0;

        /* immidiately exit if we will not be able to read the segment data */
        if (sz_segment+seg->size > size || sz_segment + seg->amount * sizeof(librg_segval_t) > size) {
            break;
        }

        for (int i = 0; i < seg->amount; ++i) {
            librg_segval_t *val = (librg_segval_t*)(buffer+sz_segment+segment_read);
            librg_entity_t *entity_blob = librg_table_ent_get(&wld->entity_map, val->id);
            int8_t action_id = -1;

            /* do preparation for entity processing */
            if (seg->type == LIBRG_WRITE_CREATE) {
                /* attempt to create an entity */
                action_id = (librg_entity_track(world, val->id) == LIBRG_OK)
                    ? LIBRG_READ_CREATE
                    : LIBRG_ERROR_CREATE;
            }
            else if (seg->type == LIBRG_WRITE_UPDATE) {
                /* try to check if entity exists, and if it is foreign OR owner and token are correct */
                action_id = (librg_entity_tracked(world, val->id) == LIBRG_TRUE
                    && (entity_blob->flag_foreign
                        || (entity_blob->owner_id == owner_id
                            && entity_blob->ownership_token == val->token)))
                    ? LIBRG_READ_UPDATE
                    : LIBRG_ERROR_UPDATE;
            }
            else if (seg->type == LIBRG_WRITE_REMOVE) {
                /* attempt to check if it does exist and only foreign */
                action_id = (librg_entity_tracked(world, val->id) == LIBRG_TRUE
                    && librg_entity_foreign(world, val->id) == LIBRG_TRUE)
                    ? LIBRG_READ_REMOVE
                    : LIBRG_ERROR_REMOVE;
            }
            else if (seg->type == LIBRG_WRITE_OWNER) {
                /* attempt to check if it does exist and only foreign */
                action_id = (librg_entity_tracked(world, val->id) == LIBRG_TRUE
                    && librg_entity_foreign(world, val->id) == LIBRG_TRUE)
                    ? LIBRG_READ_OWNER
                    : LIBRG_ERROR_OWNER;
            }

            if (action_id == -1) {
                return LIBRG_READ_INVALID;
            }

            /* do the initial entity processing */
            if (action_id == LIBRG_READ_CREATE) {
                /* mark newly created entity as foreign */
                librg_entity_t *entity = librg_table_ent_get(&wld->entity_map, val->id);
                if (!entity) return LIBRG_READ_INVALID; else entity->flag_foreign = LIBRG_TRUE;
            }

            /* fill in event */
            evt.entity_id = val->id;
            evt.type = action_id;
            evt.size = val->size;
            evt.buffer = (char*)(buffer+sz_segval);
            evt.owner_id = owner_id;
            evt.userdata = userdata;

            /* call event handlers */
            if (wld->handlers[action_id]) {
                /*ignore response*/
                wld->handlers[action_id](world, &evt);
            }

            /* do the afterwork processing */
            if (action_id == LIBRG_READ_REMOVE) {
                /* remove foreign mark from entity */
                librg_entity_t *entity = librg_table_ent_get(&wld->entity_map, val->id);
                if (!entity) return LIBRG_READ_INVALID; else entity->flag_foreign = LIBRG_FALSE;
                librg_entity_untrack(world, val->id);
            }
            else if (action_id == LIBRG_READ_OWNER) {
                librg_entity_t *entity = librg_table_ent_get(&wld->entity_map, val->id);
                if (!entity) return LIBRG_READ_INVALID;

                /* immidiately mark entity as owned, set up & override additional info */
                entity->flag_foreign = LIBRG_FALSE; /* unmark it temp, while owner is set */
                librg_entity_owner_set(world, val->id, owner_id);
                entity->ownership_token = val->token;
                entity->flag_owner_updated = LIBRG_FALSE;
                entity->flag_foreign = LIBRG_TRUE;
            }

            segment_read += sizeof(librg_segval_t) + val->size;
        }

        /* validate sizes of the data we read */
        if (segment_read != seg->size) {
            return LIBRG_READ_INVALID;
        }

        total_read += sizeof(librg_segment_t) + segment_read;
    }

    #undef sz_segment
    #undef sz_segval

    if (total_read != size) {
        return (int32_t)(size - total_read);
    }

    return LIBRG_OK;
}

LIBRG_END_C_DECLS
