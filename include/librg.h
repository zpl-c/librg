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
 * zpl_math.h
 * zpl_cull.h
 * zpl_event.h
 * enet.h
 *
 * For the demo:
 * sdl2.h
 *
 * Version History:
 * 3.0.0 - context switching and component refactor
 *     and other stuff
 *
 * 2.2.3 - fixed mem leak on net event
 * 2.2.2 - Fixed client issue with librg_message_send_instream_except
 * 2.2.1 - Fixed cpp issues with librg_data_t pointers
 * 2.2.0 - Inner message system rafactor
 * 2.1.0 - Inner bitstream refactors, with slight interface changes
 * 2.0.2 - C++ and MSVC related fixes
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

#define LIBRG_VERSION_MAJOR 3
#define LIBRG_VERSION_MINOR 0
#define LIBRG_VERSION_PATCH 0
#define LIBRG_VERSION_CREATE(major, minor, patch) (((major)<<16) | ((minor)<<8) | (patch))
#define LIBRG_VERSION_GET_MAJOR(version) (((version)>>16)&0xFF)
#define LIBRG_VERSION_GET_MINOR(version) (((version)>>8)&0xFF)
#define LIBRG_VERSION_GET_PATCH(version) ((version)&0xFF)
#define LIBRG_VERSION LIBRG_VERSION_CREATE(LIBRG_VERSION_MAJOR, LIBRG_VERSION_MINOR, LIBRG_VERSION_PATCH)

#ifndef LIBRG_CUSTOM_INCLUDES

#ifdef LIBRG_IMPLEMENTATION
#define ZPL_IMPLEMENTATION
#define ZPLM_IMPLEMENTATION
#define ZPLC_IMPLEMENTATION
#define ZPLEV_IMPLEMENTATION
#endif

#include <zpl.h>
#include <zpl_math.h>
#include <zpl_cull.h>
#include <zpl_event.h>
#include <enet/enet.h>

#endif

#ifdef LIBRG_SHARED

#if defined(_WIN32)
#define LIBRG_API ZPL_EXTERN __declspec(dllexport)
#else
#define LIBRG_API ZPL_EXTERN __attribute__((visibility("default")))
#endif
#else

#ifndef LIBRG_API
#define LIBRG_API ZPL_DEF
#endif

#endif

