/*

ZPL - ES module

License:
    This software is dual-licensed to the public domain and under the following
    license: you are granted a perpetual, irrevocable license to copy, modify,
    publish, and distribute this file as you see fit.

Warranty:
    No warranty is implied, use at your own risk.

Usage:
    #define ZPLE_IMPLEMENTATION exactly in ONE source file right BEFORE including the library, like:

    #define ZPLE_IMPLEMENTATION
    #include"zpl_ent.h"

Optional Switches:
    ZPLE_USE_128_BITS -- uses 128-bit storage for entity identification handle.

Important note:
    - Component pool of type X can be used ONLY BY ONE entity pool.
    - Entity pool can use MULTIPLE component pools of type X.
    - Entity handles SHALL NOT BE mixed-up with DIFFERENT entity pools / component pools.
    - Entity's components HAVE TO BE detached BEFORE the entity gets destroyed.


Credits:
    Dominik Madarasz (GitHub: zaklaus)
    Vladislav Gritsenko (GitHub: inlife)

Version History:
    1.0.1 -- Switch fixes
    1.0.0 -- Initial version
*/

#ifndef ZPL_INCLUDE_ZPL_ENT_H
#define ZPL_INCLUDE_ZPL_ENT_H

#ifdef  ZPLE_USE_128_BITS
#define ZPLE_ID u64
#else
#define ZPLE_ID u32
#endif

#if defined(__cplusplus)
extern "C" {
#endif

    typedef struct zple_id_t {
        ZPLE_ID id;
        ZPLE_ID generation;
    } zple_id_t, zple_entity_t;

    typedef struct zple_node_t {
        zple_id_t           handle;
        struct zple_node_t *next;
    } zple_node_t;

    typedef struct zple_pool {
        zpl_allocator_t backing;
        usize           count;
        zpl_buffer_t(zple_node_t)freelist;
        zple_node_t             *first_free;
    } zple_pool;

    ZPL_DEF void            zple_init   (zple_pool *pool, zpl_allocator_t allocator, isize count);
    ZPL_DEF void            zple_free   (zple_pool *pool);
    ZPL_DEF zple_id_t    zple_create (zple_pool *pool);
    ZPL_DEF void            zple_destroy(zple_pool *pool, zple_id_t handle);

    // NOTE(ZaKlaus): To be used as:
    #if 0

    // NOTE(ZaKlaus): Header file preferably
    typedef struct {
        i32 x,y,z;
    } ZPLE_COMP_DECLARE(position);

    // NOTE(ZaKlaus): Exactly 1 occurence in source file
    ZPLE_COMP_DEFINE(position);

    #endif
#define ZPLE_COMP_DECLARE(NAME) \
    ZPL_JOIN2(NAME, _t); \
    typedef struct ZPL_JOIN2(NAME, _meta_ent_t) { \
        zple_id_t handle; \
        ZPLE_ID   used; \
    } ZPL_JOIN2(NAME, _meta_ent_t); \
    typedef struct ZPL_JOIN2(NAME, _pool) { \
        zpl_allocator_t backing; \
        usize count; \
        zpl_buffer_t(ZPL_JOIN2(NAME, _meta_ent_t)) entities; \
        zpl_buffer_t(ZPL_JOIN2(NAME, _t))         data; \
    } ZPL_JOIN2(NAME, _pool); \
    \
    \
    void                  ZPL_JOIN2(NAME,_init)       (ZPL_JOIN2(NAME, _pool) *h, zple_pool *p, zpl_allocator_t a); \
    void                  ZPL_JOIN2(NAME,_free)       (ZPL_JOIN2(NAME, _pool) *h); \
    ZPL_JOIN2(NAME, _t) * ZPL_JOIN2(NAME,_attach)     (ZPL_JOIN2(NAME, _pool) *h, zple_id_t handle, ZPL_JOIN2(NAME, _t) data); \
    void                  ZPL_JOIN2(NAME,_detach)     (ZPL_JOIN2(NAME, _pool) *h, zple_id_t handle); \
    ZPL_JOIN2(NAME, _t) * ZPL_JOIN2(NAME,_fetch)      (ZPL_JOIN2(NAME, _pool) *h, zple_id_t handle);

#define ZPLE_COMP_DEFINE(NAME) \
    void ZPL_JOIN2(NAME,_init) (ZPL_JOIN2(NAME, _pool) *h, zple_pool *p, zpl_allocator_t a) { \
        ZPL_ASSERT(h&&p); h->backing = a; \
        h->count = p->count; \
        zpl_buffer_init(h->entities, a, p->count); \
        zpl_buffer_init(h->data, a, p->count); \
    }\
    void ZPL_JOIN2(NAME,_free) (ZPL_JOIN2(NAME, _pool) *h) { \
        ZPL_ASSERT(h); \
        zpl_buffer_free(h->entities, h->backing); \
        zpl_buffer_free(h->data, h->backing); \
    } \
    ZPL_JOIN2(NAME, _t) * ZPL_JOIN2(NAME,_attach) (ZPL_JOIN2(NAME, _pool) *h, zple_id_t handle, ZPL_JOIN2(NAME, _t) data) { \
        ZPL_ASSERT(h); \
        ZPL_JOIN2(NAME, _meta_ent_t) *meta_ent = (h->entities+handle.id); \
        meta_ent->handle = handle; \
        meta_ent->used = true; \
        *(h->data+handle.id) = data; \
        return (h->data+handle.id); \
    } \
    void ZPL_JOIN2(NAME,_detach) (ZPL_JOIN2(NAME, _pool) *h, zple_id_t handle) { \
        ZPL_ASSERT(h); \
        ZPL_JOIN2(NAME, _meta_ent_t) *meta_ent = (h->entities+handle.id); \
        meta_ent->used = false; \
    } \
    ZPL_JOIN2(NAME, _t) * ZPL_JOIN2(NAME, _fetch) (ZPL_JOIN2(NAME, _pool) *h, zple_id_t handle) { \
        ZPL_ASSERT(h); \
        ZPL_JOIN2(NAME, _meta_ent_t) *meta_ent = (h->entities+handle.id); \
        if ((meta_ent->used) && (meta_ent->handle.id == handle.id) && (meta_ent->handle.generation == handle.generation)) { \
            return (h->data+handle.id); \
        } \
        else { \
            return NULL; \
        } \
    }

#define zple_foreach(pool, comp, cb) \
    for (usize i = 0; i < pool->count; ++i) { \
        ZPL_JOIN2(comp, _meta_ent_t) ent = pool->entities[i]; \
        if (ent.used) { \
            cb(ent.handle, ZPL_JOIN2(comp, _fetch)(pool, ent.handle)); \
        } \
    }


#if defined(__cplusplus)
}
#endif

