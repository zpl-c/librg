#ifndef LIBRG_INCLUDE_H
#define LIBRG_INCLUDE_H

#define LIBRG_VERSION_MAJOR 2
#define LIBRG_VERSION_MINOR 0
#define LIBRG_VERSION_PATCH 0
#define LIBRG_VERSION_CREATE(major, minor, patch) (((major)<<16) | ((minor)<<8) | (patch))
#define LIBRG_VERSION_GET_MAJOR(version) (((version)>>16)&0xFF)
#define LIBRG_VERSION_GET_MINOR(version) (((version)>>8)&0xFF)
#define LIBRG_VERSION_GET_PATCH(version) ((version)&0xFF)
#define LIBRG_VERSION LIBRG_VERSION_CREATE(LIBRG_VERSION_MAJOR, LIBRG_VERSION_MINOR, LIBRG_VERSION_PATCH)

#ifdef LIBRG_IMPLEMENTATION
    #define ZPL_IMPLEMENTATION
    #define ZPLE_IMPLEMENTATION
    #define ZPLM_IMPLEMENTATION
    #define ZPL_EVENT_IMPLEMENTATION
#endif

#include <zpl.h>
#include <zpl_ent.h>
#include <zpl_math.h>
#include <zpl_event.h>
#include <enet/enet.h>

