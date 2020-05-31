int32_t dummy_2bytes(librg_world *world, librg_event *event) {
    zpl_unused(world);

    event->buffer[0] = 9;
    event->buffer[1] = 9;

    return 2;
}

int32_t dummy_2bytes_reject(librg_world *world, librg_event *event) {
    zpl_unused(world);

    event->buffer[0] = 9;
    event->buffer[1] = 9;

    return LIBRG_WRITE_REJECT;
}

int32_t dummy_0size(librg_world *world, librg_event *event) {
    zpl_unused(world);
    zpl_unused(event);
    // printf("attempting to write create %lld with size limit of: %zd\n", event->entity_id, event->size);
    return 0;
}

int32_t dummy_markuserdata(librg_world *w, librg_event *e) {
    zpl_unused(w);
    if (e->userdata) *((int*)e->userdata) = 1;
    return 255;
}

/* helper macro to print a segment */
#define SEGMENT_PRINT(buf, amt) \
    for(size_t i=0;i<amt;i++) printf("%02x ",buf[i]); \
    printf("\nsegment size: %zd\n", amt);

/* helper macro to calcualte size of the segment */
#define SEGMENT_SIZE(amount, data) \
    (sizeof(librg_segment_t) + ((sizeof(librg_segval_t) + data) * amount))

#define SEGMENT_CMP(a, b, n) do { for (size_t i=0;i<n;i++) EQUALS(a[i], b[i]); } while(0)

char create_expected_buffer[] = {
    0x00, 0x00, 0x03, 0x00, 0x24, 0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00,
    0x09, 0x09, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x02, 0x00, 0x09, 0x09, 0x03, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x09, 0x09, 0x00,
};

char update_expected_buffer[] = {
    0x01, 0x00, 0x03, 0x00, 0x24, 0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00,
    0x09, 0x09, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x02, 0x00, 0x09, 0x09, 0x03, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x09, 0x09, 0x00,
};

