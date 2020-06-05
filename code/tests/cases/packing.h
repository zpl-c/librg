int32_t dummy_2bytes(librg_world *world, librg_event *event) {
    zpl_unused(world);
    char *buffer = librg_event_buffer_get(world, event);

    buffer[0] = 9;
    buffer[1] = 9;

    return 2;
}

int32_t dummy_2bytes_reject(librg_world *world, librg_event *event) {
    zpl_unused(world);
    char *buffer = librg_event_buffer_get(world, event);

    buffer[0] = 9;
    buffer[1] = 9;

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
    void *userdata = librg_event_userdata_get(w, e);

    if (userdata) {
        *((int*)(userdata)) = 1;
    }

    return 0;
}

/* helper macro to print a segment */
#define SEGMENT_PRINT(buf, amt) \
    for(size_t i=0;i<amt;i++) printf("%02x ",buf[i]); \
    printf("\nsegment size: %zd\n", amt);

/* helper macro to calcualte size of the segment */
#define SEGMENT_SIZE(amount, data) \
    (sizeof(librg_segment_t) + ((sizeof(librg_segval_t) + data) * amount))

#define CREATE_SEGMENT SEGMENT_SIZE
#define UPDATE_SEGMENT SEGMENT_SIZE
#define REMOVE_SEGMENT SEGMENT_SIZE
#define OWNER_SEGMENT(a) SEGMENT_SIZE(a, 0)

#define SEGMENT_CMP(a, b, n) do { for (size_t i=0;i<n;i++) if (a[i] != '?') EQUALS(a[i], b[i]); } while(0)

char expected_buffer_create[] = {
    0x00, 0x00, 0x03, 0x00, 0x2a, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x09, 0x09, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x09, 0x09, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x09, 0x09, 0x09, 0x00, 0x01, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, '?', '?', 0x00, 0x00,
};

char expected_buffer_update[] = {
    0x01, 0x00, 0x03, 0x00, 0x2a, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x09, 0x09, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x09, 0x09, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x09, 0x09,
};