#ifdef __cplusplus
extern "C" {
#endif

    #define LIBRG_API ZPL_DEF
    #define librg_log zpl_printf

    #ifndef LIBRG_PLATFORM
    #define LIBRG_PLATFORM 1
    #endif

    #ifndef LIBRG_PROTOCOL
    #define LIBRG_PROTOCOL 1
    #endif

    #ifndef LIBRG_BUILD
    #define LIBRG_BUILD 1
    #endif


    /**
     * CORE
     */

    typedef struct librg_cfg_t {
        // core
        u16 tick_delay;

        // streamer configuration
        zplm_vec3_t world_size;

        // network configuration
        u16 port;
        u16 max_connections;
        // zpl_string_t password;

        // backend network (outdated ?)
        u8 platform_id;
        u8 proto_version;
        u8 build_version;

    } librg_cfg_t;

    typedef enum librg_mode_e {

        /* non-blocking server with run-once loop, manual librg_tick() call required */
        librg_mode_server_ev,

        /* non-blocking client with run-once loop, manual librg_tick() call required */
        librg_mode_client_ev,

    } librg_mode_e;

    /**
     * Main initialization method
     * MUST BE called in the begging of your application
     */
    LIBRG_API void  librg_init(librg_mode_e mode, librg_cfg_t config);

    /**
     * Main tick method
     * MUST BE called in your loop
     * preferably w/o delays
     */
    LIBRG_API void  librg_tick();

    /**
     * Should be called at the end of
     * execution of the program
     */
    LIBRG_API void  librg_free();

    /**
     * Old method, compatibility with
     * librg automatic mode, runs blocking while loop
     * can be used to run on a server side, for simple testing
     * @deprecated
     */
    LIBRG_API void  librg_run_blocking();

    /**
     * Is librg instance is running
     * in the server mode
     */
    LIBRG_API b32   librg_is_server();

    /**
     * Is librg instance is running
     * in the client mode
     */
    LIBRG_API b32   librg_is_client();


    /**
     * ENTITIES
     */

    /**
     * Allocator that will be used for new Entity System
     */
    #define LIBRG_ENTITY_ALLOCATOR zpl_heap_allocator

    typedef zple_id_t librg_entity_t;

    LIBRG_API librg_entity_t librg_create();
    LIBRG_API void           librg_destroy(librg_entity_t entity);

    ZPL_EXTERN zple_pool librg__entity_pool;

    #define librg_component_declare(NAME) \
        ZPL_JOIN2(NAME, _t); \
        \
        typedef struct ZPL_JOIN2(NAME, _meta_ent_t) { \
            zple_id_t handle; \
            ZPLE_ID   used; \
        } ZPL_JOIN2(NAME, _meta_ent_t); \
        \
        typedef struct ZPL_JOIN3(librg__component_, NAME, _pool_t) { \
            zpl_allocator_t backing; \
            usize count; \
            i8 initialized; \
            zpl_buffer_t(ZPL_JOIN2(NAME, _meta_ent_t)) entities; \
            zpl_buffer_t(ZPL_JOIN2(NAME, _t))         data; \
        } ZPL_JOIN3(librg__component_, NAME, _pool_t); \
        \
        \
        void                  ZPL_JOIN2(librg_init_, NAME)       (ZPL_JOIN3(librg__component_, NAME, _pool_t) *h, zple_pool *p, zpl_allocator_t a); \
        void                  ZPL_JOIN2(librg_free_, NAME)       (ZPL_JOIN3(librg__component_, NAME, _pool_t) *h); \
        ZPL_JOIN2(NAME, _t) * ZPL_JOIN2(librg_attach_, NAME)     (zple_id_t handle, ZPL_JOIN2(NAME, _t) data); \
        void                  ZPL_JOIN2(librg_detach_, NAME)     (zple_id_t handle); \
        ZPL_JOIN2(NAME, _t) * ZPL_JOIN2(librg_fetch_, NAME)      (zple_id_t handle);

    #define librg_component(NAME, CODE) \
        librg_component_declare(NAME, CODE)

    /**
     * NETWORK
     */

    LIBRG_API void librg_connect(char *host, int port);
    LIBRG_API void librg_disconnect();
    LIBRG_API b32  librg_is_connected();

    typedef void *librg_peer_t;
    typedef void *librg_net_cb;

    LIBRG_API void librg_send_all               (u64 id, librg_net_cb callback);
    LIBRG_API void librg_send_instream          (u64 id, librg_net_cb callback);
    LIBRG_API void librg_send_to                (u64 id, librg_entity_t entity, librg_net_cb callback);
    LIBRG_API void librg_send_except            (u64 id, librg_entity_t entity, librg_net_cb callback);
    LIBRG_API void librg_send_instream_except   (u64 id, librg_entity_t entity, librg_net_cb callback);

    #define librg_send librg_send_all

    LIBRG_API u64   librg_net_add       (u64 id, librg_net_cb callback);
    LIBRG_API void  librg_net_remove    (u64 id, u64 index);


    /**
     * EVENTS
     */

    typedef zplev_data_t librg_evt_t;
    typedef zplev_cb     librg_evt_cb;

    LIBRG_API u64   librg_evt_add       (u64 id, librg_evt_cb callback);
    LIBRG_API void  librg_evt_trigger   (u64 id, librg_evt_t event);
    LIBRG_API void  librg_evt_remove    (u64 id, u64 index);


    /**
     * STREAMER
     */

    LIBRG_API void librg_streamer_remove          (librg_entity_t entity);
    LIBRG_API void librg_streamer_set_visible     (librg_entity_t entity, b32 state);
    LIBRG_API void librg_streamer_set_visible_for (librg_entity_t entity, librg_entity_t target, b32 state);
    LIBRG_API void librg_streamer_client_set      (librg_entity_t entity, librg_peer_t peer);
    LIBRG_API void librg_streamer_client_remove   (librg_entity_t entity);

#ifdef __cplusplus
}
#endif

#if defined(LIBRG_IMPLEMENTATION) && !defined(LIBRG_IMPLEMENTATION_DONE)
#define LIBRG_IMPLEMENTATION_DONE

