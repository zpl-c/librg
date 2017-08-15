/**
 * LIBRG - reguider library
 *
 * A library for building simple and elegant cross-platform mmo client-server solutions.
 *
 * Usage:
 * #define LIBRG_IMPLEMENTATION exactly in ONE source file right BEFORE including the library, like:
 *
 * #define LIBRG_IMPLEMENTATION
 * #include <librg.h>
 *
 * Credits:
 * Vladislav Gritsenko (GitHub: inlife)
 * Dominik Madarasz (GitHub: zaklaus)
 *
 * Dependencies:
 * zpl.h
 * zpl_ent.h
 * zpl_math.h
 * zpl_event.h
 * enet.h
 *
 * Version History:
 * 2.0.0 - Initial C version rewrite
 *
 * Copyright 2017 Vladislav Gritsenko
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
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
#define ZPLEV_IMPLEMENTATION
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

    #ifndef LIBRG_PLATFORM_ID
    #define LIBRG_PLATFORM_ID 1
    #endif

    #ifndef LIBRG_PLATFORM_PROTOCOL
    #define LIBRG_PLATFORM_PROTOCOL 1
    #endif

    #ifndef LIBRG_PLATFORM_BUILD
    #define LIBRG_PLATFORM_BUILD 1
    #endif

    #define librg_log zpl_printf

    #ifdef LIBRG_DEBUG
    #define librg_dbg(fmt, ...) zpl_printf(fmt, ##__VA_ARGS__)
    #else
    #define librg_dbg(fmt, ...)
    #endif

    #ifndef LIBRG_DEFAULT_BS_SIZE
    #define LIBRG_DEFAULT_BS_SIZE 1024
    #endif


    /**
     * CORE
     */

    typedef enum librg_mode_e {

        /* non-blocking server with run-once loop, manual librg_tick() call required */
        librg_server_ev,

        /* non-blocking client with run-once loop, manual librg_tick() call required */
        librg_client_ev,

    } librg_mode_e;

    typedef struct librg_cfg_t {
        // core
        u16 tick_delay;

        // streamer configuration
        zplm_vec2_t world_size;

        u32 entity_limit;

        // network configuration
        u16 port;
        u16 max_connections;
        librg_mode_e mode;
        // zpl_string_t password;

        // backend network (outdated ?)
        u8 platform_id;
        u8 proto_version;
        u8 build_version;

    } librg_cfg_t;

    /**
     * Main initialization method
     * MUST BE called in the begging of your application
     */
    LIBRG_API void librg_init(librg_cfg_t config);

    /**
     * Main tick method
     * MUST BE called in your loop
     * preferably w/o delays
     */
    LIBRG_API void librg_tick();

    /**
     * Should be called at the end of
     * execution of the program
     */
    LIBRG_API void librg_free();

    /**
     * @deprecated
     * Old method, compatibility with
     * librg-cpp automatic mode, runs blocking while loop
     * can be used to run on a server side, for simple testing
     */
    LIBRG_API void librg_run_blocking();

    /**
     * Is librg instance is running
     * in the server mode
     */
    LIBRG_API b32 librg_is_server();

    /**
     * Is librg instance is running
     * in the client mode
     */
    LIBRG_API b32 librg_is_client();




    /**
     * ENTITIES
     */

    /**
     * Allocator that will be used for new Entity System
     */
    #define LIBRG_ENTITY_ALLOCATOR zpl_heap_allocator

    // todo: make custom entity handle
    // to store shared network ids
    typedef zple_id_t librg_entity_t;

    /**
     * Entity filter
     * Used for ruinning complex iterations on entity pool
     *
     * Can be used to retrieve only entites containing all
     * of the provided components (logical AND operation).
     * And entities that exclude provided components (logical NOT).
     *
     * Supports up to 8 components for "contains" operation.
     * And up to 4 components for "excludes" operation.
     *
     * First undefined component index in the list will skip all other
     * components for that operation:
     *     { .contains1 = librg_index_c1(), .contains3 = librg_index_c3() }
     *     In this case librg_index_c3() WILL NOT be added to condition.
     *
     * If you want to enable this behavior: make sure
     * you define LIBRG_ENTITY_UNOPTIMIZED_CYCLES before including the librg.h
     *
     *
     * EXAMPLES:
     *     librg_entity_filter_t filter = { librg_index_c1(), librg_index_c2() };
     * OR
     *     librg_entity_filter_t filter = { librg_index_c1(), .excludes1 = librg_index_c2() };
     */
    typedef union librg_entity_filter_t {
        struct {
            u32 contains1; u32 contains2; u32 contains3; u32 contains4;
            u32 contains5; u32 contains6; u32 contains7; u32 contains8;
            u32 excludes1; u32 excludes2; u32 excludes3; u32 excludes4;
        };

        struct {
            u32 contains[8];
            u32 excludes[4];
        };
    } librg_entity_filter_t;


    /**
     * Callback that will be used to pass
     * the entity inside the function-handler
     */
    #define LIBRG_ENTITY_CB(name) void name(librg_entity_t entity)
        typedef LIBRG_ENTITY_CB(librg_entity_cb_t);
    #undef  LIBRG_ENTITY_CB

    /**
     * If you dont want to skip filter rules if previous one is empty
     * make sure you #define LIBRG_ENTITY_UNOPTIMIZED_CYCLES
     * before including the librg.h
     */
    #if defined(LIBRG_ENTITY_UNOPTIMIZED_CYCLES)
        #define LIBRG__ENTITY_EACH_BREAK continue
    #else
        #define LIBRG__ENTITY_EACH_BREAK break
    #endif

    typedef union librg__entcache_key_t {
        u64 master;
        struct {
            u32 slot;
            b32 is_local;
        };
    } librg__entcache_key_t;

    ZPL_TABLE_DECLARE(extern, librg__entcache_t, librg__entcache_, librg_entity_t);

    /**
     * Create entity and return handle
     */
    LIBRG_API librg_entity_t librg_entity_create();

    /**
     * Create shared entity and return handle
     */
    LIBRG_API librg_entity_t librg_entity_create_shared(u32 guid);

    /**
     * Auto detach all attached components
     * and destroy entity
     */
    LIBRG_API void librg_entity_destroy(librg_entity_t entity);

    /**
     * Method used for interation on the collection of entities
     * filtered by provided conditions, and calls callback for each found entity
     *
     * Filter condition is formbed by naming component ids that are need to be included
     * or excluded, depending on the number or key of element in the structure
     *
     * NOTE: Component id order IS RELEVANT, if you want to have as much performance as possible,
     * make sure you put component with lowest amount of attached entities in the first slot
     *
     * @param filter
     * @param callback
     */
    LIBRG_API void librg_entity_each(librg_entity_filter_t filter, librg_entity_cb_t callback);

    /**
     * Macro that declares component structs and methods
     * SHOULD BE called IN ALL c/cpp files that will be using it
     * with EXACTLY THE SAME sturcture and description as in other c/cpp files
     *
     * Describes these public API methods:
     *
     * 1) attach
     *     Used to attach a component onto entity.
     *     Returns pointer onto attached component.
     *     Contains lazy-initialization call for itself.
     *
     * EXAMPLE: librg_attach_{component}(ent, somedata);
     *
     *
     * 2) fetch
     *     Used to fetch attached component from the entity.
     *     If entiy doesnt have a component, NULL will be returned.
     *     Contains lazy-initialization call for itself.
     *
     * EXAMPLE: {component}_t *foo = librg_fetch_{component}(ent);
     *
     *
     * 3) detach
     *     Used to remove attached component form the entity
     *     Contains lazy-initialization call for itself.
     *
     * EXAMPLE: librg_detach_{component}(ent);
     *
     *
     * 4) index
     *     Used to get run-time component index inside inner storage.
     *     MUST NOT be saved in any static parsistant storage, considering the fact
     *     that indexes are not persistant, and can change between application calls.
     *
     * EXAMPLE: u32 index = librg_index_{component}();
     *
     */
    #define librg_component_declare_inner(PREFIX, NAME) \
        ZPL_JOIN3(PREFIX, NAME, _t); \
        \
        typedef struct ZPL_JOIN3(librg_, NAME, _meta_ent_t) { \
            librg_entity_t handle; \
            u32 used; \
        } ZPL_JOIN3(librg_, NAME, _meta_ent_t); \
        \
        typedef struct ZPL_JOIN3(librg__component_, NAME, _pool_t) { \
            zpl_allocator_t backing; \
            usize count; \
            u32 index; \
            zpl_buffer_t(ZPL_JOIN3(librg_, NAME, _meta_ent_t)) entities; \
            zpl_buffer_t(ZPL_JOIN3(PREFIX, NAME, _t))         data; \
        } ZPL_JOIN3(librg__component_, NAME, _pool_t); \
        \
        \
        void ZPL_JOIN2(librg__init_, NAME) (ZPL_JOIN3(librg__component_, NAME, _pool_t) *h, zple_pool *p, zpl_allocator_t a); \
        void ZPL_JOIN2(librg__free_, NAME) (ZPL_JOIN3(librg__component_, NAME, _pool_t) *h); \
        ZPL_JOIN3(PREFIX, NAME, _t) * ZPL_JOIN2(librg_attach_, NAME) (librg_entity_t handle, ZPL_JOIN3(PREFIX, NAME, _t) data); \
        void ZPL_JOIN2(librg_detach_, NAME) (librg_entity_t handle); \
        ZPL_JOIN3(PREFIX, NAME, _t) * ZPL_JOIN2(librg_fetch_, NAME) (librg_entity_t handle); \
        u32 ZPL_JOIN2(librg_index_, NAME) ();

    #define librg_component_declare(NAME) librg_component_declare_inner(,NAME)
    #define librg_component(NAME) \
        librg_component_declare(NAME)


    /**
     * EVENTS
     */


    typedef struct librg_event_t {
        b32 rejected;
        union {
            void *data;
            zpl_bs_t bs;
        };
    } librg_event_t;

    typedef void (librg_event_cb_t)(librg_event_t *event);

    enum {
        LIBRG_CONNECTION_INIT,
        LIBRG_CONNECTION_REQUEST,
        LIBRG_CONNECTION_REFUSE,
        LIBRG_CONNECTION_ACCEPT,
        LIBRG_CONNECTION_DISCONNECT,

        LIBRG_ENTITY_CREATE,
        LIBRG_ENTITY_UPDATE,
        LIBRG_CLIENT_STREAMER_ADD,
        LIBRG_CLIENT_STREAMER_REMOVE,
        LIBRG_CLIENT_STREAMER_UPDATE,

        LIBRG_LAST_ENUM_NUMBER,
    };

    /**
     * Used to attach event handler
     * You can bind as many event handlers onto
     * single event, as you want
     *
     * In the callback you will need to cast event
     * to type of structure that you've triggered this event with
     *
     * @param  id usually you define event ids inside enum
     * @param  callback
     * @return index of added event, can be used to remove particular event handler
     */
    LIBRG_API u64 librg_event_add(u64 id, librg_event_cb_t callback);

    /**
     * Used to trigger execution of all attached
     * event handlers for particlar event
     *
     * You can provide pointer to any data, which will be
     * passed inside the event callback
     *
     * @param id usually you define event ids inside enum
     * @param event pointer onto data or NULL
     */
    LIBRG_API void librg_event_trigger(u64 id, librg_event_t *event);

    /**
     * Used to remove particular callback from
     * event chain, so it wont be called ever again
     *
     * @param id usually you define event ids inside enum
     * @param index returned by librg_event_add
     */
    LIBRG_API void librg_event_remove(u64 id, u64 index);

    /**
     * Used to reject some event from triggering from
     * inside of executing callback
     *
     * @param pointer on the event
     */
    LIBRG_API void librg_event_reject(librg_event_t *event);

    /**
     * Checks if current event was not rejected
     * inside any of the callbacks
     *
     * @param pointer on the event
     */
    LIBRG_API b32 librg_event_succeeded(librg_event_t *event);

    /**
     * NETWORK
     */

    #ifndef LIBRG_NETWORK_CHANNELS
        #define LIBRG_NETWORK_CHANNELS 4
    #endif

    typedef zpl_bs_t    librg_bs_t;
    typedef ENetPeer   *librg_peer_t;
    typedef ENetHost   *librg_host_t;
    typedef ENetPacket *librg_packet_t;

    typedef struct {
        char *host;
        i32 port;
    } librg_address_t;

    typedef struct {
        librg_bs_t data;
        librg_peer_t peer;
        librg_packet_t packet;
    } librg_message_t;

    typedef void (librg_message_handler_t)(librg_message_t *msg);

    ZPL_TABLE_DECLARE(extern, librg_peers_t, librg_peers_, librg_entity_t);

    typedef struct librg_network_t {
        librg_peer_t peer;
        librg_host_t host;

        librg_peers_t connected_peers;
    } librg_network_t;

    LIBRG_API b32 librg_is_connected();

    LIBRG_API void librg_network_start(librg_address_t *address);
    LIBRG_API void librg_network_stop();

    LIBRG_API void librg_send_all(u64 id, zpl_bs_t data);
    LIBRG_API void librg_send_to(u64 id, librg_peer_t peer, zpl_bs_t data);
    LIBRG_API void librg_send_except(u64 id, librg_peer_t peer, zpl_bs_t data);
    LIBRG_API void librg_send_instream(u64 id, librg_entity_t entity, zpl_bs_t data);
    LIBRG_API void librg_send_instream_except(u64 id, librg_entity_t entity, librg_peer_t peer, zpl_bs_t data);

    #define librg_send librg_send_all

    LIBRG_API u64 librg_network_add(u64 id, librg_message_handler_t callback);
    LIBRG_API void librg_network_remove(u64 id, u64 index);


    /**
     * COMPONENTS
     */

    typedef struct {
        zplm_vec3_t position;
        zplm_quat_t rotation;
    } librg_component_declare_inner(librg_, transform);

    typedef struct {
        b32 wut;
    } librg_component_declare_inner(librg_, streamable);

    typedef struct {
        librg_peer_t peer;
    } librg_component_declare_inner(librg_, client);

    #ifndef LIBRG_DISABLE_COMPONENT_SHORTCUTS
        typedef librg_transform_t transform_t;
        typedef librg_client_t    client_t;
    #endif

    /**
     * STREAMER
     */

    LIBRG_API void librg_streamer_remove(librg_entity_t entity);
    LIBRG_API void librg_streamer_set_visible(librg_entity_t entity, b32 state);
    LIBRG_API void librg_streamer_set_visible_for(librg_entity_t entity, librg_entity_t target, b32 state);
    LIBRG_API void librg_streamer_client_set(librg_entity_t entity, librg_peer_t peer);
    LIBRG_API void librg_streamer_client_remove(librg_entity_t entity);



