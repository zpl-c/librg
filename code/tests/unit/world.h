MODULE(world, {
    int8_t r = LIBRG_FAIL;

    IT("should be able to fetch all tracked entities", {
        librg_ctx *ctx = librg_context_create();

        r = librg_entity_track(ctx, 1); NEQUALS(r, LIBRG_FAIL);
        r = librg_entity_track(ctx, 2); NEQUALS(r, LIBRG_FAIL);
        r = librg_entity_track(ctx, 3); NEQUALS(r, LIBRG_FAIL);

        int64_t results[16] = {0};
        size_t amt = librg_world_fetch_all(ctx, results, 16);

        EQUALS(amt, 3);
        EQUALS(results[0], 1);
        EQUALS(results[1], 2);
        EQUALS(results[2], 3);

        librg_context_destroy(ctx);
    });

    IT("should be able to fetch entities within a chunk", {
        librg_ctx *ctx = librg_context_create();

        r = librg_entity_track(ctx, 1); NEQUALS(r, LIBRG_FAIL);
        r = librg_entity_track(ctx, 2); NEQUALS(r, LIBRG_FAIL);
        r = librg_entity_track(ctx, 3); NEQUALS(r, LIBRG_FAIL);

        r = librg_entity_chunk_set(ctx, 1, 5); NEQUALS(r, LIBRG_FAIL);
        r = librg_entity_chunk_set(ctx, 2, 5); NEQUALS(r, LIBRG_FAIL);
        r = librg_entity_chunk_set(ctx, 3, 4); NEQUALS(r, LIBRG_FAIL);

        int64_t results[16] = {0};
        size_t amt = librg_world_fetch_chunk(ctx, 5, results, 16);

        EQUALS(amt, 2);
        EQUALS(results[0], 1);
        EQUALS(results[1], 2);

        amt = librg_world_fetch_chunk(ctx, 4, results, 16);

        EQUALS(amt, 1);
        EQUALS(results[0], 3);

        librg_context_destroy(ctx);
    });

    IT("should be able to fetch entities from chunk array", {
        librg_ctx *ctx = librg_context_create();

        r = librg_entity_track(ctx, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(ctx, 2); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(ctx, 3); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(ctx, 4); EQUALS(r, LIBRG_OK);

        r = librg_entity_chunk_set(ctx, 1, 4); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(ctx, 2, 5); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(ctx, 3, 6); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(ctx, 4, 6); EQUALS(r, LIBRG_OK);

        int64_t results[16] = {0};
        librg_chunk chunks[2] = {0}; chunks[0] = 5; chunks[1] = 6;

        size_t amt = librg_world_fetch_chunkarray(ctx, chunks, 2, results, 16);

        EQUALS(amt, 3);
        EQUALS(results[0], 2);
        EQUALS(results[1], 3);
        EQUALS(results[2], 4);

        librg_context_destroy(ctx);
    });

    IT("should be able to fetch entities from owner array", {
        librg_ctx *ctx = librg_context_create();

        r = librg_entity_track(ctx, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(ctx, 2); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(ctx, 3); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(ctx, 4); EQUALS(r, LIBRG_OK);

        r = librg_entity_owner_set(ctx, 1, 4, LIBRG_OBSERVE_NONE); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(ctx, 2, 5, LIBRG_OBSERVE_NONE); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(ctx, 3, 6, LIBRG_OBSERVE_NONE); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(ctx, 4, 6, LIBRG_OBSERVE_NONE); EQUALS(r, LIBRG_OK);

        int64_t results[16] = {0};
        int64_t owners_ids[2]; owners_ids[0] = 5; owners_ids[1] = 6;
        size_t amt = librg_world_fetch_ownerarray(ctx, owners_ids, 2, results, 16);

        EQUALS(amt, 3);
        EQUALS(results[0], 2);
        EQUALS(results[1], 3);
        EQUALS(results[2], 4);

        librg_context_destroy(ctx);
    });

    IT("should be able to query and return empty result", {
        librg_ctx *ctx = librg_context_create();

        r = librg_entity_track(ctx, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(ctx, 2); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(ctx, 3); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(ctx, 4); EQUALS(r, LIBRG_OK);

        r = librg_entity_owner_set(ctx, 1, 4, LIBRG_OBSERVE_NONE); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(ctx, 2, 5, LIBRG_OBSERVE_NONE); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(ctx, 3, 6, LIBRG_OBSERVE_NONE); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(ctx, 4, 6, LIBRG_OBSERVE_NONE); EQUALS(r, LIBRG_OK);

        int64_t results[16] = {0};
        size_t amt = librg_world_query(ctx, 0, results, 16);
        EQUALS(amt, 0);

        librg_context_destroy(ctx);
    });

    IT("should query and return with own entities for owner", {
        librg_ctx *ctx = librg_context_create();

        r = librg_entity_track(ctx, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(ctx, 2); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(ctx, 3); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(ctx, 4); EQUALS(r, LIBRG_OK);

        r = librg_entity_owner_set(ctx, 1, 4, LIBRG_OBSERVE_NONE); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(ctx, 2, 5, LIBRG_OBSERVE_NONE); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(ctx, 3, 6, LIBRG_OBSERVE_NONE); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(ctx, 4, 6, LIBRG_OBSERVE_NONE); EQUALS(r, LIBRG_OK);

        int64_t results[16] = {0};
        size_t amt = librg_world_query(ctx, 6, results, 16);
        EQUALS(amt, 2);
        EQUALS(results[0], 3);
        EQUALS(results[1], 4);

        librg_context_destroy(ctx);
    });

    IT("should query and return with own entities for owner from multi dimensions", {
        librg_ctx *ctx = librg_context_create();

        r = librg_entity_track(ctx, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(ctx, 2); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(ctx, 3); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(ctx, 4); EQUALS(r, LIBRG_OK);

        r = librg_entity_owner_set(ctx, 1, 4, LIBRG_OBSERVE_NONE); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(ctx, 2, 5, LIBRG_OBSERVE_NONE); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(ctx, 3, 6, LIBRG_OBSERVE_NONE); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(ctx, 4, 6, LIBRG_OBSERVE_NONE); EQUALS(r, LIBRG_OK);

        r = librg_entity_dimension_set(ctx, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_dimension_set(ctx, 2, 2); EQUALS(r, LIBRG_OK);
        r = librg_entity_dimension_set(ctx, 3, 3); EQUALS(r, LIBRG_OK);
        r = librg_entity_dimension_set(ctx, 4, 4); EQUALS(r, LIBRG_OK);

        int64_t results[16] = {0};
        size_t amt = librg_world_query(ctx, 6, results, 16);
        EQUALS(amt, 2);
        EQUALS(results[0], 3);
        EQUALS(results[1], 4);

        librg_context_destroy(ctx);
    });

    IT("should query within same chunk", {
        librg_ctx *ctx = librg_context_create();

        r = librg_entity_track(ctx, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(ctx, 2); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(ctx, 3); EQUALS(r, LIBRG_OK);

        r = librg_entity_chunk_set(ctx, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(ctx, 2, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(ctx, 3, 1); EQUALS(r, LIBRG_OK);

        r = librg_entity_owner_set(ctx, 1, 4, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(ctx, 2, 5, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(ctx, 3, 6, 1); EQUALS(r, LIBRG_OK);

        int64_t results[16] = {0};
        size_t amt = librg_world_query(ctx, 6, results, 16);
        EQUALS(amt, 3);
        EQUALS(results[0], 3); // own entity first
        EQUALS(results[1], 1);
        EQUALS(results[2], 2);

        librg_context_destroy(ctx);
    });

    IT("should query within same chunk but different dimensions", {
        librg_ctx *ctx = librg_context_create();

        r = librg_entity_track(ctx, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(ctx, 2); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(ctx, 3); EQUALS(r, LIBRG_OK);

        r = librg_entity_chunk_set(ctx, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(ctx, 2, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(ctx, 3, 1); EQUALS(r, LIBRG_OK);

        r = librg_entity_owner_set(ctx, 1, 4, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(ctx, 2, 5, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(ctx, 3, 6, 1); EQUALS(r, LIBRG_OK);

        r = librg_entity_dimension_set(ctx, 1, 3); EQUALS(r, LIBRG_OK);
        r = librg_entity_dimension_set(ctx, 2, 2); EQUALS(r, LIBRG_OK);
        r = librg_entity_dimension_set(ctx, 3, 3); EQUALS(r, LIBRG_OK);

        int64_t results[16] = {0};
        size_t amt = librg_world_query(ctx, 6, results, 16);
        EQUALS(amt, 2);
        EQUALS(results[0], 3); // own entity first
        EQUALS(results[1], 1);
        // EQUALS(results[2], 2);

        librg_context_destroy(ctx);
    });

    IT("should query multiple owned entities in same chunk across multiple dimensions", {
        librg_ctx *ctx = librg_context_create();

        r = librg_entity_track(ctx, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(ctx, 2); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(ctx, 3); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(ctx, 4); EQUALS(r, LIBRG_OK);
        r = librg_entity_track(ctx, 5); EQUALS(r, LIBRG_OK);

        r = librg_entity_chunk_set(ctx, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(ctx, 2, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(ctx, 3, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(ctx, 4, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(ctx, 5, 1); EQUALS(r, LIBRG_OK);

        r = librg_entity_owner_set(ctx, 1, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(ctx, 2, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(ctx, 3, 2, LIBRG_OBSERVE_NONE); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(ctx, 4, 2, LIBRG_OBSERVE_NONE); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(ctx, 5, 2, LIBRG_OBSERVE_NONE); EQUALS(r, LIBRG_OK);

        r = librg_entity_dimension_set(ctx, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_dimension_set(ctx, 2, 2); EQUALS(r, LIBRG_OK);
        r = librg_entity_dimension_set(ctx, 3, 2); EQUALS(r, LIBRG_OK);
        r = librg_entity_dimension_set(ctx, 4, 1); EQUALS(r, LIBRG_OK);
        r = librg_entity_dimension_set(ctx, 5, 1); EQUALS(r, LIBRG_OK);

        int64_t results[16] = {0};
        size_t amt = librg_world_query(ctx, 1, results, 16);

        EQUALS(amt, 5);
        EQUALS(results[0], 1); // own entity first
        EQUALS(results[1], 2);
        EQUALS(results[2], 3);
        EQUALS(results[3], 4);
        EQUALS(results[4], 5);

        librg_context_destroy(ctx);
    });
});
