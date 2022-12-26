MODULE(query, {
    int8_t r = -1;

    IT("should be able to fetch all tracked entities", {
        librg_world *world = librg_world_create();

        r = librg_entity_track(world, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 2); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 3); EQUALS(r, LIBRG_OK);

        int64_t results[16] = {0};
        size_t amt = 16;
        r = librg_world_fetch_all(world, results, &amt); EQUALS(r, 0);

        EQUALS(amt, 3);
        EQUALS(results[0], 1);
        EQUALS(results[1], 2);
        EQUALS(results[2], 3);

        librg_world_destroy(world);
    });

    IT("should be able to fetch entities within a chunk", {
        librg_world *world = librg_world_create();

        r = librg_entity_track(world, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 2); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 3); EQUALS(r, LIBRG_OK);

        r = librg_entity_chunk_set(world, 1, 5); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 2, 5); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 3, 4); EQUALS(r, LIBRG_OK);

        int64_t results[16] = {0};
        size_t amt = 16;
        librg_world_fetch_chunk(world, 5, results, &amt);

        EQUALS(amt, 2);
        EQUALS(results[0], 1);
        EQUALS(results[1], 2);

        amt = 16;
        librg_world_fetch_chunk(world, 4, results, &amt);

        EQUALS(amt, 1);
        EQUALS(results[0], 3);

        librg_world_destroy(world);
    });

    IT("should be able to fetch entities from chunk array", {
        librg_world *world = librg_world_create();

        r = librg_entity_track(world, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 2); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 3); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 4); EQUALS(r, LIBRG_OK);

        r = librg_entity_chunk_set(world, 1, 4); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 2, 5); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 3, 6); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 4, 6); EQUALS(r, LIBRG_OK);

        int64_t results[16] = {0};
        librg_chunk chunks[2] = {0}; chunks[0] = 5; chunks[1] = 6;

        size_t amt = 16;
        librg_world_fetch_chunkarray(world, chunks, 2, results, &amt);

        EQUALS(amt, 3);
        EQUALS(results[0], 2);
        EQUALS(results[1], 3);
        EQUALS(results[2], 4);

        librg_world_destroy(world);
    });

    IT("should be able to fetch entities from owner array", {
        librg_world *world = librg_world_create();

        r = librg_entity_track(world, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 2); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 3); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 4); EQUALS(r, LIBRG_OK);

        r = librg_entity_owner_set(world, 1, 4); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(world, 2, 5); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(world, 3, 6); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(world, 4, 6); EQUALS(r, LIBRG_OK);

        int64_t results[16] = {0};
        int64_t owners_ids[2]; owners_ids[0] = 5; owners_ids[1] = 6;
        size_t amt = 16;
        librg_world_fetch_ownerarray(world, owners_ids, 2, results, &amt);

        EQUALS(amt, 3);
        EQUALS(results[0], 2);
        EQUALS(results[1], 3);
        EQUALS(results[2], 4);

        librg_world_destroy(world);
    });

    IT("should be able to query and return empty result", {
        librg_world *world = librg_world_create();

        r = librg_entity_track(world, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 2); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 3); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 4); EQUALS(r, LIBRG_OK);

        r = librg_entity_owner_set(world, 1, 4); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(world, 2, 5); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(world, 3, 6); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(world, 4, 6); EQUALS(r, LIBRG_OK);

        int64_t results[16] = {0};
        size_t amt = 16;
        librg_world_query(world, 0, 0, results, &amt);
        EQUALS(amt, 0);

        librg_world_destroy(world);
    });

    IT("should query and return with own entities for owner", {
        librg_world *world = librg_world_create();

        r = librg_entity_track(world, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 2); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 3); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 4); EQUALS(r, LIBRG_OK);

        r = librg_entity_owner_set(world, 1, 4); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(world, 2, 5); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(world, 3, 6); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(world, 4, 6); EQUALS(r, LIBRG_OK);

        int64_t results[16] = {0};
        size_t amt = 16;
        librg_world_query(world, 6, 0, results, &amt);
        EQUALS(amt, 2);
        EQUALS(results[0], 3);
        EQUALS(results[1], 4);

        librg_world_destroy(world);
    });

    IT("should query and return with own entities for owner from multi dimensions", {
        librg_world *world = librg_world_create();

        r = librg_entity_track(world, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 2); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 3); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 4); EQUALS(r, LIBRG_OK);

        r = librg_entity_owner_set(world, 1, 4); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(world, 2, 5); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(world, 3, 6); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(world, 4, 6); EQUALS(r, LIBRG_OK);

        r = librg_entity_dimension_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_dimension_set(world, 2, 2); EQUALS(r, LIBRG_OK);
        r = librg_entity_dimension_set(world, 3, 3); EQUALS(r, LIBRG_OK);
        r = librg_entity_dimension_set(world, 4, 4); EQUALS(r, LIBRG_OK);

        int64_t results[16] = {0};
        size_t amt = 16;
        librg_world_query(world, 6, 0, results, &amt);
        EQUALS(amt, 2);
        EQUALS(results[0], 3);
        EQUALS(results[1], 4);

        librg_world_destroy(world);
    });

    IT("should query within same chunk", {
        librg_world *world = librg_world_create();

        r = librg_entity_track(world, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 2); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 3); EQUALS(r, LIBRG_OK);

        r = librg_entity_chunk_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 2, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 3, 1); EQUALS(r, LIBRG_OK);

        r = librg_entity_owner_set(world, 1, 4); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(world, 2, 5); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(world, 3, 6); EQUALS(r, LIBRG_OK);

        int64_t results[16] = {0};
        size_t amt = 16;
        librg_world_query(world, 6, 0, results, &amt);
        EQUALS(amt, 3);
        EQUALS(results[0], 3); // own entity first
        EQUALS(results[1], 1);
        EQUALS(results[2], 2);

        librg_world_destroy(world);
    });

    IT("should query within same chunk but different dimensions", {
        librg_world *world = librg_world_create();

        r = librg_entity_track(world, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 2); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 3); EQUALS(r, LIBRG_OK);

        r = librg_entity_chunk_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 2, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 3, 1); EQUALS(r, LIBRG_OK);

        r = librg_entity_owner_set(world, 1, 4); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(world, 2, 5); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(world, 3, 6); EQUALS(r, LIBRG_OK);

        r = librg_entity_dimension_set(world, 1, 3); EQUALS(r, LIBRG_OK);
        r = librg_entity_dimension_set(world, 2, 2); EQUALS(r, LIBRG_OK);
        r = librg_entity_dimension_set(world, 3, 3); EQUALS(r, LIBRG_OK);

        int64_t results[16] = {0};
        size_t amt = 16;
        librg_world_query(world, 6, 0, results, &amt);
        EQUALS(amt, 2);
        EQUALS(results[0], 3); // own entity first
        EQUALS(results[1], 1);
        // EQUALS(results[2], 2);

        librg_world_destroy(world);
    });

    IT("should query multiple owned entities in same chunk across multiple dimensions", {
        librg_world *world = librg_world_create();

        r = librg_entity_track(world, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 2); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 3); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 4); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 5); EQUALS(r, LIBRG_OK);

        r = librg_entity_chunk_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 2, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 3, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 4, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 5, 1); EQUALS(r, LIBRG_OK);

        r = librg_entity_owner_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(world, 2, 1); EQUALS(r, LIBRG_OK);

        r = librg_entity_owner_set(world, 3, 2); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(world, 4, 2); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(world, 5, 2); EQUALS(r, LIBRG_OK);

        r = librg_entity_dimension_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_dimension_set(world, 2, 2); EQUALS(r, LIBRG_OK);
        r = librg_entity_dimension_set(world, 3, 2); EQUALS(r, LIBRG_OK);
        r = librg_entity_dimension_set(world, 4, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_dimension_set(world, 5, 1); EQUALS(r, LIBRG_OK);

        int64_t results[16] = {0};
        size_t amt = 16;
        librg_world_query(world, 1, 0, results, &amt);

        EQUALS(amt, 5);
        EQUALS(results[0], 1); // own entity first
        EQUALS(results[1], 2);
        EQUALS(results[2], 3);
        EQUALS(results[3], 4);
        EQUALS(results[4], 5);

        librg_world_destroy(world);
    });

    IT("should query entities using spherical check", {
        librg_world *world = librg_world_create();

        /* world of size 5 * 5 * 5 centered at middle */
        librg_config_chunkamount_set(world, 5, 5, 5);
        librg_config_chunkoffset_set(world, LIBRG_OFFSET_MID, LIBRG_OFFSET_MID, LIBRG_OFFSET_MID);

        const int owner_id = 1;
        const int observation_range = 2;

        r = librg_entity_track(world, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(world, 1, owner_id); EQUALS(r, LIBRG_OK);

        for (int i = 1; i <= 10; ++i)
            r = librg_entity_track(world, i); EQUALS(r, LIBRG_OK);

        librg_entity_chunk_set(world, 1, librg_chunk_from_chunkpos(world, 0, 0, 0)); // observer in the middle

        /* entities inside on each coodrinate */
        librg_entity_chunk_set(world, 2, librg_chunk_from_chunkpos(world, 1, 0, 0));
        librg_entity_chunk_set(world, 3, librg_chunk_from_chunkpos(world, 0, 1, 0));
        librg_entity_chunk_set(world, 4, librg_chunk_from_chunkpos(world, 0, 0, 1));

        /* entities inside, on the edges */
        librg_entity_chunk_set(world, 5, librg_chunk_from_chunkpos(world, -1, -1, -1));
        librg_entity_chunk_set(world, 6, librg_chunk_from_chunkpos(world, 0, 2, 0));
        librg_entity_chunk_set(world, 7, librg_chunk_from_chunkpos(world, 0, 2, 0));

        /* entities outside */
        librg_entity_chunk_set(world, 8, librg_chunk_from_chunkpos(world, -5, -1, -1));
        librg_entity_chunk_set(world, 9, librg_chunk_from_chunkpos(world, 0, 0, 3));
        librg_entity_chunk_set(world, 10, librg_chunk_from_chunkpos(world, 2323, 0, 3));

        int64_t results[16] = {0};
        size_t amt = 16;
        librg_world_query(world, owner_id, observation_range, results, &amt);

        EQUALS(amt, 7);
        EQUALS(results[0], 1); // own entity first
        EQUALS(results[1], 2);
        EQUALS(results[2], 3);
        EQUALS(results[3], 4);
        EQUALS(results[4], 5);
        EQUALS(results[5], 6);
        EQUALS(results[6], 7);

        librg_world_destroy(world);
    });

    IT("should properly exclude globally-invisible entity", {
        librg_world *world = librg_world_create();

        r = librg_entity_track(world, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 2); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 3); EQUALS(r, LIBRG_OK);

        r = librg_entity_chunk_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 2, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 3, 1); EQUALS(r, LIBRG_OK);

        r = librg_entity_owner_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_visibility_global_set(world, 2, LIBRG_VISIBLITY_NEVER); EQUALS(r, LIBRG_OK);

        int64_t results[16] = {0};
        size_t amt = 16;
        librg_world_query(world, 1, 0, results, &amt);
        EQUALS(amt, 2);
        EQUALS(results[0], 1); // own entity first
        EQUALS(results[1], 3);

        librg_world_destroy(world);
    });

    IT("should properly include always-globally-visible entity without regards of chunk radius", {
        librg_world *world = librg_world_create();

        r = librg_entity_track(world, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 2); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 3); EQUALS(r, LIBRG_OK);

        r = librg_entity_chunk_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 2, 50); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 3, 1); EQUALS(r, LIBRG_OK);

        r = librg_entity_owner_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_visibility_global_set(world, 2, LIBRG_VISIBLITY_ALWAYS); EQUALS(r, LIBRG_OK);

        int64_t results[16] = {0};
        size_t amt = 16;
        librg_world_query(world, 1, 0, results, &amt);
        EQUALS(amt, 3);
        EQUALS(results[0], 1); // own entity first
        EQUALS(results[1], 2);
        EQUALS(results[2], 3);

        librg_world_destroy(world);
    });

    IT("should properly include always-globally-visible entity without regards of dimension", {
        librg_world *world = librg_world_create();

        r = librg_entity_track(world, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 2); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 3); EQUALS(r, LIBRG_OK);

        r = librg_entity_chunk_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 2, 50); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 3, 1); EQUALS(r, LIBRG_OK);

        r = librg_entity_owner_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_dimension_set(world, 2, 3); EQUALS(r, LIBRG_OK);
        r = librg_entity_visibility_global_set(world, 2, LIBRG_VISIBLITY_ALWAYS); EQUALS(r, LIBRG_OK);

        int64_t results[16] = {0};
        size_t amt = 16;
        librg_world_query(world, 1, 0, results, &amt);
        EQUALS(amt, 3);
        EQUALS(results[0], 1); // own entity first
        EQUALS(results[1], 2);
        EQUALS(results[2], 3);

        librg_world_destroy(world);
    });

    IT("should properly exclude owner-invisible entity", {
        librg_world *world = librg_world_create();

        r = librg_entity_track(world, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 2); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 3); EQUALS(r, LIBRG_OK);

        r = librg_entity_chunk_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 2, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 3, 1); EQUALS(r, LIBRG_OK);

        r = librg_entity_owner_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_visibility_owner_set(world, 2, 1, LIBRG_VISIBLITY_NEVER); EQUALS(r, LIBRG_OK);

        int64_t results[16] = {0};
        size_t amt = 16;
        librg_world_query(world, 1, 0, results, &amt);
        EQUALS(amt, 2);
        EQUALS(results[0], 1); // own entity first
        EQUALS(results[1], 3);

        librg_world_destroy(world);
    });

    IT("should properly exclude owner-invisible own entity", {
        librg_world *world = librg_world_create();

        r = librg_entity_track(world, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 2); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 3); EQUALS(r, LIBRG_OK);

        r = librg_entity_chunk_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 2, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 3, 1); EQUALS(r, LIBRG_OK);

        r = librg_entity_owner_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_visibility_owner_set(world, 2, 1, LIBRG_VISIBLITY_NEVER); EQUALS(r, LIBRG_OK);
        r = librg_entity_visibility_owner_set(world, 1, 1, LIBRG_VISIBLITY_NEVER); EQUALS(r, LIBRG_OK);

        int64_t results[16] = {0};
        size_t amt = 16;
        librg_world_query(world, 1, 0, results, &amt);
        EQUALS(amt, 1);
        EQUALS(results[0], 3);

        librg_world_destroy(world);
    });

    IT("should properly include default-owner-visible entity without regards of chunk radius", {
        librg_world *world = librg_world_create();

        r = librg_entity_track(world, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 2); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 3); EQUALS(r, LIBRG_OK);

        r = librg_entity_chunk_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 2, 50); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 3, 1); EQUALS(r, LIBRG_OK);

        r = librg_entity_owner_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_visibility_owner_set(world, 2, 1, LIBRG_VISIBLITY_ALWAYS); EQUALS(r, LIBRG_OK);

        int64_t results[16] = {0};
        size_t amt = 16;
        librg_world_query(world, 1, 0, results, &amt);
        EQUALS(amt, 3);
        EQUALS(results[0], 1); // own entity first
        EQUALS(results[1], 2);
        EQUALS(results[2], 3);

        librg_world_destroy(world);
    });

    IT("should properly include default-owner-visible entity without regards of dimension", {
        librg_world *world = librg_world_create();

        r = librg_entity_track(world, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 2); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 3); EQUALS(r, LIBRG_OK);

        r = librg_entity_chunk_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 2, 50); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 3, 1); EQUALS(r, LIBRG_OK);

        r = librg_entity_owner_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_dimension_set(world, 2, 3); EQUALS(r, LIBRG_OK);
        r = librg_entity_visibility_owner_set(world, 2, 1, LIBRG_VISIBLITY_ALWAYS); EQUALS(r, LIBRG_OK);

        int64_t results[16] = {0};
        size_t amt = 16;
        librg_world_query(world, 1, 0, results, &amt);
        EQUALS(amt, 3);
        EQUALS(results[0], 1); // own entity first
        EQUALS(results[1], 2);
        EQUALS(results[2], 3);

        librg_world_destroy(world);
    });

    IT("should properly exclude owner-invisible entity overriding always-globally-visible", {
        librg_world *world = librg_world_create();

        r = librg_entity_track(world, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 2); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 3); EQUALS(r, LIBRG_OK);

        r = librg_entity_chunk_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 2, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 3, 1); EQUALS(r, LIBRG_OK);

        r = librg_entity_owner_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_visibility_owner_set(world, 2, 1, LIBRG_VISIBLITY_NEVER); EQUALS(r, LIBRG_OK);
        r = librg_entity_visibility_global_set(world, 2, LIBRG_VISIBLITY_ALWAYS); EQUALS(r, LIBRG_OK);

        int64_t results[16] = {0};
        size_t amt = 16;
        librg_world_query(world, 1, 0, results, &amt);
        EQUALS(amt, 2);
        EQUALS(results[0], 1); // own entity first
        EQUALS(results[1], 3);

        librg_world_destroy(world);
    });

    IT("should exclude default-owner-visible entity overriding always-globally-visible", {
        librg_world *world = librg_world_create();

        r = librg_entity_track(world, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 2); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 3); EQUALS(r, LIBRG_OK);

        r = librg_entity_chunk_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 2, 50); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 3, 1); EQUALS(r, LIBRG_OK);

        r = librg_entity_owner_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_visibility_owner_set(world, 2, 1, LIBRG_VISIBLITY_ALWAYS); EQUALS(r, LIBRG_OK);
        r = librg_entity_visibility_global_set(world, 2, LIBRG_VISIBLITY_ALWAYS); EQUALS(r, LIBRG_OK);

        int64_t results[16] = {0};
        size_t amt = 16;
        librg_world_query(world, 1, 0, results, &amt);
        EQUALS(amt, 3);
        EQUALS(results[0], 1); // own entity first
        EQUALS(results[1], 2);
        EQUALS(results[2], 3);

        librg_world_destroy(world);
    });

    IT("should query entities with boundary checks on top-left", {
        librg_world *world = librg_world_create();
        r = librg_config_chunkamount_set(world, 10, 10, 1); EQUALS(r, LIBRG_OK);
        r = librg_config_chunkoffset_set(world, LIBRG_OFFSET_BEG, LIBRG_OFFSET_BEG, LIBRG_OFFSET_BEG);

        r = librg_entity_track(world, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 2); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 3); EQUALS(r, LIBRG_OK);

        r = librg_entity_chunk_set(world, 1, librg_chunk_from_chunkpos(world, 9, 3, 0)); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 2, librg_chunk_from_chunkpos(world, 4, 4, 0)); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 3, librg_chunk_from_chunkpos(world, 0, 4, 0)); EQUALS(r, LIBRG_OK);

        r = librg_entity_owner_set(world, 3, 1); EQUALS(r, LIBRG_OK);

        int64_t results[16] = {0};
        size_t amt = 16;
        librg_world_query(world, 1, 4, results, &amt);
        EQUALS(amt, 2);
        EQUALS(results[0], 3); // own entity first
        EQUALS(results[1], 2);

        librg_world_destroy(world);
    });

    IT("should query entities with boundary checks on mid-mid", {
        librg_world *world = librg_world_create();
        librg_chunk chid = LIBRG_CHUNK_INVALID;

        r = librg_config_chunkamount_set(world, 9, 9, 1); EQUALS(r, LIBRG_OK);
        r = librg_config_chunkoffset_set(world, LIBRG_OFFSET_MID, LIBRG_OFFSET_MID, LIBRG_OFFSET_MID);

        r = librg_entity_track(world, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 2); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 3); EQUALS(r, LIBRG_OK);

        chid = librg_chunk_from_chunkpos(world, 4, -1, 0); NEQUALS(chid, LIBRG_CHUNK_INVALID);
        r = librg_entity_chunk_set(world, 1, chid); EQUALS(r, LIBRG_OK);
        chid = librg_chunk_from_chunkpos(world, -3, 0, 0); NEQUALS(chid, LIBRG_CHUNK_INVALID);
        r = librg_entity_chunk_set(world, 2, chid); EQUALS(r, LIBRG_OK);
        chid = librg_chunk_from_chunkpos(world, -4, 0, 0); NEQUALS(chid, LIBRG_CHUNK_INVALID);
        r = librg_entity_chunk_set(world, 3, chid); EQUALS(r, LIBRG_OK);

        r = librg_entity_owner_set(world, 3, 1); EQUALS(r, LIBRG_OK);

        int64_t results[16] = {0};
        size_t amt = 16;
        librg_world_query(world, 1, 4, results, &amt);
        EQUALS(amt, 2);
        EQUALS(results[0], 3); // own entity first
        EQUALS(results[1], 2);

        librg_world_destroy(world);
    });

    IT("should query entities with 0 radius", {
        librg_world *world = librg_world_create();
        r = librg_config_chunkamount_set(world, 3, 3, 1); EQUALS(r, LIBRG_OK);
        r = librg_config_chunkoffset_set(world, LIBRG_OFFSET_BEG, LIBRG_OFFSET_BEG, LIBRG_OFFSET_BEG);

        r = librg_entity_track(world, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 2); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 3); EQUALS(r, LIBRG_OK);

        r = librg_entity_chunk_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 2, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 3, 2); EQUALS(r, LIBRG_OK);

        r = librg_entity_owner_set(world, 2, 1); EQUALS(r, LIBRG_OK);

        int64_t results[16] = {0};
        size_t amt = 16;
        librg_world_query(world, 1, 0, results, &amt);
        EQUALS(amt, 2);
        EQUALS(results[0], 2); // own entity first
        EQUALS(results[1], 1);

        librg_world_destroy(world);
    });

    IT("should query entities within big worlds", {
        librg_world *world = librg_world_create();

        r = librg_config_chunksize_set(world, 10, 10, UINT16_MAX); EQUALS(r, LIBRG_OK);
        r = librg_config_chunkamount_set(world, 1024, 1024, 1); EQUALS(r, LIBRG_OK);
        r = librg_config_chunkoffset_set(world, LIBRG_OFFSET_MID, LIBRG_OFFSET_MID, LIBRG_OFFSET_MID);

        r = librg_entity_track(world, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 2); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 3); EQUALS(r, LIBRG_OK);

        librg_chunk test_chunk = 32768;

        r = librg_entity_chunk_set(world, 1, test_chunk); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 2, test_chunk); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 3, test_chunk); EQUALS(r, LIBRG_OK);

        r = librg_entity_owner_set(world, 2, 100); EQUALS(r, LIBRG_OK);

        int64_t results[16] = {0}; size_t amt = 16;
        librg_world_query(world, 100, 0, results, &amt);

        EQUALS(amt, 3);
        EQUALS(results[0], 2); // own entity first
        EQUALS(results[1], 1);
        EQUALS(results[2], 3);

        librg_world_destroy(world);
    });

    IT("it should run query with exact value on limit", {
        librg_world *world = librg_world_create();

        r = librg_entity_track(world, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 2); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 3); EQUALS(r, LIBRG_OK);

        r = librg_entity_chunk_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 2, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 3, 1); EQUALS(r, LIBRG_OK);

        r = librg_entity_owner_set(world, 2, 100); EQUALS(r, LIBRG_OK);

        int64_t results[10] = {0}; size_t amt = 3;
        r = librg_world_query(world, 100, 1, results, &amt); EQUALS(r, 0);

        EQUALS(amt, 3);
        EQUALS(results[0], 2);

        librg_world_destroy(world);
    });

    IT("it should properly calculate recommended size for the query", {
        librg_world *world = librg_world_create();

        r = librg_entity_track(world, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 2); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 3); EQUALS(r, LIBRG_OK);

        r = librg_entity_chunk_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 2, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(world, 3, 1); EQUALS(r, LIBRG_OK);

        r = librg_entity_owner_set(world, 2, 100); EQUALS(r, LIBRG_OK);

        int64_t results[10] = {0}; size_t amt = 2;
        r = librg_world_query(world, 100, 1, results, &amt); EQUALS(r, 1);

        EQUALS(amt, 2);
        EQUALS(results[0], 2);

        librg_world_destroy(world);
    });
});