#if defined(ZPLE_IMPLEMENTATION) && !defined(ZPLE_IMPLEMENTATION_DONE)

#if defined(__cplusplus)
extern "C" {
#endif

    void zple_init(zple_pool *pool, zpl_allocator_t allocator, isize count) {
        ZPL_ASSERT(pool);

        zpl_allocator_t a = pool->backing = allocator;
        pool->count = count;

        zpl_buffer_init(pool->freelist, a, zpl_size_of(zple_node_t)*count);

        // NOTE(ZaKlaus): Build the freelist
        pool->first_free = pool->freelist;
        for (usize i = 0; i < count; ++i) {
            zple_node_t *f = (pool->freelist+i);
            f->handle = (zple_id_t) { i, 0 };
            f->next = (pool->freelist+i+1);
        }
        (pool->freelist+count-1)->next = NULL;
    }

    void zple_free(zple_pool *pool) {
        ZPL_ASSERT(pool);

        zpl_buffer_free(pool->freelist, pool->backing);
    }

    zple_id_t zple_create(zple_pool *pool) {
        ZPL_ASSERT(pool);
        ZPL_ASSERT(pool->first_free);

        zple_node_t *f = pool->first_free;
        zple_id_t ent = f->handle;
        pool->first_free = f->next;

        return ent;
    }

    void zple_destroy(zple_pool *pool, zple_id_t handle) {
        ZPL_ASSERT(pool);

        if (pool->first_free == NULL) {
            zple_node_t *f = pool->first_free = pool->freelist;
            f->handle = handle;
            f->next   = NULL;
            ++f->handle.generation;
        }
        else {
            ZPL_ASSERT(pool->first_free - 1 >= pool->freelist);
            zple_node_t f_ = { (zple_id_t){ handle.id, handle.generation+1 }, pool->first_free };
            zple_node_t *f = (pool->first_free - 1);
            *f = f_;
            pool->first_free = f;
        }
    }

#if defined(__cplusplus)
}
#endif

#endif

#endif // ZPL_INCLUDE_ZPL_ENT_H