#ifdef __cplusplus
extern "C" {
#endif


    /**
     * ENTITIES
     */

    zple_pool librg__entity_pool;

    zple_id_t librg_create() {
        return zple_create(&librg__entity_pool);
    }

    void librg_destroy(librg_entity_t entity) {
        return zple_destroy(&librg__entity_pool, entity);
    }


    #define librg_component_define(NAME) \
        ZPL_JOIN3(librg__component_, NAME, _pool_t) ZPL_JOIN3(librg__component_, NAME, _pool); \
        \
        void ZPL_JOIN2(librg_init_, NAME) (ZPL_JOIN3(librg__component_, NAME, _pool_t) *h, zple_pool *p, zpl_allocator_t a) { \
            ZPL_ASSERT(h&&p); h->backing = a; \
            h->count = p->count; \
            h->initialized = 1; \
            zpl_buffer_init(h->entities, a, p->count); \
            zpl_buffer_init(h->data, a, p->count); \
        }\
        void ZPL_JOIN2(librg_free_, NAME) (ZPL_JOIN3(librg__component_, NAME, _pool_t) *h) { \
            ZPL_ASSERT(h); \
            zpl_buffer_free(h->entities, h->backing); \
            zpl_buffer_free(h->data, h->backing); \
        } \
        ZPL_JOIN2(NAME, _t) * ZPL_JOIN2(librg_attach_, NAME) (zple_id_t handle, ZPL_JOIN2(NAME, _t) data) { \
            if (!ZPL_JOIN3(librg__component_, NAME, _pool).initialized) { \
                ZPL_JOIN2(librg_init_, NAME)(&ZPL_JOIN3(librg__component_, NAME, _pool), &librg__entity_pool, LIBRG_ENTITY_ALLOCATOR()); \
            } \
            ZPL_JOIN2(NAME, _meta_ent_t) *meta_ent = (ZPL_JOIN3(librg__component_, NAME, _pool).entities+handle.id); \
            meta_ent->handle = handle; \
            meta_ent->used = true; \
            *(ZPL_JOIN3(librg__component_, NAME, _pool).data+handle.id) = data; \
            return (ZPL_JOIN3(librg__component_, NAME, _pool).data+handle.id); \
        } \
        void ZPL_JOIN2(librg_detach_, NAME) (zple_id_t handle) { \
            if (!ZPL_JOIN3(librg__component_, NAME, _pool).initialized) { \
                ZPL_JOIN2(librg_init_, NAME)(&ZPL_JOIN3(librg__component_, NAME, _pool), &librg__entity_pool, LIBRG_ENTITY_ALLOCATOR()); \
            } \
            ZPL_JOIN2(NAME, _meta_ent_t) *meta_ent = (ZPL_JOIN3(librg__component_, NAME, _pool).entities+handle.id); \
            meta_ent->used = false; \
        } \
        ZPL_JOIN2(NAME, _t) * ZPL_JOIN2(librg_fetch_, NAME) (zple_id_t handle) { \
            if (!ZPL_JOIN3(librg__component_, NAME, _pool).initialized) { \
                ZPL_JOIN2(librg_init_, NAME)(&ZPL_JOIN3(librg__component_, NAME, _pool), &librg__entity_pool, LIBRG_ENTITY_ALLOCATOR()); \
            } \
            ZPL_JOIN2(NAME, _meta_ent_t) *meta_ent = (ZPL_JOIN3(librg__component_, NAME, _pool).entities+handle.id); \
            if ((meta_ent->used) && (meta_ent->handle.id == handle.id) && (meta_ent->handle.generation == handle.generation)) { \
                return (ZPL_JOIN3(librg__component_, NAME, _pool).data+handle.id); \
            } \
            else { \
                return NULL; \
            } \
        }

    #define librg_foreach(NAME, cb) \
        for (usize i = 0; i < ZPL_JOIN3(librg__component_, NAME, _pool).count; ++i) { \
            ZPL_JOIN2(NAME, _meta_ent_t) ent = ZPL_JOIN3(librg__component_, NAME, _pool).entities[i]; \
            if (ent.used) { \
                cb(ent.handle, ZPL_JOIN2(librg_fetch_, NAME)(ent.handle)); \
            } \
        }



    #undef librg_component
    #define librg_component(NAME) \
        librg_component_declare(NAME) \
        librg_component_define(NAME)


#ifdef __cplusplus
}
#endif

#endif // LIBRG_IMPLEMENTATION
#endif // LIBRG_INCLUDE_H
