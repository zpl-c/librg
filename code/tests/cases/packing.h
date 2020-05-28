size_t dummy_2bytes(librg_world *world, librg_event *event) {
    event->buffer[0] = 9;
    event->buffer[1] = 9;

    return 2;
}

size_t dummy_2bytes_reject(librg_world *world, librg_event *event) {
    event->buffer[0] = 9;
    event->buffer[1] = 9;

    return LIBRG_EVENT_REJECT;
}

size_t dummy_size(librg_world *world, librg_event *event) {
    // printf("attempting to write create %lld with size limit of: %zd\n", event->entity_id, event->size);
    return 0;
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
    int8_t r = LIBRG_FAIL;

    // =======================================================================//
    // !
    // ! Create
    // !
    // =======================================================================//

    IT("should write create section", {
        librg_world *world = librg_world_create();

        r = librg_entity_track(world, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 2); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 3); EQUALS(r, LIBRG_OK);

        r = librg_entity_chunk_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 2, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 3, 1); EQUALS(r, LIBRG_OK);

        r = librg_entity_owner_set(world, 1, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_event_set(world, LIBRG_WRITE_CREATE, dummy_size); EQUALS(r, LIBRG_OK);

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

    IT("should write create section with rejection and ignore data", {
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
        r = librg_event_set(world, LIBRG_WRITE_CREATE, dummy_size); EQUALS(r, LIBRG_OK);

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
        r = librg_event_set(world, LIBRG_WRITE_CREATE, dummy_size); EQUALS(r, LIBRG_OK);

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

    IT("should write update section with rejection and ignore data", {
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
        r = librg_event_set(world, LIBRG_WRITE_UPDATE, dummy_size); EQUALS(r, LIBRG_OK);

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
        r = librg_event_set(world, LIBRG_WRITE_UPDATE, dummy_size); EQUALS(r, LIBRG_OK);

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
});
