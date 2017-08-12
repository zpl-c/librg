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
        zplm_vec2_t world_size;

        u32 entity_limit;

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
    LIBRG_API void librg_init(librg_mode_e mode, librg_cfg_t config);

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

    /**
     * Create entity and return handle
     */
    LIBRG_API librg_entity_t librg_entity_create();

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
    #define librg_component_declare(NAME) \
        ZPL_JOIN2(NAME, _t); \
        \
        typedef struct ZPL_JOIN3(librg_, NAME, _meta_ent_t) { \
            zple_id_t handle; \
            ZPLE_ID   used; \
        } ZPL_JOIN3(librg_, NAME, _meta_ent_t); \
        \
        typedef struct ZPL_JOIN3(librg__component_, NAME, _pool_t) { \
            zpl_allocator_t backing; \
            usize count; \
            u32 index; \
            zpl_buffer_t(ZPL_JOIN3(librg_, NAME, _meta_ent_t)) entities; \
            zpl_buffer_t(ZPL_JOIN2(NAME, _t))         data; \
        } ZPL_JOIN3(librg__component_, NAME, _pool_t); \
        \
        \
        void                  ZPL_JOIN2(librg__init_, NAME)     (ZPL_JOIN3(librg__component_, NAME, _pool_t) *h, zple_pool *p, zpl_allocator_t a); \
        void                  ZPL_JOIN2(librg__free_, NAME)     (ZPL_JOIN3(librg__component_, NAME, _pool_t) *h); \
        ZPL_JOIN2(NAME, _t) * ZPL_JOIN2(librg_attach_, NAME)    (zple_id_t handle, ZPL_JOIN2(NAME, _t) data); \
        void                  ZPL_JOIN2(librg_detach_, NAME)    (zple_id_t handle); \
        ZPL_JOIN2(NAME, _t) * ZPL_JOIN2(librg_fetch_, NAME)     (zple_id_t handle); \
        u32                   ZPL_JOIN2(librg_index_, NAME)     ();

    #define librg_component(NAME) \
        librg_component_declare(NAME)




    /**
     * EVENTS
     */

    typedef zplev_data_t librg_evt_t;
    typedef zplev_cb     librg_evt_cb;

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
    LIBRG_API u64 librg_event_add(u64 id, librg_evt_cb callback);

    /**
     * Used to trigger execution of all attached
     * event handlers for particlar event
     *
     * You can provide pointer to any data, which will be
     * passed inside the event callback
     *
     * @param id usually you define event ids inside enum
     * @param  event pointer onto data or NULL
     */
    LIBRG_API void librg_event_trigger(u64 id, librg_evt_t event);

    /**
     * Used to remove particular callback from
     * event chain, so it wont be called ever again
     *
     * @param  id usually you define event ids inside enum
     * @param  index returned by librg_event_add
     */
    LIBRG_API void librg_event_remove(u64 id, u64 index);



    /**
     * NETWORK
     */

    LIBRG_API void librg_connect(char *host, int port);
    LIBRG_API void librg_disconnect();
    LIBRG_API b32 librg_is_connected();

    typedef void *librg_peer_t;
    typedef void *librg_net_cb;

    LIBRG_API void librg_send_all(u64 id, librg_net_cb callback);
    LIBRG_API void librg_send_instream(u64 id, librg_net_cb callback);
    LIBRG_API void librg_send_to(u64 id, librg_entity_t entity, librg_net_cb callback);
    LIBRG_API void librg_send_except(u64 id, librg_entity_t entity, librg_net_cb callback);
    LIBRG_API void librg_send_instream_except(u64 id, librg_entity_t entity, librg_net_cb callback);

    #define librg_send librg_send_all

    LIBRG_API u64 librg_net_add(u64 id, librg_net_cb callback);
    LIBRG_API void librg_net_remove(u64 id, u64 index);



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
    librg_mode_e    librg__mode;
    zpl_timer_pool  librg__timers;

    /**
     * Global variable with array of component pools
     */
    zpl_array_t(void *) librg__component_pool;

    /**
     * Create dummy component to
     * use it as template for removal in destroy
     */
    typedef struct {} librg_component_declare(_dummy);




    void librg_tick() {
        zpl_timer_update(librg__timers);
    }

    ZPL_TIMER_CB(librg__tick_cb) {
        librg_log("Hello, Sailor!\n");
    }

    void librg_init(librg_mode_e mode, librg_cfg_t config) {
        librg__mode   = mode;
        librg__config = config;

        // apply default settings (if no user provided)
        librg__set_default(librg__config.tick_delay, 32);
        librg__set_default(librg__config.entity_limit, 2048);
        librg__set_default(librg__config.world_size.x, 4096.0f);
        librg__set_default(librg__config.world_size.y, 4096.0f);

        // init entity system
        zple_init(&librg__entity_pool, LIBRG_ENTITY_ALLOCATOR(), librg__config.entity_limit);
        zpl_array_init(librg__component_pool, LIBRG_ENTITY_ALLOCATOR());

        zplev_init(&librg__events, zpl_heap_allocator());

        // init timers
        zpl_array_init(librg__timers, zpl_heap_allocator());
        zpl_timer_t *tick_timer = zpl_timer_add(librg__timers);
        zpl_timer_set(tick_timer, 1000 * librg__config.tick_delay, -1, librg__tick_cb);
        zpl_timer_start(tick_timer, 1000);


    }

    void librg_free() {
        // free all timers and events first
        zpl_array_free(librg__timers);
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
        zple_free(&librg__entity_pool);
    }

    b32 librg_is_server() {
        return librg__mode == librg_mode_server_ev;
    }

    b32 librg_is_client() {
        return librg__mode == librg_mode_client_ev;
    }





    zple_id_t librg_entity_create() {
        return zple_create(&librg__entity_pool);
    }

    void librg_entity_destroy(librg_entity_t entity) {
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


    #define librg_component_define(NAME) \
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
        ZPL_JOIN2(NAME, _t) * ZPL_JOIN2(librg_attach_, NAME) (zple_id_t handle, ZPL_JOIN2(NAME, _t) data) { \
            if (ZPL_JOIN3(librg__component_, NAME, _pool).count == 0) { \
                ZPL_JOIN2(librg__init_, NAME)(&ZPL_JOIN3(librg__component_, NAME, _pool), &librg__entity_pool, LIBRG_ENTITY_ALLOCATOR()); \
            } \
            ZPL_JOIN3(librg_, NAME, _meta_ent_t) *meta_ent = (ZPL_JOIN3(librg__component_, NAME, _pool).entities+handle.id); \
            meta_ent->handle = handle; \
            meta_ent->used = true; \
            *(ZPL_JOIN3(librg__component_, NAME, _pool).data+handle.id) = data; \
            return (ZPL_JOIN3(librg__component_, NAME, _pool).data+handle.id); \
        } \
        void ZPL_JOIN2(librg_detach_, NAME) (zple_id_t handle) { \
            if (ZPL_JOIN3(librg__component_, NAME, _pool).count == 0) { \
                ZPL_JOIN2(librg__init_, NAME)(&ZPL_JOIN3(librg__component_, NAME, _pool), &librg__entity_pool, LIBRG_ENTITY_ALLOCATOR()); \
            } \
            ZPL_JOIN3(librg_, NAME, _meta_ent_t) *meta_ent = (ZPL_JOIN3(librg__component_, NAME, _pool).entities+handle.id); \
            meta_ent->used = false; \
        } \
        ZPL_JOIN2(NAME, _t) * ZPL_JOIN2(librg_fetch_, NAME) (zple_id_t handle) { \
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

    #undef librg_component
    #define librg_component(NAME) \
        librg_component_declare(NAME) \
        librg_component_define(NAME)




    u64 librg_event_add(u64 id, librg_evt_cb callback) {
        return zplev_add(&librg__events, id, callback);
    }

    void librg_event_trigger(u64 id, librg_evt_t event) {
        zplev_trigger(&librg__events, id, event);
    }

    void librg_event_remove(u64 id, u64 index) {
        zplev_remove(&librg__events, id, index);
    }



#ifdef __cplusplus
}
#endif

#endif // LIBRG_IMPLEMENTATION
#endif // LIBRG_INCLUDE_H