#ifdef __cplusplus
}
#endif

#if defined(LIBRG_IMPLEMENTATION) && !defined(LIBRG_IMPLEMENTATION_DONE)
#define LIBRG_IMPLEMENTATION_DONE

#ifdef __cplusplus
extern "C" {
#endif

    #define librg__set_default(expr, value) if (!expr) expr = value

    /**
     * Storage containers
     * for inner librg stuff
     */
    zple_pool       librg__entity_pool;
    zplev_pool      librg__events;
    librg_cfg_t     librg__config;
    zpl_timer_pool  librg__timers;
    librg_network_t librg__network;
    librg__entcache_t librg__entcache;

    zpl_array_t(librg_message_handler_t *) librg__messages;

    /**
     * Global variable with array of component pools
     */
    zpl_array_t(void *) librg__component_pool;

    ZPL_TABLE_DEFINE(librg_peers_t, librg_peers_, librg_entity_t);
    ZPL_TABLE_DEFINE(librg__entcache_t, librg__entcache_, librg_entity_t);

    /**
     * Create dummy component to
     * use it as template for removal in destroy
     */
    typedef struct {} librg_component_declare(_dummy);

    void librg__dummy(char const *fmt, ...) {}

    zpl_inline void librg__entity_store(librg_entity_t id, u32 slot, b32 remote) {
        librg__entcache_key_t key = { .slot = slot, .is_local = !remote };
        librg__entcache_set(&librg__entcache, key.master, id);
    }

    librg_entity_t librg_entity_create() {
        return zple_create(&librg__entity_pool);
    }

    librg_entity_t librg_entity_create_shared(u32 guid) {
        librg_entity_t id = librg_entity_create();

        librg_entity_t remote_id = id;
        remote_id.id = guid;
        librg__entity_store(remote_id, id.id, false);

        id.part[0] = true;
        librg__entity_store(id, guid, true);

        id.id = guid;

        return id;
    }

    librg_entity_t librg__entity_get(librg_entity_t id) {
        librg__entcache_key_t key;
        key.slot = id.id;
        key.is_local = !id.part[0];

        librg_entity_t *handle = librg__entcache_get(&librg__entcache, key.master);
        return (handle) ? *handle : id;
    }

    void librg_entity_destroy(librg_entity_t id) {
        librg_entity_t entity = librg__entity_get(id);

        for (i32 i = 0; i < zpl_array_count(librg__component_pool); i++) {
            librg__dummy_meta_ent_t *meta_ent = (cast(librg__component__dummy_pool_t*)librg__component_pool[i])->entities+entity.id;
            ZPL_ASSERT(meta_ent);
            meta_ent->used = false;
        }

        return zple_destroy(&librg__entity_pool, entity);
    }

    void librg_entity_each(librg_entity_filter_t filter, librg_entity_cb_t callback) {
        u32 index = filter.contains1;
        ZPL_ASSERT(index != 0);
        ZPL_ASSERT(index <= zpl_array_count(librg__component_pool));

        librg__component__dummy_pool_t *pool = cast(librg__component__dummy_pool_t*)librg__component_pool[index - 1];
        ZPL_ASSERT(pool);

        // iterating on each entity having the first provided component
        for (usize i = 0; i < pool->count; ++i) {
            librg__dummy_meta_ent_t meta_ent = pool->entities[i];
            if (!meta_ent.used) continue;

            librg_entity_t handle = meta_ent.handle;
            b32 used = true;

            // iterate the rest contained components, and make sure they exist
            for (usize j = 1; j < 8; j++) {
                if (filter.contains[j] == 0) LIBRG__ENTITY_EACH_BREAK;
                librg__component__dummy_pool_t *sub_pool = cast(librg__component__dummy_pool_t*)librg__component_pool[filter.contains[j] - 1];
                ZPL_ASSERT(sub_pool);

                librg__dummy_meta_ent_t *sub_meta_ent = cast(librg__dummy_meta_ent_t *)(sub_pool->entities+handle.id);
                ZPL_ASSERT(sub_meta_ent);

                // if not used - skip
                if (!sub_meta_ent->used) {
                    used = false; break;
                }
            }

            // early skip if some component is not attached
            if (!used) continue;

            for (usize j = 0; j < 4; j++) {
                if (filter.excludes[j] == 0) LIBRG__ENTITY_EACH_BREAK;
                librg__component__dummy_pool_t *sub_pool = cast(librg__component__dummy_pool_t*)librg__component_pool[filter.excludes[j] - 1];
                ZPL_ASSERT(sub_pool);

                librg__dummy_meta_ent_t *sub_meta_ent = cast(librg__dummy_meta_ent_t *)(sub_pool->entities+handle.id);
                ZPL_ASSERT(sub_meta_ent);

                // if not used - skip
                if (sub_meta_ent->used) {
                    used = false; break;
                }
            }

            // component exists for all "contains" conditions
            if (used) callback(handle);
        }
    }

    #define librg_component_define_inner(PREFIX, NAME) \
        ZPL_JOIN3(librg__component_, NAME, _pool_t) ZPL_JOIN3(librg__component_, NAME, _pool); \
        \
        void ZPL_JOIN2(librg__init_, NAME) (ZPL_JOIN3(librg__component_, NAME, _pool_t) *h, zple_pool *p, zpl_allocator_t a) { \
            ZPL_ASSERT(h&&p); h->backing = a; \
            h->count = p->count; \
            zpl_buffer_init(h->entities, a, p->count); \
            zpl_buffer_init(h->data, a, p->count); \
            zpl_array_append(librg__component_pool, cast(void *)h); \
            h->index = zpl_array_count(librg__component_pool);\
        }\
        void ZPL_JOIN2(librg__free_, NAME) (ZPL_JOIN3(librg__component_, NAME, _pool_t) *h) { \
            ZPL_ASSERT(h); \
            zpl_buffer_free(h->entities, h->backing); \
            zpl_buffer_free(h->data, h->backing); \
        } \
        u32 ZPL_JOIN2(librg_index_, NAME)() { \
            return ZPL_JOIN3(librg__component_, NAME, _pool).index; \
        } \
        ZPL_JOIN3(PREFIX, NAME, _t) * ZPL_JOIN2(librg_attach_, NAME) (librg_entity_t id, ZPL_JOIN3(PREFIX, NAME, _t) data) { \
            librg_entity_t handle = librg__entity_get(id); \
            if (ZPL_JOIN3(librg__component_, NAME, _pool).count == 0) { \
                ZPL_JOIN2(librg__init_, NAME)(&ZPL_JOIN3(librg__component_, NAME, _pool), &librg__entity_pool, LIBRG_ENTITY_ALLOCATOR()); \
            } \
            ZPL_JOIN3(librg_, NAME, _meta_ent_t) *meta_ent = (ZPL_JOIN3(librg__component_, NAME, _pool).entities+handle.id); \
            meta_ent->handle = handle; \
            meta_ent->used = true; \
            *(ZPL_JOIN3(librg__component_, NAME, _pool).data+handle.id) = data; \
            return (ZPL_JOIN3(librg__component_, NAME, _pool).data+handle.id); \
        } \
        void ZPL_JOIN2(librg_detach_, NAME) (librg_entity_t id) { \
            librg_entity_t handle = librg__entity_get(id); \
            if (ZPL_JOIN3(librg__component_, NAME, _pool).count == 0) { \
                ZPL_JOIN2(librg__init_, NAME)(&ZPL_JOIN3(librg__component_, NAME, _pool), &librg__entity_pool, LIBRG_ENTITY_ALLOCATOR()); \
            } \
            ZPL_JOIN3(librg_, NAME, _meta_ent_t) *meta_ent = (ZPL_JOIN3(librg__component_, NAME, _pool).entities+handle.id); \
            meta_ent->used = false; \
        } \
        ZPL_JOIN3(PREFIX, NAME, _t) * ZPL_JOIN2(librg_fetch_, NAME) (librg_entity_t id) { \
            librg_entity_t handle = librg__entity_get(id); \
            if (ZPL_JOIN3(librg__component_, NAME, _pool).count == 0) { \
                ZPL_JOIN2(librg__init_, NAME)(&ZPL_JOIN3(librg__component_, NAME, _pool), &librg__entity_pool, LIBRG_ENTITY_ALLOCATOR()); \
            } \
            ZPL_JOIN3(librg_, NAME, _meta_ent_t) *meta_ent = (ZPL_JOIN3(librg__component_, NAME, _pool).entities+handle.id); \
            if ((meta_ent->used) && (meta_ent->handle.id == handle.id) && (meta_ent->handle.generation == handle.generation)) { \
                return (ZPL_JOIN3(librg__component_, NAME, _pool).data+handle.id); \
            } \
            else { \
                return NULL; \
            } \
        }
    #define librg_component_define(NAME) librg_component_define_inner(,NAME)
    #undef librg_component
    #define librg_component(NAME) \
        librg_component_declare(NAME) \
        librg_component_define(NAME)

    librg_component_define_inner(librg_, transform);
    librg_component_define_inner(librg_, client);

    u64 librg_event_add(u64 id, librg_event_cb_t callback) {
        return zplev_add(&librg__events, id, (zplev_cb *)callback);
    }

    void librg_event_trigger(u64 id, librg_event_t *event) {
        zplev_block *block = zplev_pool_get(&librg__events, id);
        if (!block) return;

        for (isize i = 0; i < zpl_array_count(*block) && !event->rejected; ++i) {
            (*block)[i](event);
        }
    }

    void librg_event_remove(u64 id, u64 index) {
        zplev_remove(&librg__events, id, index);
    }

    void librg_event_reject(librg_event_t *event) {
        ZPL_ASSERT(event);
        event->rejected = true;
    }

    b32 librg_event_succeeded(librg_event_t *event) {
        ZPL_ASSERT(event);
        return !event->rejected;
    }

    b32 librg_is_server() {
        return librg__config.mode == librg_server_ev;
    }

    b32 librg_is_client() {
        return librg__config.mode == librg_client_ev;
    }

    b32 librg_is_connected() {
        return librg__network.peer && librg__network.peer->state == ENET_PEER_STATE_CONNECTED;
    }

    /**
     * SHARED
     */
    void librg__connection_init(librg_message_t *msg) {
        librg_dbg("librg__connection_init\n");

        char my_host[16];

        enet_address_get_host_ip(&msg->peer->address, my_host, 16);
        librg_log("a new connection attempt at %s:%u.\n", my_host, msg->peer->address.port);

        if (librg_is_client()) {
            librg_event_t event;
            zpl_bs_init(event.bs, zpl_heap_allocator(), LIBRG_DEFAULT_BS_SIZE);

            librg_event_trigger(LIBRG_CONNECTION_REQUEST, &event);
            librg_send_to(LIBRG_CONNECTION_REQUEST, msg->peer, event.bs);

            zpl_bs_free(event.bs);
        }
    }

    /**
     * SERVER SIDE
     */
    void librg__connection_request(librg_message_t *msg) {
        librg_dbg("librg__connection_request\n");

        librg_event_t event = { .bs = msg->data };
        librg_event_trigger(LIBRG_CONNECTION_REQUEST, &event);

        if (librg_event_succeeded(&event)) {
            librg_entity_t entity = librg_entity_create();

            // assign default compoenents
            librg_attach_transform(entity, (librg_transform_t){});
            librg_attach_client(entity, (librg_client_t){ msg->peer });

            // add client peer to storage
            librg_peers_set(&librg__network.connected_peers, cast(u64)msg->peer, entity);
            librg_send_to(LIBRG_CONNECTION_ACCEPT, msg->peer, NULL);
        }
        else {
            librg_send_to(LIBRG_CONNECTION_REFUSE, msg->peer, NULL);
        }
    }

    /**
     * CLIENT SIDE
     */
    void librg__connection_refuse(librg_message_t *msg) {
        librg_event_t event = { .bs = msg->data };
        librg_event_trigger(LIBRG_CONNECTION_REFUSE, &event);
    }

    /**
     * CLIENT SIDE
     */
    void librg__connection_accept(librg_message_t *msg) {
        librg_dbg("librg__connection_accept\n");

        // there is a place i need to add shared id
        librg_entity_t entity = librg_entity_create();

        // assign default compoennets
        librg_attach_transform(entity, (librg_transform_t){});
        // librg_attach_client(entity, (librg_client_t){ msg->peer });
        // entity.assign<transform_t>();
        // entity.assign<server_owned_t>(guid);

        // add server peer to storage
        // streamer::entity_pool.insert(std::make_pair(guid, entity));
        librg_peers_set(&librg__network.connected_peers, cast(u64)msg->peer, entity);
        librg_event_trigger(LIBRG_CONNECTION_ACCEPT, msg->data);
    }

    /**
     * SHARED
     */
    void librg__connection_disconnect(librg_message_t *msg) {
        librg_dbg("librg__connection_disconnect\n");

        // librg_event_trigger(LIBRG_CONNECTION_DISCONNECT, msg->data);
    }

    void librg__entity_create(librg_message_t *msg) {
        u16 query_size = zpl_bs_read_u16(msg->data);

        // for (int i = 0; i < query_size; ++i) {
        //     auto guid = data->read_uint64();
        //     auto type = data->read_uint8();

        //     transform_t newtransform;
        //     data->read(newtransform);

        //     auto entity         = entities->create();
        //     auto streamable     = entity.assign<streamable_t>();
        //     auto transform      = entity.assign<transform_t>();
        //     auto server_owned   = entity.assign<server_owned_t>();

        //     streamable->type    = type;
        //     server_owned->guid  = guid;
        //     *transform          = newtransform;

        //     streamer::entity_pool.insert(std::make_pair(guid, entity));

        //     events::trigger(events::on_create, new events::event_bs_entity_t(data, entity, guid, type));
        // }

        // uint16_t remove_size = 0;
        // data->read(remove_size);

        // for (int i = 0; i < remove_size; ++i) {
        //     auto guid = data->read_uint64();

        //     if (streamer::entity_pool.find(guid) != streamer::entity_pool.end()) {
        //         auto entity     = streamer::entity_pool[guid];
        //         auto streamable = entity.component<streamable_t>();

        //         events::trigger(events::on_remove, new events::event_bs_entity_t(data, entity, guid, streamable->type));

        //         streamer::entity_pool.erase(guid);
        //         entity.destroy();
        //     }
        //     else {
        //         core::log("unexpected entity %lld on remove", guid);
        //     }
        // }
    }

    void librg__entity_update(librg_message_t *msg) {

    }

    void librg__entity_client_streamer_add(librg_message_t *msg) {

    }

    void librg__entity_client_streamer_remove(librg_message_t *msg) {

    }

    void librg__entity_client_streamer_update(librg_message_t *msg) {

    }

    void librg_send_all(u64 id, zpl_bs_t data) {

    }

    void librg_send_to(u64 id, librg_peer_t peer, zpl_bs_t data) {

    }

    void librg_send_except(u64 id, librg_peer_t peer, zpl_bs_t data) {

    }

    void librg_send_instream(u64 id, librg_entity_t entity, zpl_bs_t data) {

    }

    void librg_send_instream_except(u64 id, librg_entity_t entity, librg_peer_t peer, zpl_bs_t data) {

    }

    void librg_tick() {
        zpl_timer_update(librg__timers);

        if (!librg__network.host) {
            return; /* occasion where we are not started network yet */
        }

        ENetEvent event;

        while (enet_host_service(librg__network.host, &event, 0) > 0) {
            librg_message_t msg = {
                .data   = NULL,
                .peer   = event.peer,
                .packet = event.packet,
            };

            switch (event.type) {
                case ENET_EVENT_TYPE_RECEIVE:
                    {
                        zpl_bs_t data;
                        zpl_bs_init(data, zpl_heap_allocator(), event.packet->dataLength);
                        zpl_bs_write_size(data, event.packet->data, event.packet->dataLength);

                        u16 id = zpl_bs_read_u16(data);

                        if (id < zpl_array_count(librg__messages) && librg__messages[id]) {
                            msg.data = data; librg__messages[id](&msg);
                        }
                        else {
                            librg_log("network: unknown message: %d", id);
                        }

                        zpl_bs_free(data);
                        enet_packet_destroy(event.packet);
                    }
                    break;
                case ENET_EVENT_TYPE_CONNECT:    librg__messages[LIBRG_CONNECTION_INIT](&msg); break;
                case ENET_EVENT_TYPE_DISCONNECT: librg__messages[LIBRG_CONNECTION_DISCONNECT](&msg); break;
                case ENET_EVENT_TYPE_NONE: break;
            }
        }
    }

    ZPL_TIMER_CB(librg__tick_cb) {
        // todo: streamer update
    }

    void librg_network_start(librg_address_t *addr) {
        librg_dbg("librg_network_start\n");

        librg_peers_init(&librg__network.connected_peers, zpl_heap_allocator());

        if (librg_is_server()) {
            ENetAddress address;

            address.port = addr->port;
            address.host = ENET_HOST_ANY;

            // setup server host
            librg__network.host = enet_host_create(&address, librg__config.max_connections, LIBRG_NETWORK_CHANNELS, 0, 0);
            ZPL_ASSERT_MSG(librg__network.host, "could not start server at provided port");
        }
        else {
            ENetAddress address;

            address.port = addr->port;
            enet_address_set_host(&address, addr->host);

            // setup client host
            librg__network.host = enet_host_create(NULL, 1, LIBRG_NETWORK_CHANNELS, 57600 / 8, 14400 / 8);
            ZPL_ASSERT_MSG(librg__network.host, "could not start client");

            // create peer connecting to server
            librg_log("connecting to server %s:%u\n", addr->host, addr->port);
            librg__network.peer = enet_host_connect(librg__network.host, &address, LIBRG_NETWORK_CHANNELS, 0);
            ZPL_ASSERT_MSG(librg__network.peer, "could not setup peer for provided address");
        }
    }

    void librg_network_stop() {
        librg_dbg("librg_network_stop\n");

        if (librg__network.peer) {
            ENetEvent event;

            // disconnect and emit event
            enet_peer_disconnect(librg__network.peer, 0);
            enet_host_service(librg__network.host, &event, 100);

            // reset our peer
            enet_peer_reset(librg__network.peer);
        }

        librg_peers_destroy(&librg__network.connected_peers);
    }

    void librg_init(librg_cfg_t config) {
        librg_dbg("librg_init\n");
        librg__config = config;

        // apply default settings (if no user provided)
        librg__set_default(librg__config.tick_delay, 32);
        librg__set_default(librg__config.entity_limit, 2048);
        librg__set_default(librg__config.max_connections, 16);
        librg__set_default(librg__config.mode, librg_server_ev);
        librg__set_default(librg__config.world_size.x, 4096.0f);
        librg__set_default(librg__config.world_size.y, 4096.0f);

        // init entity system
        zple_init(&librg__entity_pool, LIBRG_ENTITY_ALLOCATOR(), librg__config.entity_limit);
        zpl_array_init(librg__component_pool, LIBRG_ENTITY_ALLOCATOR());
        librg__entcache_init(&librg__entcache, zpl_heap_allocator());

        // events
        zplev_init(&librg__events, zpl_heap_allocator());
        zpl_array_init_reserve(librg__messages, zpl_heap_allocator(), 10);
        zpl_array_count(librg__messages) = 10; /* 10 messages in the bottom */

        // init timers
        zpl_array_init(librg__timers, zpl_heap_allocator());
        zpl_timer_t *tick_timer = zpl_timer_add(librg__timers);
        zpl_timer_set(tick_timer, 1000 * librg__config.tick_delay, -1, librg__tick_cb);
        zpl_timer_start(tick_timer, 1000);

        // network
        ZPL_ASSERT_MSG(enet_initialize() == 0, "cannot initialize enet");

        // add event handlers for our network stuufz
        librg__messages[LIBRG_CONNECTION_INIT]        = librg__connection_init;
        librg__messages[LIBRG_CONNECTION_REQUEST]     = librg__connection_request;
        librg__messages[LIBRG_CONNECTION_REFUSE]      = librg__connection_refuse;
        librg__messages[LIBRG_CONNECTION_ACCEPT]      = librg__connection_accept;
        librg__messages[LIBRG_CONNECTION_DISCONNECT]  = librg__connection_disconnect;
        librg__messages[LIBRG_ENTITY_CREATE]          = librg__entity_create;
        librg__messages[LIBRG_ENTITY_UPDATE]          = librg__entity_update;
        librg__messages[LIBRG_CLIENT_STREAMER_ADD]    = librg__entity_client_streamer_add;
        librg__messages[LIBRG_CLIENT_STREAMER_REMOVE] = librg__entity_client_streamer_remove;
        librg__messages[LIBRG_CLIENT_STREAMER_UPDATE] = librg__entity_client_streamer_update;
    }

    void librg_free() {
        librg_dbg("librg_free\n");
        // free all timers and events first
        zpl_array_free(librg__timers);
        zpl_array_free(librg__messages);
        zplev_destroy(&librg__events);

        // free the entity component pools
        for (i32 i = 0; i < zpl_array_count(librg__component_pool); i++) {
            ZPL_ASSERT(librg__component_pool[i]);
            librg__component__dummy_pool_t *h = cast(librg__component__dummy_pool_t*)librg__component_pool[i];
            zpl_buffer_free(h->entities, h->backing);
            zpl_buffer_free(h->data, h->backing);
        }

        // free containers and entity pool
        zpl_array_free(librg__component_pool);
        librg__entcache_destroy(&librg__entcache);
        zple_free(&librg__entity_pool);

        enet_deinitialize();
    }

#ifdef __cplusplus
}
#endif

#endif // LIBRG_IMPLEMENTATION
#endif // LIBRG_INCLUDE_H
