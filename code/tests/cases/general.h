MODULE(general, {
    int8_t r = -1;

    IT("should be able to allocate and free an instance", {
        librg_world *world = librg_world_create();
        NEQUALS(world, NULL);
        r = librg_world_destroy(world);
        EQUALS(r, LIBRG_OK);
    });

    IT("should be return proper check for is valid", {
        librg_world *world = librg_world_create();
        EQUALS(librg_world_valid(world), 1);
        librg_world_destroy(world);
    });

    IT("should allow to set and retrieve a custom userdata", {
        librg_world *world = librg_world_create();

        int a[2] = {0};
        a[0] = 1;
        a[1] = 2;

        r = librg_world_userdata_set(world, &a); EQUALS(r, LIBRG_OK);
        int *b = librg_world_userdata_get(world);

        EQUALS(b[0], 1);
        EQUALS(b[1], 2);

        librg_world_destroy(world);
    });

    IT("should properly set a chunk world size (amount)", {
        librg_world *world = librg_world_create();

        r = librg_config_chunkamount_set(world, 12, 13, 14); EQUALS(r, LIBRG_OK);

        uint16_t x;
        uint16_t y;
        uint16_t z;

        r = librg_config_chunkamount_get(world, &x, &y, &z); EQUALS(r, LIBRG_OK);

        EQUALS(x, 12);
        EQUALS(y, 13);
        EQUALS(z, 14);

        librg_world_destroy(world);
    });

    IT("should force world size to have at least one chunk in each direction", {
        librg_world *world = librg_world_create();

        r = librg_config_chunkamount_set(world, 0, 0, 0); EQUALS(r, LIBRG_OK);

        uint16_t x;
        uint16_t y;
        uint16_t z;

        r = librg_config_chunkamount_get(world, &x, &y, &z); EQUALS(r, LIBRG_OK);

        EQUALS(x, 1);
        EQUALS(y, 1);
        EQUALS(z, 1);

        librg_world_destroy(world);
    });

    IT("should properly set a chunk size", {
        librg_world *world = librg_world_create();

        r = librg_config_chunksize_set(world, 12, 13, 14); EQUALS(r, LIBRG_OK);

        uint16_t x;
        uint16_t y;
        uint16_t z;

        r = librg_config_chunksize_get(world, &x, &y, &z); EQUALS(r, LIBRG_OK);

        EQUALS(x, 12);
        EQUALS(y, 13);
        EQUALS(z, 14);

        librg_world_destroy(world);
    });

    IT("should properly set a chunk offsets", {
        librg_world *world = librg_world_create();

        r = librg_config_chunkoffset_set(world, LIBRG_OFFSET_BEG, 14, LIBRG_OFFSET_MID);
        EQUALS(r, LIBRG_OK);

        int16_t x;
        int16_t y;
        int16_t z;

        r = librg_config_chunkoffset_get(world, &x, &y, &z); EQUALS(r, LIBRG_OK);

        EQUALS(x, LIBRG_OFFSET_BEG);
        EQUALS(y, 14);
        EQUALS(z, LIBRG_OFFSET_MID);

        librg_world_destroy(world);
    });

    IT("should properly handle set of chunk line calculations", {
        EQUALS(librg_util_chunkoffset_line(0, LIBRG_OFFSET_BEG, 4), 0);
        EQUALS(librg_util_chunkoffset_line(0, LIBRG_OFFSET_MID, 4), 2);
        EQUALS(librg_util_chunkoffset_line(0, LIBRG_OFFSET_END, 4), 4);

        EQUALS(librg_util_chunkoffset_line(2, LIBRG_OFFSET_BEG, 4), 2);
        EQUALS(librg_util_chunkoffset_line(2, LIBRG_OFFSET_MID, 4), 4);
        EQUALS(librg_util_chunkoffset_line(2, LIBRG_OFFSET_END, 4), 6);

        EQUALS(librg_util_chunkoffset_line(-2, LIBRG_OFFSET_BEG, 4), -2);
        EQUALS(librg_util_chunkoffset_line(-2, LIBRG_OFFSET_MID, 4), 0);
        EQUALS(librg_util_chunkoffset_line(-2, LIBRG_OFFSET_END, 4), 2);

        EQUALS(librg_util_chunkoffset_line(0, LIBRG_OFFSET_BEG, 5), 0);
        EQUALS(librg_util_chunkoffset_line(0, LIBRG_OFFSET_MID, 5), 2);
        EQUALS(librg_util_chunkoffset_line(0, LIBRG_OFFSET_END, 5), 5);

        EQUALS(librg_util_chunkoffset_line(2, LIBRG_OFFSET_BEG, 5), 2);
        EQUALS(librg_util_chunkoffset_line(2, LIBRG_OFFSET_MID, 5), 4);
        EQUALS(librg_util_chunkoffset_line(2, LIBRG_OFFSET_END, 5), 7);

        EQUALS(librg_util_chunkoffset_line(-2, LIBRG_OFFSET_BEG, 5), -2);
        EQUALS(librg_util_chunkoffset_line(-2, LIBRG_OFFSET_MID, 5), 0);
        EQUALS(librg_util_chunkoffset_line(-2, LIBRG_OFFSET_END, 5), 3);

    });

    IT("should correctly calculate weird world sizes", {
        librg_world *world = librg_world_create();
        r = librg_config_chunkamount_set(world, 1, 3, 1); EQUALS(r, LIBRG_OK);
        r = librg_config_chunkoffset_set(world, LIBRG_OFFSET_BEG, LIBRG_OFFSET_BEG, LIBRG_OFFSET_BEG); EQUALS(r, LIBRG_OK);

        librg_chunk id = LIBRG_CHUNK_INVALID;

        id = librg_chunk_from_chunkpos(world, 0, 0, 0); EQUALS(id, 0);
        id = librg_chunk_from_chunkpos(world, 0, 2, 0); EQUALS(id, 2);

        r = librg_config_chunkamount_set(world, 1, 1, 3); EQUALS(r, LIBRG_OK);

        id = librg_chunk_from_chunkpos(world, 0, 0, 0); EQUALS(id, 0);
        id = librg_chunk_from_chunkpos(world, 0, 0, 2); EQUALS(id, 2);

        librg_world_destroy(world);
    });

    IT("should correctly calculate chunk id for 2d top-left mode", {
        librg_world *world = librg_world_create();
        r = librg_config_chunkamount_set(world, 16, 16, 1); EQUALS(r, LIBRG_OK);
        r = librg_config_chunkoffset_set(world, LIBRG_OFFSET_BEG, LIBRG_OFFSET_BEG, 0); EQUALS(r, LIBRG_OK);

        librg_chunk id = LIBRG_CHUNK_INVALID;

        id = librg_chunk_from_chunkpos(world, 0, 0, 0); EQUALS(id, 0);
        id = librg_chunk_from_chunkpos(world, 1, 0, 0); EQUALS(id, 1);
        id = librg_chunk_from_chunkpos(world, 0, 1, 0); EQUALS(id, 16);
        id = librg_chunk_from_chunkpos(world, 15, 15, 0); EQUALS(id, 255);
        id = librg_chunk_from_chunkpos(world, 18, 18, 0); EQUALS(id, LIBRG_CHUNK_INVALID);

        r = librg_config_chunkamount_set(world, 3, 3, 0); EQUALS(r, LIBRG_OK);

        id = librg_chunk_from_chunkpos(world, 0, 0, 0); EQUALS(id, 0);
        id = librg_chunk_from_chunkpos(world, 1, 1, 0); EQUALS(id, 4);
        id = librg_chunk_from_chunkpos(world, -1, -1, 0); EQUALS(id, LIBRG_CHUNK_INVALID);
        id = librg_chunk_from_chunkpos(world, -1, -2, 0); EQUALS(id, LIBRG_CHUNK_INVALID);

        librg_world_destroy(world);
    });

    IT("should correctly calculate chunk id for 2d mid-mid (centered) mode", {
        librg_world *world = librg_world_create();
        r = librg_config_chunkamount_set(world, 3, 3, 0); EQUALS(r, LIBRG_OK);
        r = librg_config_chunkoffset_set(world, LIBRG_OFFSET_MID, LIBRG_OFFSET_MID, 0); EQUALS(r, LIBRG_OK);

        librg_chunk id = LIBRG_CHUNK_INVALID;

        id = librg_chunk_from_chunkpos(world, 0, 0, 0); EQUALS(id, 4);
        id = librg_chunk_from_chunkpos(world, 1, 1, 0); EQUALS(id, 8);
        id = librg_chunk_from_chunkpos(world, -1, -1, 0); EQUALS(id, 0);

        librg_world_destroy(world);
    });

    IT("should correctly calculate chunk id for 2d bottom-right mode", {
        librg_world *world = librg_world_create();
        r = librg_config_chunkamount_set(world, 3, 3, 0); EQUALS(r, LIBRG_OK);
        r = librg_config_chunkoffset_set(world, LIBRG_OFFSET_END, LIBRG_OFFSET_END, 0); EQUALS(r, LIBRG_OK);

        librg_chunk id = LIBRG_CHUNK_INVALID;

        id = librg_chunk_from_chunkpos(world, 0, 0, 0); EQUALS(id, LIBRG_CHUNK_INVALID);
        id = librg_chunk_from_chunkpos(world, 1, 1, 0); EQUALS(id, LIBRG_CHUNK_INVALID);
        id = librg_chunk_from_chunkpos(world, -1, 0, 0); EQUALS(id, LIBRG_CHUNK_INVALID);
        id = librg_chunk_from_chunkpos(world, -1, -1, 0); EQUALS(id, 8);
        id = librg_chunk_from_chunkpos(world, -2, -2, 0); EQUALS(id, 4);
        id = librg_chunk_from_chunkpos(world, -3, -3, 0); EQUALS(id, 0);
        id = librg_chunk_from_chunkpos(world, -3, -1, 0); EQUALS(id, 6);

        librg_world_destroy(world);
    });

    IT("should correctly calculate chunk id for 3d top-left mode", {
        librg_world *world = librg_world_create();
        r = librg_config_chunkamount_set(world, 3, 3, 3); EQUALS(r, LIBRG_OK);
        r = librg_config_chunkoffset_set(world, LIBRG_OFFSET_BEG, LIBRG_OFFSET_BEG, LIBRG_OFFSET_BEG); EQUALS(r, LIBRG_OK);

        librg_chunk id = LIBRG_CHUNK_INVALID;

        id = librg_chunk_from_chunkpos(world, 0, 0, 0); EQUALS(id, 0);
        id = librg_chunk_from_chunkpos(world, 2, 2, 2); EQUALS(id, 26);

        librg_world_destroy(world);
    });

    IT("should correctly calculate chunk id for 3d mid-mid (centered) mode", {
        librg_world *world = librg_world_create();
        r = librg_config_chunkamount_set(world, 3, 3, 3); EQUALS(r, LIBRG_OK);
        r = librg_config_chunkoffset_set(world, LIBRG_OFFSET_MID, LIBRG_OFFSET_MID, LIBRG_OFFSET_MID); EQUALS(r, LIBRG_OK);

        librg_chunk id = LIBRG_CHUNK_INVALID;

        id = librg_chunk_from_chunkpos(world, -1, -1, -1); EQUALS(id, 0);
        id = librg_chunk_from_chunkpos(world, -1, -1,  0); EQUALS(id, 9);
        id = librg_chunk_from_chunkpos(world, -1,  0,  0); EQUALS(id, 12);
        id = librg_chunk_from_chunkpos(world,  0,  0,  0); EQUALS(id, 13);
        id = librg_chunk_from_chunkpos(world,  1,  0,  0); EQUALS(id, 14);
        id = librg_chunk_from_chunkpos(world,  1,  1,  0); EQUALS(id, 17);
        id = librg_chunk_from_chunkpos(world,  1,  1,  1); EQUALS(id, 26);

        librg_world_destroy(world);
    });

    IT("should correctly calculate chunk id for 3d bottom-right mode", {
        librg_world *world = librg_world_create();
        r = librg_config_chunkamount_set(world, 3, 3, 3); EQUALS(r, LIBRG_OK);
        r = librg_config_chunkoffset_set(world, LIBRG_OFFSET_END, LIBRG_OFFSET_END, LIBRG_OFFSET_END); EQUALS(r, LIBRG_OK);

        librg_chunk id = LIBRG_CHUNK_INVALID;

        id = librg_chunk_from_chunkpos(world,  0,  0,  0); EQUALS(id, LIBRG_CHUNK_INVALID);
        id = librg_chunk_from_chunkpos(world,  -1, -1,  -1); EQUALS(id, 26);
        id = librg_chunk_from_chunkpos(world, -3, -3, -3); EQUALS(id, 0);

        librg_world_destroy(world);
    });

    IT("should calculate chunk id from a floating position in 2d top-left mode", {
        librg_world *world = librg_world_create();
        r = librg_config_chunksize_set(world, 16, 16, 0); EQUALS(r, LIBRG_OK);
        r = librg_config_chunkamount_set(world, 16, 16, 0); EQUALS(r, LIBRG_OK);
        r = librg_config_chunkoffset_set(world, LIBRG_OFFSET_BEG, LIBRG_OFFSET_BEG, LIBRG_OFFSET_BEG); EQUALS(r, LIBRG_OK);

        librg_chunk id = LIBRG_CHUNK_INVALID;

        id = librg_chunk_from_realpos(world,  0.f,  0.f, 0); EQUALS(id, 0);
        id = librg_chunk_from_realpos(world, 15.f, 15.f, 0); EQUALS(id, 0);
        id = librg_chunk_from_realpos(world, 16.f, 15.f, 0); EQUALS(id, 1);
        id = librg_chunk_from_realpos(world, 16.f, 17.f, 0); EQUALS(id, 17);

        librg_world_destroy(world);
    });

    IT("should calculate chunk id from a floating position in 3d top-left mode", {
        librg_world *world = librg_world_create();
        r = librg_config_chunksize_set(world, 16, 16, 16); EQUALS(r, LIBRG_OK);
        r = librg_config_chunkamount_set(world, 16, 16, 16); EQUALS(r, LIBRG_OK);
        r = librg_config_chunkoffset_set(world, LIBRG_OFFSET_BEG, LIBRG_OFFSET_BEG, LIBRG_OFFSET_BEG); EQUALS(r, LIBRG_OK);

        librg_chunk id = LIBRG_CHUNK_INVALID;

        id = librg_chunk_from_realpos(world,  0.f,  0.f,  0.f); EQUALS(id, 0);
        id = librg_chunk_from_realpos(world, 15.f, 15.f, 15.f); EQUALS(id, 0);
        id = librg_chunk_from_realpos(world, 16.f, 17.f, 15.f); EQUALS(id, 17);
        id = librg_chunk_from_realpos(world, 16.f, 17.f, 18.f); EQUALS(id, 273);

        librg_world_destroy(world);
    });

    IT("should calculate chunk coords from chunk id from weird world size", {
        librg_world *world = librg_world_create();
        r = librg_config_chunkamount_set(world, 1, 1, 3); EQUALS(r, LIBRG_OK);
        r = librg_config_chunkoffset_set(world, LIBRG_OFFSET_BEG, LIBRG_OFFSET_BEG, LIBRG_OFFSET_BEG); EQUALS(r, LIBRG_OK);

        int16_t x = 0;
        int16_t y = 0;
        int16_t z = 0;

        r = librg_chunk_to_chunkpos(world, 0, &x, &y, &z); EQUALS(r, LIBRG_OK); EQUALS(x, 0); EQUALS(y, 0); EQUALS(z, 0);
        r = librg_chunk_to_chunkpos(world, 2, &x, &y, &z); EQUALS(r, LIBRG_OK); EQUALS(x, 0); EQUALS(y, 0); EQUALS(z, 2);

        r = librg_config_chunkamount_set(world, 3, 3, 3); EQUALS(r, LIBRG_OK);
        r = librg_config_chunkoffset_set(world, LIBRG_OFFSET_BEG, LIBRG_OFFSET_BEG, LIBRG_OFFSET_BEG); EQUALS(r, LIBRG_OK);

        r = librg_chunk_to_chunkpos(world, 26, &x, &y, &z); EQUALS(r, LIBRG_OK); EQUALS(x, 2); EQUALS(y, 2); EQUALS(z, 2);
    });

    IT("should calculate chunk coords from chunk id from world size 3x3 and offsets", {
        librg_world *world = librg_world_create();
        r = librg_config_chunkoffset_set(world, LIBRG_OFFSET_MID, LIBRG_OFFSET_MID, LIBRG_OFFSET_MID); EQUALS(r, LIBRG_OK);

        int16_t x = 0;
        int16_t y = 0;
        int16_t z = 0;

        r = librg_config_chunkamount_set(world, 3, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_chunk_to_chunkpos(world, 2, &x, &y, &z); EQUALS(r, LIBRG_OK); EQUALS(x, 1); EQUALS(y, 0); EQUALS(z, 0);
        r = librg_chunk_to_chunkpos(world, 0, &x, &y, &z); EQUALS(r, LIBRG_OK); EQUALS(x, -1); EQUALS(y, 0); EQUALS(z, 0);

        r = librg_config_chunkamount_set(world, 1, 3, 1); EQUALS(r, LIBRG_OK);
        r = librg_chunk_to_chunkpos(world, 2, &x, &y, &z); EQUALS(r, LIBRG_OK); EQUALS(x, 0); EQUALS(y, 1); EQUALS(z, 0);
        r = librg_chunk_to_chunkpos(world, 0, &x, &y, &z); EQUALS(r, LIBRG_OK); EQUALS(x, 0); EQUALS(y, -1); EQUALS(z, 0);

        r = librg_config_chunkamount_set(world, 1, 1, 3); EQUALS(r, LIBRG_OK);
        r = librg_chunk_to_chunkpos(world, 2, &x, &y, &z); EQUALS(r, LIBRG_OK); EQUALS(x, 0); EQUALS(y, 0); EQUALS(z, 1);
        r = librg_chunk_to_chunkpos(world, 0, &x, &y, &z); EQUALS(r, LIBRG_OK); EQUALS(x, 0); EQUALS(y, 0); EQUALS(z, -1);

        r = librg_config_chunkamount_set(world, 3, 3, 1); EQUALS(r, LIBRG_OK);
        r = librg_chunk_to_chunkpos(world, 4, &x, &y, &z); EQUALS(r, LIBRG_OK); EQUALS(x, 0); EQUALS(y, 0); EQUALS(z, 0);
        r = librg_chunk_to_chunkpos(world, 0, &x, &y, &z); EQUALS(r, LIBRG_OK); EQUALS(x, -1); EQUALS(y, -1); EQUALS(z, 0);
        r = librg_chunk_to_chunkpos(world, 8, &x, &y, &z); EQUALS(r, LIBRG_OK); EQUALS(x, 1); EQUALS(y, 1); EQUALS(z, 0);

        r = librg_config_chunkamount_set(world, 1, 3, 3); EQUALS(r, LIBRG_OK);
        r = librg_chunk_to_chunkpos(world, 4, &x, &y, &z); EQUALS(r, LIBRG_OK); EQUALS(x, 0); EQUALS(y, 0); EQUALS(z, 0);
        r = librg_chunk_to_chunkpos(world, 0, &x, &y, &z); EQUALS(r, LIBRG_OK); EQUALS(x, 0); EQUALS(y, -1); EQUALS(z, -1);
        r = librg_chunk_to_chunkpos(world, 8, &x, &y, &z); EQUALS(r, LIBRG_OK); EQUALS(x, 0); EQUALS(y, 1); EQUALS(z, 1);

        r = librg_config_chunkamount_set(world, 3, 3, 3); EQUALS(r, LIBRG_OK);
        r = librg_chunk_to_chunkpos(world, 13, &x, &y, &z); EQUALS(r, LIBRG_OK); EQUALS(x, 0); EQUALS(y, 0); EQUALS(z, 0);
        r = librg_chunk_to_chunkpos(world, 0, &x, &y, &z); EQUALS(r, LIBRG_OK); EQUALS(x, -1); EQUALS(y, -1); EQUALS(z, -1);
        r = librg_chunk_to_chunkpos(world, 26, &x, &y, &z); EQUALS(r, LIBRG_OK); EQUALS(x, 1); EQUALS(y, 1); EQUALS(z, 1);
    });

    IT("should calculate chunk coords from chunk id from world size 4x4 and offsets", {
        librg_world *world = librg_world_create();
        r = librg_config_chunkoffset_set(world, LIBRG_OFFSET_MID, LIBRG_OFFSET_MID, LIBRG_OFFSET_MID); EQUALS(r, LIBRG_OK);

        int16_t x = 0;
        int16_t y = 0;
        int16_t z = 0;

        r = librg_config_chunkamount_set(world, 4, 1, 1); EQUALS(r, LIBRG_OK);
        r = librg_chunk_to_chunkpos(world, 2, &x, &y, &z); EQUALS(r, LIBRG_OK); EQUALS(x, 0); EQUALS(y, 0); EQUALS(z, 0);
        r = librg_chunk_to_chunkpos(world, 0, &x, &y, &z); EQUALS(r, LIBRG_OK); EQUALS(x, -2); EQUALS(y, 0); EQUALS(z, 0);

        r = librg_config_chunkamount_set(world, 1, 4, 1); EQUALS(r, LIBRG_OK);
        r = librg_chunk_to_chunkpos(world, 2, &x, &y, &z); EQUALS(r, LIBRG_OK); EQUALS(x, 0); EQUALS(y, 0); EQUALS(z, 0);
        r = librg_chunk_to_chunkpos(world, 0, &x, &y, &z); EQUALS(r, LIBRG_OK); EQUALS(x, 0); EQUALS(y, -2); EQUALS(z, 0);

        r = librg_config_chunkamount_set(world, 1, 1, 4); EQUALS(r, LIBRG_OK);
        r = librg_chunk_to_chunkpos(world, 2, &x, &y, &z); EQUALS(r, LIBRG_OK); EQUALS(x, 0); EQUALS(y, 0); EQUALS(z, 0);
        r = librg_chunk_to_chunkpos(world, 0, &x, &y, &z); EQUALS(r, LIBRG_OK); EQUALS(x, 0); EQUALS(y, 0); EQUALS(z, -2);

        r = librg_config_chunkamount_set(world, 4, 4, 1); EQUALS(r, LIBRG_OK);
        r = librg_chunk_to_chunkpos(world, 10, &x, &y, &z); EQUALS(r, LIBRG_OK); EQUALS(x, 0); EQUALS(y, 0); EQUALS(z, 0);
        r = librg_chunk_to_chunkpos(world, 0, &x, &y, &z); EQUALS(r, LIBRG_OK); EQUALS(x, -2); EQUALS(y, -2); EQUALS(z, 0);
        r = librg_chunk_to_chunkpos(world, 15, &x, &y, &z); EQUALS(r, LIBRG_OK); EQUALS(x, 1); EQUALS(y, 1); EQUALS(z, 0);

        r = librg_config_chunkamount_set(world, 1, 4, 4); EQUALS(r, LIBRG_OK);
        r = librg_chunk_to_chunkpos(world, 10, &x, &y, &z); EQUALS(r, LIBRG_OK); EQUALS(x, 0); EQUALS(y, 0); EQUALS(z, 0);
        r = librg_chunk_to_chunkpos(world, 0, &x, &y, &z); EQUALS(r, LIBRG_OK); EQUALS(x, 0); EQUALS(y, -2); EQUALS(z, -2);
        r = librg_chunk_to_chunkpos(world, 15, &x, &y, &z); EQUALS(r, LIBRG_OK); EQUALS(x, 0); EQUALS(y, 1); EQUALS(z, 1);

        r = librg_config_chunkamount_set(world, 4, 4, 4); EQUALS(r, LIBRG_OK);
        r = librg_chunk_to_chunkpos(world, 42, &x, &y, &z); EQUALS(r, LIBRG_OK); EQUALS(x, 0); EQUALS(y, 0); EQUALS(z, 0);
        r = librg_chunk_to_chunkpos(world, 0, &x, &y, &z); EQUALS(r, LIBRG_OK); EQUALS(x, -2); EQUALS(y, -2); EQUALS(z, -2);
        r = librg_chunk_to_chunkpos(world, 63, &x, &y, &z); EQUALS(r, LIBRG_OK); EQUALS(x, 1); EQUALS(y, 1); EQUALS(z, 1);
    });
});