MODULE(packing, {
    int32_t r = -1;
    size_t buffer_size = 0;

    // =======================================================================//
    // !
    // ! Create
    // !
    // =======================================================================//

    IT("should write create for single entity", {
        librg_world *world = librg_world_create();

        r = librg_entity_track(world, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_radius_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_event_set(world, LIBRG_WRITE_CREATE, dummy_0size); EQUALS(r, LIBRG_OK);

        char buffer[4096] = {0};
        buffer_size = 4096;
        r = librg_world_write(world, 1, buffer, &buffer_size, NULL);
        size_t expected = CREATE_SEGMENT(1, 0) + OWNER_SEGMENT(1); EQUALS(buffer_size, expected);

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

        r = librg_entity_owner_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_radius_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_event_set(world, LIBRG_WRITE_CREATE, dummy_0size); EQUALS(r, LIBRG_OK);

        char buffer[4096] = {0};
        buffer_size = 4096;
        r = librg_world_write(world, 1, buffer, &buffer_size, NULL);
        size_t expected = CREATE_SEGMENT(3, 0) + OWNER_SEGMENT(1); EQUALS(buffer_size, expected);


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

        r = librg_entity_owner_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_radius_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_event_set(world, LIBRG_WRITE_CREATE, dummy_2bytes); EQUALS(r, LIBRG_OK);

        char buffer[4096] = {0};
        buffer_size = 4096;
        r = librg_world_write(world, 1, buffer, &buffer_size, NULL);
        size_t expected = CREATE_SEGMENT(3, 2) + OWNER_SEGMENT(1); EQUALS(buffer_size, expected);

        SEGMENT_CMP(expected_buffer_create, buffer, buffer_size);

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

        r = librg_entity_owner_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_radius_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_event_set(world, LIBRG_WRITE_CREATE, dummy_2bytes_reject); EQUALS(r, LIBRG_OK);

        char buffer[4096] = {0};
        buffer_size = 4096;
        r = librg_world_write(world, 1, buffer, &buffer_size, NULL);
        size_t expected = 0; EQUALS(buffer_size, expected);

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

        r = librg_entity_owner_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_radius_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_event_set(world, LIBRG_WRITE_CREATE, dummy_0size); EQUALS(r, LIBRG_OK);

        char buffer[4096] = {0};
        buffer_size = 34;
        r = librg_world_write(world, 1, buffer, &buffer_size, NULL); EQUALS(r, 38);
        size_t expected = CREATE_SEGMENT(2, 0); EQUALS(buffer_size, expected);

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

        r = librg_entity_owner_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_radius_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_event_set(world, LIBRG_WRITE_CREATE, dummy_0size); EQUALS(r, LIBRG_OK);

        char buffer[4096] = {0};
        buffer_size = 34;
        r = librg_world_write(world, 1, buffer, &buffer_size, NULL);
        size_t expected = CREATE_SEGMENT(2, 0); EQUALS(buffer_size, expected);

        buffer_size = 30;
        r = librg_world_write(world, 1, buffer, &buffer_size, NULL);
        expected = CREATE_SEGMENT(1, 0); EQUALS(buffer_size, expected);

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

        r = librg_entity_owner_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_radius_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_event_set(world, LIBRG_WRITE_CREATE, dummy_2bytes); EQUALS(r, LIBRG_OK);

        char buffer[4096] = {0};
        buffer_size = 35;
        r = librg_world_write(world, 1, buffer, &buffer_size, NULL);
        size_t expected = CREATE_SEGMENT(2, 2); EQUALS(buffer_size, expected);

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

        r = librg_entity_owner_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_radius_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_event_set(world, LIBRG_WRITE_CREATE, dummy_2bytes); EQUALS(r, LIBRG_OK);

        char buffer[4096] = {0};
        buffer_size = 35;
        r = librg_world_write(world, 1, buffer, &buffer_size, NULL);
        size_t expected = CREATE_SEGMENT(2, 2); EQUALS(buffer_size, expected);

        buffer_size = 35;
        r = librg_world_write(world, 1, buffer, &buffer_size, NULL);
        expected = CREATE_SEGMENT(1, 2); EQUALS(buffer_size, expected);

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

        r = librg_entity_owner_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_radius_set(world, 1, 1); EQUALS(r, LIBRG_OK);

        /* call 2 times, 1st - to create the entities, and second to update */
        char buffer[4096] = {0};
        buffer_size = 4096;
        r = librg_world_write(world, 1, buffer, &buffer_size, NULL);
        buffer_size = 4096;
        r = librg_world_write(world, 1, buffer, &buffer_size, NULL);
        size_t expected = UPDATE_SEGMENT(3, 0); EQUALS(buffer_size, expected);

        r = librg_world_destroy(world); EQUALS(r, LIBRG_OK);
    });

    IT("should write update for single entity", {
        librg_world *world = librg_world_create();

        r = librg_entity_track(world, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_radius_set(world, 1, 1); EQUALS(r, LIBRG_OK);

        char buffer[4096] = {0};
        buffer_size = 4096;
        r = librg_world_write(world, 1, buffer, &buffer_size, NULL);
        buffer_size = 4096;
        r = librg_world_write(world, 1, buffer, &buffer_size, NULL);
        size_t expected = UPDATE_SEGMENT(1, 0); EQUALS(buffer_size, expected);

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

        r = librg_entity_owner_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_radius_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_event_set(world, LIBRG_WRITE_UPDATE, dummy_2bytes); EQUALS(r, LIBRG_OK);

        char buffer[4096] = {0};
        buffer_size = 4096;
        r = librg_world_write(world, 1, buffer, &buffer_size, NULL);
        buffer_size = 4096;
        r = librg_world_write(world, 1, buffer, &buffer_size, NULL);
        size_t expected = UPDATE_SEGMENT(3, 2); EQUALS(buffer_size, expected);

        SEGMENT_CMP(buffer, expected_buffer_update, buffer_size);

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

        r = librg_entity_owner_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_radius_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_event_set(world, LIBRG_WRITE_UPDATE, dummy_2bytes_reject); EQUALS(r, LIBRG_OK);

        char buffer[4096] = {0};
        buffer_size = 4096;
        r = librg_world_write(world, 1, buffer, &buffer_size, NULL);
        buffer_size = 4096;
        r = librg_world_write(world, 1, buffer, &buffer_size, NULL);
        size_t expected = 0; EQUALS(buffer_size, expected);

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

        r = librg_entity_owner_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_radius_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_event_set(world, LIBRG_WRITE_UPDATE, dummy_0size); EQUALS(r, LIBRG_OK);

        char buffer[4096] = {0};
        buffer_size = 4096;
        r = librg_world_write(world, 1, buffer, &buffer_size, NULL);

        buffer_size = 34;
        r = librg_world_write(world, 1, buffer, &buffer_size, NULL);
        size_t expected = UPDATE_SEGMENT(2, 0); EQUALS(buffer_size, expected);

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

        r = librg_entity_owner_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_radius_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_event_set(world, LIBRG_WRITE_UPDATE, dummy_0size); EQUALS(r, LIBRG_OK);

        char buffer[4096] = {0};
        buffer_size = 4096;
        r = librg_world_write(world, 1, buffer, &buffer_size, NULL);

        buffer_size = 34;
        r = librg_world_write(world, 1, buffer, &buffer_size, NULL);
        size_t expected = UPDATE_SEGMENT(2, 0); EQUALS(buffer_size, expected);

        buffer_size = 34;
        r = librg_world_write(world, 1, buffer, &buffer_size, NULL);
        expected = UPDATE_SEGMENT(2, 0); EQUALS(buffer_size, expected);

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

        r = librg_entity_owner_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_radius_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_event_set(world, LIBRG_WRITE_UPDATE, dummy_2bytes); EQUALS(r, LIBRG_OK);

        char buffer[4096] = {0};
        buffer_size = 4096;
        r = librg_world_write(world, 1, buffer, &buffer_size, NULL);
        buffer_size = 35;
        r = librg_world_write(world, 1, buffer, &buffer_size, NULL);
        size_t expected = UPDATE_SEGMENT(2, 2); EQUALS(buffer_size, expected);

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

        r = librg_entity_owner_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_radius_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_event_set(world, LIBRG_WRITE_UPDATE, dummy_2bytes); EQUALS(r, LIBRG_OK);

        char buffer[4096] = {0};
        buffer_size = 4096;
        r = librg_world_write(world, 1, buffer, &buffer_size, NULL);
        buffer_size = 35;
        r = librg_world_write(world, 1, buffer, &buffer_size, NULL);
        size_t expected = UPDATE_SEGMENT(2, 2); EQUALS(buffer_size, expected);

        buffer_size = 35;
        r = librg_world_write(world, 1, buffer, &buffer_size, NULL);
        expected = UPDATE_SEGMENT(2, 2); EQUALS(buffer_size, expected);

        r = librg_world_destroy(world); EQUALS(r, LIBRG_OK);
    });

    // =======================================================================//
    // !
    // ! Remove
    // !
    // =======================================================================//

    IT("should write remove section", {
        librg_world *world = librg_world_create();

        r = librg_entity_track(world, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 2); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 3); EQUALS(r, LIBRG_OK);

        r = librg_entity_chunk_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 2, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 3, 1); EQUALS(r, LIBRG_OK);

        r = librg_entity_owner_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_radius_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_event_set(world, LIBRG_WRITE_REMOVE, dummy_markuserdata); EQUALS(r, LIBRG_OK);

        int value = 0;
        char buffer[4096] = {0};
        size_t expected = 0;

        buffer_size = 4096;
        r = librg_world_write(world, 1, buffer, &buffer_size, NULL);
        expected = CREATE_SEGMENT(3, 0) + OWNER_SEGMENT(1); EQUALS(buffer_size, expected);

        buffer_size = 4096;
        r = librg_world_write(world, 1, buffer, &buffer_size, NULL);
        expected = UPDATE_SEGMENT(3, 0); EQUALS(buffer_size, expected);

        r = librg_entity_untrack(world, 2); EQUALS(r, LIBRG_OK);

        buffer_size = 4096;
        r = librg_world_write(world, 1, buffer, &buffer_size, &value);
        expected = UPDATE_SEGMENT(2, 0) + REMOVE_SEGMENT(1, 0); EQUALS(buffer_size, expected);

        EQUALS(value, 1);

        buffer_size = 4096;
        r = librg_world_write(world, 1, buffer, &buffer_size, NULL);
        expected = UPDATE_SEGMENT(2, 0); EQUALS(buffer_size, expected);

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

        r = librg_entity_owner_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_radius_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_event_set(world, LIBRG_WRITE_REMOVE, dummy_2bytes); EQUALS(r, LIBRG_OK);

        char buffer[4096] = {0};

        buffer_size = 4096;
        r = librg_world_write(world, 1, buffer, &buffer_size, NULL);
        librg_entity_untrack(world, 2);

        buffer_size = 4096;
        r = librg_world_write(world, 1, buffer, &buffer_size, NULL);
        size_t expected = UPDATE_SEGMENT(2, 0) + REMOVE_SEGMENT(1, 2); EQUALS(buffer_size, expected);


        buffer_size = 4096;
        r = librg_world_write(world, 1, buffer, &buffer_size, NULL);
        expected = UPDATE_SEGMENT(2, 0); EQUALS(buffer_size, expected);

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

        r = librg_entity_owner_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_radius_set(world, 1, 1); EQUALS(r, LIBRG_OK);

        char buffer[4096] = {0};

        buffer_size = 4096;
        r = librg_world_write(world, 1, buffer, &buffer_size, NULL);
        librg_entity_untrack(world, 2);
        librg_entity_untrack(world, 3);

        buffer_size = 34;
        r = librg_world_write(world, 1, buffer, &buffer_size, NULL);
        size_t expected = UPDATE_SEGMENT(1, 0); EQUALS(buffer_size, expected);

        buffer_size = 48; r = librg_world_write(world, 1, buffer, &buffer_size, NULL);
        expected = UPDATE_SEGMENT(1, 0) + REMOVE_SEGMENT(1, 0); EQUALS(buffer_size, expected);

        buffer_size = 48; r = librg_world_write(world, 1, buffer, &buffer_size, NULL);
        expected = UPDATE_SEGMENT(1, 0) + REMOVE_SEGMENT(1, 0); EQUALS(buffer_size, expected);

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

        r = librg_entity_owner_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_radius_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_event_set(world, LIBRG_WRITE_REMOVE, dummy_2bytes_reject); EQUALS(r, LIBRG_OK);

        char buffer[4096] = {0};
        buffer_size = 4096;
        r = librg_world_write(world, 1, buffer, &buffer_size, NULL);
        librg_entity_untrack(world, 2);

        buffer_size = 4096;
        r = librg_world_write(world, 1, buffer, &buffer_size, NULL);
        size_t expected = UPDATE_SEGMENT(2, 0); EQUALS(buffer_size, expected);

        buffer_size = 40; r = librg_world_write(world, 1, buffer, &buffer_size, NULL);
        expected = UPDATE_SEGMENT(2, 0); EQUALS(buffer_size, expected);

        r = librg_world_destroy(world); EQUALS(r, LIBRG_OK);
    });

    // =======================================================================//
    // !
    // ! Reading create
    // !
    // =======================================================================//

    IT("should read create for single entity", {
        librg_world *world1 = librg_world_create();
        librg_world *world2 = librg_world_create();

        r = librg_entity_track(world1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world1, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(world1, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_radius_set(world1, 1, 1); EQUALS(r, LIBRG_OK);

        r = librg_event_set(world2, LIBRG_READ_CREATE, dummy_markuserdata); EQUALS(r, LIBRG_OK);

        int value = 0;
        char buffer[4096] = {0};
        buffer_size = 4096;
        r = librg_world_write(world1, 1, buffer, &buffer_size, NULL);

        r = librg_entity_count(world2); EQUALS(r, 0);
        r = librg_world_read(world2, 1, buffer, buffer_size, &value); EQUALS(r, LIBRG_OK);
        r = librg_entity_count(world2); EQUALS(r, 1);

        r = (int32_t)librg_entity_owner_get(world2, 1); EQUALS(r, 1);

        EQUALS(value, 1); /* value == 1 means the LIBRG_READ_CREATE call was indeed executed */

        r = librg_world_destroy(world1); EQUALS(r, LIBRG_OK);
        r = librg_world_destroy(world2); EQUALS(r, LIBRG_OK);
    });

    IT("should fail to read create for single entity with extra data", {
        librg_world *world1 = librg_world_create();
        librg_world *world2 = librg_world_create();

        r = librg_entity_track(world1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world1, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(world1, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_radius_set(world1, 1, 1); EQUALS(r, LIBRG_OK);

        r = librg_event_set(world1, LIBRG_WRITE_CREATE, dummy_2bytes); EQUALS(r, LIBRG_OK);
        r = librg_event_set(world2, LIBRG_READ_CREATE, dummy_2bytes); EQUALS(r, LIBRG_OK);

        char buffer[4096] = {0};
        buffer_size = 4096;
        r = librg_world_write(world1, 1, buffer, &buffer_size, NULL);

        r = librg_entity_count(world2); EQUALS(r, 0);
        r = librg_world_read(world2, 1, buffer, buffer_size, NULL); EQUALS(r, LIBRG_OK);
        r = librg_entity_count(world2); EQUALS(r, 1);

        r = librg_world_destroy(world1); EQUALS(r, LIBRG_OK);
        r = librg_world_destroy(world2); EQUALS(r, LIBRG_OK);
    });

    IT("should fail to read create for single entity with invalid data", {
        librg_world *world1 = librg_world_create();
        librg_world *world2 = librg_world_create();

        r = librg_entity_track(world1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world1, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(world1, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_radius_set(world1, 1, 1); EQUALS(r, LIBRG_OK);

        r = librg_event_set(world2, LIBRG_READ_CREATE, dummy_markuserdata); EQUALS(r, LIBRG_OK);

        char buffer[4096] = {0};
        buffer_size = 4096;
        r = librg_world_write(world1, 1, buffer, &buffer_size, NULL);

        r = librg_entity_count(world2); EQUALS(r, 0);
        r = librg_world_read(world2, 1, buffer, buffer_size-25, NULL); NEQUALS(r, LIBRG_OK);
        r = librg_entity_count(world2); EQUALS(r, 0);

        r = librg_world_destroy(world1); EQUALS(r, LIBRG_OK);
        r = librg_world_destroy(world2); EQUALS(r, LIBRG_OK);
    });

    IT("should read created entity should be foreign, and untrack should error out", {
        librg_world *world1 = librg_world_create();
        librg_world *world2 = librg_world_create();

        r = librg_entity_track(world1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world1, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(world1, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_radius_set(world1, 1, 1); EQUALS(r, LIBRG_OK);

        r = librg_event_set(world2, LIBRG_READ_CREATE, dummy_markuserdata); EQUALS(r, LIBRG_OK);

        char buffer[4096] = {0};
        buffer_size = 4096;
        r = librg_world_write(world1, 1, buffer, &buffer_size, NULL);

        r = librg_entity_count(world2); EQUALS(r, 0);
        r = librg_world_read(world2, 1, buffer, buffer_size, NULL); EQUALS(r, LIBRG_OK);
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
        r = librg_entity_owner_set(world1, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_radius_set(world1, 1, 1); EQUALS(r, LIBRG_OK);

        r = librg_event_set(world2, LIBRG_ERROR_CREATE, dummy_markuserdata); EQUALS(r, LIBRG_OK);

        char buffer[4096] = {0};
        buffer_size = 4096;
        r = librg_world_write(world1, 1, buffer, &buffer_size, NULL);

        int value = 0;
        r = librg_entity_track(world2, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_count(world2); EQUALS(r, 1);
        r = librg_world_read(world2, 1, buffer, buffer_size, &value); EQUALS(r, LIBRG_OK);
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

    IT("should read update for single entity", {
        librg_world *world1 = librg_world_create();
        librg_world *world2 = librg_world_create();

        r = librg_entity_track(world1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world1, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(world1, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_radius_set(world1, 1, 1); EQUALS(r, LIBRG_OK);

        r = librg_event_set(world2, LIBRG_READ_UPDATE, dummy_markuserdata); EQUALS(r, LIBRG_OK);

        int value = 0;
        char buffer[4096] = {0};
        buffer_size = 4096;
        r = librg_world_write(world1, 1, buffer, &buffer_size, NULL);
        r = librg_entity_count(world2); EQUALS(r, 0);
        r = librg_world_read(world2, 1, buffer, buffer_size, &value); EQUALS(r, LIBRG_OK);
        r = librg_entity_count(world2); EQUALS(r, 1);

        buffer_size = 4096;
        r = librg_world_write(world1, 1, buffer, &buffer_size, NULL);
        r = librg_world_read(world2, 1, buffer, buffer_size, &value); EQUALS(r, LIBRG_OK);

        EQUALS(value, 1); /* value == 1 means the LIBRG_READ_UPDATE call was indeed executed */

        r = librg_world_destroy(world1); EQUALS(r, LIBRG_OK);
        r = librg_world_destroy(world2); EQUALS(r, LIBRG_OK);
    });

    IT("should fail to read update for single entity with extra data", {
        librg_world *world1 = librg_world_create();
        librg_world *world2 = librg_world_create();

        r = librg_entity_track(world1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world1, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(world1, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_radius_set(world1, 1, 1); EQUALS(r, LIBRG_OK);

        r = librg_event_set(world1, LIBRG_WRITE_UPDATE, dummy_2bytes); EQUALS(r, LIBRG_OK);
        r = librg_event_set(world2, LIBRG_READ_UPDATE, dummy_2bytes); EQUALS(r, LIBRG_OK);

        int value = 0;
        char buffer[4096] = {0};

        buffer_size = 4096;
        r = librg_world_write(world1, 1, buffer, &buffer_size, NULL);
        r = librg_world_read(world2, 1, buffer, buffer_size, &value); EQUALS(r, LIBRG_OK);

        buffer_size = 4096;
        r = librg_world_write(world1, 1, buffer, &buffer_size, NULL);
        r = librg_world_read(world2, 1, buffer, buffer_size, &value); EQUALS(r, LIBRG_OK);

        r = librg_world_destroy(world1); EQUALS(r, LIBRG_OK);
        r = librg_world_destroy(world2); EQUALS(r, LIBRG_OK);
    });

    IT("should fail to read update for single entity with invalid data", {
        librg_world *world1 = librg_world_create();
        librg_world *world2 = librg_world_create();

        r = librg_entity_track(world1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world1, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(world1, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_radius_set(world1, 1, 1); EQUALS(r, LIBRG_OK);

        int value = 0;
        char buffer[4096] = {0};

        buffer_size = 4096;
        r = librg_world_write(world1, 1, buffer, &buffer_size, NULL);
        r = librg_world_read(world2, 1, buffer, buffer_size, &value); EQUALS(r, LIBRG_OK);

        buffer_size = 4096;
        r = librg_world_write(world1, 1, buffer, &buffer_size, NULL);
        r = librg_world_read(world2, 1, buffer, buffer_size-1, &value); NEQUALS(r, LIBRG_OK);

        r = librg_world_destroy(world1); EQUALS(r, LIBRG_OK);
        r = librg_world_destroy(world2); EQUALS(r, LIBRG_OK);
    });

    IT("should read updated entity should be foreign, and error out if it is not", {
        librg_world *world1 = librg_world_create();
        librg_world *world2 = librg_world_create();

        r = librg_entity_track(world1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world1, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(world1, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_radius_set(world1, 1, 1); EQUALS(r, LIBRG_OK);

        r = librg_event_set(world2, LIBRG_ERROR_UPDATE, dummy_markuserdata); EQUALS(r, LIBRG_OK);

        int value = 0;
        char buffer[4096] = {0};

        r = librg_entity_track(world2, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_count(world2); EQUALS(r, 1);

        buffer_size = 4096;
        r = librg_world_write(world1, 1, buffer, &buffer_size, NULL);
        r = librg_world_read(world2, 1, buffer, buffer_size, &value); EQUALS(r, LIBRG_OK);
        r = librg_entity_count(world2); EQUALS(r, 1);

        buffer_size = 4096;
        r = librg_world_write(world1, 1, buffer, &buffer_size, NULL);
        r = librg_world_read(world2, 1, buffer, buffer_size, &value); EQUALS(r, LIBRG_OK);

        EQUALS(value, 1); /* value == 1 means the LIBRG_ERROR_UPDATE call was indeed executed */

        r = librg_world_destroy(world1); EQUALS(r, LIBRG_OK);
        r = librg_world_destroy(world2); EQUALS(r, LIBRG_OK);
    });

    // =======================================================================//
    // !
    // ! Reading remove
    // !
    // =======================================================================//

    IT("should read remove for single entity", {
        librg_world *world1 = librg_world_create();
        librg_world *world2 = librg_world_create();

        r = librg_entity_track(world1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world1, 2); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world1, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world1, 2, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(world1, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_radius_set(world1, 1, 1); EQUALS(r, LIBRG_OK);

        r = librg_event_set(world2, LIBRG_READ_REMOVE, dummy_markuserdata); EQUALS(r, LIBRG_OK);

        int value = 0;
        char buffer[4096] = {0};

        buffer_size = 4096;
        r = librg_world_write(world1, 1, buffer, &buffer_size, NULL);
        r = librg_entity_count(world2); EQUALS(r, 0);
        r = librg_world_read(world2, 1, buffer, buffer_size, &value); EQUALS(r, LIBRG_OK);
        r = librg_entity_count(world2); EQUALS(r, 2);

        buffer_size = 4096;
        r = librg_world_write(world1, 1, buffer, &buffer_size, NULL);
        r = librg_world_read(world2, 1, buffer, buffer_size, &value); EQUALS(r, LIBRG_OK);
        r = librg_entity_count(world2); EQUALS(r, 2);

        librg_entity_untrack(world1, 2);

        buffer_size = 4096;
        r = librg_world_write(world1, 1, buffer, &buffer_size, NULL);
        r = librg_entity_count(world2); EQUALS(r, 2);
        r = librg_world_read(world2, 1, buffer, buffer_size, &value); EQUALS(r, LIBRG_OK);
        r = librg_entity_count(world2); EQUALS(r, 1);

        EQUALS(value, 1); /* value == 1 means the LIBRG_READ_REMOVE call was indeed executed */

        r = librg_world_destroy(world1); EQUALS(r, LIBRG_OK);
        r = librg_world_destroy(world2); EQUALS(r, LIBRG_OK);
    });

    IT("should fail to read remove for single entity with extra data", {
        librg_world *world1 = librg_world_create();
        librg_world *world2 = librg_world_create();

        r = librg_entity_track(world1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world1, 2); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world1, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world1, 2, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(world1, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_radius_set(world1, 1, 1); EQUALS(r, LIBRG_OK);

        r = librg_event_set(world1, LIBRG_WRITE_REMOVE, dummy_2bytes); EQUALS(r, LIBRG_OK);
        r = librg_event_set(world2, LIBRG_READ_REMOVE, dummy_2bytes); EQUALS(r, LIBRG_OK);

        int value = 0;
        char buffer[4096] = {0};

        buffer_size = 4096;
        r = librg_world_write(world1, 1, buffer, &buffer_size, NULL);
        r = librg_entity_count(world2); EQUALS(r, 0);
        r = librg_world_read(world2, 1, buffer, buffer_size, &value); EQUALS(r, LIBRG_OK);
        r = librg_entity_count(world2); EQUALS(r, 2);

        buffer_size = 4096;
        r = librg_world_write(world1, 1, buffer, &buffer_size, NULL);
        r = librg_world_read(world2, 1, buffer, buffer_size, &value); EQUALS(r, LIBRG_OK);
        r = librg_entity_count(world2); EQUALS(r, 2);

        librg_entity_untrack(world1, 2);

        buffer_size = 4096;
        r = librg_world_write(world1, 1, buffer, &buffer_size, NULL);
        r = librg_entity_count(world2); EQUALS(r, 2);
        r = librg_world_read(world2, 1, buffer, buffer_size, &value); EQUALS(r, LIBRG_OK);
        r = librg_entity_count(world2); EQUALS(r, 1);

        r = librg_world_destroy(world1); EQUALS(r, LIBRG_OK);
        r = librg_world_destroy(world2); EQUALS(r, LIBRG_OK);
    });

    IT("should fail to read remove for single entity with invalid data", {
        librg_world *world1 = librg_world_create();
        librg_world *world2 = librg_world_create();

        r = librg_entity_track(world1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world1, 2); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world1, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world1, 2, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(world1, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_radius_set(world1, 1, 1); EQUALS(r, LIBRG_OK);

        int value = 0;
        char buffer[4096] = {0};

        buffer_size = 4096;
        r = librg_world_write(world1, 1, buffer, &buffer_size, NULL);
        r = librg_entity_count(world2); EQUALS(r, 0);
        r = librg_world_read(world2, 1, buffer, buffer_size, &value); EQUALS(r, LIBRG_OK);
        r = librg_entity_count(world2); EQUALS(r, 2);

        buffer_size = 4096;
        r = librg_world_write(world1, 1, buffer, &buffer_size, NULL);
        r = librg_world_read(world2, 1, buffer, buffer_size, &value); EQUALS(r, LIBRG_OK);
        r = librg_entity_count(world2); EQUALS(r, 2);

        librg_entity_untrack(world1, 2);

        buffer_size = 4096;
        r = librg_world_write(world1, 1, buffer, &buffer_size, NULL);
        r = librg_entity_count(world2); EQUALS(r, 2);
        r = librg_world_read(world2, 1, buffer, buffer_size-2, &value); NEQUALS(r, LIBRG_OK);
        r = librg_entity_count(world2); EQUALS(r, 2);

        r = librg_world_destroy(world1); EQUALS(r, LIBRG_OK);
        r = librg_world_destroy(world2); EQUALS(r, LIBRG_OK);
    });

    IT("should prevent foreign entity from be untracked manually", {
        librg_world *world1 = librg_world_create();
        librg_world *world2 = librg_world_create();

        r = librg_entity_track(world1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world1, 2); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world1, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world1, 2, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(world1, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_radius_set(world1, 1, 1); EQUALS(r, LIBRG_OK);

        r = librg_event_set(world2, LIBRG_READ_REMOVE, dummy_markuserdata); EQUALS(r, LIBRG_OK);

        int value = 0;
        char buffer[4096] = {0};

        buffer_size = 4096;
        r = librg_world_write(world1, 1, buffer, &buffer_size, NULL);
        r = librg_entity_count(world2); EQUALS(r, 0);
        r = librg_world_read(world2, 1, buffer, buffer_size, &value); EQUALS(r, LIBRG_OK);
        r = librg_entity_count(world2); EQUALS(r, 2);

        buffer_size = 4096;
        r = librg_world_write(world1, 1, buffer, &buffer_size, NULL);
        r = librg_world_read(world2, 1, buffer, buffer_size, &value); EQUALS(r, LIBRG_OK);
        r = librg_entity_count(world2); EQUALS(r, 2);

        r = librg_entity_untrack(world1, 2); EQUALS(r, LIBRG_OK);
        r = librg_entity_untrack(world2, 2); NEQUALS(r, LIBRG_OK);

        buffer_size = 4096;
        r = librg_world_write(world1, 1, buffer, &buffer_size, NULL);
        r = librg_entity_count(world2); EQUALS(r, 2);
        r = librg_world_read(world2, 1, buffer, buffer_size, &value); EQUALS(r, LIBRG_OK);
        r = librg_entity_count(world2); EQUALS(r, 1);

        EQUALS(value, 1); /* value == 1 means the LIBRG_READ_REMOVE call was indeed executed */

        r = librg_world_destroy(world1); EQUALS(r, LIBRG_OK);
        r = librg_world_destroy(world2); EQUALS(r, LIBRG_OK);
    });

    // =======================================================================//
    // !
    // ! Other
    // !
    // =======================================================================//

    IT("should read update backwards (from foreign entity) and call update event", {
        librg_world *world1 = librg_world_create();
        librg_world *world2 = librg_world_create();

        r = librg_entity_track(world1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world1, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(world1, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_radius_set(world1, 1, 1); EQUALS(r, LIBRG_OK);

        r = librg_event_set(world1, LIBRG_READ_UPDATE, dummy_markuserdata); EQUALS(r, LIBRG_OK);
        r = librg_event_set(world2, LIBRG_READ_UPDATE, dummy_markuserdata); EQUALS(r, LIBRG_OK);

        int value = 0;
        char buffer[4096] = {0};

        buffer_size = 4096;
        r = librg_world_write(world1, 1, buffer, &buffer_size, NULL);

        r = librg_entity_count(world2); EQUALS(r, 0);
        r = librg_world_read(world2, 1, buffer, buffer_size, &value); EQUALS(r, LIBRG_OK);
        r = librg_entity_count(world2); EQUALS(r, 1);

        buffer_size = 4096;
        r = librg_world_write(world1, 1, buffer, &buffer_size, NULL);
        r = librg_world_read(world2, 1, buffer, buffer_size, &value); EQUALS(r, LIBRG_OK);

        EQUALS(value, 1); /* value == 1 means the LIBRG_READ_UPDATE call was indeed executed */

        /* read backwards */
        value = 0;
        buffer_size = 4096; r = librg_world_write(world2, 1, buffer, &buffer_size, NULL); GREATER(buffer_size, 0);
        r = librg_world_read(world1, 1, buffer, buffer_size, &value);  EQUALS(r, LIBRG_OK);

        EQUALS(value, 1); /* value == 1 means the LIBRG_READ_UPDATE call was indeed executed */

        r = librg_world_destroy(world1); EQUALS(r, LIBRG_OK);
        r = librg_world_destroy(world2); EQUALS(r, LIBRG_OK);
    });

    IT("should not read update backwards (from foreign entity) if token has changed", {
        librg_world *world1 = librg_world_create();
        librg_world *world2 = librg_world_create();

        r = librg_entity_track(world1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world1, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(world1, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_radius_set(world1, 1, 1); EQUALS(r, LIBRG_OK);

        r = librg_event_set(world1, LIBRG_ERROR_UPDATE, dummy_markuserdata); EQUALS(r, LIBRG_OK);
        r = librg_event_set(world2, LIBRG_READ_UPDATE, dummy_markuserdata); EQUALS(r, LIBRG_OK);

        int value = 0;
        char buffer[4096] = {0};

        buffer_size = 4096;
        r = librg_world_write(world1, 1, buffer, &buffer_size, NULL);

        r = librg_entity_count(world2); EQUALS(r, 0);
        r = librg_world_read(world2, 1, buffer, buffer_size, &value); EQUALS(r, LIBRG_OK);
        r = librg_entity_count(world2); EQUALS(r, 1);

        buffer_size = 4096;
        r = librg_world_write(world1, 1, buffer, &buffer_size, NULL);
        r = librg_world_read(world2, 1, buffer, buffer_size, &value); EQUALS(r, LIBRG_OK);

        EQUALS(value, 1); /* value == 1 means the LIBRG_READ_UPDATE call was indeed executed */

        /* set same owner, but token will be updated */
        librg_entity_owner_set(world1, 1, 1);

        /* read backwards */
        value = 0;
        buffer_size = 4096; r = librg_world_write(world2, 1, buffer, &buffer_size, NULL); GREATER(buffer_size, 0);
        r = librg_world_read(world1, 1, buffer, buffer_size, &value);  EQUALS(r, LIBRG_OK);

        EQUALS(value, 1); /* value == 1 means the LIBRG_READ_UPDATE call was indeed executed */

        r = librg_world_destroy(world1); EQUALS(r, LIBRG_OK);
        r = librg_world_destroy(world2); EQUALS(r, LIBRG_OK);
    });
});
