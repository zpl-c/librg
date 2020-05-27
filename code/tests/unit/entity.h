MODULE(entity, {
    int8_t r = LIBRG_FAIL;

    IT("should be able to track and untrack an entity", {
        librg_ctx *ctx = librg_context_create();

        r = librg_entity_tracked(ctx, 15); EQUALS(r, LIBRG_FALSE);
        r = librg_entity_track(ctx, 15); EQUALS(r, LIBRG_OK);
        r = librg_entity_tracked(ctx, 15); EQUALS(r, LIBRG_TRUE);
        r = librg_entity_untrack(ctx, 15); EQUALS(r, LIBRG_OK);
        r = librg_entity_tracked(ctx, 15); EQUALS(r, LIBRG_FALSE);

        librg_context_destroy(ctx);
    });

    IT("should be able to properly set a userdata", {
        librg_ctx *ctx = librg_context_create();
        librg_entity_track(ctx, 15);

        r = librg_entity_userdata_set(ctx, 15, (void *)15); EQUALS(r, LIBRG_OK);
        r = (int)librg_entity_userdata_get(ctx, 15); EQUALS(r, 15);

        librg_entity_untrack(ctx, 15);
        librg_context_destroy(ctx);
    });

    IT("should be able to properly set a type", {
        librg_ctx *ctx = librg_context_create();
        librg_entity_track(ctx, 15);

        r = librg_entity_type_set(ctx, 15, LIBRG_ENTITY_DYNAMIC); EQUALS(r, LIBRG_OK);
        r = librg_entity_type_get(ctx, 15); EQUALS(r, LIBRG_ENTITY_DYNAMIC);

        librg_entity_untrack(ctx, 15);
        librg_context_destroy(ctx);
    });

    IT("should be able to properly set a chunk", {
        librg_ctx *ctx = librg_context_create();
        librg_entity_track(ctx, 15);

        r = librg_entity_chunk_set(ctx, 15, 15); EQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_get(ctx, 15); EQUALS(r, 15);

        librg_entity_untrack(ctx, 15);
        librg_context_destroy(ctx);
    });

    IT("should be able to properly set an owner", {
        librg_ctx *ctx = librg_context_create();
        librg_entity_track(ctx, 15);

        r = librg_entity_owner_set(ctx, 15, 15, 0); EQUALS(r, LIBRG_OK);
        r = librg_entity_owner_get(ctx, 15); EQUALS(r, 15);

        librg_entity_untrack(ctx, 15);
        librg_context_destroy(ctx);
    });

    IT("should be able to properly set a dimension", {
        librg_ctx *ctx = librg_context_create();
        librg_entity_track(ctx, 15);

        r = librg_entity_dimension_set(ctx, 15, 15); EQUALS(r, LIBRG_OK);
        r = librg_entity_dimension_get(ctx, 15); EQUALS(r, 15);

        librg_entity_untrack(ctx, 15);
        librg_context_destroy(ctx);
    });

    IT("should be able to properly set an array of chunks", {
        librg_ctx *ctx = librg_context_create();
        librg_entity_track(ctx, 15);

        librg_chunk arr[3]; arr[0] = 1; arr[1] = 2; arr[2] = 3;
        r = librg_entity_chunkarray_set(ctx, 15, arr, 3); EQUALS(r, LIBRG_OK);

        librg_chunk res[5];
        int amt = librg_entity_chunkarray_get(ctx, 15, res, 5);

        EQUALS(amt, 3);
        EQUALS(res[0], 1);
        EQUALS(res[1], 2);
        EQUALS(res[2], 3);

        librg_entity_untrack(ctx, 15);
        librg_context_destroy(ctx);
    });

    IT("should be able to properly set an array of chunks with overflow", {
        librg_ctx *ctx = librg_context_create();
        librg_entity_track(ctx, 15);

        librg_chunk arr[3]; arr[0] = 1; arr[1] = 2; arr[2] = 3;
        r = librg_entity_chunkarray_set(ctx, 15, arr, 3); EQUALS(r, LIBRG_OK);

        librg_chunk res[3] = {0};
        int amt = librg_entity_chunkarray_get(ctx, 15, res, 2);

        EQUALS(amt, 2);
        EQUALS(res[0], 1);
        EQUALS(res[1], 2);
        EQUALS(res[2], 0);

        librg_entity_untrack(ctx, 15);
        librg_context_destroy(ctx);
    });

    IT("should fail when attempting to set data onto untracked entity", {
        librg_ctx *ctx = librg_context_create();

        r = librg_entity_userdata_set(ctx, 15, (void *)15); NEQUALS(r, LIBRG_OK);
        r = librg_entity_type_set(ctx, 15, LIBRG_ENTITY_DYNAMIC); NEQUALS(r, LIBRG_OK);
        r = librg_entity_chunk_set(ctx, 15, 15); NEQUALS(r, LIBRG_OK);
        r = librg_entity_owner_set(ctx, 15, 15, 15); NEQUALS(r, LIBRG_OK);
        r = librg_entity_dimension_set(ctx, 15, 15); NEQUALS(r, LIBRG_OK);

        librg_context_destroy(ctx);
    });
});
