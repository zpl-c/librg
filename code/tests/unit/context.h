MODULE(context, {
    int8_t r = LIBRG_FAIL;

    IT("should be able to allocate and free an instance", {
        librg_ctx *ctx = librg_context_create();

        NEQUALS(ctx, NULL);

        r = librg_context_destroy(ctx);
        EQUALS(r, LIBRG_OK);
    });

    IT("should be return proper check for is valid", {
        librg_ctx *ctx = librg_context_create();
        EQUALS(librg_context_valid(ctx), 1);
        librg_context_destroy(ctx);
    });

    IT("should allow to set and retrieve a custom userdata", {
        librg_ctx *ctx = librg_context_create();

        int a[2] = {0};
        a[0] = 1;
        a[1] = 2;

        r = librg_context_userdata_set(ctx, &a); EQUALS(r, LIBRG_OK);
        int *b = librg_context_userdata_get(ctx);

        EQUALS(b[0], 1);
        EQUALS(b[1], 2);

        librg_context_destroy(ctx);
    });

    IT("should properly set a world size", {
        librg_ctx *ctx = librg_context_create();

        r = librg_config_worldsize_set(ctx, 12, 13, 14); EQUALS(r, LIBRG_OK);

        uint16_t x;
        uint16_t y;
        uint16_t z;

        r = librg_config_worldsize_get(ctx, &x, &y, &z); EQUALS(r, LIBRG_OK);

        EQUALS(x, 12);
        EQUALS(y, 13);
        EQUALS(z, 14);

        librg_context_destroy(ctx);
    });

    IT("should force world size to have at least one chunk in each direction", {
        librg_ctx *ctx = librg_context_create();

        r = librg_config_worldsize_set(ctx, 0, 0, 0); EQUALS(r, LIBRG_OK);

        uint16_t x;
        uint16_t y;
        uint16_t z;

        r = librg_config_worldsize_get(ctx, &x, &y, &z); EQUALS(r, LIBRG_OK);

        EQUALS(x, 1);
        EQUALS(y, 1);
        EQUALS(z, 1);

        librg_context_destroy(ctx);
    });

    IT("should properly set a chunk size", {
        librg_ctx *ctx = librg_context_create();

        r = librg_config_chunksize_set(ctx, 12, 13, 14); EQUALS(r, LIBRG_OK);

        uint16_t x;
        uint16_t y;
        uint16_t z;

        r = librg_config_chunksize_get(ctx, &x, &y, &z); EQUALS(r, LIBRG_OK);

        EQUALS(x, 12);
        EQUALS(y, 13);
        EQUALS(z, 14);

        librg_context_destroy(ctx);
    });

    IT("should properly set a chunk offsets", {
        librg_ctx *ctx = librg_context_create();

        r = librg_config_chunkoffset_set(ctx, LIBRG_OFFSET_BEG, 14, LIBRG_OFFSET_MID);
        EQUALS(r, LIBRG_OK);

        int16_t x;
        int16_t y;
        int16_t z;

        r = librg_config_chunkoffset_get(ctx, &x, &y, &z); EQUALS(r, LIBRG_OK);

        EQUALS(x, LIBRG_OFFSET_BEG);
        EQUALS(y, 14);
        EQUALS(z, LIBRG_OFFSET_MID);

        librg_context_destroy(ctx);
    });

    IT("should correctly calculate chunk id for 2d top-left mode", {
        librg_ctx *ctx = librg_context_create();
        r = librg_config_worldsize_set(ctx, 16, 16, 1); EQUALS(r, LIBRG_OK);
        r = librg_config_chunkoffset_set(ctx, LIBRG_OFFSET_BEG, LIBRG_OFFSET_BEG, 0); EQUALS(r, LIBRG_OK);

        librg_chunk id = LIBRG_CHUNK_INVALID;

        id = librg_chunk_from_chunkpos(ctx, 0, 0, 0); EQUALS(id, 0);
        id = librg_chunk_from_chunkpos(ctx, 1, 0, 0); EQUALS(id, 1);
        id = librg_chunk_from_chunkpos(ctx, 0, 1, 0); EQUALS(id, 16);
        id = librg_chunk_from_chunkpos(ctx, 15, 15, 0); EQUALS(id, 255);
        id = librg_chunk_from_chunkpos(ctx, 18, 18, 0); EQUALS(id, LIBRG_CHUNK_INVALID);

        librg_context_destroy(ctx);
    });

    IT("should correctly calculate chunk id for 2d mid-mid (centered) mode", {
        librg_ctx *ctx = librg_context_create();
        r = librg_config_worldsize_set(ctx, 3, 3, 0); EQUALS(r, LIBRG_OK);
        r = librg_config_chunkoffset_set(ctx, LIBRG_OFFSET_MID, LIBRG_OFFSET_MID, 0); EQUALS(r, LIBRG_OK);

        librg_chunk id = LIBRG_CHUNK_INVALID;

        id = librg_chunk_from_chunkpos(ctx, 0, 0, 0); EQUALS(id, 4);
        id = librg_chunk_from_chunkpos(ctx, 1, 1, 0); EQUALS(id, 8);
        id = librg_chunk_from_chunkpos(ctx, -1, -1, 0); EQUALS(id, 0);

        librg_context_destroy(ctx);
    });

    IT("should correctly calculate chunk id for 3d top-left mode", {
        librg_ctx *ctx = librg_context_create();
        r = librg_config_worldsize_set(ctx, 3, 3, 3); EQUALS(r, LIBRG_OK);
        r = librg_config_chunkoffset_set(ctx, LIBRG_OFFSET_BEG, LIBRG_OFFSET_BEG, LIBRG_OFFSET_BEG); EQUALS(r, LIBRG_OK);

        librg_chunk id = LIBRG_CHUNK_INVALID;

        id = librg_chunk_from_chunkpos(ctx, 0, 0, 0); EQUALS(id, 0);
        id = librg_chunk_from_chunkpos(ctx, 2, 2, 2); EQUALS(id, 26);

        librg_context_destroy(ctx);
    });

    IT("should correctly calculate chunk id for 3d mid-mid (centered) mode", {
        librg_ctx *ctx = librg_context_create();
        r = librg_config_worldsize_set(ctx, 3, 3, 3); EQUALS(r, LIBRG_OK);
        r = librg_config_chunkoffset_set(ctx, LIBRG_OFFSET_MID, LIBRG_OFFSET_MID, LIBRG_OFFSET_MID); EQUALS(r, LIBRG_OK);

        librg_chunk id = LIBRG_CHUNK_INVALID;

        id = librg_chunk_from_chunkpos(ctx, -1, -1, -1); EQUALS(id, 0);
        id = librg_chunk_from_chunkpos(ctx, -1, -1,  0); EQUALS(id, 9);
        id = librg_chunk_from_chunkpos(ctx, -1,  0,  0); EQUALS(id, 12);
        id = librg_chunk_from_chunkpos(ctx,  0,  0,  0); EQUALS(id, 13);
        id = librg_chunk_from_chunkpos(ctx,  1,  0,  0); EQUALS(id, 14);
        id = librg_chunk_from_chunkpos(ctx,  1,  1,  0); EQUALS(id, 17);
        id = librg_chunk_from_chunkpos(ctx,  1,  1,  1); EQUALS(id, 26);

        librg_context_destroy(ctx);
    });

    IT("should correctly calculate chunk id for 3d bottom-right mode", {
        librg_ctx *ctx = librg_context_create();
        r = librg_config_worldsize_set(ctx, 3, 3, 3); EQUALS(r, LIBRG_OK);
        r = librg_config_chunkoffset_set(ctx, LIBRG_OFFSET_END, LIBRG_OFFSET_END, LIBRG_OFFSET_END); EQUALS(r, LIBRG_OK);

        librg_chunk id = LIBRG_CHUNK_INVALID;

        id = librg_chunk_from_chunkpos(ctx,  0,  0,  0); EQUALS(id, 26);
        id = librg_chunk_from_chunkpos(ctx, -2, -2, -2); EQUALS(id, 0);

        librg_context_destroy(ctx);
    });

    IT("should calculate chunk id from a floating position in 3d top-left mode", {
        librg_ctx *ctx = librg_context_create();
        r = librg_config_chunksize_set(ctx, 16, 16, 16); EQUALS(r, LIBRG_OK);
        r = librg_config_worldsize_set(ctx, 16, 16, 16); EQUALS(r, LIBRG_OK);
        r = librg_config_chunkoffset_set(ctx, LIBRG_OFFSET_BEG, LIBRG_OFFSET_BEG, LIBRG_OFFSET_BEG); EQUALS(r, LIBRG_OK);

        librg_chunk id = LIBRG_CHUNK_INVALID;

        id = librg_chunk_from_realpos(ctx,  0.f,  0.f,  0.f); EQUALS(id, 0);
        id = librg_chunk_from_realpos(ctx, 15.f, 15.f, 15.f); EQUALS(id, 0);
        id = librg_chunk_from_realpos(ctx, 16.f, 17.f, 15.f); EQUALS(id, 17);
        id = librg_chunk_from_realpos(ctx, 16.f, 17.f, 18.f); EQUALS(id, 273);

        librg_context_destroy(ctx);
    });
});