#ifdef __cplusplus
extern "C" {
#endif


    /**
     *
     * BASIC DEFINITOINS
     *
     */

    #define librg_log           zpl_printf
    #define librg_global        zpl_global
    #define librg_inline        zpl_inline
    #define librg_internal      zpl_internal
    #define librg_assert        ZPL_ASSERT
    #define librg_assert_msg    ZPL_ASSERT_MSG
    #define librg_lambda(name)  name

    #if defined(__cplusplus) || defined(_MSC_VER)
    #define librg_void char
    #else
    #define librg_void void
    #endif

    #ifdef LIBRG_DEBUG
    #define librg_dbg(fmt, ...) librg_log(fmt, ##__VA_ARGS__)
    #define librg_measure(TITLE, CODE) do { \
            u64 start  = zpl_utc_time_now(); CODE; \
            f32 result = (zpl_utc_time_now() - start) / 1000.0; \
            librg_log("%s: took %f ms.\n", TITLE, result); \
        } while(0)
    #else
    #define librg_dbg(fmt, ...)
    #define librg_measure(TITLE, CODE)
    #endif

    #define librg_bit_size 32
    #define librg_ceiling(x,y)     ( ((x) + (y) - 1) / (y) )
    #define librg_bit_set(A,k)     ( A[(k/32)] |= (1 << (k%32)) )
    #define librg_bit_clear(A,k)   ( A[(k/32)] &= ~(1 << (k%32)) )
    #define librg_bit_test(A,k)    ( A[(k/32)] & (1 << (k%32)) )

    #define LIBRG_DATA_STREAMS_AMOUNT 4

    /**
     *
     * OPTIONS
     *
     */

    typedef enum {
        LIBRG_PLATFORM_ID,
        LIBRG_PLATFORM_PROTOCOL,
        LIBRG_PLATFORM_BUILD,

        LIBRG_DEFAULT_CLIENT_TYPE,
        LIBRG_DEFAULT_DATA_SIZE,

        LIBRG_NETWORK_CAPACITY,
        LIBRG_NETWORK_CHANNELS,
        LIBRG_NETWORK_PRIMARY_CHANNEL,
        LIBRG_NETWORK_SECONDARY_CHANNEL,
        LIBRG_NETWORK_MESSAGE_CHANNEL,

        LIBRG_OPTIONS_SIZE,
    } librg_options_e;

    enum {
        LIBRG_MODE_SERVER,
        LIBRG_MODE_CLIENT,
    };

    /**
     * Default built-in events
     * define your events likes this:
     *     enum {
     *         MY_NEW_EVENT_1 = LIBRG_LAST_EVENT,
     *         MY_NEW_EVENT_2,
     *         MY_NEW_EVENT_3,
     *     };
     */
    enum {
        LIBRG_CONNECTION_INIT,
        LIBRG_CONNECTION_REQUEST,
        LIBRG_CONNECTION_REFUSE,
        LIBRG_CONNECTION_ACCEPT,
        LIBRG_CONNECTION_DISCONNECT,

        LIBRG_ENTITY_CREATE,
        LIBRG_ENTITY_UPDATE,
        LIBRG_ENTITY_REMOVE,
        LIBRG_CLIENT_STREAMER_ADD,
        LIBRG_CLIENT_STREAMER_REMOVE,
        LIBRG_CLIENT_STREAMER_UPDATE,

        LIBRG_EVENT_LAST,
    };

    /**
     * Table for various entity bool storages
     */
    ZPL_TABLE_DECLARE(extern, librg_table_t, librg_table_, u32);

    /**
     *
     * CORE
     *
     */

    struct librg_ctx_t;

    typedef u32 librg_entity_t;

    typedef ENetPeer   librg_peer_t;
    typedef ENetHost   librg_host_t;
    typedef ENetPacket librg_packet_t;

    typedef struct {
        void *rawptr;

        usize capacity;
        usize read_pos;
        usize write_pos;

        zpl_allocator_t allocator;
    } librg_data_t;


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
     *     librg_filter_t filter = { librg_index_c1(), librg_index_c2() };
     * OR
     *     librg_filter_t filter = { librg_index_c1(), .excludes1 = librg_index_c2() };
     */
    typedef union librg_filter_t {
        struct {
            u32 contains1; u32 contains2; u32 contains3; u32 contains4;
            u32 contains5; u32 contains6; u32 contains7; u32 contains8;
            u32 excludes1; u32 excludes2; u32 excludes3; u32 excludes4;
        };

        struct {
            u32 contains[8];
            u32 excludes[4];
        };
    } librg_filter_t;


    /**
     * Storage containers
     * for inner librg stuff
     */

    typedef struct {
        usize offset;
        usize size;
        zpl_buffer_t(b32) used;
    } librg_component_meta;

    typedef struct {
        u32 cursor;
        u32 count;
        u32 limit_upper;
        u32 limit_lower;
    } librg_entity_pool_t;

    /**
     * Simple host address
     * used to configure network on start
     */
    typedef struct {
        char *host;
        i32 port;
    } librg_address_t;

    /**
     * Message strure
     * gets returned inside network handler
     * on each incoming message
     */
    typedef struct librg_message_t {
        struct librg_ctx_t *ctx;
        librg_data_t *data;
        librg_peer_t *peer;
        librg_packet_t *packet;
    } librg_message_t;

    typedef struct librg_event_t {
        struct librg_ctx_t *ctx;
        librg_data_t *data;
        librg_entity_t entity;
        void *userptr;
        b32 rejected;
    } librg_event_t;

    /**
     * Callbacks
     */

    typedef void (librg_entity_cb)(struct librg_ctx_t *ctx, librg_entity_t entity);
    typedef void (librg_component_cb)(struct librg_ctx_t *ctx);
    typedef void (librg_message_cb)(librg_message_t *msg);
    typedef void (librg_event_cb)(librg_event_t *event);

    /**
     *
     * COMPONENTS
     *
     */

    typedef struct {
        zplm_vec3_t position;
    } librg_transform_t;

    typedef struct {
        librg_peer_t *peer;
    } librg_control_t;

    typedef struct {
        u32 range;
    } librg_stream_t;

    typedef struct {
        u32 type;
        librg_table_t ignored;
    } librg_meta_t;

    typedef struct {
        librg_peer_t *peer;
        librg_table_t last_snapshot;
    } librg_client_t;

    enum {
        librg_dummmy,
        librg_meta,
        librg_transform,
        librg_stream,
        librg_control,
        librg_client,
        librg_component_last,
    };

    typedef struct librg_ctx_t {
        zpl_allocator_t allocator;
        zpl_timer_pool  timers;
        zplev_pool      events;
        zplc_t          streamer;

        // core
        u8 mode;
        u16 tick_delay;

        // streamer configuration
        u16 max_connections;
        u32 max_entities;
        u32 max_components;
        zplm_vec3_t world_size;

        zpl_buffer_t(librg_message_cb *) messages;

        struct {
            librg_peer_t *peer;
            librg_host_t *host;
            librg_table_t connected_peers;
        } network;

        union {
            struct {
                librg_data_t stream_input;
                librg_data_t stream_output;
                librg_data_t stream_upd_reliable;
                librg_data_t stream_upd_unreliable;
            };

            librg_data_t streams[LIBRG_DATA_STREAMS_AMOUNT];
        };

        struct {
            librg_void *data;
            usize size;
            usize count;
            zpl_buffer_t(librg_component_meta) headers;
        } components;

        struct {
            librg_table_t ignored;
            librg_entity_pool_t shared;
            librg_entity_pool_t native;
            zpl_array_t(librg_entity_t) remove_queue;
        } entity;

        // temp service stuff
        f32 last_update;

    } librg_ctx_t;

    librg_global u32 librg_options[LIBRG_OPTIONS_SIZE] = {
        /*LIBRG_PLATFORM_ID*/               1,
        /*LIBRG_PLATFORM_PROTOCOL*/         1,
        /*LIBRG_PLATFORM_BUILD*/            1,
        /*LIBRG_DEFAULT_CLIENT_TYPE*/       0,
        /*LIBRG_DEFAULT_DATA_SIZE*/         1024,
        /*LIBRG_NETWORK_CAPACITY*/          2048,
        /*LIBRG_NETWORK_CHANNELS*/          4,
        /*LIBRG_NETWORK_PRIMARY_CHANNEL*/   1,
        /*LIBRG_NETWORK_SECONDARY_CHANNEL*/ 2,
        /*LIBRG_NETWORK_MESSAGE_CHANNEL*/   3
    };


    /**
     * Set global cross-instance option for librg
     */
    LIBRG_API void librg_options_set(librg_options_e option, u32 value);

    /**
     * Get global cross-instance option for librg
     */
    LIBRG_API u32 librg_options_get(librg_options_e option);

    /**
     * Main initialization method
     * MUST BE called in the begging of your application
     */
    LIBRG_API void librg_init(librg_ctx_t *ctx, librg_component_cb component_callback);

    /**
     * Main tick method
     * MUST BE called in your loop
     * preferably w/o delays
     */
    LIBRG_API void librg_tick(librg_ctx_t *ctx);

    /**
     * Should be called at the end of
     * execution of the program
     */
    LIBRG_API void librg_free(librg_ctx_t *ctx);

    /**
     * Frees a pointer allocated by library
     * usually used in bindings.
     */
    LIBRG_API void librg_release(void *ptr);

    /**
     * Frees an array allocated by library
     * usually used in bindings.
     */
    LIBRG_API void librg_release_array(void *ptr);

    /**
     * Is librg instance is running
     * in the server mode
     */
    LIBRG_API b32 librg_is_server(librg_ctx_t *ctx);

    /**
     * Is librg instance is running
     * in the client mode
     */
    LIBRG_API b32 librg_is_client(librg_ctx_t *ctx);




    /**
     *
     * ENTITIES
     *
     */


    /**
     * Create entity and return handle
     */
    LIBRG_API librg_entity_t librg_entity_create(librg_ctx_t *ctx, u32 type);

    /**
     * Create shared entity and return handle
     */
    LIBRG_API librg_entity_t librg_entity_create_shared(librg_ctx_t *ctx, librg_entity_t remote, u32 type);

    /**
     * Check if provided entity is a valid entity
     */
    LIBRG_API b32 librg_entity_valid(librg_ctx_t *ctx, librg_entity_t entity);

    /**
     * Return entity type
     */
    LIBRG_API u32 librg_entity_type(librg_ctx_t *ctx, librg_entity_t entity);

    /**
     * Auto detach all attached components
     * and destroy entity
     */
    LIBRG_API void librg_entity_destroy(librg_ctx_t *ctx, librg_entity_t entity);


    /**
     * Query for entities that are in stream zone
     * for current entity, and are visible to this entity
     */
    LIBRG_API zpl_array_t(librg_entity_t) librg_entity_query(librg_ctx_t *ctx, librg_entity_t entity);

    /**
     * Query for entities that are in stream zone
     * for current entity, and are visible to this entity
     * Suitable for bindings as it returns raw pointer with size.
     * Free the `result` pointer using librg_release_array once you don't need it!
     */
    LIBRG_API usize librg_entity_query_raw(librg_ctx_t *ctx, librg_entity_t entity, librg_entity_t **result);

    /**
     * Get entity by peer
     */
    LIBRG_API librg_entity_t librg_entity_get(librg_ctx_t *ctx, librg_peer_t *peer);

    /**
     * Set particular entity visible or invisible
     * for other entities in stream zone
     */
    LIBRG_API void librg_entity_set_visible(librg_ctx_t *ctx, librg_entity_t entity, b32 state);

    /**
     * Set particular entity visible or invisible
     * for other particular entity
     */
    LIBRG_API void librg_entity_set_visible_for(librg_ctx_t *ctx, librg_entity_t entity, librg_entity_t target, b32 state);

    /**
     * Get particular entity visible or invisible
     * for other entities in stream zone
     */
    LIBRG_API b32 librg_entity_get_visible(librg_ctx_t *ctx, librg_entity_t entity);

    /**
     * Get particular entity visible or invisible
     * for other particular entity
     */
    LIBRG_API b32 librg_entity_get_visible_for(librg_ctx_t *ctx, librg_entity_t entity, librg_entity_t target);

    /**
     * Set some entity as client streamable
     * Which means, that client will become responsive for sending
     * updates about this entity
     *
     * And this entity wont be sent to the client, until he stops being the streamer
     *
     * Setting other client as streamer, will remove previous streamer from entity
     */
    LIBRG_API void librg_entity_control_set(librg_ctx_t *ctx, librg_entity_t entity, librg_peer_t *peer);

    /**
     * Get controller of the entity
     */
    LIBRG_API librg_peer_t * librg_entity_control_get(librg_ctx_t *ctx, librg_entity_t entity);

    /**
     * Remove some entity from stream ownership of the client
     */
    LIBRG_API void librg_entity_control_remove(librg_ctx_t *ctx, librg_entity_t entity);

    /**
     * Try to register provided component
     */
    LIBRG_API void librg_component_register(librg_ctx_t *ctx, u32 index, usize component_size);

    /**
     * Try to attach provided component to/from a particular entity
     */
    LIBRG_API librg_void *librg_component_attach(librg_ctx_t *ctx, u32 index, librg_entity_t entity, librg_void *data);

    /**
     * Try to fetch provided component to/from a particular entity
     */
    LIBRG_API librg_void *librg_component_fetch(librg_ctx_t *ctx, u32 index, librg_entity_t entity);

    /**
     * Try to detach provided component to/from a particular entity
     */
    LIBRG_API void librg_component_detach(librg_ctx_t *ctx, u32 index, librg_entity_t entity);

    /**
     * Try to interate on each entity with particular component
     */
    LIBRG_API void librg_component_each(librg_ctx_t *ctx, u32 index, librg_entity_cb callback);

    /**
     * Try to interate on each entity with provided component filter
     */
    LIBRG_API void librg_entity_each(librg_ctx_t *ctx, librg_filter_t filter, librg_entity_cb callback);

    /**
     * Try to interate on each entity with particular component
     * (macro version, can be used in C/C++ based projects)
     */
    #define librg_component_eachx(ctx, index, name, code) do {  \
        librg_assert(ctx); librg_assert(index < ctx->components.count); \
        librg_component_meta *header = &ctx->components.headers[index]; librg_assert(header);   \
        for (isize i = 0; i < zpl_buffer_count(header->used); i++)  \
            if (header->used[i]) { librg_entity_t name = i; code; } \
    } while(0);

    /**
     * Try to interate on each entity with provided component filter
     * (macro version, can be used in C/C++ based projects)
     */
    #define librg_entity_eachx(ctx, filter, name, code) do {                                                        \
        librg_assert(ctx); if (filter.contains1 == 0) break;                                                        \
        for (usize _ent = 0, valid = 0; valid < (ctx->entity.native.count + ctx->entity.shared.count)               \
            && _ent < (librg_is_server(ctx) ? ctx->max_entities : ctx->max_entities * 2); _ent++) {                 \
            /* check if entity valid */                                                                             \
            if (!ctx->components.headers[librg_meta].used[_ent]) continue; else valid++;                            \
            b32 _used = true;                                                                                       \
            /* check for included components */                                                                     \
            for (isize k = 0; k < 8 && _used; k++) {                                                                \
                if (filter.contains[k] == 0) break;                                                                 \
                librg_component_meta *header = &ctx->components.headers[filter.contains[k]]; librg_assert(header);  \
                if (!header->used[_ent]) { _used = false; }                                                         \
            }                                                                                                       \
            /* check for excluded components */                                                                     \
            for (isize k = 0; k < 4 && _used; k++) {                                                                \
                if (filter.excludes[k] == 0) break;                                                                 \
                librg_component_meta *header = &ctx->components.headers[filter.excludes[k]]; librg_assert(header);  \
                if (header->used[_ent]) { _used = false; }                                                          \
            }                                                                                                       \
            /* execute code */                                                                                      \
            if (_used) { librg_entity_t name = _ent; code; }                                                        \
        }                                                                                                           \
    } while(0)



    /**
     *
     * EVENTS
     *
     */


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
    LIBRG_API u64 librg_event_add(librg_ctx_t *ctx, u64 id, librg_event_cb callback);

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
    LIBRG_API void librg_event_trigger(librg_ctx_t *ctx, u64 id, librg_event_t *event);

    /**
     * Used to remove particular callback from
     * event chain, so it wont be called ever again
     *
     * @param id usually you define event ids inside enum
     * @param index returned by librg_event_add
     */
    LIBRG_API void librg_event_remove(librg_ctx_t *ctx, u64 id, u64 index);

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
     *
     * BINARY DATA (BITSTREAM)
     *
     */


    /**
     * Initialize new bitstream with default mem size
     */
    LIBRG_API void librg_data_init(librg_data_t *data);

    /**
     * Initialize new bitstream with custom mem size
     */
    LIBRG_API void librg_data_init_size(librg_data_t *data, usize size);

    /**
     * Free initialized bitstream
     */
    LIBRG_API void librg_data_free(librg_data_t *data);

    /**
     * Reset initialized bitstream
     * NOTE: doesnt remove any data, just resets read and write pos to 0
     */
    LIBRG_API void librg_data_reset(librg_data_t *data);

    /**
     * Increase size of bitstream
     */
    LIBRG_API void librg_data_grow(librg_data_t *data, usize min_size);

    /**
     * Methods for getting various parameters of bitstream
     */
    LIBRG_API usize librg_data_capacity(librg_data_t *data);
    LIBRG_API usize librg_data_get_rpos(librg_data_t *data);
    LIBRG_API usize librg_data_get_wpos(librg_data_t *data);
    LIBRG_API void  librg_data_set_rpos(librg_data_t *data, usize position);
    LIBRG_API void  librg_data_set_wpos(librg_data_t *data, usize position);

    /**
     * Read and write methods for custom sized data
     */
    LIBRG_API void librg_data_rptr(librg_data_t *data, void *ptr, usize size);
    LIBRG_API void librg_data_wptr(librg_data_t *data, void *ptr, usize size);

    /**
     * Read and write methods for custom sized data
     * at particular position in memory
     */
    LIBRG_API void librg_data_rptr_at(librg_data_t *data, void *ptr, usize size, isize position);
    LIBRG_API void librg_data_wptr_at(librg_data_t *data, void *ptr, usize size, isize position);

    /**
     * General one-line methods for reading/writing different types
     */
    #define LIBRG_GEN_DATA_READWRITE(TYPE) \
        LIBRG_API TYPE ZPL_JOIN2(librg_data_r,TYPE)(librg_data_t *data); \
        LIBRG_API void ZPL_JOIN2(librg_data_w,TYPE)(librg_data_t *data, TYPE value); \
        LIBRG_API TYPE ZPL_JOIN3(librg_data_r,TYPE,_at)(librg_data_t *data, isize position); \
        LIBRG_API void ZPL_JOIN3(librg_data_w,TYPE,_at)(librg_data_t *data, TYPE value, isize position); \

        LIBRG_GEN_DATA_READWRITE( i8);
        LIBRG_GEN_DATA_READWRITE( u8);
        LIBRG_GEN_DATA_READWRITE(i16);
        LIBRG_GEN_DATA_READWRITE(u16);
        LIBRG_GEN_DATA_READWRITE(i32);
        LIBRG_GEN_DATA_READWRITE(u32);
        LIBRG_GEN_DATA_READWRITE(i64);
        LIBRG_GEN_DATA_READWRITE(u64);
        LIBRG_GEN_DATA_READWRITE(f32);
        LIBRG_GEN_DATA_READWRITE(f64);
        LIBRG_GEN_DATA_READWRITE( b8);
        LIBRG_GEN_DATA_READWRITE(b16);
        LIBRG_GEN_DATA_READWRITE(b32);

    #undef LIBRG_GEN_DATA_READWRITE

    /**
     * Read/write methods for entity (aliases for u32)
     */
    #define librg_data_rentity librg_data_ru32
    #define librg_data_wentity librg_data_wu32



    /**
     *
     * NETWORK
     *
     */


    /**
     * Check are we connected
     */
    LIBRG_API b32 librg_is_connected(librg_ctx_t *ctx);

    /**
     * Starts network connection
     * Requires you to provide .port (if running as server)
     * or both .port and .host (if running as client)
     *
     * For server mode - starts server
     * For client mode - starts client, and connects to provided host & port
     */
    LIBRG_API void librg_network_start(librg_ctx_t *ctx, librg_address_t address);

    /**
     * Disconnects (if connected), stops network
     * and releases resources
     */
    LIBRG_API void librg_network_stop(librg_ctx_t *ctx);

    /**
     * Can be used to add handler
     * to a particular message id
     */
    LIBRG_API void librg_network_add(librg_ctx_t *ctx, u64 id, librg_message_cb callback);

    /**
     * Can be used to remove a handler
     * from particular message id
     */
    LIBRG_API void librg_network_remove(librg_ctx_t *ctx, u64 id);

    /**
     * Part of message API
     * Takes in initialized void of size pointer with written packet id
     * and sends data to all connected peers ( or to server if its client )
     */
    LIBRG_API void librg_message_send_all(librg_ctx_t *ctx, librg_void *data, usize size);

    /**
     * Part of message API
     * Applies all from previous mehod
     * But data will be sent only to particular provided peer
     */
    LIBRG_API void librg_message_send_to(librg_ctx_t *ctx, librg_peer_t *peer, librg_void *data, usize size);

    /**
     * Part of message API
     * Applies all from previous mehod
     * But data will be sent to all except provided peer
     */
    LIBRG_API void librg_message_send_except(librg_ctx_t *ctx, librg_peer_t *peer, librg_void *data, usize size);

    /**
     * Part of message API
     * Applies all from previous mehod
     * Data will be sent only to entities, which are inside streamzone
     * for provided entity
     */
    LIBRG_API void librg_message_send_instream(librg_ctx_t *ctx, librg_entity_t entity, librg_void *data, usize size);

    /**
     * Part of message API
     * Applies all from previous mehod
     * Data will be sent only to entities, which are inside streamzone
     * for provided entity except peer
     */
    LIBRG_API void librg_message_send_instream_except(librg_ctx_t *ctx, librg_entity_t entity, librg_peer_t *peer, librg_void *data, usize size);


    /**
     *
     * EXTENSIONS
     *
     */

    #define librg_send_all(CTX, ID, NAME, CALLBACK_CODE) do {            \
        librg_data_t NAME; librg_data_init(&NAME);                  \
        librg_data_wu64(&NAME, ID); CALLBACK_CODE;                  \
        librg_message_send_all(CTX, NAME.rawptr, librg_data_get_wpos(&NAME));   \
        librg_data_free(&NAME);                                     \
    } while(0);

    #define librg_send_to(CTX, ID, PEER, NAME, CALLBACK_CODE) do {       \
        librg_data_t NAME; librg_data_init(&NAME);                  \
        librg_data_wu64(&NAME, ID); CALLBACK_CODE;                  \
        librg_message_send_to(CTX, PEER, NAME.rawptr,                           \
            librg_data_get_wpos(&NAME));                            \
        librg_data_free(&NAME);                                     \
    } while(0);

    #define librg_send_except(CTX, ID, PEER, NAME, CALLBACK_CODE) do {   \
        librg_data_t NAME; librg_data_init(&NAME);                  \
        librg_data_wu64(&NAME, ID); CALLBACK_CODE;                  \
        librg_message_send_except(CTX, PEER, NAME.rawptr,                       \
            librg_data_get_wpos(&NAME));                            \
        librg_data_free(&NAME);                                     \
    } while(0);

    #define librg_send_instream(CTX, ID, ENTITY, NAME, CALLBACK_CODE) do { \
        librg_data_t NAME; librg_data_init(&NAME);                  \
        librg_data_wu64(&NAME, ID); CALLBACK_CODE;                  \
        librg_message_send_instream(CTX, ENTITY, NAME.rawptr,                   \
            librg_data_get_wpos(&NAME));                            \
        librg_data_free(&NAME);                                     \
    } while(0);

    #define librg_send_instream_except(CTX, ID, ENTITY, PEER, NAME, CALLBACK_CODE) do { \
        librg_data_t NAME; librg_data_init(&NAME);                  \
        librg_data_wu64(&NAME, ID); CALLBACK_CODE;                  \
        librg_message_send_instream_except(CTX, ENTITY, PEER, NAME.rawptr,      \
            librg_data_get_wpos(&NAME));                            \
        librg_data_free(&NAME);                                     \
    } while(0);


    #define librg_send librg_send_all

    static librg_inline librg_client_t *librg_fetch_client(librg_ctx_t *ctx, librg_entity_t entity) {
        return (librg_client_t *)librg_component_fetch(ctx, librg_client, entity);
    }

    static librg_inline librg_transform_t *librg_fetch_transform(librg_ctx_t *ctx, librg_entity_t entity) {
        return (librg_transform_t *)librg_component_fetch(ctx, librg_transform, entity);
    }

    static librg_inline librg_meta_t *librg_fetch_meta(librg_ctx_t *ctx, librg_entity_t entity) {
        return (librg_meta_t *)librg_component_fetch(ctx, librg_meta, entity);
    }

    #define librg_component(NAME, INDEX, COMP) \
        static librg_inline COMP *ZPL_JOIN2(librg_attach_,NAME) (librg_ctx_t *ctx, librg_entity_t entity, COMP *component) { return (COMP *)librg_component_attach(ctx, INDEX, entity, (void *)component); } \
        static librg_inline COMP *ZPL_JOIN2(librg_fetch_ ,NAME) (librg_ctx_t *ctx, librg_entity_t entity) { return (COMP *)librg_component_fetch(ctx, INDEX, entity); } \
        static librg_inline void  ZPL_JOIN2(librg_detach_,NAME) (librg_ctx_t *ctx, librg_entity_t entity) { librg_component_detach(ctx, INDEX, entity); }

