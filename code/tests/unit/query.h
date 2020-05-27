MODULE(query, {
    int8_t r = LIBRG_FAIL;

    IT("should be able to fetch all tracked entities", {
        librg_world *world = librg_world_create();

        r = librg_entity_track(world, 1); NEQUALS(r, LIBRG_FAIL);
        r = librg_entity_track(world, 2); NEQUALS(r, LIBRG_FAIL);
        r = librg_entity_track(world, 3); NEQUALS(r, LIBRG_FAIL);

        int64_t results[16] = {0};
        size_t amt = librg_world_fetch_all(world, results, 16);

        EQUALS(amt, 3);
        EQUALS(results[0], 1);
        EQUALS(results[1], 2);
        EQUALS(results[2], 3);

        librg_world_destroy(world);
    });

    IT("should be able to fetch entities within a chunk", {
        librg_world *world = librg_world_create();

        r = librg_entity_track(world, 1); NEQUALS(r, LIBRG_FAIL);
        r = librg_entity_track(world, 2); NEQUALS(r, LIBRG_FAIL);
        r = librg_entity_track(world, 3); NEQUALS(r, LIBRG_FAIL);

        r = librg_entity_chunk_set(world, 1, 5); NEQUALS(r, LIBRG_FAIL);
        r = librg_entity_chunk_set(world, 2, 5); NEQUALS(r, LIBRG_FAIL);
        r = librg_entity_chunk_set(world, 3, 4); NEQUALS(r, LIBRG_FAIL);

        int64_t results[16] = {0};
        size_t amt = librg_world_fetch_chunk(world, 5, results, 16);

        EQUALS(amt, 2);
        EQUALS(results[0], 1);
        EQUALS(results[1], 2);

        amt = librg_world_fetch_chunk(world, 4, results, 16);

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

        size_t amt = librg_world_fetch_chunkarray(world, chunks, 2, results, 16);

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

        r = librg_entity_owner_set(world, 1, 4, LIBRG_OBSERVE_NONE); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(world, 2, 5, LIBRG_OBSERVE_NONE); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(world, 3, 6, LIBRG_OBSERVE_NONE); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(world, 4, 6, LIBRG_OBSERVE_NONE); EQUALS(r, LIBRG_OK);

        int64_t results[16] = {0};
        int64_t owners_ids[2]; owners_ids[0] = 5; owners_ids[1] = 6;
        size_t amt = librg_world_fetch_ownerarray(world, owners_ids, 2, results, 16);

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

        r = librg_entity_owner_set(world, 1, 4, LIBRG_OBSERVE_NONE); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(world, 2, 5, LIBRG_OBSERVE_NONE); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(world, 3, 6, LIBRG_OBSERVE_NONE); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(world, 4, 6, LIBRG_OBSERVE_NONE); EQUALS(r, LIBRG_OK);

        int64_t results[16] = {0};
        size_t amt = librg_world_query(world, 0, results, 16);
        EQUALS(amt, 0);

        librg_world_destroy(world);
    });

    IT("should query and return with own entities for owner", {
        librg_world *world = librg_world_create();

        r = librg_entity_track(world, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 2); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 3); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(world, 4); EQUALS(r, LIBRG_OK);

        r = librg_entity_owner_set(world, 1, 4, LIBRG_OBSERVE_NONE); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(world, 2, 5, LIBRG_OBSERVE_NONE); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(world, 3, 6, LIBRG_OBSERVE_NONE); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(world, 4, 6, LIBRG_OBSERVE_NONE); EQUALS(r, LIBRG_OK);

        int64_t results[16] = {0};
        size_t amt = librg_world_query(world, 6, results, 16);
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

        r = librg_entity_owner_set(world, 1, 4, LIBRG_OBSERVE_NONE); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(world, 2, 5, LIBRG_OBSERVE_NONE); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(world, 3, 6, LIBRG_OBSERVE_NONE); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(world, 4, 6, LIBRG_OBSERVE_NONE); EQUALS(r, LIBRG_OK);

        r = librg_entity_dimension_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_dimension_set(world, 2, 2); EQUALS(r, LIBRG_OK);
        r = librg_entity_dimension_set(world, 3, 3); EQUALS(r, LIBRG_OK);
        r = librg_entity_dimension_set(world, 4, 4); EQUALS(r, LIBRG_OK);

        int64_t results[16] = {0};
        size_t amt = librg_world_query(world, 6, results, 16);
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

        r = librg_entity_owner_set(world, 1, 4, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(world, 2, 5, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(world, 3, 6, 1); EQUALS(r, LIBRG_OK);

        int64_t results[16] = {0};
        size_t amt = librg_world_query(world, 6, results, 16);
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

        r = librg_entity_owner_set(world, 1, 4, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(world, 2, 5, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(world, 3, 6, 1); EQUALS(r, LIBRG_OK);

        r = librg_entity_dimension_set(world, 1, 3); EQUALS(r, LIBRG_OK);
        r = librg_entity_dimension_set(world, 2, 2); EQUALS(r, LIBRG_OK);
        r = librg_entity_dimension_set(world, 3, 3); EQUALS(r, LIBRG_OK);

        int64_t results[16] = {0};
        size_t amt = librg_world_query(world, 6, results, 16);
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

        r = librg_entity_owner_set(world, 1, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(world, 2, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(world, 3, 2, LIBRG_OBSERVE_NONE); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(world, 4, 2, LIBRG_OBSERVE_NONE); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(world, 5, 2, LIBRG_OBSERVE_NONE); EQUALS(r, LIBRG_OK);

        r = librg_entity_dimension_set(world, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_dimension_set(world, 2, 2); EQUALS(r, LIBRG_OK);
        r = librg_entity_dimension_set(world, 3, 2); EQUALS(r, LIBRG_OK);
        r = librg_entity_dimension_set(world, 4, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_dimension_set(world, 5, 1); EQUALS(r, LIBRG_OK);

        int64_t results[16] = {0};
        size_t amt = librg_world_query(world, 1, results, 16);

        EQUALS(amt, 5);
        EQUALS(results[0], 1); // own entity first
        EQUALS(results[1], 2);
        EQUALS(results[2], 3);
        EQUALS(results[3], 4);
        EQUALS(results[4], 5);

        librg_world_destroy(world);
    });
});
