
/*

ZPL - Event module

License:
    This software is dual-licensed to the public domain and under the following
    license: you are granted a perpetual, irrevocable license to copy, modify,
    publish, and distribute this file as you see fit.

Warranty:
    No warranty is implied, use at your own risk.

Usage:
    #define ZPLEV_IMPLEMENTATION exactly in ONE source file right BEFORE including the library, like:

    #define ZPLEV_IMPLEMENTATION
    #include"zpl_event.h"

Dependencies:
    zpl.h

Credits:
    Dominik Madarasz (GitHub: zaklaus)

Version History:
    1.00 - Initial version

*/

#ifndef ZPL_INCLUDE_ZPL_EVENT_H
#define ZPL_INCLUDE_ZPL_EVENT_H

#ifdef __cplusplus
extern "C" {
#endif

    //////////////////////////////////////////////////////
    //
    // Event handler
    //
    // Uses hash table to store array of callbacks per
    // each valid event type.
    //
    // Each event callback receives an anonymous pointer
    // which has to be casted to proper base type.
    //
    // Usage:
    // - Initialize event pool.
    // - Declare your event callbacks and any data layout
    //   used by the events.
    // - Add event callbacks to the pool. (Returns callback ID.)
    // - Trigger arbitrary event in pool with specified dataset.
    // - (Optional) Remove arbitrary event callback
    //   by refering to it through event type and its callback ID.
    //

    typedef void *zplev_data_t;

#define ZPLEV(name) void name(zplev_data_t evt)
    typedef ZPLEV(zplev_cb);

#define ZPLEV_CAST(Type, name) Type * name = cast(Type *)evt

    typedef zpl_array_t(zplev_cb*) zplev_block;

    ZPL_TABLE_DECLARE(static, zplev_pool, zplev_pool_, zplev_block);

    ZPL_DEF void zplev_init   (zplev_pool *pool, zpl_allocator_t alloc);
    ZPL_DEF void zplev_destroy(zplev_pool *pool);
    ZPL_DEF u64  zplev_add    (zplev_pool *pool, u64 slot, zplev_cb cb);
    ZPL_DEF void zplev_remove (zplev_pool *pool, u64 slot, u64 index);
    ZPL_DEF void zplev_trigger(zplev_pool *pool, u64 slot, zplev_data_t evt);

#ifdef __cplusplus
}
#endif

#if defined(ZPLEV_IMPLEMENTATION) && !defined(ZPLEV_IMPLEMENTATION_DONE)
#define ZPLEV_IMPLEMENTATION_DONE

#ifdef __cplusplus
extern "C" {
#endif

    ZPL_TABLE_DEFINE(zplev_pool, zplev_pool_, zplev_block);

    zpl_inline void zplev_init   (zplev_pool *pool, zpl_allocator_t alloc) {
        zplev_pool_init(pool, alloc);
    }

    zpl_inline void zplev_destroy(zplev_pool *pool) {
        for (isize i = 0; i < zpl_array_count(pool->entries); ++i) {
            zplev_block *block = &pool->entries[i].value;

            if (block) {
                zpl_array_free(*block);
            }
        }

        zplev_pool_destroy(pool);
    }

    u64 zplev_add(zplev_pool *pool, u64 slot, zplev_cb cb) {
        zplev_block *block = zplev_pool_get(pool, slot);

        if (!block) {
            zplev_block arr;
            zpl_array_init(arr, zpl_heap_allocator());
            zplev_pool_set(pool, slot, arr);
            block = zplev_pool_get(pool, slot);
        }

        u64 offset = zpl_array_count(block);
        zpl_array_append(*block, cb);
        return offset;
    }

    void zplev_remove(zplev_pool *pool, u64 slot, u64 index) {
        zplev_block *block = zplev_pool_get(pool, slot);

        if (block) {
            zpl_array_remove_at(*block, index);
        }
    }

    void zplev_trigger(zplev_pool *pool, u64 slot, zplev_data_t evt) {
        zplev_block *block = zplev_pool_get(pool, slot);

        if (block) {
            for (isize i = 0; i < zpl_array_count(*block); ++i) {
                (*block)[i](evt);
            }
        }
    }


#ifdef __cplusplus
}
#endif

#endif
#endif //ZPL_INCLUDE_ZPL_EVENT_H