#ifdef __cplusplus
}
#endif

#if defined(LIBRG_IMPLEMENTATION) && !defined(LIBRG_IMPLEMENTATION_DONE)
#define LIBRG_IMPLEMENTATION_DONE

#ifdef __cplusplus
extern "C" {
#endif

    ZPL_TABLE_DEFINE(librg_table_t, librg_table_, u32);

    librg_inline void librg_options_set(librg_options_e option, u32 value) {
        librg_options[option] = value;
    }

    librg_inline u32 librg_options_get(librg_options_e option) {
        return librg_options[option];
    }


    /**
     *
     * EVENTS
     *
     */

    librg_inline u64 librg_event_add(librg_ctx_t *ctx, u64 id, librg_event_cb callback) {
        return zplev_add(&ctx->events, id, (zplev_cb *)callback);
    }

    void librg_event_trigger(librg_ctx_t *ctx, u64 id, librg_event_t *event) {
        librg_assert(event); event->ctx = ctx;
        zplev_block *block = zplev_pool_get(&ctx->events, id);
        if (!block) return;

        for (isize i = 0; i < zpl_array_count(*block) && !event->rejected; ++i) {
            (*block)[i](event);
        }
    }

    librg_inline void librg_event_remove(librg_ctx_t *ctx, u64 id, u64 index) {
        zplev_remove(&ctx->events, id, index);
    }

    librg_inline void librg_event_reject(librg_event_t *event) {
        librg_assert(event);
        event->rejected = true;
    }

    librg_inline b32 librg_event_succeeded(librg_event_t *event) {
        librg_assert(event);
        return !event->rejected;
    }



    /**
     *
     * DATA STREAM (BITSREAM)
     *
     */

    librg_inline void librg_data_init_size(librg_data_t *data, usize size) {
        librg_assert(data);

        data->capacity  = size;
        data->read_pos  = 0;
        data->write_pos = 0;
        data->allocator = zpl_heap_allocator();
        data->rawptr    = zpl_alloc(data->allocator, size);
    }

    librg_inline void librg_data_init(librg_data_t *data) {
        librg_assert(data);
        librg_data_init_size(data, librg_options_get(LIBRG_DEFAULT_DATA_SIZE));
    }

    librg_inline void librg_data_free(librg_data_t *data) {
        librg_assert(data && data->rawptr);
        zpl_free(data->allocator, data->rawptr);
    }

    librg_inline void librg_data_reset(librg_data_t *data) {
        librg_data_set_wpos(data, 0);
        librg_data_set_rpos(data, 0);
    }

    librg_inline void librg_data_grow(librg_data_t *data, usize min_size) {
        librg_assert(data && data->rawptr);

        usize new_capacity = ZPL_BS_GROW_FORMULA(data->capacity);
        if (new_capacity < (min_size))
            new_capacity = (min_size);

        void *newptr = zpl_alloc(data->allocator, new_capacity);

        zpl_memmove(newptr, data->rawptr, data->capacity);
        zpl_free(data->allocator, data->rawptr);

        data->capacity = new_capacity;
        data->rawptr = newptr;
    }

    librg_inline usize librg_data_capacity(librg_data_t *data) {
        librg_assert(data); return data->capacity;
    }

    librg_inline usize librg_data_get_rpos(librg_data_t *data) {
        librg_assert(data); return data->read_pos;
    }

    librg_inline usize librg_data_get_wpos(librg_data_t *data) {
        librg_assert(data); return data->write_pos;
    }

    librg_inline void librg_data_set_rpos(librg_data_t *data, usize position) {
        librg_assert(data); data->read_pos = position;
    }

    librg_inline void librg_data_set_wpos(librg_data_t *data, usize position) {
        librg_assert(data); data->write_pos = position;
    }


    /**
     * Pointer writers and readers
     */

    librg_inline void librg_data_rptr_at(librg_data_t *data, void *ptr, usize size, isize position) {
        librg_assert(data && data->rawptr && ptr);

        librg_assert_msg(position + size <= librg_data_capacity(data),
            "librg_data: trying to read from outside of the bounds");

        zpl_memcopy(ptr, (char *)data->rawptr + position, size);
    }

    librg_inline void librg_data_wptr_at(librg_data_t *data, void *ptr, usize size, isize position) {
        librg_assert(data && data->rawptr && ptr);

        if (position + size > librg_data_capacity(data)) {
            librg_data_grow(data, librg_data_capacity(data) + size + position);
        }

        zpl_memcopy((char *)data->rawptr + position, ptr, size);
    }

    librg_inline void librg_data_rptr(librg_data_t *data, void *ptr, usize size) {
        librg_assert(data && data->rawptr && ptr);
        librg_data_rptr_at(data, ptr, size, librg_data_get_rpos(data));
        data->read_pos += size;
    }

    librg_inline void librg_data_wptr(librg_data_t *data, void *ptr, usize size) {
        librg_assert(data && data->rawptr && ptr);
        librg_data_wptr_at(data, ptr, size, librg_data_get_wpos(data));
        data->write_pos += size;
    }

    /**
     * Value writers and readers
     */

    #define LIBRG_GEN_DATA_READWRITE(TYPE) \
        librg_inline TYPE ZPL_JOIN2(librg_data_r,TYPE)(librg_data_t *data) { \
            TYPE value; librg_data_rptr(data, &value, sizeof(value)); return value; \
        } \
        librg_inline void ZPL_JOIN2(librg_data_w,TYPE)(librg_data_t *data, TYPE value) { \
            librg_data_wptr(data, &value, sizeof(value)); \
        } \
        librg_inline TYPE ZPL_JOIN3(librg_data_r,TYPE,_at)(librg_data_t *data, isize position) { \
            TYPE value; librg_data_rptr_at(data, &value, sizeof(value), position); return value; \
        } \
        librg_inline void ZPL_JOIN3(librg_data_w,TYPE,_at)(librg_data_t *data, TYPE value, isize position) { \
            librg_data_wptr_at(data, &value, sizeof(value), position); \
        } \


        LIBRG_GEN_DATA_READWRITE( i8);
        LIBRG_GEN_DATA_READWRITE( u8);
        LIBRG_GEN_DATA_READWRITE(i16);
        LIBRG_GEN_DATA_READWRITE(u16);
        LIBRG_GEN_DATA_READWRITE(i32);
        LIBRG_GEN_DATA_READWRITE(u32);
        LIBRG_GEN_DATA_READWRITE(i64);
        LIBRG_GEN_DATA_READWRITE(u64);
        LIBRG_GEN_DATA_READWRITE(f32);
        LIBRG_GEN_DATA_READWRITE(f64);
        LIBRG_GEN_DATA_READWRITE( b8);
        LIBRG_GEN_DATA_READWRITE(b16);
        LIBRG_GEN_DATA_READWRITE(b32);

    #undef LIBRG_GEN_DATA_READWRITE



    /**
     *
     * NETWORK
     *
     */

    void librg_network_start(librg_ctx_t *ctx, librg_address_t addr) {
        librg_dbg("librg_network_start\n");

        librg_table_init(&ctx->network.connected_peers, ctx->allocator);

        if (librg_is_server(ctx)) {
            ENetAddress address;

            address.port = addr.port;
            address.host = ENET_HOST_ANY;

            // setup server host
            ctx->network.host = enet_host_create(&address, ctx->max_connections, librg_options_get(LIBRG_NETWORK_CHANNELS), 0, 0);
            librg_assert_msg(ctx->network.host, "could not start server at provided port");
        }
        else {
            ENetAddress address;

            address.port = addr.port;
            enet_address_set_host(&address, addr.host);

            // setup client host
            // TODO: add override for bandwidth
            ctx->network.host = enet_host_create(NULL, 1, librg_options_get(LIBRG_NETWORK_CHANNELS), 0, 0);
            librg_assert_msg(ctx->network.host, "could not start client");

            // create peer connecting to server
            librg_dbg("connecting to server %s:%u\n", addr.host, addr.port);
            ctx->network.peer = enet_host_connect(ctx->network.host, &address, librg_options_get(LIBRG_NETWORK_CHANNELS), 0);
            librg_assert_msg(ctx->network.peer, "could not setup peer for provided address");
        }
    }

    void librg_network_stop(librg_ctx_t *ctx) {
        librg_dbg("librg_network_stop\n");

        if (ctx->network.peer) {
            ENetEvent event;

            // disconnect and emit event
            enet_peer_disconnect(ctx->network.peer, 0);
            enet_host_service(ctx->network.host, &event, 100);

            // reset our peer
            enet_peer_reset(ctx->network.peer);
        }

        librg_table_destroy(&ctx->network.connected_peers);
    }

    /**
     * Network helpers
     */

    librg_inline b32 librg_is_server(librg_ctx_t *ctx) {
        return ctx->mode == LIBRG_MODE_SERVER;
    }

    librg_inline b32 librg_is_client(librg_ctx_t *ctx) {
        return ctx->mode == LIBRG_MODE_CLIENT;
    }

    librg_inline b32 librg_is_connected(librg_ctx_t *ctx) {
        return ctx->network.peer && ctx->network.peer->state == ENET_PEER_STATE_CONNECTED;
    }


    /**
     * Network messages
     */

    librg_inline void librg_network_add(librg_ctx_t *ctx, u64 id, librg_message_cb callback) {
        ctx->messages[id] = callback;
    }

    librg_inline void librg_network_remove(librg_ctx_t *ctx, u64 id) {
        ctx->messages[id] = NULL;
    }

    /**
     * Senders
     */

    void librg_message_send_all(librg_ctx_t *ctx, librg_void *data, usize size) {
        if (librg_is_client(ctx)) {
            librg_message_send_to(ctx, ctx->network.peer, data, size);
			return;
        }

        librg_message_send_except(ctx, NULL, data, size);
    }

    void librg_message_send_to(librg_ctx_t *ctx, librg_peer_t *peer, librg_void *data, usize size) {
        zpl_unused(ctx);
        enet_peer_send(peer, librg_options_get(LIBRG_NETWORK_MESSAGE_CHANNEL),
            enet_packet_create(data, size, ENET_PACKET_FLAG_RELIABLE)
        );
    }

    void librg_message_send_except(librg_ctx_t *ctx, librg_peer_t *peer, librg_void *data, usize size) {
        librg_filter_t filter = { librg_client };

        // fixme
        librg_entity_eachx(ctx, filter, librg_lambda(entity), {
            librg_client_t *client = librg_fetch_client(ctx, entity);

            if (client->peer != peer) {
                enet_peer_send(client->peer, librg_options_get(LIBRG_NETWORK_MESSAGE_CHANNEL),
                    enet_packet_create(data, size, ENET_PACKET_FLAG_RELIABLE)
                );
            }
        });
    }

    librg_inline void librg_message_send_instream(librg_ctx_t *ctx, librg_entity_t entity, librg_void *data, usize size) {
        librg_message_send_instream_except(ctx, entity, NULL, data, size);
    }

    void librg_message_send_instream_except(librg_ctx_t *ctx, librg_entity_t entity, librg_peer_t * ignored, librg_void *data, usize size) {
        zpl_array_t(librg_entity_t) queue = librg_entity_query(ctx, entity);

        for (isize i = 0; i < zpl_array_count(queue); i++) {
            librg_entity_t target = queue[i];

            librg_client_t *client = librg_fetch_client(ctx, target);
            if (!client) continue;

            librg_peer_t *peer = client->peer;
            librg_assert(peer);

            if (peer == ignored) {
                continue;
            }

            enet_peer_send(peer, librg_options_get(LIBRG_NETWORK_MESSAGE_CHANNEL),
                enet_packet_create(data, size, ENET_PACKET_FLAG_RELIABLE)
            );
        }

        zpl_array_free(queue);
    }

    /**
     * Main ticker
     */

    void librg_tick(librg_ctx_t *ctx) {
        zpl_timer_update(ctx->timers);

        if (!ctx->network.host) {
            return; /* occasion where we are not started network yet */
        }

        ENetEvent event;
        librg_data_t data;
        librg_data_init(&data);

        while (enet_host_service(ctx->network.host, &event, 0) > 0) {
            librg_message_t msg = {0};
            msg.ctx = ctx;
            msg.data = NULL;
            msg.peer = event.peer;
            msg.packet = event.packet;

            switch (event.type) {
                case ENET_EVENT_TYPE_RECEIVE: {
                    // read our data (TODO: remove copying, insert raw poninter from enet)
                    librg_data_wptr(&data,
                        event.packet->data,
                        event.packet->dataLength
                    );

                    // get curernt packet id
                    u64 id = librg_data_ru64(&data);

                    if (ctx->messages[id]) {
                        msg.data = &data;
                        ctx->messages[id](&msg);
                    }
                    else {
                        librg_dbg("network: unknown message: %llu\n", id);
                    }

                    librg_data_reset(&data);
                    enet_packet_destroy(event.packet);
                } break;
                case ENET_EVENT_TYPE_CONNECT:    ctx->messages[LIBRG_CONNECTION_INIT](&msg); break;
                case ENET_EVENT_TYPE_DISCONNECT: ctx->messages[LIBRG_CONNECTION_DISCONNECT](&msg); break;
                case ENET_EVENT_TYPE_NONE: break;
            }
        }

        librg_data_free(&data);
    }





    /**
     *
     * ENTITIES
     *
     */


    librg_inline b32 librg_entity_valid(librg_ctx_t *ctx, librg_entity_t entity) {
        return (librg_component_fetch(ctx, librg_meta, entity) != NULL);
    }

    void librg_component_register(librg_ctx_t *ctx, u32 index, usize component_size) {
        librg_assert(ctx); librg_assert(component_size);
        librg_assert_msg(ctx->components.count == index, "you should register components in order");

        librg_component_meta *header = &ctx->components.headers[index]; librg_assert(header);
        usize size = component_size * ctx->max_entities;

        header->offset = ctx->components.size;
        header->size   = component_size;

        ctx->components.size += size;
        ctx->components.count++;

        zpl_buffer_init(header->used, ctx->allocator, ctx->max_entities);
    }

    librg_void *librg_component_attach(librg_ctx_t *ctx, u32 index, librg_entity_t entity, librg_void *data) {
        librg_component_meta *header = &ctx->components.headers[index];
        librg_assert_msg(header && header->size, "make sure you registered component you are trying to use");
        header->used[entity] = true;
        librg_void *cdata = ctx->components.data + header->offset;
        if (data == NULL) zpl_memset(&cdata[entity * header->size], 0, (usize)header->size);
        else zpl_memcopy(&cdata[entity * header->size], data, (usize)header->size);
        return &cdata[entity * header->size];
    }

    librg_inline librg_void *librg_component_fetch(librg_ctx_t *ctx, u32 index, librg_entity_t entity) {
        librg_component_meta *header = &ctx->components.headers[index]; librg_assert(header);
        librg_void *cdata = ctx->components.data + header->offset;
        return header->used[entity] ? &cdata[entity * header->size] : NULL;
    }

    librg_inline void librg_component_detach(librg_ctx_t *ctx, u32 index, librg_entity_t entity) {
        librg_component_meta *header = &ctx->components.headers[index]; librg_assert(header);
        header->used[entity] = false;
    }

    librg_inline void librg_component_each(librg_ctx_t *ctx, u32 index, librg_entity_cb callback) {
        librg_component_eachx(ctx, index, entity, { callback(ctx, entity); });
    }

    void librg_entity_each(librg_ctx_t *ctx, librg_filter_t filter, librg_entity_cb callback) {
        librg_entity_eachx(ctx, filter, entity, { callback(ctx, entity); });
    }

    librg_inline u32 librg_entity_type(librg_ctx_t *ctx, librg_entity_t entity) {
        librg_meta_t *meta = librg_fetch_meta(ctx, entity); librg_assert(meta);
        return meta->type;
    }


    /**
     * Entity create methods
     *
     * Attaches some default components
     * and inits custom data storages inside (if needed)
     */

    librg_inline void librg__entity_attach_default(librg_ctx_t *ctx, librg_entity_t entity) {
        librg_transform_t t = {0};
        librg_stream_t    s = {250};
        librg_meta_t      m = {0};

        librg_component_attach(ctx, librg_transform, entity, (librg_void *)&t);
        librg_component_attach(ctx, librg_stream,    entity, (librg_void *)&s);
        librg_component_attach(ctx, librg_meta,      entity, (librg_void *)&m);
    }

    librg_inline librg_entity_t librg__entity_create(librg_ctx_t *ctx, librg_entity_pool_t *pool) {
        librg_assert_msg(++pool->count < pool->limit_upper, "entity limit");

        if (pool->cursor == pool->limit_upper || pool->cursor == 0) {
            pool->cursor = pool->limit_lower;
        }

        for (; pool->cursor < pool->limit_upper; ++pool->cursor) {
            librg_entity_t entity = pool->cursor;
            if (librg_entity_valid(ctx, entity)) continue;
            librg__entity_attach_default(ctx, entity);

            if (librg_is_server(ctx)) {
                librg_table_init(&librg_fetch_meta(ctx, entity)->ignored, ctx->allocator);
            }

            return entity;
        }

        librg_assert_msg(false, "no entities to spawn");
        return 0;
    }

    librg_entity_t librg_entity_create(librg_ctx_t *ctx, u32 type) {
        librg_entity_pool_t *pool = &ctx->entity.shared;

        if (librg_is_client(ctx)) {
            pool = &ctx->entity.native;
        }

        librg_entity_t entity = librg__entity_create(ctx, pool);
        librg_fetch_meta(ctx, entity)->type = type;
        return entity;
    }

    librg_entity_t librg_entity_create_shared(librg_ctx_t *ctx, u32 entity, u32 type) {
        librg_assert_msg(librg_is_client(ctx), "librg_entity_create_shared: can be executed only on client");
        librg_assert_msg(!librg_entity_valid(ctx, entity), "entity with such id already exsits");

        librg_entity_pool_t *pool = &ctx->entity.shared;
        librg_assert_msg(++pool->count < pool->limit_upper, "entity limit");
        librg__entity_attach_default(ctx, entity);
        librg_fetch_meta(ctx, entity)->type = type;

        return entity;
    }

    /**
     * Entity destructors
     */

    librg_internal void librg__entity_destroy(librg_ctx_t *ctx, librg_entity_t entity) {
        librg_entity_pool_t *pool = &ctx->entity.shared;
        if (entity > ctx->entity.shared.limit_upper) {
            pool = &ctx->entity.native;
        }

        librg_assert(librg_entity_valid(ctx, entity));

        if (librg_is_server(ctx)) {
            librg_table_destroy(&librg_fetch_meta(ctx, entity)->ignored);
            librg_entity_set_visible(ctx, entity, true);
        }

        // decrease amount
        pool->count--;

        // detach all components
        for (usize i = 0; i < ctx->components.count; i++) {
            ctx->components.headers[i].used[entity] = false;
        }
    }

    void librg_entity_destroy(librg_ctx_t *ctx, librg_entity_t id) {
        if (librg_is_client(ctx)) {
            librg__entity_destroy(ctx, id);
			return;
        }

        zpl_array_append(ctx->entity.remove_queue, id);
    }


    void librg_entity_set_visible(librg_ctx_t *ctx, librg_entity_t entity, b32 state) {
        librg_table_set(&ctx->entity.ignored, entity, (u32)!state);
    }

    void librg_entity_set_visible_for(librg_ctx_t *ctx, librg_entity_t entity, librg_entity_t target, b32 state) {
        librg_meta_t *meta = librg_fetch_meta(ctx, entity); librg_assert(meta);
        librg_table_set(&meta->ignored, target, (u32)!state);
    }

    zpl_array_t(librg_entity_t) librg_entity_query(librg_ctx_t *ctx, librg_entity_t entity) {
        // TODO: optimize the query. add preallocated array
        zpl_array_t(zplc_node_t) search_temp;
        zpl_array_t(librg_entity_t) search_result;

        zpl_array_init(search_temp, ctx->allocator);
        zpl_array_init(search_result, ctx->allocator);

        librg_transform_t *transform = (librg_transform_t *)librg_component_fetch(ctx, librg_transform, entity);
        librg_stream_t    *stream    = (librg_stream_t *)   librg_component_fetch(ctx, librg_stream, entity);
        librg_assert(transform && stream);

        zplc_bounds_t search_bounds;
        search_bounds.centre = transform->position;
        search_bounds.half_size = zplm_vec3((f32)stream->range, (f32)stream->range, (f32)stream->range);

        zplc_query(&ctx->streamer, search_bounds, &search_temp);

        for (isize i = 0; i < zpl_array_count(search_temp); i++) {
            librg_entity_t target = (u32)search_temp[i].tag;
            if (!librg_entity_valid(ctx, target)) continue;

            u32 *global = librg_table_get(&ctx->entity.ignored, target);
            u32 *local  = librg_table_get(&librg_fetch_meta(ctx, target)->ignored, entity);

            if ((global && *global) || (local && *local)) continue;

            zpl_array_append(search_result, target);
        }

        zpl_array_free(search_temp);
        return search_result;
    }

    usize librg_entity_query_raw(librg_ctx_t *ctx, librg_entity_t entity, librg_entity_t **result) {
        librg_assert(result);
        usize size = 0;
        zpl_array_t(librg_entity_t) array = librg_entity_query(ctx, entity);
        size = zpl_array_count(array) * sizeof(librg_entity_t);
        *result = array;

        return size;
    }

    librg_entity_t librg_entity_get(librg_ctx_t *ctx, librg_peer_t *peer) {
        librg_assert(ctx && peer);
        librg_entity_t *entity = librg_table_get(&ctx->network.connected_peers, (u64)peer);
        librg_assert(entity);

        return *entity;
    }

    void librg_entity_control_set(librg_ctx_t *ctx, librg_entity_t entity, librg_peer_t *peer) {
        librg_assert(ctx && peer);
        librg_control_t *control = cast(librg_control_t *)librg_component_fetch(ctx, librg_control, entity);

        // replace current entity owner
        if (control) {
            if (control->peer == peer) {
                return;
            }

            control->peer = peer;
        }
        // attach new entity owner
        else {
            librg_control_t cs = { peer };
            librg_component_attach(ctx, librg_control, entity, (librg_void *)&cs);
        }

        librg_send_to(ctx, LIBRG_CLIENT_STREAMER_ADD, peer, librg_lambda(data), {
            librg_data_wentity(&data, entity);
        });
    }

    void librg_entity_control_remove(librg_ctx_t *ctx, librg_entity_t entity) {
        librg_assert(ctx);
        librg_control_t *control = cast(librg_control_t *)librg_component_fetch(ctx, librg_control, entity);

        if (!control) {
            return;
        }

        librg_send_to(ctx, LIBRG_CLIENT_STREAMER_REMOVE, control->peer, librg_lambda(data), {
            librg_data_wentity(&data, entity);
        });

        librg_component_detach(ctx, librg_control, entity);
    }

    /**
     *
     * NETWORK BUILTIN CALLBACKS
     *
     */


    /**
     * SHARED
     */
    librg_internal void librg__callback_connection_init(librg_message_t *msg) {
        librg_dbg("librg__connection_init\n");

        #if defined(LIBRG_DEBUG)
        char my_host[16];

        enet_address_get_host_ip(&msg->peer->address, my_host, 16);
        librg_dbg("librg__connection_init: a new connection attempt at %s:%u.\n", my_host, msg->peer->address.port);
        #endif

        if (librg_is_client(msg->ctx)) {
            librg_send_to(msg->ctx, LIBRG_CONNECTION_REQUEST, msg->peer, librg_lambda(data), {
                librg_data_wu16(&data, librg_options_get(LIBRG_PLATFORM_ID));
                librg_data_wu16(&data, librg_options_get(LIBRG_PLATFORM_BUILD));
                librg_data_wu16(&data, librg_options_get(LIBRG_PLATFORM_PROTOCOL));

                librg_event_t event = { 0 }; event.data = &data;
                librg_event_trigger(msg->ctx, LIBRG_CONNECTION_REQUEST, &event);
            });
        }
    }

    /**
     * SERVER SIDE
     */
    librg_internal void librg__callback_connection_request(librg_message_t *msg) {
        librg_dbg("librg__connection_request\n");

        u16 platform_id       = librg_data_ru16(msg->data);
        u16 platform_build    = librg_data_ru16(msg->data);
        u16 platform_protocol = librg_data_ru16(msg->data);

        b32 blocked = (platform_id != librg_options_get(LIBRG_PLATFORM_ID) || platform_protocol != librg_options_get(LIBRG_PLATFORM_PROTOCOL));

        if (platform_build != librg_options_get(LIBRG_PLATFORM_BUILD)) {
            librg_dbg("NOTICE: librg platform build mismatch client %u, server: %u\n", platform_build, librg_options_get(LIBRG_PLATFORM_BUILD));
        }

        librg_event_t event = { 0 }; event.data = msg->data;
        librg_event_trigger(msg->ctx, LIBRG_CONNECTION_REQUEST, &event);

        if (librg_event_succeeded(&event) && !blocked) {
            librg_entity_t entity = librg_entity_create(msg->ctx, librg_options_get(LIBRG_DEFAULT_CLIENT_TYPE));

            // assign default compoenents
            librg_client_t client = { msg->peer };
            librg_client_t *c = (librg_client_t *)librg_component_attach(msg->ctx, librg_client, entity, (librg_void *)&client);
            librg_table_init(&c->last_snapshot, msg->ctx->allocator);

            // add client peer to storage
            librg_table_set(&msg->ctx->network.connected_peers, cast(u64)msg->peer, entity);

            // send accept
            librg_send_to(msg->ctx, LIBRG_CONNECTION_ACCEPT, msg->peer, librg_lambda(data), {
                librg_data_wu32(&data, entity);
            });

            librg_event_t acptevt = { 0 }; acptevt.entity = entity;
            librg_event_trigger(msg->ctx, LIBRG_CONNECTION_ACCEPT, &acptevt);
        }
        else {
            librg_send_to(msg->ctx, LIBRG_CONNECTION_REFUSE, msg->peer, librg_lambda(data), {});

            librg_event_t rfsevt = { 0 };
            librg_event_trigger(msg->ctx, LIBRG_CONNECTION_REFUSE, &rfsevt);
        }
    }

    /**
     * CLIENT SIDE
     */
    librg_internal void librg__callback_connection_refuse(librg_message_t *msg) {
        librg_event_t event = { 0 }; event.data = msg->data;
        librg_event_trigger(msg->ctx, LIBRG_CONNECTION_REFUSE, &event);
    }

    /**
     * CLIENT SIDE
     */
    librg_internal void librg__callback_connection_accept(librg_message_t *msg) {
        librg_dbg("librg__connection_accept\n");

        librg_entity_t remote = librg_data_ru32(msg->data);
        librg_entity_t entity = librg_entity_create_shared(msg->ctx, remote, LIBRG_DEFAULT_CLIENT_TYPE);

        // add server peer to storage
        librg_table_set(&msg->ctx->network.connected_peers, cast(u64)msg->peer, entity);

        librg_event_t event = { 0 };
        event.data = msg->data; event.entity = entity;
        librg_event_trigger(msg->ctx, LIBRG_CONNECTION_ACCEPT, &event);
    }

    /**
     * SHARED
     */
    librg_internal void librg__callback_connection_disconnect(librg_message_t *msg) {
        librg_dbg("librg__connection_disconnect\n");

        if (librg_is_server(msg->ctx)) {
            librg_entity_t *entity = librg_table_get(&msg->ctx->network.connected_peers, cast(u64)msg->peer);
            if (!entity || !librg_entity_valid(msg->ctx, *entity)) return;

            librg_event_t event = {0};
            event.entity = *entity; event.userptr = (void *)msg->peer;
            librg_event_trigger(msg->ctx, LIBRG_CONNECTION_DISCONNECT, &event);

            librg_table_destroy(&librg_fetch_client(msg->ctx, *entity)->last_snapshot);
            librg_component_detach(msg->ctx, librg_client, *entity);
            librg_entity_destroy(msg->ctx, *entity);
        }
        else {
            librg_event_t event = {0};
            librg_event_trigger(msg->ctx, LIBRG_CONNECTION_DISCONNECT, &event);
        }
    }

    librg_internal void librg__callback_entity_create(librg_message_t *msg) {
        u32 query_size = librg_data_ru32(msg->data);

        for (usize i = 0; i < query_size; ++i) {
            librg_entity_t entity = librg_data_rentity(msg->data);
            u32 type = librg_data_ru32(msg->data);

            librg_transform_t transform;
            librg_data_rptr(msg->data, &transform, sizeof(transform));

            librg_entity_create_shared(msg->ctx, entity, type);
            *librg_fetch_transform(msg->ctx, entity) = transform;

            librg_event_t event = {0};
            event.data = msg->data; event.entity = entity;
            librg_event_trigger(msg->ctx, LIBRG_ENTITY_CREATE, &event);
        }

        u32 remove_size = librg_data_ru32(msg->data);

        for (usize i = 0; i < remove_size; ++i) {
            librg_entity_t entity = librg_data_rentity(msg->data);

            if (librg_entity_valid(msg->ctx,entity)) {
                librg_event_t event = {0};
                event.data = msg->data; event.entity = entity;
                librg_event_trigger(msg->ctx, LIBRG_ENTITY_REMOVE, &event);
                librg_entity_destroy(msg->ctx, entity);
            }
            else {
                librg_dbg("unexpected entity %u on remove\n", entity);
            }
        }
    }

    librg_internal void librg__callback_entity_update(librg_message_t *msg) {
        u32 query_size = librg_data_ru32(msg->data);

        for (usize i = 0; i < query_size; ++i) {
            librg_entity_t entity = librg_data_ru32(msg->data);

            librg_transform_t transform;
            librg_data_rptr(msg->data, &transform, sizeof(transform));

            if (!librg_entity_valid(msg->ctx, entity)) {
                continue;
            }

            *librg_fetch_transform(msg->ctx, entity) = transform;

            librg_event_t event = {0};
            event.data = msg->data; event.entity = entity;
            librg_event_trigger(msg->ctx, LIBRG_ENTITY_UPDATE, &event);
        }
    }

    librg_internal void librg__callback_entity_client_streamer_add(librg_message_t *msg) {
        librg_entity_t entity = librg_data_ru32(msg->data);

        if (!librg_entity_valid(msg->ctx, entity)) {
            librg_dbg("trying to add unknown entity to clientstream!");
            return;
        }

        librg_control_t *control = cast(librg_control_t *)librg_component_fetch(msg->ctx, librg_control, entity);

        if (!control) {
            librg_control_t cs = {0};
            librg_component_attach(msg->ctx, librg_control, entity, (librg_void *)&cs);

            librg_event_t event = {0};
            event.data = msg->data; event.entity = entity;
            librg_event_trigger(msg->ctx, LIBRG_CLIENT_STREAMER_ADD, &event);
        }
    }

    librg_internal void librg__callback_entity_client_streamer_remove(librg_message_t *msg) {
        librg_entity_t entity = librg_data_ru32(msg->data);

        if (!librg_entity_valid(msg->ctx, entity)) {
            librg_dbg("trying to remove unknown entity from clientstream!\n");
            return;
        }

        librg_control_t *control = cast(librg_control_t *)librg_component_fetch(msg->ctx, librg_control, entity);

        if (control) {
            librg_component_detach(msg->ctx, librg_control, entity);

            librg_event_t event = {0};
            event.data = msg->data; event.entity = entity;
            librg_event_trigger(msg->ctx, LIBRG_CLIENT_STREAMER_REMOVE, &event);
        }
    }

    librg_internal void librg__callback_entity_client_streamer_update(librg_message_t *msg) {
        u32 amount = librg_data_ru32(msg->data);

        for (usize i = 0; i < amount; i++) {
            librg_entity_t entity = librg_data_ru32(msg->data);
            u32 size = librg_data_ru32(msg->data);

            if (!librg_entity_valid(msg->ctx, entity)) {
                librg_dbg("invalid entity on client streamer update\n");
                librg_data_set_rpos(msg->data, librg_data_get_rpos(msg->data) + size);
                librg_assert(false);
                continue;
            }

            librg_control_t *control = cast(librg_control_t *)librg_component_fetch(msg->ctx, librg_control, entity);

            if (!control || control->peer != msg->peer) {
                librg_dbg("no component, or peer is different\n");
                librg_data_set_rpos(msg->data, librg_data_get_rpos(msg->data) + size);
                continue;
            }

            librg_event_t event = {0};
            event.data = msg->data; event.entity = entity;
            librg_event_trigger(msg->ctx, LIBRG_CLIENT_STREAMER_UPDATE, &event);

            librg_transform_t transform;
            librg_data_rptr(msg->data, &transform, sizeof(transform));
            *librg_fetch_transform(msg->ctx, entity) = transform;
        }
    }



    /**
     * CLIENT-SIDE
     *
     * Responsive for updating the client side streamer
     */
    librg_internal void librg__execute_client_update(librg_ctx_t *ctx) {
        u32 amount = 0;
        librg_data_t data;
        librg_data_init(&data);

        librg_data_wu64(&data, LIBRG_CLIENT_STREAMER_UPDATE);
        librg_data_wu32(&data, 0); // amount of entities to be sent (updates)

        librg_filter_t filter = {
            librg_control, librg_stream,
        };

        librg_entity_eachx(ctx, filter, librg_lambda(entity), {
            librg_transform_t *transform = librg_fetch_transform(ctx, entity);

            librg_data_t subdata;
            librg_data_init(&subdata);

            librg_event_t event = {0};
            event.data = &subdata; event.entity = entity;
            librg_event_trigger(ctx, LIBRG_CLIENT_STREAMER_UPDATE, &event);

            librg_data_wptr(&subdata, transform, sizeof(librg_transform_t));
            librg_data_wu32(&data, entity);
            librg_data_wu32(&data, librg_data_get_wpos(&subdata));

            // write sub-bitstream to main bitstream
            librg_data_wptr(&data, subdata.rawptr, librg_data_get_wpos(&subdata));
            librg_data_free(&subdata);

            amount++;
        });

        if (amount < 1) {
            return;
        }

        // write amountafter packet id
        librg_data_wu32_at(&data, amount, sizeof(u64));

        enet_peer_send(ctx->network.peer, librg_options_get(LIBRG_NETWORK_SECONDARY_CHANNEL),
            enet_packet_create(data.rawptr, librg_data_get_wpos(&data), ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT)
        );

        librg_data_free(&data);
    }

    /**
     * SERVER-SIDE
     *
     * Responsive for udpating the server-side streamer
     */
    librg_internal void librg__execute_server_entity_update(librg_ctx_t *ctx) {
        librg_filter_t filter = { librg_client };

        // create data and write inital stuff
        librg_data_t for_create;
        librg_data_t for_update;

        librg_data_init(&for_create);
        librg_data_init(&for_update);

        librg_entity_eachx(ctx, filter, librg_lambda(player), {
            librg_client_t *client = librg_fetch_client(ctx, player);

            // get old, and preapre new snapshot handlers
            librg_table_t *last_snapshot = &client->last_snapshot;
            librg_table_t next_snapshot = { 0 };
            librg_table_init(&next_snapshot, ctx->allocator);

            // fetch entities in the steram zone
            zpl_array_t(librg_entity_t) queue = librg_entity_query(ctx, player);

            u32 created_entities = 0;
            u32 updated_entities = (u32) zpl_array_count(queue);
            u32 removed_entities = 0;

            // write packet headers
            librg_data_wu64(&for_create, LIBRG_ENTITY_CREATE);
            librg_data_wu32(&for_create, created_entities);

            librg_data_wu64(&for_update, LIBRG_ENTITY_UPDATE);
            librg_data_wu32(&for_update, updated_entities);

            // add entity creates and updates
            for (isize i = 0; i < zpl_array_count(queue); ++i) {
                librg_entity_t entity = (u32)queue[i];

                // fetch value of entity in the last snapshot
                u32 *existed_in_last = librg_table_get(last_snapshot, entity);

                // write create
                if (!existed_in_last) {
                    updated_entities--;

                    // skip entity create if this is player's entity
                    if (entity == player) continue;

                    // increase write amount for create counter
                    created_entities++;

                    // write all basic data
                    librg_data_wu32(&for_create, entity);
                    librg_data_wu32(&for_create, librg_fetch_meta(ctx, entity)->type);
                    librg_data_wptr(&for_create, librg_fetch_transform(ctx, entity), sizeof(librg_transform_t));

                    // request custom data from user
                    librg_event_t event = {0};
                    event.data = &for_create; event.entity = entity;
                    librg_event_trigger(ctx, LIBRG_ENTITY_CREATE, &event);
                }
                else {
                    // mark entity as still alive, for the remove cycle
                    librg_table_set(last_snapshot, entity, 0);

                    // fetch client streamer
                    librg_control_t *control = cast(librg_control_t *)librg_component_fetch(ctx, librg_control, entity);

                    // if this entity is client streamable and this client is owner
                    if (control && control->peer == client->peer) {
                        updated_entities--;
                    }
                    // write update
                    else {
                        librg_data_wu32(&for_update, entity);
                        librg_data_wptr(&for_update, librg_fetch_transform(ctx, entity), sizeof(librg_transform_t));

                        // request custom data from user
                        librg_event_t event = {0};
                        event.data = &for_update; event.entity = entity;
                        librg_event_trigger(ctx, LIBRG_ENTITY_UPDATE, &event);
                    }
                }

                // mark entity as existed for the next update
                librg_table_set(&next_snapshot, entity, 1);
            }

            // write our calcualted amounts right after packet id (from the beginning)
            librg_data_wu32_at(&for_create, created_entities, sizeof(u64));
            librg_data_wu32_at(&for_update, updated_entities, sizeof(u64));

            // save pos for remove data counter
            usize write_pos = librg_data_get_wpos(&for_create);
            librg_data_wu32(&for_create, 0);

            // add entity removes
            for (isize i = 0; i < zpl_array_count(last_snapshot->entries); ++i) {
                librg_entity_t entity = (u32)last_snapshot->entries[i].key;
                b32 not_existed = last_snapshot->entries[i].value;
                if (not_existed == 0) continue;

                // skip entity delete if this is player's entity
                if (entity == player) continue;

                // write id
                librg_data_wu32(&for_create, entity);
                removed_entities++;

                // write the rest
                librg_event_t event = {0};
                event.data = &for_create; event.entity = entity;
                librg_event_trigger(ctx, LIBRG_ENTITY_REMOVE, &event);
            }
            librg_data_wu32_at(&for_create, removed_entities, write_pos);


            librg_table_destroy(&client->last_snapshot);
            *last_snapshot = next_snapshot;

            // send the data, via differnt channels and reliability setting
            if (librg_data_get_wpos(&for_create) > (sizeof(u64) + sizeof(u32) * 2)) {
                enet_peer_send(client->peer, librg_options_get(LIBRG_NETWORK_PRIMARY_CHANNEL),
                    enet_packet_create(for_create.rawptr, librg_data_get_wpos(&for_create), ENET_PACKET_FLAG_RELIABLE)
                );
            }

            enet_peer_send(client->peer, librg_options_get(LIBRG_NETWORK_SECONDARY_CHANNEL),
                enet_packet_create(for_update.rawptr, librg_data_get_wpos(&for_update), ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT)
            );

            // and cleanup
            zpl_array_free(queue);
            librg_data_reset(&for_create);
            librg_data_reset(&for_update);
        });

        librg_data_free(&for_create);
        librg_data_free(&for_update);
    }

    librg_inline void librg__execute_server_entity_insert(librg_ctx_t *ctx) {
        librg_filter_t filter = { librg_stream };

        // clear
        zplc_clear(&ctx->streamer);

        // fill up
        librg_entity_eachx(ctx, filter, librg_lambda(entity), {
            librg_transform_t *transform = librg_fetch_transform(ctx, entity);
            librg_assert(transform);

            zplc_node_t node = { 0 };

            node.tag      = entity;
            node.position = transform->position;

            zplc_insert(&ctx->streamer, node);
        });
    }

    librg_inline void librg__execute_server_entity_destroy(librg_ctx_t *ctx) {
        for (isize i = 0; i < zpl_array_count(ctx->entity.remove_queue); i++) {
            librg__entity_destroy(ctx, ctx->entity.remove_queue[i]);
        }

        zpl_array_clear(ctx->entity.remove_queue);
    }

    librg_internal void librg__tick_cb(void *data) {
        u64 start  = zpl_utc_time_now();
        librg_ctx_t *ctx = (librg_ctx_t *)data;
        librg_assert(ctx);

        if (librg_is_server(ctx)) {
            librg__execute_server_entity_insert(ctx); /* create the server cull tree */
            librg__execute_server_entity_update(ctx); /* create and send updates to all clients */
            librg__execute_server_entity_destroy(ctx); /* destroy queued entities */
        } else {
            librg__execute_client_update(ctx); /* send information about client updates */
        }

        ctx->last_update = (zpl_utc_time_now() - start) / 1000.0;
    }





    /**
     *
     * CORE
     *
     */

    void librg_init(librg_ctx_t *ctx, librg_component_cb component_callback) {
        librg_dbg("librg_init\n");

        #define librg_set_default(expr, value) if (!expr) expr = value

        // apply default settings (if no user provided)
        librg_set_default(ctx->tick_delay, 32);
        librg_set_default(ctx->max_connections, 16);
        librg_set_default(ctx->max_entities, 8192);
        librg_set_default(ctx->max_components, 64);
        librg_set_default(ctx->world_size.x, 4096.0f);
        librg_set_default(ctx->world_size.y, 4096.0f);
        librg_set_default(ctx->mode, LIBRG_MODE_SERVER);

        if (!ctx->allocator.proc && !ctx->allocator.data) {
            ctx->allocator = zpl_heap_allocator();
        }

        #undef librg_set_default

        // init entity system
        ctx->entity.shared.limit_lower = 0;
        ctx->entity.shared.limit_upper = ctx->max_entities;
        ctx->entity.native.limit_lower = ctx->max_entities;
        ctx->entity.native.limit_upper = ctx->max_entities * 2;

        zpl_buffer_init(ctx->components.headers, ctx->allocator, ctx->max_components);
        zpl_array_init(ctx->entity.remove_queue, ctx->allocator);

        librg_component_register(ctx, librg_dummmy,     4);
        librg_component_register(ctx, librg_meta,       sizeof(librg_meta_t));
        librg_component_register(ctx, librg_transform,  sizeof(librg_transform_t));
        librg_component_register(ctx, librg_stream,     sizeof(librg_stream_t));
        librg_component_register(ctx, librg_control,    sizeof(librg_control_t));
        librg_component_register(ctx, librg_client,     sizeof(librg_client_t));

        if (component_callback) component_callback(ctx);
        ctx->components.data = (librg_void *)zpl_malloc(ctx->components.size);

        for (i8 i = 0; i < LIBRG_DATA_STREAMS_AMOUNT; ++i) {
            librg_data_init(&ctx->streams[i]);
        }

        // streamer // TODO: make 3d
        zplc_bounds_t world = {0};
        world.centre = zplm_vec3(0, 0, 0);
        world.half_size = zplm_vec3(ctx->world_size.x, ctx->world_size.y, 0.0f);
        zplc_init(&ctx->streamer, ctx->allocator, zplc_dim_2d_ev, world, 4);
        librg_table_init(&ctx->entity.ignored, ctx->allocator);

        // events
        zplev_init(&ctx->events, ctx->allocator);
        zpl_buffer_init(ctx->messages, ctx->allocator, librg_options_get(LIBRG_NETWORK_CAPACITY));

        // init timers
        zpl_array_init(ctx->timers, ctx->allocator);
        zpl_timer_t *tick_timer = zpl_timer_add(ctx->timers);
        tick_timer->userptr = (void *)ctx; /* provide ctx as a argument to timer */
        zpl_timer_set(tick_timer, 1000 * ctx->tick_delay, -1, librg__tick_cb);
        zpl_timer_start(tick_timer, 1000);

        // network
        librg_assert_msg(enet_initialize() == 0, "cannot initialize enet");

        // add event handlers for our network stufz
        ctx->messages[LIBRG_CONNECTION_INIT]        = librg__callback_connection_init;
        ctx->messages[LIBRG_CONNECTION_REQUEST]     = librg__callback_connection_request;
        ctx->messages[LIBRG_CONNECTION_REFUSE]      = librg__callback_connection_refuse;
        ctx->messages[LIBRG_CONNECTION_ACCEPT]      = librg__callback_connection_accept;
        ctx->messages[LIBRG_CONNECTION_DISCONNECT]  = librg__callback_connection_disconnect;
        ctx->messages[LIBRG_ENTITY_CREATE]          = librg__callback_entity_create;
        ctx->messages[LIBRG_ENTITY_UPDATE]          = librg__callback_entity_update;
        ctx->messages[LIBRG_CLIENT_STREAMER_ADD]    = librg__callback_entity_client_streamer_add;
        ctx->messages[LIBRG_CLIENT_STREAMER_REMOVE] = librg__callback_entity_client_streamer_remove;
        ctx->messages[LIBRG_CLIENT_STREAMER_UPDATE] = librg__callback_entity_client_streamer_update;
    }

    void librg_free(librg_ctx_t *ctx) {
        librg_dbg("librg_free\n");

        // free all timers and events first
        zpl_array_free(ctx->timers);
        zpl_buffer_free(ctx->messages, ctx->allocator);
        zplev_destroy(&ctx->events);
        zpl_array_free(ctx->entity.remove_queue);

        // streamer
        zplc_free(&ctx->streamer);
        librg_table_destroy(&ctx->entity.ignored);

        for (usize i = 0; i < ctx->components.count; ++i) {
            librg_component_meta *header = &ctx->components.headers[i]; librg_assert(header);
            zpl_buffer_free(header->used, ctx->allocator);
        }

        for (isize i = 0; i < LIBRG_DATA_STREAMS_AMOUNT; ++i) {
            librg_data_free(&ctx->streams[i]);
        }

        zpl_free(ctx->allocator, ctx->components.data);
        zpl_buffer_free(ctx->components.headers, ctx->allocator);

        enet_deinitialize();
    }

    zpl_inline void librg_release(void *ptr) {
        zpl_mfree(ptr);
    }

    zpl_inline void librg_release_array(void *ptr) {
        librg_assert(ptr);
        zpl_array_free(ptr);
    }

#ifdef __cplusplus
}
#endif

#endif // LIBRG_IMPLEMENTATION
#endif // LIBRG_INCLUDE_H
