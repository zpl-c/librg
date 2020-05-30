// file: source/packing.c

#ifdef LIBRG_EDITOR
#include <librg.h>
#include <zpl.h>
#endif

LIBRG_BEGIN_C_DECLS

// =======================================================================//
// !
// ! World data (de)packing methods
// !
// =======================================================================//

enum {
    LIBRG_OWNER_SET = LIBRG_READ_REMOVE+1,
    LIBRG_OWNER_REMOVE,
    LIBRG_OWNER_UPDATE,
};

LIBRG_PRAGMA(pack(push, 1));
typedef struct {
    uint64_t id;
    uint16_t size;
} librg_segval_t;

typedef struct {
    uint8_t  type;
    uint8_t  unused_;
    uint16_t amount;
    uint32_t size;
} librg_segment_t;
LIBRG_PRAGMA(pack(pop));

LIBRG_STATIC_ASSERT(sizeof(librg_segval_t) == 10, "packed librg_segval_t should have a valid size");
LIBRG_STATIC_ASSERT(sizeof(librg_segment_t) == 8, "packed librg_segment_t should have a valid size");

size_t librg_world_write(librg_world *world, int64_t owner_id, char *buffer, size_t buffer_limit, void *userdata) {
    LIBRG_ASSERT(world); if (!world) return LIBRG_WORLD_INVALID;
    librg_world_t *wld = (librg_world_t *)world;
    librg_table_i64 *last_snapshot = librg_table_tbl_get(&wld->owner_map, owner_id);

    /* no snapshot - means we are asking an invalid owner */
    if (!last_snapshot) return LIBRG_OWNER_INVALID;

    /* get old, and preapre new snapshot handlers */
    librg_table_i64 next_snapshot = {0};
    librg_table_i64_init(&next_snapshot, wld->allocator);

    int64_t results[LIBRG_WORLDWRITE_MAXQUERY] = {0};
    size_t total_amount = librg_world_query(world, owner_id, results, LIBRG_WORLDWRITE_MAXQUERY);
    size_t total_written = 0;

    librg_event evt = {0};
    evt.owner_id = owner_id;
    evt.userdata = userdata;

    #define sz_total (total_written + sizeof(librg_segment_t))
    #define sz_value (sz_total + value_written + sizeof(librg_segval_t))

    uint8_t action_id = LIBRG_WRITE_CREATE;

librg_lbl_ww:

    /* create and update */
    if (sz_total < buffer_limit) {
        librg_segment_t *seg = (librg_segment_t*)(buffer+total_written);
        char *segend = (buffer + sz_total);

        size_t amount = 0;
        size_t value_written = 0;
        size_t iterations = total_amount;

        int64_t entity_id = LIBRG_ENTITY_INVALID;
        int32_t condition = LIBRG_TRUE;

        /* for deletions we are iterating something else */
        if (action_id == LIBRG_WRITE_REMOVE) {
            iterations = zpl_array_count(last_snapshot->entries);
        }

        for (size_t i = 0; i < iterations; ++i) {
            int16_t action_rejected = LIBRG_TRUE;
            int32_t data_size = 0;

            /* preparation */
            if (action_id == LIBRG_WRITE_CREATE) {
                entity_id = results[i];
                condition = librg_table_i64_get(last_snapshot, entity_id) == NULL; /* it did not exist */
            }
            else if (action_id == LIBRG_WRITE_UPDATE) {
                entity_id = results[i];
                condition = librg_table_i64_get(last_snapshot, entity_id) != NULL;  /* it did exist */

                /* mark entity as still alive, to prevent it from being removed */
                librg_table_i64_set(last_snapshot, entity_id, 2);
            }
            else if (action_id == LIBRG_WRITE_REMOVE) {
                entity_id = last_snapshot->entries[i].key;
                condition = last_snapshot->entries[i].value != 2; /* it was not marked as updated */
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

                if (wld->handlers[action_id]) {
                    data_size = (int32_t)wld->handlers[action_id](world, &evt);
                }

                /* if user returned < 0, we consider that event rejected */
                if (data_size >= 0) {
                    /* fill in segval */
                    val->id = entity_id;
                    val->size = data_size;

                    /* increase the total size written */
                    value_written += sizeof(librg_segval_t) + val->size;
                    action_rejected = LIBRG_FALSE;
                    amount++;
                }
            }

            /* finaliztion */
            if (action_id == LIBRG_WRITE_CREATE && !action_rejected) {
                /* mark entity as created, so it can start updating */
                librg_table_i64_set(&next_snapshot, entity_id, 1);
            }
            else if (action_id == LIBRG_WRITE_UPDATE) {
                /* consider entitry updated, without regards was it written or not */
                librg_table_i64_set(&next_snapshot, entity_id, 1);
            }
            else if (action_id == LIBRG_WRITE_REMOVE && action_rejected) {
                /* consider entity alive, till we are able to send it */
                librg_table_i64_set(&next_snapshot, entity_id, 1);
            }
        }

        if (amount > 0) {
            seg->type = action_id;
            seg->size = value_written;
            seg->amount = amount;

            total_written += sizeof(librg_segment_t) + seg->size;
        }
    }

    if (action_id == LIBRG_WRITE_CREATE) {
        action_id = LIBRG_WRITE_UPDATE;
        goto librg_lbl_ww;
    }

    /* iterate it again till all tasks are finished */
    switch (action_id) {
        case LIBRG_WRITE_CREATE: action_id = LIBRG_WRITE_UPDATE; goto librg_lbl_ww;
        case LIBRG_WRITE_UPDATE: action_id = LIBRG_WRITE_REMOVE; goto librg_lbl_ww;
        // case LIBRG_WRITE_REMOVE: action_id = LIBRG_WRITE_OWNER; goto librg_lbl_ww;
    }

    /* swap snapshot tables */
    librg_table_i64_destroy(last_snapshot);
    librg_table_tbl_set(&wld->owner_map, owner_id, next_snapshot);

    #undef sz_total
    #undef sz_value

    return total_written;
}

// int8_t librg_world_read(librg_world *, int64_t owner_id, char *buffer, size_t size, void *userdata) {

// }

LIBRG_END_C_DECLS
