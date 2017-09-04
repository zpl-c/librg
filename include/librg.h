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

#ifdef __cplusplus
extern "C" {
#endif



    /**
     *
     * BASIC DEFINITOINS
     *
     */

    #define LIBRG_API ZPL_DEF
    #define LIBRG_ENTITY_ALLOCATOR zpl_heap_allocator
    #define LIBRG_NETWORK_STREAM_PRIMARY_CHANNEL 1
    #define LIBRG_NETWORK_STREAM_SECONDARY_CHANNEL 1
    #define LIBRG_NETWORK_MESSAGE_CHANNEL 2

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

    #define librg_log zpl_printf
    #define librg_assert ZPL_ASSERT
    #define librg_assert_msg ZPL_ASSERT_MSG
    #define librg_inline zpl_inline
    #define librg_internal zpl_internal
    #define librg_lambda(name) name

    #define librg_bit_size 32
    #define librg_ceiling(x,y)     ( ((x) + (y) - 1) / (y) )
    #define librg_bit_set(A,k)     ( A[(k/32)] |= (1 << (k%32)) )
    #define librg_bit_clear(A,k)   ( A[(k/32)] &= ~(1 << (k%32)) )
    #define librg_bit_test(A,k)    ( A[(k/32)] & (1 << (k%32)) )

    /**
     *
     * OPTIONS
     *
     */

    #ifndef LIBRG_PLATFORM_ID
    #define LIBRG_PLATFORM_ID 1
    #endif

    #ifndef LIBRG_PLATFORM_PROTOCOL
    #define LIBRG_PLATFORM_PROTOCOL 1
    #endif

    #ifndef LIBRG_PLATFORM_BUILD
    #define LIBRG_PLATFORM_BUILD 1
    #endif

    #ifndef LIBRG_DEFAULT_BS_SIZE
    #define LIBRG_DEFAULT_BS_SIZE 64
    #endif

    #ifndef LIBRG_NETWORK_MESSAGE_CAPACITY
    #define LIBRG_NETWORK_MESSAGE_CAPACITY 2048
    #endif

    #ifndef LIBRG_NETWORK_CHANNELS
    #define LIBRG_NETWORK_CHANNELS 4
    #endif

    #ifndef LIBRG_DEFAULT_CLIENT_TYPE
    #define LIBRG_DEFAULT_CLIENT_TYPE 0
    #endif


    /**
     *
     * CORE
     *
     */

    typedef enum librg_mode_e {

        /* non-blocking server with run-once loop, manual librg_tick() call required */
        LIBRG_MODE_SERVER,

        /* non-blocking client with run-once loop, manual librg_tick() call required */
        LIBRG_MODE_CLIENT,

    } librg_mode_e;

    typedef struct librg_config_t {
        // core
        u16 tick_delay;

        // streamer configuration
        zplm_vec2_t world_size;
        u32 max_entities;

        // network configuration
        u16 port;
        u16 max_connections;
        librg_mode_e mode;

    } librg_config_t;

    /**
     * Main initialization method
     * MUST BE called in the begging of your application
     */
    LIBRG_API void librg_init(librg_config_t config);

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
     *
     * ENTITIES
     *
     */

    typedef u32 librg_entity_t;

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
    typedef void (librg_entity_cb_t)(librg_entity_t entity);

    /**
     * Table for various entity bool storages
     */
    ZPL_TABLE_DECLARE(extern, librg_table_t, librg_table_, u32);

    /**
     * Create entity and return handle
     */
    LIBRG_API librg_entity_t librg_entity_create(u32 type);

    /**
     * Create shared entity and return handle
     */
    LIBRG_API librg_entity_t librg_entity_create_shared(librg_entity_t remote, u32 type);

    /**
     * Check if provided entity is a valid entity
     */
    LIBRG_API b32 librg_entity_valid(librg_entity_t entity);

    /**
     * Return entity type
     */
    LIBRG_API u32 librg_entity_type(librg_entity_t entity);

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
    #define librg_component_declare_inner(PREFIX, NAME)                                                                         \
        ZPL_JOIN3(PREFIX, NAME, _t);                                                                                            \
                                                                                                                                \
        typedef struct ZPL_JOIN3(librg_, NAME, _meta_ent_t) {                                                                   \
            librg_entity_t handle;                                                                                              \
            u32 used;                                                                                                           \
        } ZPL_JOIN3(librg_, NAME, _meta_ent_t);                                                                                 \
                                                                                                                                \
        typedef struct ZPL_JOIN3(librg__component_, NAME, _pool_t) {                                                            \
            zpl_allocator_t backing;                                                                                            \
            usize count;                                                                                                        \
            u32 index;                                                                                                          \
            zpl_buffer_t(ZPL_JOIN3(librg_, NAME, _meta_ent_t)) entities;                                                        \
            zpl_buffer_t(ZPL_JOIN3(PREFIX, NAME, _t))         data;                                                             \
        } ZPL_JOIN3(librg__component_, NAME, _pool_t);                                                                          \
                                                                                                                                \
                                                                                                                                \
        void ZPL_JOIN2(librg__init_, NAME) (ZPL_JOIN3(librg__component_, NAME, _pool_t) *h, zpl_allocator_t a);                 \
        void ZPL_JOIN2(librg__free_, NAME) (ZPL_JOIN3(librg__component_, NAME, _pool_t) *h);                                    \
        ZPL_JOIN3(PREFIX, NAME, _t) * ZPL_JOIN2(librg_attach_, NAME) (librg_entity_t handle, ZPL_JOIN3(PREFIX, NAME, _t) data); \
        void ZPL_JOIN2(librg_detach_, NAME) (librg_entity_t handle);                                                            \
        ZPL_JOIN3(PREFIX, NAME, _t) * ZPL_JOIN2(librg_fetch_, NAME) (librg_entity_t handle);                                    \
        b32 ZPL_JOIN2(librg_has_, NAME) (librg_entity_t handle);                                                                \
        u32 ZPL_JOIN2(librg_index_, NAME) ();




    /**
     *
     * EVENTS
     *
     */

    typedef struct librg_event_t {
        b32 rejected;
        void *data;
        librg_entity_t entity;
    } librg_event_t;

    /**
     * Callback for event
     */
    typedef void (librg_event_cb_t)(librg_event_t *event);

    /**
     * Default built-in events
     * define your events likes this:
     *     enum {
     *         MY_NEW_EVENT_1 = LIBRG_LAST_ENUM_NUMBER,
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
     *
     * BINARY DATA (BITSTREAM)
     *
     */

    typedef char *librg_data_t;

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

    typedef ENetPeer   *librg_peer_t;
    typedef ENetHost   *librg_host_t;
    typedef ENetPacket *librg_packet_t;

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
    typedef struct {
        librg_data_t data;
        librg_peer_t peer;
        librg_packet_t packet;
    } librg_message_t;

    /**
     * Callback definition
     * for network message handler
     */
    typedef void (librg_message_handler_t)(librg_message_t *msg);

    /**
     * Storage for entities stored by peer
     * Hashtable, peer pointer is key, associated entity is value
     */
    ZPL_TABLE_DECLARE(extern, librg_peers_t, librg_peers_, librg_entity_t);

    /**
     * Builtin network storage table
     */
    typedef struct librg_network_t {
        librg_peer_t peer;
        librg_host_t host;

        librg_peers_t connected_peers;
    } librg_network_t;

    extern librg_network_t librg_network;

    /**
     * Check are we connected
     */
    LIBRG_API b32 librg_is_connected();

    /**
     * Starts network connection
     * Requires you to provide .port (if running as server)
     * or both .port and .host (if running as client)
     *
     * For server mode - starts server
     * For client mode - starts client, and connects to provided host & port
     */
    LIBRG_API void librg_network_start(librg_address_t address);

    /**
     * Disconnects (if connected), stops network
     * and releases resources
     */
    LIBRG_API void librg_network_stop();

    /**
     * Can be used to add handler
     * to a particular message id
     */
    LIBRG_API void librg_network_add(u64 id, librg_message_handler_t callback);

    /**
     * Can be used to remove a handler
     * from particular message id
     */
    LIBRG_API void librg_network_remove(u64 id);

    /**
     * Part of message API
     * Used to create message header
     * Returns initialized bitstream pointer
     */
    LIBRG_API librg_data_t librg_message_start(u64 id, usize size);

    /**
     * Part of message API
     * Takes in initialized bitstream pointer with written packet id
     * ( from call to librg_message_start)
     * and sends data to all connected peers ( or to server if its client )
     */
    LIBRG_API void librg_message_send_all(librg_data_t data);

    /**
     * Part of message API
     * Applies all from previous mehod
     * But data will be sent only to particular provided peer
     */
    LIBRG_API void librg_message_send_to(librg_peer_t peer, librg_data_t data);

    /**
     * Part of message API
     * Applies all from previous mehod
     * But data will be sent to all except provided peer
     */
    LIBRG_API void librg_message_send_except(librg_peer_t peer, librg_data_t data);

    /**
     * Part of message API
     * Applies all from previous mehod
     * Data will be sent only to entities, which are inside streamzone
     * for provided entity
     */
    LIBRG_API void librg_message_send_instream(librg_entity_t entity, librg_data_t data);

    /**
     * Part of message API
     * Applies all from previous mehod
     * Data will be sent only to entities, which are inside streamzone
     * for provided entity except peer
     */
    LIBRG_API void librg_message_send_instream_except(librg_entity_t entity, librg_peer_t peer, librg_data_t data);




    /**
     *
     * STREAMER
     *
     */

    /**
     * Query for entities that are in stream zone
     * for current entity, and are visible to this entity
     */
    LIBRG_API zpl_array_t(librg_entity_t) librg_streamer_query(librg_entity_t entity);

    /**
     * Set particular entity visible or invisible
     * for other entities in stream zone
     */
    LIBRG_API void librg_streamer_set_visible(librg_entity_t entity, b32 state);

    /**
     * Set particular entity visible or invisible
     * for other particular entity
     */
    LIBRG_API void librg_streamer_set_visible_for(librg_entity_t entity, librg_entity_t target, b32 state);

    /**
     * Set some entity as client streamable
     * Which means, that client will become responsive for sending
     * updates about this entity
     *
     * And this entity wont be sent to the client, until he stops being the streamer
     *
     * Setting other client as streamer, will remove previous streamer from entity
     */
    LIBRG_API void librg_streamer_client_set(librg_entity_t entity, librg_peer_t peer);

    /**
     * Remove some entity from stream ownership of the client
     */
    LIBRG_API void librg_streamer_client_remove(librg_entity_t entity);

    /**
     * Get entity by peer
     */
    LIBRG_API librg_entity_t librg_get_client_entity(librg_peer_t peer);



    /**
     *
     * COMPONENTS
     *
     */

    typedef struct {
        zplm_vec3_t position;
        zplm_quat_t rotation;
    } librg_component_declare_inner(librg_, transform);

    typedef struct {
        u32 range;
    } librg_component_declare_inner(librg_, streamable);

    typedef struct {
        u32 type;
        librg_table_t ignored;
    } librg_component_declare_inner(librg_, entitymeta);

    typedef struct {
        librg_peer_t peer;
    } librg_component_declare_inner(librg_, clientstream);

    typedef struct {
        librg_peer_t peer;
        librg_table_t last_snapshot;
    } librg_component_declare_inner(librg_, client);





    /**
     *
     * EXTENSIONS
     *
     */

    #define librg_send_all(ID, NAME, CODE) do {                                 \
            librg_data_t NAME = librg_message_start(ID, LIBRG_DEFAULT_BS_SIZE); \
            CODE; librg_message_send_all(NAME);                                 \
        } while(0);

    #define librg_send_to(ID, WHO, NAME, CODE) do {                             \
            librg_data_t NAME = librg_message_start(ID, LIBRG_DEFAULT_BS_SIZE); \
            CODE; librg_message_send_to(WHO, NAME);                             \
        } while(0);

    #define librg_send_except(ID, WHO, NAME, CODE) do {                         \
            librg_data_t NAME = librg_message_start(ID, LIBRG_DEFAULT_BS_SIZE); \
            CODE; librg_message_send_except(WHO, NAME);                         \
        } while(0)

    #define librg_send_instream(ID, WHO, NAME, CODE) do {                       \
            librg_data_t NAME = librg_message_start(ID, LIBRG_DEFAULT_BS_SIZE); \
            CODE; librg_message_send_instream(WHO, NAME);                       \
        } while(0)

    #define librg_send_instream_except(ID, HOW, WHO, NAME, CODE) do {           \
            librg_data_t NAME = librg_message_start(ID, LIBRG_DEFAULT_BS_SIZE); \
            CODE; librg_message_send_instream_except(HOW, WHO, NAME);           \
        } while(0)

    #define librg_send librg_send_all

    #define librg_component_declare(NAME) librg_component_declare_inner(,NAME)
    #define librg_component(NAME) librg_component_declare(NAME)

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
     * Create dummy component to
     * use it as template for removal in destroy
     */
    typedef struct {} librg_component_declare(_dummy);
    #define __dummypool_t librg__component__dummy_pool_t
    #define __dummymeta_t librg__dummy_meta_ent_t


    ZPL_TABLE_DEFINE(librg_peers_t, librg_peers_, librg_entity_t);
    ZPL_TABLE_DEFINE(librg_table_t, librg_table_, u32);


    /**
     * Storage containers
     * for inner librg stuff
     */
    zplc_t              librg__streamer;
    zplev_pool          librg__events;
    librg_config_t      librg__config;
    zpl_timer_pool      librg__timers;
    librg_network_t     librg_network;

    struct {
        librg_data_t input;
        librg_data_t output;
    } librg__streams;

    typedef struct {
        u32 cursor;
        u32 count;
        u32 limit_upper;
        u32 limit_lower;
    } librg__entity_pool_t;

    struct {
        librg_table_t ignored;
        librg__entity_pool_t shared;
        librg__entity_pool_t native;
        u32 count;
    } librg__entity;

    zpl_array_t(void *) librg__component_pool;
    zpl_array_t(librg_entity_t) librg__entity_remove_queue;
    zpl_buffer_t(librg_message_handler_t *) librg__messages;







    /**
     *
     * ENTITIES
     *
     */


    /**
     * Entity create methods
     *
     * Attaches some default components
     * and inits custom data storages inside (if needed)
     */

    librg_inline librg_entity_t librg__entity_create(librg__entity_pool_t *pool) {
        librg_assert_msg(++pool->count < pool->limit_upper, "entity limit");

        if (pool->cursor == pool->limit_upper || pool->cursor == 0) {
            pool->cursor = pool->limit_lower;
        }

        for (; pool->cursor < pool->limit_upper; ++pool->cursor) {
            librg_entity_t entity = pool->cursor;
            if (librg_entity_valid(entity)) continue;

            librg_attach_transform(entity,  {0});
            librg_attach_entitymeta(entity, {0});
            librg_attach_streamable(entity, { 250 });

            if (librg_is_server()) {
                librg_table_init(&librg_fetch_entitymeta(entity)->ignored, zpl_heap_allocator());
            }

            return entity;
        }

        librg_assert_msg(false, "no entities to spawn");
    }

    librg_entity_t librg_entity_create(u32 type) {
        librg__entity_pool_t *pool = &librg__entity.shared;

        if (librg_is_client()) {
            pool = &librg__entity.native;
        }

        librg_entity_t entity = librg__entity_create(pool);
        librg_fetch_entitymeta(entity)->type = type;
        return entity;
    }

    librg_entity_t librg_entity_create_shared(u32 entity, u32 type) {
        librg_assert_msg(librg_is_client(), "librg_entity_create_shared: can be executed only on client");
        librg_assert(!librg_entity_valid(entity));

        librg__entity_pool_t *pool = &librg__entity.shared;
        librg_assert_msg(++pool->count < pool->limit_upper, "entity limit");

        librg_attach_transform(entity,  {0});
        librg_attach_entitymeta(entity, { type, 0 });
        librg_attach_streamable(entity, { 250 });

        return entity;
    }

    librg_internal void librg__entity_destroy(librg_entity_t entity) {
        librg__entity_pool_t *pool = &librg__entity.shared;
        if (entity > librg__entity.shared.limit_upper) {
            pool = &librg__entity.native;
        }

        librg_assert(librg_entity_valid(entity));

        if (librg_is_server()) {
            librg_table_destroy(&librg_fetch_entitymeta(entity)->ignored);
            librg_streamer_set_visible(entity, true);
        }

        // decrease amount
        pool->count--;

        // detach all components
        for (i32 i = 0; i < zpl_array_count(librg__component_pool); i++) {
            __dummymeta_t *meta = (cast(__dummypool_t*)librg__component_pool[i])->entities+entity;
            librg_assert(meta);
            meta->used = false;
        }
    }

    librg_inline b32 librg_entity_valid(librg_entity_t entity) {
        return (librg_fetch_entitymeta(entity) != NULL);
    }

    /**
     * Entity destructors
     */

    void librg__entity_execute_destroy() {
        for (isize i = 0; i < zpl_array_count(librg__entity_remove_queue); i++) {
            librg__entity_destroy(librg__entity_remove_queue[i]);
        }

        zpl_array_clear(librg__entity_remove_queue);
    }

    void librg_entity_destroy(librg_entity_t id) {
        if (librg_is_client()) {
            return librg__entity_destroy(id);
        }

        zpl_array_append(librg__entity_remove_queue, id);
    }

    /**
     * Entity types
     */

    librg_inline u32 librg_entity_type(librg_entity_t entity) {
        return librg_fetch_entitymeta(entity)->type;
    }

    /**
     * Entity iterators
     */
    #define librg__eachmeta(INDEX, ENTITY)                                                                          \
        __dummypool_t *pool = cast(__dummypool_t *)librg__component_pool[INDEX - 1]; librg_assert(pool);            \
        __dummymeta_t *meta = cast(__dummymeta_t *)(pool->entities + ENTITY); librg_assert(meta);

    #define librg_entity_eachx(FILTER, NAME, CODE) do {                                                             \
        u32 entitymeta_id = librg_index_entitymeta();                                                               \
        if (entitymeta_id == 0 || FILTER.contains1 == 0) break;                                                     \
        for (usize _ent = 0, valid = 0; valid < (librg__entity.native.count + librg__entity.shared.count)           \
            && _ent < (librg_is_server() ? librg__config.max_entities : librg__config.max_entities * 2); _ent++) {  \
            /* check if entity valid */                                                                             \
            { librg__eachmeta(entitymeta_id, _ent); if (!meta->used) continue; } valid++;                           \
            b32 _used = true;                                                                                       \
            /* check for included components */                                                                     \
            for (isize k = 0; k < 8 && _used; k++) {                                                                \
                if (FILTER.contains[k] == 0) break;                                                                 \
                librg__eachmeta(FILTER.contains[k], _ent);                                                          \
                if (!meta->used) { _used = false; }                                                                 \
            }                                                                                                       \
            /* check for excluded components */                                                                     \
            for (isize k = 0; k < 4 && _used; k++) {                                                                \
                if (FILTER.excludes[k] == 0) break;                                                                 \
                librg__eachmeta(FILTER.excludes[k], _ent);                                                          \
                if (meta->used) { _used = false; }                                                                  \
            }                                                                                                       \
            /* execute code */                                                                                      \
            if (_used) { librg_entity_t NAME = _ent; CODE; }                                                        \
        }                                                                                                           \
    } while(0)

    #define librg__entity_each(INDEX, NAME, CODE) do {                                                              \
        if (INDEX == 0) {break;} u32 entitymeta_id = librg_index_entitymeta();                                      \
        __dummypool_t *pool = cast(__dummypool_t *)librg__component_pool[INDEX - 1]; librg_assert(pool);            \
        for (usize _ent = 0, valid = 0; valid < (librg__entity.native.count + librg__entity.shared.count)           \
            && _ent < (librg_is_server() ? librg__config.max_entities : librg__config.max_entities * 2); _ent++) {  \
            /*{ librg__eachmeta(entitymeta_id, _ent); if (!meta->used) continue; } valid++;*/                       \
            __dummymeta_t *meta = cast(__dummymeta_t *)(pool->entities[_ent]); librg_assert(meta);                  \
            if (meta->used) { librg_entity_t NAME = _ent; CODE; }                                                   \
        }                                                                                                           \
    } while (0)

    void librg_entity_each(librg_entity_filter_t filter, librg_entity_cb_t callback) {
        librg_entity_eachx(filter, librg_lambda(entity), { callback(entity); });
    }




    /**
     *
     * COMPONENTS
     *
     */
    // librg_dbg("initializing %s pool, approximate size: %f kb\n", #NAME, (zpl_size_of(ZPL_JOIN3(PREFIX, NAME, _t))*h->count) / 1000.0)       \

    #define librg_component_define_inner(PREFIX, NAME)                                                                                      \
        ZPL_JOIN3(librg__component_, NAME, _pool_t) ZPL_JOIN3(librg__component_, NAME, _pool);                                              \
                                                                                                                                            \
        void ZPL_JOIN2(librg__init_, NAME) (ZPL_JOIN3(librg__component_, NAME, _pool_t) *h, zpl_allocator_t a) {                            \
            librg_assert(h); h->backing = a;                                                                                                \
            h->count = librg_is_server() ? librg__config.max_entities : librg__config.max_entities * 2;                                     \
            zpl_buffer_init(h->entities, a, zpl_size_of(ZPL_JOIN3(librg_, NAME, _meta_ent_t)) * h->count);                                  \
            zpl_buffer_init(h->data, a, zpl_size_of(ZPL_JOIN3(PREFIX, NAME, _t)) * h->count);                                               \
            zpl_array_append(librg__component_pool, cast(void *)h);                                                                         \
            h->index = zpl_array_count(librg__component_pool);                                                                              \
        }                                                                                                                                   \
        void ZPL_JOIN2(librg__free_, NAME) (ZPL_JOIN3(librg__component_, NAME, _pool_t) *h) {                                               \
            librg_assert(h);                                                                                                                \
            zpl_buffer_free(h->entities, h->backing);                                                                                       \
            zpl_buffer_free(h->data, h->backing);                                                                                           \
        }                                                                                                                                   \
        u32 ZPL_JOIN2(librg_index_, NAME)() {                                                                                               \
            return ZPL_JOIN3(librg__component_, NAME, _pool).index;                                                                         \
        }                                                                                                                                   \
        ZPL_JOIN3(PREFIX, NAME, _t) * ZPL_JOIN2(librg_attach_, NAME) (librg_entity_t handle, ZPL_JOIN3(PREFIX, NAME, _t) data) {            \
            if (ZPL_JOIN3(librg__component_, NAME, _pool).count == 0) {                                                                     \
                ZPL_JOIN2(librg__init_, NAME)(&ZPL_JOIN3(librg__component_, NAME, _pool), LIBRG_ENTITY_ALLOCATOR());                        \
            }                                                                                                                               \
            ZPL_JOIN3(librg_, NAME, _meta_ent_t) *meta_ent = (ZPL_JOIN3(librg__component_, NAME, _pool).entities+handle);                   \
            meta_ent->handle = handle;                                                                                                      \
            meta_ent->used = true;                                                                                                          \
            *(ZPL_JOIN3(librg__component_, NAME, _pool).data+handle) = data;                                                                \
            return (ZPL_JOIN3(librg__component_, NAME, _pool).data+handle);                                                                 \
        }                                                                                                                                   \
        void ZPL_JOIN2(librg_detach_, NAME) (librg_entity_t handle) {                                                                       \
            if (ZPL_JOIN3(librg__component_, NAME, _pool).count == 0) {                                                                     \
                ZPL_JOIN2(librg__init_, NAME)(&ZPL_JOIN3(librg__component_, NAME, _pool), LIBRG_ENTITY_ALLOCATOR());                        \
            }                                                                                                                               \
            ZPL_JOIN3(librg_, NAME, _meta_ent_t) *meta_ent = (ZPL_JOIN3(librg__component_, NAME, _pool).entities+handle);                   \
            meta_ent->used = false;                                                                                                         \
        }                                                                                                                                   \
        b32 ZPL_JOIN2(librg_has_, NAME) (librg_entity_t handle) {                                                                           \
            if (ZPL_JOIN3(librg__component_, NAME, _pool).count == 0) {                                                                     \
                ZPL_JOIN2(librg__init_, NAME)(&ZPL_JOIN3(librg__component_, NAME, _pool), LIBRG_ENTITY_ALLOCATOR());                        \
            }                                                                                                                               \
            ZPL_JOIN3(librg_, NAME, _meta_ent_t) *meta_ent = (ZPL_JOIN3(librg__component_, NAME, _pool).entities+handle);                   \
            return ((meta_ent->used) && (meta_ent->handle == handle));                                                                      \
        }                                                                                                                                   \
        ZPL_JOIN3(PREFIX, NAME, _t) * ZPL_JOIN2(librg_fetch_, NAME) (librg_entity_t handle) {                                               \
            if (ZPL_JOIN3(librg__component_, NAME, _pool).count == 0) {                                                                     \
                ZPL_JOIN2(librg__init_, NAME)(&ZPL_JOIN3(librg__component_, NAME, _pool), LIBRG_ENTITY_ALLOCATOR());                        \
            }                                                                                                                               \
            ZPL_JOIN3(librg_, NAME, _meta_ent_t) *meta_ent = (ZPL_JOIN3(librg__component_, NAME, _pool).entities+handle);                   \
            if ((meta_ent->used) && (meta_ent->handle == handle)) {                                                                         \
                return (ZPL_JOIN3(librg__component_, NAME, _pool).data+handle);                                                             \
            }                                                                                                                               \
            else {                                                                                                                          \
                return NULL;                                                                                                                \
            }                                                                                                                               \
        }


    /**
     * Built-in components defintion
     */

    librg_component_define_inner(librg_, transform);
    librg_component_define_inner(librg_, streamable);
    librg_component_define_inner(librg_, entitymeta);
    librg_component_define_inner(librg_, clientstream);
    librg_component_define_inner(librg_, client);





    /**
     *
     * EVENTS
     *
     */

    librg_inline u64 librg_event_add(u64 id, librg_event_cb_t callback) {
        return zplev_add(&librg__events, id, (zplev_cb *)callback);
    }

    void librg_event_trigger(u64 id, librg_event_t *event) {
        zplev_block *block = zplev_pool_get(&librg__events, id);
        if (!block) return;

        for (isize i = 0; i < zpl_array_count(*block) && !event->rejected; ++i) {
            (*block)[i](event);
        }
    }

    librg_inline void librg_event_remove(u64 id, u64 index) {
        zplev_remove(&librg__events, id, index);
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

    librg_inline void librg_data_init(librg_data_t *data) {
        librg_assert_msg(data, "librg_data_init: you need to provide data with &");
        zpl_bs_init(*data, zpl_heap_allocator(), LIBRG_DEFAULT_BS_SIZE);
    }

    librg_inline void librg_data_init_size(librg_data_t *data, usize size) {
        librg_assert_msg(data, "librg_data_init: you need to provide data with &");
        zpl_bs_init(*data, zpl_heap_allocator(), size);
    }

    librg_inline void librg_data_free(librg_data_t *data) {
        zpl_bs_free(*data);
    }

    librg_inline void librg_data_reset(librg_data_t *data) {
        ZPL_BS_HEADER(*data)->read_pos = 0;
        ZPL_BS_HEADER(*data)->read_pos = 0;
    }

    librg_inline void librg_data_grow(librg_data_t *data, usize min_size) {
        zpl_bs_grow(*data, min_size);
    }

    librg_inline usize librg_data_capacity(librg_data_t *data) {
        return ZPL_BS_HEADER(*data)->capacity;
    }

    librg_inline usize librg_data_get_rpos(librg_data_t *data) {
        return ZPL_BS_HEADER(*data)->read_pos;
    }

    librg_inline usize librg_data_get_wpos(librg_data_t *data) {
        return ZPL_BS_HEADER(*data)->write_pos;
    }

    /**
     * Pointer writers and readers
     */

    librg_inline void librg_data_rptr(librg_data_t *data, void *ptr, usize size) {
        librg_data_rptr_at(data, ptr, size, librg_data_get_rpos(data));
        ZPL_BS_HEADER(*data)->read_pos += size;
    }

    librg_inline void librg_data_wptr(librg_data_t *data, void *ptr, usize size) {
        librg_data_wptr_at(data, ptr, size, librg_data_get_wpos(data));
        ZPL_BS_HEADER(*data)->write_pos += size;
    }

    librg_inline void librg_data_rptr_at(librg_data_t *data, void *ptr, usize size, isize position) {
        librg_assert(*data);
        librg_assert_msg(position + size <= librg_data_capacity(data),
            "librg_data: trying to read from outside of the bounds");

        zpl_memcopy(ptr, *data + position, size);
    }

    librg_inline void librg_data_wptr_at(librg_data_t *data, void *ptr, usize size, isize position) {
        librg_assert(*data);
        if (position + size > librg_data_capacity(data)) {
            librg_data_grow(data, librg_data_capacity(data) + size + position);
        }

        zpl_memcopy(*data + position, ptr, size);
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

    void librg_network_start(librg_address_t addr) {
        librg_dbg("librg_network_start\n");

        librg_peers_init(&librg_network.connected_peers, zpl_heap_allocator());

        if (librg_is_server()) {
            ENetAddress address;

            address.port = addr.port;
            address.host = ENET_HOST_ANY;

            // setup server host
            librg_network.host = enet_host_create(&address, librg__config.max_connections, LIBRG_NETWORK_CHANNELS, 0, 0);
            librg_assert_msg(librg_network.host, "could not start server at provided port");
        }
        else {
            ENetAddress address;

            address.port = addr.port;
            enet_address_set_host(&address, addr.host);

            // setup client host
            librg_network.host = enet_host_create(NULL, 1, LIBRG_NETWORK_CHANNELS, 57600 / 8, 14400 / 8);
            librg_assert_msg(librg_network.host, "could not start client");

            // create peer connecting to server
            librg_dbg("connecting to server %s:%u\n", addr.host, addr.port);
            librg_network.peer = enet_host_connect(librg_network.host, &address, LIBRG_NETWORK_CHANNELS, 0);
            librg_assert_msg(librg_network.peer, "could not setup peer for provided address");
        }
    }

    void librg_network_stop() {
        librg_dbg("librg_network_stop\n");

        if (librg_network.peer) {
            ENetEvent event;

            // disconnect and emit event
            enet_peer_disconnect(librg_network.peer, 0);
            enet_host_service(librg_network.host, &event, 100);

            // reset our peer
            enet_peer_reset(librg_network.peer);
        }

        librg_peers_destroy(&librg_network.connected_peers);
    }

    /**
     * Network helpers
     */

    librg_inline b32 librg_is_server() {
        return librg__config.mode == LIBRG_MODE_SERVER;
    }

    librg_inline b32 librg_is_client() {
        return librg__config.mode == LIBRG_MODE_CLIENT;
    }

    librg_inline b32 librg_is_connected() {
        return librg_network.peer && librg_network.peer->state == ENET_PEER_STATE_CONNECTED;
    }


    /**
     * Network messages
     */

    librg_inline void librg_network_add(u64 id, librg_message_handler_t callback) {
        librg__messages[id] = callback;
    }

    librg_inline void librg_network_remove(u64 id) {
        librg__messages[id] = NULL;
    }

    librg_data_t librg_message_start(u64 id, usize size) {
        librg_data_t data;
        zpl_bs_init(data, zpl_heap_allocator(), size + sizeof(u64));
        zpl_bs_write_u64(data, id);
        return data;
    }

    /**
     * Senders
     */

    void librg_message_send_all(librg_data_t data) {
        if (librg_is_client()) {
            return librg_message_send_to(librg_network.peer, data);
        }

        librg_message_send_except(NULL, data);
    }

    void librg_message_send_to(librg_peer_t peer, librg_data_t data) {
        enet_peer_send(peer, LIBRG_NETWORK_MESSAGE_CHANNEL, enet_packet_create(
            data, zpl_bs_size(data), ENET_PACKET_FLAG_RELIABLE
        ));

        zpl_bs_free(data);
    }

    void librg_message_send_except(librg_peer_t peer, librg_data_t data) {
        librg_entity_filter_t filter = { librg_index_client() };

        librg_entity_eachx(filter, librg_lambda(entity2), {
            librg_client_t *client = librg_fetch_client(entity2);

            if (client->peer != peer) {
                enet_peer_send(client->peer, LIBRG_NETWORK_MESSAGE_CHANNEL, enet_packet_create(
                     data, zpl_bs_size(data), ENET_PACKET_FLAG_RELIABLE
                ));
            }
        });

        zpl_bs_free(data);
    }

    librg_inline void librg_message_send_instream(librg_entity_t entity, librg_data_t data) {
        librg_message_send_instream_except(entity, NULL, data);
    }

    void librg_message_send_instream_except(librg_entity_t entity, librg_peer_t ignored, librg_data_t data) {
        zpl_array_t(librg_entity_t) queue = librg_streamer_query(entity);

        for (isize i = 0; i < zpl_array_count(queue); i++) {
            librg_entity_t target = queue[i];

            librg_assert(
                librg_fetch_client(target) &&
                librg_fetch_client(target)->peer
            );

            librg_peer_t peer = librg_fetch_client(target)->peer;

            if (peer == ignored) {
                continue;
            }

            enet_peer_send(peer, LIBRG_NETWORK_MESSAGE_CHANNEL, enet_packet_create(
                data, zpl_bs_size(data), ENET_PACKET_FLAG_RELIABLE
            ));
        }

        zpl_array_free(queue);
        zpl_bs_free(data);
    }

    /**
     * Main ticker
     */

    void librg_tick() {
        zpl_timer_update(librg__timers);

        if (!librg_network.host) {
            return; /* occasion where we are not started network yet */
        }

        ENetEvent event;

        while (enet_host_service(librg_network.host, &event, 0) > 0) {
            librg_message_t msg;
            msg.data = NULL;
            msg.peer = event.peer;
            msg.packet = event.packet;

            switch (event.type) {
                case ENET_EVENT_TYPE_RECEIVE: {
                    // read our data
                    librg_data_t data;
                    zpl_bs_init(data, zpl_heap_allocator(), event.packet->dataLength);
                    zpl_bs_write_size(data, event.packet->data, event.packet->dataLength);
                    msg.data = data;

                    // get curernt packet id
                    u64 id = zpl_bs_read_u64(data);

                    if (librg__messages[id]) {
                        librg__messages[id](&msg);
                    }
                    else {
                        librg_dbg("network: unknown message: %llu\n", id);
                    }

                    zpl_bs_free(data);
                    enet_packet_destroy(event.packet);
                } break;
                case ENET_EVENT_TYPE_CONNECT:    librg__messages[LIBRG_CONNECTION_INIT](&msg); break;
                case ENET_EVENT_TYPE_DISCONNECT: librg__messages[LIBRG_CONNECTION_DISCONNECT](&msg); break;
                case ENET_EVENT_TYPE_NONE: break;
            }
        }
    }






    /**
     *
     * NETWORK BUILTIN CALLBACKS
     *
     */
#if 1

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

        if (librg_is_client()) {
            librg_send_to(LIBRG_CONNECTION_REQUEST, msg->peer, librg_lambda(data), {
                librg_data_wu16(&data, LIBRG_PLATFORM_ID);
                librg_data_wu16(&data, LIBRG_PLATFORM_BUILD);
                librg_data_wu16(&data, LIBRG_PLATFORM_PROTOCOL);

                librg_event_t event = { 0 };
                event.data = data;
                librg_event_trigger(LIBRG_CONNECTION_REQUEST, &event);
            });
        }
    }

    /**
     * SERVER SIDE
     */
    librg_internal void librg__callback_connection_request(librg_message_t *msg) {
        librg_dbg("librg__connection_request\n");

        u16 platform_id       = librg_data_ru16(&msg->data);
        u16 platform_build    = librg_data_ru16(&msg->data);
        u16 platform_protocol = librg_data_ru16(&msg->data);

        b32 blocked = (platform_id != LIBRG_PLATFORM_ID || platform_protocol != LIBRG_PLATFORM_PROTOCOL);

        if (platform_build != LIBRG_PLATFORM_BUILD) {
            librg_dbg("NOTICE: librg platform build mismatch client %u, server: %u\n", platform_build, LIBRG_PLATFORM_BUILD);
        }

        librg_event_t event = { 0 }; event.data = msg->data;
        librg_event_trigger(LIBRG_CONNECTION_REQUEST, &event);

        if (librg_event_succeeded(&event) && !blocked) {
            librg_entity_t entity = librg_entity_create(LIBRG_DEFAULT_CLIENT_TYPE);

            // assign default compoenents
            librg_attach_client(entity, { msg->peer });
            librg_table_init(&librg_fetch_client(entity)->last_snapshot, zpl_heap_allocator());

            // add client peer to storage
            librg_peers_set(&librg_network.connected_peers, cast(u64)msg->peer, entity);

            // send accept
            librg_send_to(LIBRG_CONNECTION_ACCEPT, msg->peer, librg_lambda(data), {
                zpl_bs_write_u32(data, entity);
            });

            librg_event_t acptevt = { 0 }; acptevt.entity = entity;
            librg_event_trigger(LIBRG_CONNECTION_ACCEPT, &acptevt);
        }
        else {
            librg_send_to(LIBRG_CONNECTION_REFUSE, msg->peer, librg_lambda(data), {});

            librg_event_t rfsevt = { 0 };
            librg_event_trigger(LIBRG_CONNECTION_REFUSE, &rfsevt);
        }
    }

    /**
     * CLIENT SIDE
     */
    librg_internal void librg__callback_connection_refuse(librg_message_t *msg) {
        librg_event_t event = { 0 }; event.data = msg->data;
        librg_event_trigger(LIBRG_CONNECTION_REFUSE, &event);
    }

    /**
     * CLIENT SIDE
     */
    librg_internal void librg__callback_connection_accept(librg_message_t *msg) {
        librg_dbg("librg__connection_accept\n");

        librg_entity_t remote = zpl_bs_read_u32(msg->data);
        librg_entity_t entity = librg_entity_create_shared(remote, LIBRG_DEFAULT_CLIENT_TYPE);

        // add server peer to storage
        librg_peers_set(&librg_network.connected_peers, cast(u64)msg->peer, entity);

        librg_event_t event = { 0 };
        event.data = msg->data; event.entity = entity;
        librg_event_trigger(LIBRG_CONNECTION_ACCEPT, &event);
    }

    /**
     * SHARED
     */
    librg_internal void librg__callback_connection_disconnect(librg_message_t *msg) {
        librg_dbg("librg__connection_disconnect\n");

        if (librg_is_server()) {
            librg_entity_t *entity = librg_peers_get(&librg_network.connected_peers, cast(u64)msg->peer);
            if (!entity || !librg_entity_valid(*entity)) return;
            librg_table_destroy(&librg_fetch_client(*entity)->last_snapshot);
            librg_detach_client(*entity);
            librg_entity_destroy(*entity);
        }
    }

    librg_internal void librg__callback_entity_create(librg_message_t *msg) {
        u32 query_size = librg_data_ru32(&msg->data);

        for (int i = 0; i < query_size; ++i) {
            librg_entity_t entity = librg_data_rentity(&msg->data);
            u32 type = librg_data_ru32(&msg->data);

            librg_transform_t transform;
            librg_data_rptr(&msg->data, &transform, sizeof(transform));

            librg_entity_create_shared(entity, type);
            librg_attach_transform(entity, transform);

            librg_event_t event = {0};
            event.data = msg->data; event.entity = entity;
            librg_event_trigger(LIBRG_ENTITY_CREATE, &event);
        }

        u32 remove_size = librg_data_ru32(&msg->data);

        for (int i = 0; i < remove_size; ++i) {
            librg_entity_t entity = librg_data_rentity(&msg->data);

            if (librg_entity_valid(entity)) {
                librg_event_t event = {0};
                event.data = msg->data; event.entity = entity;
                librg_event_trigger(LIBRG_ENTITY_REMOVE, &event);
                librg_entity_destroy(entity);
            }
            else {
                librg_dbg("unexpected entity %u on remove\n", entity);
            }
        }
    }

    librg_internal void librg__callback_entity_update(librg_message_t *msg) {
        u32 query_size = zpl_bs_read_u32(msg->data);

        for (int i = 0; i < query_size; ++i) {
            librg_entity_t entity = zpl_bs_read_u32(msg->data);

            librg_transform_t transform;
            zpl_bs_read_size(msg->data, &transform, sizeof(transform));

            if (!librg_entity_valid(entity)) {
                continue;
            }

            *librg_fetch_transform(entity) = transform;

            librg_event_t event = {0};
            event.data = msg->data; event.entity = entity;
            librg_event_trigger(LIBRG_ENTITY_UPDATE, &event);
        }
    }

    librg_internal void librg__callback_entity_client_streamer_add(librg_message_t *msg) {
        librg_entity_t entity = zpl_bs_read_u32(msg->data);

        if (!librg_entity_valid(entity)) {
            librg_dbg("trying to add unknown entity to clientstream!");
            return;
        }

        librg_clientstream_t *cli_stream = librg_fetch_clientstream(entity);

        if (!cli_stream) {
            librg_attach_clientstream(entity,{});

            librg_event_t event = {0};
            event.data = msg->data; event.entity = entity;
            librg_event_trigger(LIBRG_CLIENT_STREAMER_ADD, &event);
        }
    }

    librg_internal void librg__callback_entity_client_streamer_remove(librg_message_t *msg) {
        librg_entity_t entity = zpl_bs_read_u32(msg->data);

        if (!librg_entity_valid(entity)) {
            librg_dbg("trying to remove unknown entity from clientstream!\n");
            return;
        }

        librg_clientstream_t *cli_stream = librg_fetch_clientstream(entity);

        if (cli_stream) {
            librg_detach_clientstream(entity);

            librg_event_t event = {0};
            event.data = msg->data; event.entity = entity;
            librg_event_trigger(LIBRG_CLIENT_STREAMER_REMOVE, &event);
        }
    }

    librg_internal void librg__callback_entity_client_streamer_update(librg_message_t *msg) {
        u32 amount = zpl_bs_read_u32(msg->data);

        for (isize i = 0; i < amount; i++) {
            librg_entity_t entity = zpl_bs_read_u32(msg->data);
            u32 size = zpl_bs_read_u32(msg->data);

            if (!librg_entity_valid(entity)) {
                librg_dbg("invalid entity on client streamer update\n");
                zpl_bs_read_pos(msg->data) += size;
                librg_assert(false);
                continue;
            }

            librg_clientstream_t *cli_stream = librg_fetch_clientstream(entity);

            if (!cli_stream || cli_stream->peer != msg->peer) {
                librg_dbg("no component, or peer is different\n");
                zpl_bs_read_pos(msg->data) += size;
                continue;
            }

            librg_event_t event = {0};
            event.data = msg->data; event.entity = entity;
            librg_event_trigger(LIBRG_CLIENT_STREAMER_UPDATE, &event);

            librg_transform_t transform;
            zpl_bs_read_size(msg->data, &transform, sizeof(transform));
            *librg_fetch_transform(entity) = transform;
        }
    }
#endif





    /**
     *
     * STREAMER
     *
     */


    void librg_streamer_set_visible(librg_entity_t entity, b32 state) {
        librg_table_set(&librg__entity.ignored, entity, (u32)!state);
    }

    void librg_streamer_set_visible_for(librg_entity_t entity, librg_entity_t target, b32 state) {
        librg_assert(librg_fetch_entitymeta(entity));
        librg_table_set(&librg_fetch_entitymeta(entity)->ignored, target, (u32)!state);
    }

    zpl_array_t(librg_entity_t) librg_streamer_query(librg_entity_t entity) {
        zpl_array_t(zplc_node_t) search_temp;
        zpl_array_t(librg_entity_t) search_result;

        zpl_array_init(search_temp, zpl_heap_allocator());
        zpl_array_init(search_result, zpl_heap_allocator());

        librg_transform_t  *transform  = librg_fetch_transform(entity);
        librg_streamable_t *streamable = librg_fetch_streamable(entity);
        librg_assert(transform && streamable);

        zplc_bounds_t search_bounds;
        search_bounds.centre = transform->position;
        search_bounds.half_size = { (f32)streamable->range, (f32)streamable->range, (f32)streamable->range };

        zplc_query(&librg__streamer, search_bounds, &search_temp);

        for (isize i = 0; i < zpl_array_count(search_temp); i++) {
            librg_entity_t target = search_temp[i].tag;
            if (!librg_entity_valid(target)) continue;

            u32 *global = librg_table_get(&librg__entity.ignored, target);
            u32 *local  = librg_table_get(&librg_fetch_entitymeta(target)->ignored, entity);

            if ((global && *global) || (local && *local)) continue;

            zpl_array_append(search_result, target);
        }

        zpl_array_free(search_temp);
        return search_result;
    }

    /**
     * CLIENT-SIDE
     *
     * Responsive for updating the client side streamer
     */
    librg_internal void librg__update_client() {
        u32 amount = 0;
        librg_data_t data;
        librg_data_init(&data);

        librg_data_wu64(&data, LIBRG_CLIENT_STREAMER_UPDATE);
        librg_data_wu32(&data, 0); // amount of entities to be sent (updates)

        librg_entity_filter_t filter = {
            librg_index_clientstream(),
            librg_index_streamable(),
        };

        librg_entity_eachx(filter, librg_lambda(entity), {
            librg_transform_t *transform = librg_fetch_transform(entity);

            librg_data_t subdata;
            librg_data_init(&subdata);

            librg_event_t event = {0};
            event.data = subdata; event.entity = entity;
            librg_event_trigger(LIBRG_CLIENT_STREAMER_UPDATE, &event);

            librg_data_wptr(&subdata, transform, sizeof(librg_transform_t));
            librg_data_wu32(&data, entity);
            librg_data_wu32(&data, librg_data_get_wpos(&subdata));

            // write sub-bitstream to main bitstream
            librg_data_wptr(&data, subdata, librg_data_get_wpos(&subdata));
            librg_data_free(&subdata);

            amount++;
        });

        if (amount < 1) {
            return;
        }

        // write amountafter packet id
        librg_data_wu32_at(&data, amount, sizeof(u64));

        enet_peer_send(librg_network.peer, LIBRG_NETWORK_STREAM_SECONDARY_CHANNEL, enet_packet_create(
            data, librg_data_get_wpos(&data), ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT
        ));

        librg_data_free(&data);
    }

    /**
     * SERVER-SIDE
     *
     * Responsive for udpating the server-side streamer
     */
    librg_internal void librg__update_server() {
        librg_entity_filter_t filter = { librg_index_client() };

        // create data and write inital stuff
        librg_data_t for_create;
        librg_data_t for_update;

        librg_data_init(&for_create);
        librg_data_init(&for_update);

        librg_entity_eachx(filter, librg_lambda(player), {
            librg_client_t *client = librg_fetch_client(player);

            // get old, and preapre new snapshot handlers
            librg_table_t *last_snapshot = &client->last_snapshot;
            librg_table_t next_snapshot = { 0 };
            librg_table_init(&next_snapshot, zpl_heap_allocator());

            // fetch entities in the steram zone
            zpl_array_t(librg_entity_t) queue = librg_streamer_query(player);

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
                librg_entity_t entity = queue[i];

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
                    librg_data_wu32(&for_create, librg_fetch_entitymeta(entity)->type);
                    librg_data_wptr(&for_create, librg_fetch_transform(entity), sizeof(librg_transform_t));

                    // request custom data from user
                    librg_event_t event = {0};
                    event.data = for_create; event.entity = entity;
                    librg_event_trigger(LIBRG_ENTITY_CREATE, &event);
                }
                else {
                    // mark entity as still alive, for the remove cycle
                    librg_table_set(last_snapshot, entity, 0);

                    // fetch client streamer
                    librg_clientstream_t *cli_stream = librg_fetch_clientstream(entity);

                    // if this entity is client streamable and this client is owner
                    if (cli_stream && cli_stream->peer == client->peer) {
                        updated_entities--;
                    }
                    // write update
                    else {
                        librg_data_wu32(&for_update, entity);
                        librg_data_wptr(&for_update, librg_fetch_transform(entity), sizeof(librg_transform_t));

                        // request custom data from user
                        librg_event_t event = {0};
                        event.data = for_update; event.entity = entity;
                        librg_event_trigger(LIBRG_ENTITY_UPDATE, &event);
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
                librg_entity_t entity = last_snapshot->entries[i].key;
                b32 not_existed = last_snapshot->entries[i].value;
                if (not_existed == 0) continue;

                // skip entity delete if this is player's entity
                if (entity == player) continue;

                // write id
                librg_data_wu32(&for_create, entity);
                removed_entities++;

                // write the rest
                librg_event_t event = {0};
                event.data = for_create; event.entity = entity;
                librg_event_trigger(LIBRG_ENTITY_REMOVE, &event);
            }
            librg_data_wu32_at(&for_create, removed_entities, write_pos);


            librg_table_destroy(&client->last_snapshot);
            *last_snapshot = next_snapshot;
            // zpl_array_copy_init(client->last_snapshot.entries, next_snapshot.entries);
            // zpl_array_copy_init(client->last_snapshot.hashes, next_snapshot.hashes);

            // send the data, via differnt channels and reliability settings
            enet_peer_send(client->peer, LIBRG_NETWORK_STREAM_PRIMARY_CHANNEL, enet_packet_create(
                for_create, librg_data_get_wpos(&for_create), ENET_PACKET_FLAG_RELIABLE));
            enet_peer_send(client->peer, LIBRG_NETWORK_STREAM_SECONDARY_CHANNEL, enet_packet_create(
                for_update, librg_data_get_wpos(&for_update), ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT));

            // and cleanup
            zpl_array_free(queue);
            ZPL_BS_HEADER(for_create)->write_pos = 0;
            ZPL_BS_HEADER(for_update)->write_pos = 0;
        });

        zpl_bs_free(for_create);
        zpl_bs_free(for_update);
    }

    librg_inline void librg__entity_execute_insert() {
        librg_entity_filter_t filter = { librg_index_streamable() };

        // clear
        zplc_clear(&librg__streamer);

        // fill up
        librg_entity_eachx(filter, librg_lambda(entity), {
            librg_transform_t *transform = librg_fetch_transform(entity);
            librg_assert(transform);

            zplc_node_t node = { 0 };

            node.tag      = entity;
            node.position = transform->position;

            zplc_insert(&librg__streamer, node);
        });
    }

    librg_internal ZPL_TIMER_CB(librg__tick_cb) {
        u64 start = zpl_utc_time_now();

        if (librg_is_client()) {
            return librg__update_client();
        }
        else {
            // create the server cull tree
            librg__entity_execute_insert();

            librg__update_server();

            // destroy entities queued for removal
            librg__entity_execute_destroy();
        }

        librg_dbg(" [update: %fms]   \r", (zpl_utc_time_now() - start) / 1000.f);
    }

    void librg_streamer_client_set(librg_entity_t entity, librg_peer_t peer) {
        librg_assert(peer);
        librg_clientstream_t *component = librg_fetch_clientstream(entity);

        // replace current entity owner
        if (component) {
            if (component->peer == peer) {
                return;
            }

            component->peer = peer;
        }
        // attach new entity owner
        else {
            librg_attach_clientstream(entity, { peer });
        }

        librg_send_to(LIBRG_CLIENT_STREAMER_ADD, peer, librg_lambda(data), {
            librg_data_wentity(&data, entity);
        });
    }

    void librg_streamer_client_remove(librg_entity_t entity) {
        librg_clientstream_t *component = librg_fetch_clientstream(entity);

        if (!component) {
            return;
        }

        librg_send_to(LIBRG_CLIENT_STREAMER_REMOVE, component->peer, librg_lambda(data), {
            librg_data_wentity(&data, entity);
        });

        librg_detach_clientstream(entity);
    }

    librg_entity_t librg_get_client_entity(librg_peer_t peer) {
        librg_assert(peer);
        librg_entity_t *entity = librg_peers_get(&librg_network.connected_peers, (u64)peer);
        librg_assert(entity);

        return *entity;
    }




    /**
     *
     * CORE
     *
     */

    void librg_init(librg_config_t config) {
        librg_dbg("librg_init\n");
        librg__config = config;

        // apply default settings (if no user provided)
        librg__set_default(librg__config.tick_delay, 32);
        librg__set_default(librg__config.max_connections, 16);
        librg__set_default(librg__config.max_entities, 16384);
        librg__set_default(librg__config.mode, LIBRG_MODE_SERVER);
        librg__set_default(librg__config.world_size.x, 4096.0f);
        librg__set_default(librg__config.world_size.y, 4096.0f);

        // init entity system
        librg__entity.shared.limit_lower = 0;
        librg__entity.shared.limit_upper = librg__config.max_entities;
        librg__entity.native.limit_lower = librg__config.max_entities;
        librg__entity.native.limit_upper = librg__config.max_entities * 2;

        zpl_array_init(librg__component_pool, LIBRG_ENTITY_ALLOCATOR());
        zpl_array_init(librg__entity_remove_queue, zpl_heap_allocator());

        // streamer
        zplc_bounds_t world = {0};
        world.centre = zplm_vec3(0, 0, 0);
        world.half_size = zplm_vec3(librg__config.world_size.x, librg__config.world_size.y, 0);
        zplc_init(&librg__streamer, zpl_heap_allocator(), zplc_dim_2d_ev, world, 4);
        librg_table_init(&librg__entity.ignored, zpl_heap_allocator());

        // events
        zplev_init(&librg__events, zpl_heap_allocator());
        zpl_buffer_init(librg__messages, zpl_heap_allocator(), LIBRG_NETWORK_MESSAGE_CAPACITY);

        // init timers
        zpl_array_init(librg__timers, zpl_heap_allocator());
        zpl_timer_t *tick_timer = zpl_timer_add(librg__timers);
        zpl_timer_set(tick_timer, 1000 * librg__config.tick_delay, -1, librg__tick_cb);
        zpl_timer_start(tick_timer, 1000);

        // network
        librg_assert_msg(enet_initialize() == 0, "cannot initialize enet");

        // add event handlers for our network stuufz
        librg__messages[LIBRG_CONNECTION_INIT]        = librg__callback_connection_init;
        librg__messages[LIBRG_CONNECTION_REQUEST]     = librg__callback_connection_request;
        librg__messages[LIBRG_CONNECTION_REFUSE]      = librg__callback_connection_refuse;
        librg__messages[LIBRG_CONNECTION_ACCEPT]      = librg__callback_connection_accept;
        librg__messages[LIBRG_CONNECTION_DISCONNECT]  = librg__callback_connection_disconnect;
        librg__messages[LIBRG_ENTITY_CREATE]          = librg__callback_entity_create;
        librg__messages[LIBRG_ENTITY_UPDATE]          = librg__callback_entity_update;
        librg__messages[LIBRG_CLIENT_STREAMER_ADD]    = librg__callback_entity_client_streamer_add;
        librg__messages[LIBRG_CLIENT_STREAMER_REMOVE] = librg__callback_entity_client_streamer_remove;
        librg__messages[LIBRG_CLIENT_STREAMER_UPDATE] = librg__callback_entity_client_streamer_update;
    }

    void librg_free() {
        librg_dbg("librg_free\n");

        // free all timers and events first
        zpl_array_free(librg__timers);
        zpl_buffer_free(librg__messages, zpl_heap_allocator());
        zplev_destroy(&librg__events);
        zpl_array_free(librg__entity_remove_queue);

        // streamer
        zplc_free(&librg__streamer);
        librg_table_destroy(&librg__entity.ignored);

        // free the entity component pools
        for (i32 i = 0; i < zpl_array_count(librg__component_pool); i++) {
            librg_assert(librg__component_pool[i]);
            __dummypool_t *h = cast(__dummypool_t*)librg__component_pool[i];
            zpl_buffer_free(h->entities, h->backing);
            zpl_buffer_free(h->data, h->backing);
        }

        // free containers and entity pool
        zpl_array_free(librg__component_pool);

        enet_deinitialize();
    }


    /**
     *
     * EXTENSIONS
     *
     */

    #undef __dummypool_t
    #undef __dummymeta_t

    #define librg_component_define(NAME) librg_component_define_inner(,NAME)
    #undef librg_component
    #define librg_component(NAME) \
        librg_component_declare(NAME) \
        librg_component_define(NAME)

#ifdef __cplusplus
}
#endif

#endif // LIBRG_IMPLEMENTATION
#endif // LIBRG_INCLUDE_H