MODULE(packing, {
    int32_t r = LIBRG_FAIL;

    // =======================================================================//
    // !
    // ! Create
    // !
    // =======================================================================//

    printf(" * WRITE:CREATE\n");

    IT("should write create for single entity", {
        librg_world *world = librg_world_create();

        r = librg_entity_track(world, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(world, 1, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_event_set(world, LIBRG_WRITE_CREATE, dummy_0size); EQUALS(r, LIBRG_OK);

        char buffer[4096] = {0};
        size_t amount = librg_world_write(world, 1, buffer, 4096, NULL);
        size_t expected = SEGMENT_SIZE(1, 0); EQUALS(amount, expected);

        r = librg_world_destroy(world); EQUALS(r, LIBRG_OK);
    });

    IT("should write create section", {
        librg_world *world = librg_world_create();

        r = librg_entity_track(world, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 2); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 3); EQUALS(r, LIBRG_OK);

        r = librg_entity_chunk_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 2, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 3, 1); EQUALS(r, LIBRG_OK);

        r = librg_entity_owner_set(world, 1, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_event_set(world, LIBRG_WRITE_CREATE, dummy_0size); EQUALS(r, LIBRG_OK);

        char buffer[4096] = {0};
        size_t amount = librg_world_write(world, 1, buffer, 4096, NULL);
        size_t expected = SEGMENT_SIZE(3, 0); EQUALS(amount, expected);

        r = librg_world_destroy(world); EQUALS(r, LIBRG_OK);
    });

    IT("should write create section with extra data", {
        librg_world *world = librg_world_create();

        r = librg_entity_track(world, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 2); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 3); EQUALS(r, LIBRG_OK);

        r = librg_entity_chunk_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 2, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 3, 1); EQUALS(r, LIBRG_OK);

        r = librg_entity_owner_set(world, 1, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_event_set(world, LIBRG_WRITE_CREATE, dummy_2bytes); EQUALS(r, LIBRG_OK);

        char buffer[4096] = {0};
        size_t amount = librg_world_write(world, 1, buffer, 4096, NULL);
        size_t expected = SEGMENT_SIZE(3, 2); EQUALS(amount, expected);

        SEGMENT_CMP(buffer, create_expected_buffer, amount);

        r = librg_world_destroy(world); EQUALS(r, LIBRG_OK);
    });

    IT("should skip writing create section with rejection and ignore data", {
        librg_world *world = librg_world_create();

        r = librg_entity_track(world, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 2); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 3); EQUALS(r, LIBRG_OK);

        r = librg_entity_chunk_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 2, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 3, 1); EQUALS(r, LIBRG_OK);

        r = librg_entity_owner_set(world, 1, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_event_set(world, LIBRG_WRITE_CREATE, dummy_2bytes_reject); EQUALS(r, LIBRG_OK);

        char buffer[4096] = {0};
        size_t amount = librg_world_write(world, 1, buffer, 4096, NULL);
        size_t expected = 0; EQUALS(amount, expected);

        r = librg_world_destroy(world); EQUALS(r, LIBRG_OK);
    });

    IT("should write create section with size limit", {
        librg_world *world = librg_world_create();

        r = librg_entity_track(world, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 2); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 3); EQUALS(r, LIBRG_OK);

        r = librg_entity_chunk_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 2, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 3, 1); EQUALS(r, LIBRG_OK);

        r = librg_entity_owner_set(world, 1, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_event_set(world, LIBRG_WRITE_CREATE, dummy_0size); EQUALS(r, LIBRG_OK);

        char buffer[4096] = {0};
        size_t amount = librg_world_write(world, 1, buffer, 30, NULL);
        size_t expected = SEGMENT_SIZE(2, 0); EQUALS(amount, expected);

        r = librg_world_destroy(world); EQUALS(r, LIBRG_OK);
    });

    IT("should write create section with size limit and give rest on second call", {
        librg_world *world = librg_world_create();

        r = librg_entity_track(world, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 2); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 3); EQUALS(r, LIBRG_OK);

        r = librg_entity_chunk_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 2, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 3, 1); EQUALS(r, LIBRG_OK);

        r = librg_entity_owner_set(world, 1, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_event_set(world, LIBRG_WRITE_CREATE, dummy_0size); EQUALS(r, LIBRG_OK);

        char buffer[4096] = {0};
        size_t amount = librg_world_write(world, 1, buffer, 30, NULL);
        size_t expected = SEGMENT_SIZE(2, 0); EQUALS(amount, expected);

        amount = librg_world_write(world, 1, buffer, 30, NULL);
        expected = SEGMENT_SIZE(1, 0); EQUALS(amount, expected);

        r = librg_world_destroy(world); EQUALS(r, LIBRG_OK);
    });

    IT("should write create section with limit + data", {
        librg_world *world = librg_world_create();

        r = librg_entity_track(world, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 2); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 3); EQUALS(r, LIBRG_OK);

        r = librg_entity_chunk_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 2, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 3, 1); EQUALS(r, LIBRG_OK);

        r = librg_entity_owner_set(world, 1, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_event_set(world, LIBRG_WRITE_CREATE, dummy_2bytes); EQUALS(r, LIBRG_OK);

        char buffer[4096] = {0};
        size_t amount = librg_world_write(world, 1, buffer, 35, NULL);
        size_t expected = SEGMENT_SIZE(2, 2); EQUALS(amount, expected);

        r = librg_world_destroy(world); EQUALS(r, LIBRG_OK);
    });

    IT("should write create section with limit and give rest on second call + data", {
        librg_world *world = librg_world_create();

        r = librg_entity_track(world, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 2); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 3); EQUALS(r, LIBRG_OK);

        r = librg_entity_chunk_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 2, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 3, 1); EQUALS(r, LIBRG_OK);

        r = librg_entity_owner_set(world, 1, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_event_set(world, LIBRG_WRITE_CREATE, dummy_2bytes); EQUALS(r, LIBRG_OK);

        char buffer[4096] = {0};
        size_t amount = librg_world_write(world, 1, buffer, 35, NULL);
        size_t expected = SEGMENT_SIZE(2, 2); EQUALS(amount, expected);

        amount = librg_world_write(world, 1, buffer, 35, NULL);
        expected = SEGMENT_SIZE(1, 2); EQUALS(amount, expected);

        r = librg_world_destroy(world); EQUALS(r, LIBRG_OK);
    });

    // =======================================================================//
    // !
    // ! Update
    // !
    // =======================================================================//

    printf(" * WRITE:UPDATE\n");

    IT("should write update section", {
        librg_world *world = librg_world_create();

        r = librg_entity_track(world, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 2); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 3); EQUALS(r, LIBRG_OK);

        r = librg_entity_chunk_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 2, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 3, 1); EQUALS(r, LIBRG_OK);

        r = librg_entity_owner_set(world, 1, 1, 1); EQUALS(r, LIBRG_OK);

        /* call 2 times, 1st - to create the entities, and second to update */
        char buffer[4096] = {0};
        librg_world_write(world, 1, buffer, 4096, NULL);
        size_t amount = librg_world_write(world, 1, buffer, 4096, NULL);
        size_t expected = SEGMENT_SIZE(3, 0); EQUALS(amount, expected);

        r = librg_world_destroy(world); EQUALS(r, LIBRG_OK);
    });

    IT("should write update for single entity", {
        librg_world *world = librg_world_create();

        r = librg_entity_track(world, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(world, 1, 1, 1); EQUALS(r, LIBRG_OK);

        char buffer[4096] = {0};
        librg_world_write(world, 1, buffer, 4096, NULL);
        size_t amount = librg_world_write(world, 1, buffer, 4096, NULL);
        size_t expected = SEGMENT_SIZE(1, 0); EQUALS(amount, expected);

        r = librg_world_destroy(world); EQUALS(r, LIBRG_OK);
    });

    IT("should write update section with extra data", {
        librg_world *world = librg_world_create();

        r = librg_entity_track(world, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 2); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 3); EQUALS(r, LIBRG_OK);

        r = librg_entity_chunk_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 2, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 3, 1); EQUALS(r, LIBRG_OK);

        r = librg_entity_owner_set(world, 1, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_event_set(world, LIBRG_WRITE_UPDATE, dummy_2bytes); EQUALS(r, LIBRG_OK);

        char buffer[4096] = {0};
        librg_world_write(world, 1, buffer, 4096, NULL);
        size_t amount = librg_world_write(world, 1, buffer, 4096, NULL);
        size_t expected = SEGMENT_SIZE(3, 2); EQUALS(amount, expected);

        SEGMENT_CMP(buffer, update_expected_buffer, amount);

        r = librg_world_destroy(world); EQUALS(r, LIBRG_OK);
    });

    IT("should skip writing update section with rejection and ignore data", {
        librg_world *world = librg_world_create();

        r = librg_entity_track(world, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 2); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 3); EQUALS(r, LIBRG_OK);

        r = librg_entity_chunk_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 2, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 3, 1); EQUALS(r, LIBRG_OK);

        r = librg_entity_owner_set(world, 1, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_event_set(world, LIBRG_WRITE_UPDATE, dummy_2bytes_reject); EQUALS(r, LIBRG_OK);

        char buffer[4096] = {0};
        librg_world_write(world, 1, buffer, 4096, NULL);
        size_t amount = librg_world_write(world, 1, buffer, 4096, NULL);
        size_t expected = 0; EQUALS(amount, expected);

        r = librg_world_destroy(world); EQUALS(r, LIBRG_OK);
    });

    IT("should write update section with size limit", {
        librg_world *world = librg_world_create();

        r = librg_entity_track(world, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 2); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 3); EQUALS(r, LIBRG_OK);

        r = librg_entity_chunk_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 2, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 3, 1); EQUALS(r, LIBRG_OK);

        r = librg_entity_owner_set(world, 1, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_event_set(world, LIBRG_WRITE_UPDATE, dummy_0size); EQUALS(r, LIBRG_OK);

        char buffer[4096] = {0};
        librg_world_write(world, 1, buffer, 4096, NULL);
        size_t amount = librg_world_write(world, 1, buffer, 30, NULL);
        size_t expected = SEGMENT_SIZE(2, 0); EQUALS(amount, expected);

        r = librg_world_destroy(world); EQUALS(r, LIBRG_OK);
    });

    IT("should write update section with size limit and repeat same on second call", {
        librg_world *world = librg_world_create();

        r = librg_entity_track(world, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 2); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 3); EQUALS(r, LIBRG_OK);

        r = librg_entity_chunk_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 2, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 3, 1); EQUALS(r, LIBRG_OK);

        r = librg_entity_owner_set(world, 1, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_event_set(world, LIBRG_WRITE_UPDATE, dummy_0size); EQUALS(r, LIBRG_OK);

        char buffer[4096] = {0};
        librg_world_write(world, 1, buffer, 4096, NULL);
        size_t amount = librg_world_write(world, 1, buffer, 30, NULL);
        size_t expected = SEGMENT_SIZE(2, 0); EQUALS(amount, expected);
        amount = librg_world_write(world, 1, buffer, 30, NULL);
        expected = SEGMENT_SIZE(2, 0); EQUALS(amount, expected);

        r = librg_world_destroy(world); EQUALS(r, LIBRG_OK);
    });

    IT("should write update section with limit + data", {
        librg_world *world = librg_world_create();

        r = librg_entity_track(world, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 2); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 3); EQUALS(r, LIBRG_OK);

        r = librg_entity_chunk_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 2, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 3, 1); EQUALS(r, LIBRG_OK);

        r = librg_entity_owner_set(world, 1, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_event_set(world, LIBRG_WRITE_UPDATE, dummy_2bytes); EQUALS(r, LIBRG_OK);

        char buffer[4096] = {0};
        librg_world_write(world, 1, buffer, 4096, NULL);
        size_t amount = librg_world_write(world, 1, buffer, 35, NULL);
        size_t expected = SEGMENT_SIZE(2, 2); EQUALS(amount, expected);

        r = librg_world_destroy(world); EQUALS(r, LIBRG_OK);
    });

    IT("should write update section with limit and give repeat same on second call + data", {
        librg_world *world = librg_world_create();

        r = librg_entity_track(world, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 2); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 3); EQUALS(r, LIBRG_OK);

        r = librg_entity_chunk_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 2, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 3, 1); EQUALS(r, LIBRG_OK);

        r = librg_entity_owner_set(world, 1, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_event_set(world, LIBRG_WRITE_UPDATE, dummy_2bytes); EQUALS(r, LIBRG_OK);

        char buffer[4096] = {0};
        librg_world_write(world, 1, buffer, 4096, NULL);
        size_t amount = librg_world_write(world, 1, buffer, 35, NULL);
        size_t expected = SEGMENT_SIZE(2, 2); EQUALS(amount, expected);

        amount = librg_world_write(world, 1, buffer, 35, NULL);
        expected = SEGMENT_SIZE(2, 2); EQUALS(amount, expected);

        r = librg_world_destroy(world); EQUALS(r, LIBRG_OK);
    });

    // =======================================================================//
    // !
    // ! Remove
    // !
    // =======================================================================//

    printf(" * WRITE:REMOVE\n");

    IT("should write remove section", {
        librg_world *world = librg_world_create();

        r = librg_entity_track(world, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 2); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 3); EQUALS(r, LIBRG_OK);

        r = librg_entity_chunk_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 2, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 3, 1); EQUALS(r, LIBRG_OK);

        r = librg_entity_owner_set(world, 1, 1, 1); EQUALS(r, LIBRG_OK);

        char buffer[4096] = {0};

        librg_world_write(world, 1, buffer, 4096, NULL);
        librg_entity_untrack(world, 2);

        size_t amount = librg_world_write(world, 1, buffer, 4096, NULL);
        size_t expected = SEGMENT_SIZE(2, 0) + SEGMENT_SIZE(1, 0); EQUALS(amount, expected);

        amount = librg_world_write(world, 1, buffer, 4096, NULL);
        expected = SEGMENT_SIZE(2, 0); EQUALS(amount, expected);

        r = librg_world_destroy(world); EQUALS(r, LIBRG_OK);
    });

    IT("should write remove section with extra data", {
        librg_world *world = librg_world_create();

        r = librg_entity_track(world, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 2); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 3); EQUALS(r, LIBRG_OK);

        r = librg_entity_chunk_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 2, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 3, 1); EQUALS(r, LIBRG_OK);

        r = librg_entity_owner_set(world, 1, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_event_set(world, LIBRG_WRITE_REMOVE, dummy_2bytes); EQUALS(r, LIBRG_OK);

        char buffer[4096] = {0};

        librg_world_write(world, 1, buffer, 4096, NULL);
        librg_entity_untrack(world, 2);

        size_t amount = librg_world_write(world, 1, buffer, 4096, NULL);
        size_t expected = SEGMENT_SIZE(2, 0) + SEGMENT_SIZE(1, 2); EQUALS(amount, expected);


        amount = librg_world_write(world, 1, buffer, 4096, NULL);
        expected = SEGMENT_SIZE(2, 0); EQUALS(amount, expected);

        r = librg_world_destroy(world); EQUALS(r, LIBRG_OK);
    });

    IT("should write remove section on second call if first was limited", {
        librg_world *world = librg_world_create();

        r = librg_entity_track(world, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 2); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 3); EQUALS(r, LIBRG_OK);

        r = librg_entity_chunk_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 2, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 3, 1); EQUALS(r, LIBRG_OK);

        r = librg_entity_owner_set(world, 1, 1, 1); EQUALS(r, LIBRG_OK);

        char buffer[4096] = {0};

        librg_world_write(world, 1, buffer, 4096, NULL);
        librg_entity_untrack(world, 2);
        librg_entity_untrack(world, 3);

        size_t amount = librg_world_write(world, 1, buffer, 30, NULL);
        size_t expected = SEGMENT_SIZE(1, 0); EQUALS(amount, expected);

        amount = librg_world_write(world, 1, buffer, 40, NULL);
        expected = SEGMENT_SIZE(1, 0) + SEGMENT_SIZE(1, 0); EQUALS(amount, expected);

        amount = librg_world_write(world, 1, buffer, 40, NULL);
        expected = SEGMENT_SIZE(1, 0) + SEGMENT_SIZE(1, 0); EQUALS(amount, expected);

        r = librg_world_destroy(world); EQUALS(r, LIBRG_OK);
    });

    IT("should skip writing remove section with rejection and ignore data", {
        librg_world *world = librg_world_create();

        r = librg_entity_track(world, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 2); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 3); EQUALS(r, LIBRG_OK);

        r = librg_entity_chunk_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 2, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 3, 1); EQUALS(r, LIBRG_OK);

        r = librg_entity_owner_set(world, 1, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_event_set(world, LIBRG_WRITE_REMOVE, dummy_2bytes_reject); EQUALS(r, LIBRG_OK);

        char buffer[4096] = {0};
        librg_world_write(world, 1, buffer, 4096, NULL);
        librg_entity_untrack(world, 2);

        size_t amount = librg_world_write(world, 1, buffer, 4096, NULL);
        size_t expected = SEGMENT_SIZE(2, 0); EQUALS(amount, expected);

        amount = librg_world_write(world, 1, buffer, 40, NULL);
        expected = SEGMENT_SIZE(2, 0); EQUALS(amount, expected);

        r = librg_world_destroy(world); EQUALS(r, LIBRG_OK);
    });

    // =======================================================================//
    // !
    // ! Reading create
    // !
    // =======================================================================//

    printf(" * READ:CREATE\n");

    IT("should read create for single entity", {
        librg_world *world1 = librg_world_create();
        librg_world *world2 = librg_world_create();

        r = librg_entity_track(world1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world1, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(world1, 1, 1, 1); EQUALS(r, LIBRG_OK);

        r = librg_event_set(world2, LIBRG_READ_CREATE, dummy_markuserdata); EQUALS(r, LIBRG_OK);

        int value = 0;
        char buffer[4096] = {0};
        size_t amount = librg_world_write(world1, 1, buffer, 4096, NULL);

        r = librg_entity_count(world2); EQUALS(r, 0);
        r = librg_world_read(world2, 1, buffer, amount, &value); EQUALS(r, LIBRG_OK);
        r = librg_entity_count(world2); EQUALS(r, 1);

        EQUALS(value, 1); /* value == 1 means the LIBRG_READ_CREATE call was indeed executed */

        r = librg_world_destroy(world1); EQUALS(r, LIBRG_OK);
        r = librg_world_destroy(world2); EQUALS(r, LIBRG_OK);
    });

    IT("should fail to read create for single entity with extra data", {
        librg_world *world1 = librg_world_create();
        librg_world *world2 = librg_world_create();

        r = librg_entity_track(world1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world1, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(world1, 1, 1, 1); EQUALS(r, LIBRG_OK);

        r = librg_event_set(world1, LIBRG_WRITE_CREATE, dummy_2bytes); EQUALS(r, LIBRG_OK);
        r = librg_event_set(world2, LIBRG_READ_CREATE, dummy_2bytes); EQUALS(r, LIBRG_OK);

        char buffer[4096] = {0};
        size_t amount = librg_world_write(world1, 1, buffer, 4096, NULL);

        r = librg_entity_count(world2); EQUALS(r, 0);
        r = librg_world_read(world2, 1, buffer, amount, NULL); EQUALS(r, LIBRG_OK);
        r = librg_entity_count(world2); EQUALS(r, 1);

        r = librg_world_destroy(world1); EQUALS(r, LIBRG_OK);
        r = librg_world_destroy(world2); EQUALS(r, LIBRG_OK);
    });

    IT("should fail to read create for single entity with invalid data", {
        librg_world *world1 = librg_world_create();
        librg_world *world2 = librg_world_create();

        r = librg_entity_track(world1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world1, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(world1, 1, 1, 1); EQUALS(r, LIBRG_OK);

        r = librg_event_set(world2, LIBRG_READ_CREATE, dummy_markuserdata); EQUALS(r, LIBRG_OK);

        char buffer[4096] = {0};
        size_t amount = librg_world_write(world1, 1, buffer, 4096, NULL);

        r = librg_entity_count(world2); EQUALS(r, 0);
        r = librg_world_read(world2, 1, buffer, amount-2, NULL); NEQUALS(r, LIBRG_OK);
        r = librg_entity_count(world2); EQUALS(r, 0);

        r = librg_world_destroy(world1); EQUALS(r, LIBRG_OK);
        r = librg_world_destroy(world2); EQUALS(r, LIBRG_OK);
    });

    IT("should read created entity should be foreign, and untrack should error out", {
        librg_world *world1 = librg_world_create();
        librg_world *world2 = librg_world_create();

        r = librg_entity_track(world1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world1, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(world1, 1, 1, 1); EQUALS(r, LIBRG_OK);

        r = librg_event_set(world2, LIBRG_READ_CREATE, dummy_markuserdata); EQUALS(r, LIBRG_OK);

        char buffer[4096] = {0};
        size_t amount = librg_world_write(world1, 1, buffer, 4096, NULL);

        r = librg_entity_count(world2); EQUALS(r, 0);
        r = librg_world_read(world2, 1, buffer, amount, NULL); EQUALS(r, LIBRG_OK);
        r = librg_entity_count(world2); EQUALS(r, 1);

        r = librg_entity_foreign(world2, 1); EQUALS(r, LIBRG_TRUE);
        r = librg_entity_untrack(world2, 1); NEQUALS(r, LIBRG_OK);
        r = librg_entity_count(world2); EQUALS(r, 1);

        r = librg_world_destroy(world1); EQUALS(r, LIBRG_OK);
        r = librg_world_destroy(world2); EQUALS(r, LIBRG_OK);
    });

    IT("should read create for single entity, and call error event if entity is already tracked", {
        librg_world *world1 = librg_world_create();
        librg_world *world2 = librg_world_create();

        r = librg_entity_track(world1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world1, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(world1, 1, 1, 1); EQUALS(r, LIBRG_OK);

        r = librg_event_set(world2, LIBRG_ERROR_CREATE, dummy_markuserdata); EQUALS(r, LIBRG_OK);

        char buffer[4096] = {0};
        size_t amount = librg_world_write(world1, 1, buffer, 4096, NULL);

        int value = 0;
        r = librg_entity_track(world2, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_count(world2); EQUALS(r, 1);
        r = librg_world_read(world2, 1, buffer, amount, &value); EQUALS(r, LIBRG_OK);
        r = librg_entity_count(world2); EQUALS(r, 1);

        EQUALS(value, 1); /* value == 1 means the LIBRG_ERROR_CREATE call was indeed executed */

        r = librg_world_destroy(world1); EQUALS(r, LIBRG_OK);
        r = librg_world_destroy(world2); EQUALS(r, LIBRG_OK);
    });

    // =======================================================================//
    // !
    // ! Reading update
    // !
    // =======================================================================//

    printf(" * READ:UPDATE\n");

    IT("should read update for single entity", {
        librg_world *world1 = librg_world_create();
        librg_world *world2 = librg_world_create();

        r = librg_entity_track(world1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world1, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(world1, 1, 1, 1); EQUALS(r, LIBRG_OK);

        r = librg_event_set(world2, LIBRG_READ_UPDATE, dummy_markuserdata); EQUALS(r, LIBRG_OK);

        int value = 0;
        size_t amount = 0;
        char buffer[4096] = {0};
        amount = librg_world_write(world1, 1, buffer, 4096, NULL);
        r = librg_entity_count(world2); EQUALS(r, 0);
        r = librg_world_read(world2, 1, buffer, amount, &value); EQUALS(r, LIBRG_OK);
        r = librg_entity_count(world2); EQUALS(r, 1);

        amount = librg_world_write(world1, 1, buffer, 4096, NULL);
        r = librg_world_read(world2, 1, buffer, amount, &value); EQUALS(r, LIBRG_OK);

        EQUALS(value, 1); /* value == 1 means the LIBRG_READ_UPDATE call was indeed executed */

        r = librg_world_destroy(world1); EQUALS(r, LIBRG_OK);
        r = librg_world_destroy(world2); EQUALS(r, LIBRG_OK);
    });

    IT("should fail to read update for single entity with extra data", {
        librg_world *world1 = librg_world_create();
        librg_world *world2 = librg_world_create();

        r = librg_entity_track(world1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world1, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(world1, 1, 1, 1); EQUALS(r, LIBRG_OK);

        r = librg_event_set(world1, LIBRG_WRITE_UPDATE, dummy_2bytes); EQUALS(r, LIBRG_OK);
        r = librg_event_set(world2, LIBRG_READ_UPDATE, dummy_2bytes); EQUALS(r, LIBRG_OK);

        int value = 0;
        size_t amount = 0;
        char buffer[4096] = {0};

        amount = librg_world_write(world1, 1, buffer, 4096, NULL);
        r = librg_world_read(world2, 1, buffer, amount, &value); EQUALS(r, LIBRG_OK);

        amount = librg_world_write(world1, 1, buffer, 4096, NULL);
        r = librg_world_read(world2, 1, buffer, amount, &value); EQUALS(r, LIBRG_OK);

        r = librg_world_destroy(world1); EQUALS(r, LIBRG_OK);
        r = librg_world_destroy(world2); EQUALS(r, LIBRG_OK);
    });

    IT("should fail to read update for single entity with invalid data", {
        librg_world *world1 = librg_world_create();
        librg_world *world2 = librg_world_create();

        r = librg_entity_track(world1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world1, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(world1, 1, 1, 1); EQUALS(r, LIBRG_OK);

        int value = 0;
        size_t amount = 0;
        char buffer[4096] = {0};

        amount = librg_world_write(world1, 1, buffer, 4096, NULL);
        r = librg_world_read(world2, 1, buffer, amount, &value); EQUALS(r, LIBRG_OK);

        amount = librg_world_write(world1, 1, buffer, 4096, NULL);
        r = librg_world_read(world2, 1, buffer, amount-1, &value); NEQUALS(r, LIBRG_OK);

        r = librg_world_destroy(world1); EQUALS(r, LIBRG_OK);
        r = librg_world_destroy(world2); EQUALS(r, LIBRG_OK);
    });

    IT("should read updated entity should be foreign, and error out if it is not", {
        librg_world *world1 = librg_world_create();
        librg_world *world2 = librg_world_create();

        r = librg_entity_track(world1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world1, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(world1, 1, 1, 1); EQUALS(r, LIBRG_OK);

        r = librg_event_set(world2, LIBRG_ERROR_UPDATE, dummy_markuserdata); EQUALS(r, LIBRG_OK);

        int value = 0;
        size_t amount = 0;
        char buffer[4096] = {0};

        r = librg_entity_track(world2, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_count(world2); EQUALS(r, 1);

        amount = librg_world_write(world1, 1, buffer, 4096, NULL);
        r = librg_world_read(world2, 1, buffer, amount, &value); EQUALS(r, LIBRG_OK);
        r = librg_entity_count(world2); EQUALS(r, 1);

        amount = librg_world_write(world1, 1, buffer, 4096, NULL);
        r = librg_world_read(world2, 1, buffer, amount, &value); EQUALS(r, LIBRG_OK);

        EQUALS(value, 1); /* value == 1 means the LIBRG_ERROR_UPDATE call was indeed executed */

        r = librg_world_destroy(world1); EQUALS(r, LIBRG_OK);
        r = librg_world_destroy(world2); EQUALS(r, LIBRG_OK);
    });
});
