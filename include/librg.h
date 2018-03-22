/**
 * librg - a library for building simple and elegant cross-platform mmo client-server solutions.
 *
 * Usage:
 * #define LIBRG_IMPLEMENTATION exactly in ONE source file right BEFORE including the library, like:
 *
 * #define LIBRG_IMPLEMENTATION
 * #include <librg.h>
 *
 * Credits:
 * Vladyslav Hrytsenko (GitHub: inlife)
 * Dominik Madarasz (GitHub: zaklaus)
 *
 * Dependencies:
 * zpl.h
 * zpl_math.h
 * enet.h
 *
 * For the demo:
 * sdl2.h
 *
 * Version History:
 * 3.2.0
 * - Fixed minor memory client-side memory leak with empty control list
 * - Fixed issue with client stream update and removed entity on server
 * - Updated zpl to new major version, watch out for possible incompatibilities
 * - Added method for alloc/dealloc the librg_ctx_t, librg_data_t, librg_event_t for the bindings
 * - Added experimental support for update buffering, disabled by default, and not recommended to use
 * - Added built-in timesyncer, working on top of monotonic time, syncing client clock to server one
 * - Added helper methods: librg_time_now, librg_standard_deviation
 * - Changed ctx->tick_delay from u16 to f64 (slightly more precision)
 *
 * Version History:
 * 3.1.0
 * - Removed zpl_cull and zpl_event dependencies
 * - added librg_network_kick()
 * - saving current librg_address_t to ctx->network
 * - refactor to proper disconnection code
 * - exclude local client entity from LIBRG_CONNECTION_DISCONNECT
 * - moved options and some few other things to the implementation part
 * - fixed issue with replacing entity control
 * - fixed issue with add control queuing beign sent before create entity packet
 *
 * 3.0.7 - Fix for entity query dublication for player entities
 * 3.0.5 - Patched librg_callback_cb arg value
 * 3.0.4 - Fixed Android and iOS support
 * 3.0.3 - Small fixes
 * 3.0.2 - Dependency updates
 * 3.0.1 - minor api patch
 * 3.0.0 - contexts, major api changes, fried potato, other stuff
 *
 * 2.2.3 - fixed mem leak on net event
 * 2.2.2 - Fixed client issue with librg_message_send_instream_except
 * 2.2.1 - Fixed cpp issues with librg_data_t pointers
 * 2.2.0 - Inner message system rafactor
 * 2.1.0 - Inner bitstream refactors, with slight interface changes
 * 2.0.2 - C++ and MSVC related fixes
 * 2.0.0 - Initial C version rewrite
 *
 * Things TODO:
 * v3.3.0?
 * - Add method to check if entity is in stream of other entity
 * - Add DEBUG packet size validation (FEATURE)
 * - refactoring librg_table_t (FEATURE)
 * - remove entity ignore for target entity that was disconnected/deleted (BUG)
 * - remove controller peer for entity, on owner disconnect (BUG)
 * - possibly adding stream_range to the query, to make it bi-sided (FEATURE)
 * - tree/space node flattening (FEATURE)
 *                                                                 
 * https://antriel.com/post/online-platformer-5/#server-update-data
 *                                                                 
 * Copyright 2017 Vladyslav Hrytsenko
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
#define LIBRG_VERSION_MINOR 2
#define LIBRG_VERSION_PATCH 0
#define LIBRG_VERSION_CREATE(major, minor, patch) (((major)<<16) | ((minor)<<8) | (patch))
#define LIBRG_VERSION_GET_MAJOR(version) (((version)>>16)&0xFF)
#define LIBRG_VERSION_GET_MINOR(version) (((version)>>8)&0xFF)
#define LIBRG_VERSION_GET_PATCH(version) ((version)&0xFF)
#define LIBRG_VERSION LIBRG_VERSION_CREATE(LIBRG_VERSION_MAJOR, LIBRG_VERSION_MINOR, LIBRG_VERSION_PATCH)

// disable asserts for release build
#if !defined(LIBRG_DEBUG) || defined(LIBRG_NO_ASSERT)
#define ZPL_ASSERT_MSG(cond, msg, ...)
#endif

/* include definitions */
#ifndef LIBRG_CUSTOM_INCLUDES
    #ifdef ZPL_SYSTEM_WINDOWS
        #define _WINSOCK_DEPRECATED_NO_WARNINGS
    #endif
    #if defined(ZPL_SYSTEM_UNIX) && !defined(HAS_SOCKLEN_T)
        #define HAS_SOCKLEN_T
    #endif
    #ifdef LIBRG_IMPLEMENTATION
        #define ZPL_IMPLEMENTATION
        #define ZPLM_IMPLEMENTATION
        #define ENET_IMPLEMENTATION
    #endif
    #include "zpl.h"
    #include "zpl_math.h"
    #include "enet.h"
#endif

/* library mode (stastic or dynamic) */
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

#ifndef LIBRG_INTERNAL
#define LIBRG_INTERNAL zpl_internal
#endif

#define librg_global        zpl_global
#define librg_inline        zpl_inline
#define librg_internal      zpl_internal
#define librg_assert        ZPL_ASSERT
#define librg_assert_msg    ZPL_ASSERT_MSG
#define librg_lambda(name)  name

/* default constants/methods used */
#ifndef LIBRG_DATA_GROW_FORMULA
#define LIBRG_DATA_GROW_FORMULA(x) (2*(x) + 16)
#endif

#if !defined(librg_log)
#define librg_log zpl_printf
#endif

#if defined(LIBRG_DEBUG)
#define librg_dbg(fmt, ...) librg_log(fmt, ##__VA_ARGS__)
#else
#define librg_dbg(fmt, ...)
#endif

#ifdef __cplusplus
extern "C" {
#endif

// =======================================================================//
// !
// ! Public API
// !
// =======================================================================//

struct librg_ctx_t;
struct librg_data_t;
struct librg_entity_t;
struct librg_address_t;
struct librg_message_t;
struct librg_event_t;

typedef ENetPeer   librg_peer_t;
typedef ENetHost   librg_host_t;
typedef ENetPacket librg_packet_t;

enum librg_mode         { LIBRG_MODE_SERVER, LIBRG_MODE_CLIENT };
enum librg_space_type   { LIBRG_SPACE_2D = 2, LIBRG_SPACE_3D = 3 };
enum librg_pointer_type { LIBRG_POINTER_CTX, LIBRG_POINTER_DATA, LIBRG_POINTER_EVENT };
enum librg_thread_state { LIBRG_THREAD_IDLE, LIBRG_THREAD_WORK, LIBRG_THREAD_EXIT };

/**
 * Simple host address
 * used to configure network on start
 */
typedef struct librg_address_t { i32 port; char *host; } librg_address_t;

typedef void (librg_entity_cb)(struct librg_ctx_t *ctx, struct librg_entity_t *entity);
typedef void (librg_message_cb)(struct librg_message_t *msg);
typedef void (librg_event_cb)(struct librg_event_t *event);
typedef zpl_array(librg_event_cb *) librg_event_block;

ZPL_TABLE_DECLARE(extern, librg_table_t, librg_table_, u32);
ZPL_TABLE_DECLARE(static, librg_event_pool, librg_event_pool_, librg_event_block);

/**
 * Allocate librg ctx
 * (to be used inside bindings)
 */
LIBRG_API void *librg_allocate_ptr(enum librg_pointer_type pointer_type);
/**
 * Frees a pointer allocated by library
 * usually used in bindings.
 */
LIBRG_API void librg_release_ptr(void *ptr);
/**
 * Main initialization method
 * MUST BE called in the begging of your application
 */
LIBRG_API void librg_init(struct librg_ctx_t *ctx);
/**
 * Should be called at the end of
 * execution of the program
 */
LIBRG_API void librg_free(struct librg_ctx_t *ctx);
/**
 * Main tick method
 * MUST BE called in your loop
 * preferably w/o delays
 *
 * Calling tick ensures that packets will be sent and received,
 * world state will be recalculated and new update data will be created/used
 */
LIBRG_API void librg_tick(struct librg_ctx_t *ctx);

// =======================================================================//
// !
// ! Entities
// !
// =======================================================================//

#define librg_entity_id u32

enum librg_entity_flag {
    LIBRG_ENTITY_NONE       = 0, /* general flag, all destroyed/non-created entities have it */
    LIBRG_ENTITY_ALIVE      = (1 << 0), /* general flag, all created entities have it */
    LIBRG_ENTITY_CLIENT     = (1 << 1), /* flag describing entities created for client peer */
    LIBRG_ENTITY_IGNORING   = (1 << 2), /* flag showing that entity has ignore overrides */
    LIBRG_ENTITY_QUERIED    = (1 << 3), /* flag showing that entity has a cached culler query */
    LIBRG_ENTITY_CONTROLLED = (1 << 4), /* flag showing if the entity is controlled(streamed) by some peer */
    LIBRG_ENTITY_UNUSED     = (1 << 5), /* flag showing whether the entity's space is unused */
};

typedef struct librg_entity_t {
    u32 id;
    u32 type;
    u64 flags;

    zplm_vec3 position;
    f32 stream_range;

    void *user_data;
    struct librg_space_t *stream_branch;

    librg_table_t ignored;
    librg_table_t last_snapshot;

    librg_peer_t *client_peer;
    librg_peer_t *control_peer;

    zpl_array(librg_entity_id) last_query;
} librg_entity_t;

/**
 * Create entity and return handle
 */
LIBRG_API struct librg_entity_t *librg_entity_create(struct librg_ctx_t *ctx, u32 type);
/**
 * Try to find an existing entity by a provided id, or NULL
 */
LIBRG_API struct librg_entity_t *librg_entity_fetch(struct librg_ctx_t *ctx, librg_entity_id entity_id);
/**
 * Reuqest an entity destruction (will be marked as destroyed, and send to be destroyed on clients on the next tick)
 */
LIBRG_API void librg_entity_destroy(struct librg_ctx_t *ctx, librg_entity_id entity_id);
/**
 * Query for entities that are in stream zone
 * for current entity, and are visible to this entity
 * an uninitialized pointer to the pointer of librg_entity_id should be provided:
 *
 * EXAMPLE:
 * librg_entity_id *result;
 * usize amount = librg_entity_query(&ctx, 15, &result);
 */
LIBRG_API usize librg_entity_query(struct librg_ctx_t *ctx, librg_entity_id entity_id, librg_entity_id **result);
/**
 * Check if provided entity is a valid entity
 */
LIBRG_API b32 librg_entity_valid(struct librg_ctx_t *ctx, librg_entity_id id);
/**
 * Try to find an entity that is assossiated with a particular peer, or NULL
 */
LIBRG_API struct librg_entity_t *librg_entity_find(struct librg_ctx_t *ctx, librg_peer_t *peer);
/**
 * Set particular entity visible or invisible
 * for other entities in stream zone
 */
LIBRG_API void librg_entity_visibility_set(struct librg_ctx_t *ctx, librg_entity_id entity_id, b32 state);
/**
 * Set particular entity visible or invisible
 * for other particular entity
 */
LIBRG_API void librg_entity_visibility_set_for(struct librg_ctx_t *ctx, librg_entity_id entity_id, librg_entity_id target, b32 state);
/**
 * Get particular entity visible or invisible
 * for other entities in stream zone
 */
LIBRG_API b32 librg_entity_visibility_get(struct librg_ctx_t *ctx, librg_entity_id entity_id);
/**
 * Get particular entity visible or invisible
 * for other particular entity
 */
LIBRG_API b32 librg_entity_visibility_get_for(struct librg_ctx_t *ctx, librg_entity_id entity_id, librg_entity_id target);
/**
 * Set some entity as client streamable
 * Which means, that client will become responsive for sending
 * updates about this entity
 *
 * And this entity wont be sent to the client, until he stops being the streamer
 *
 * Setting other client as streamer, will remove previous streamer from entity
 */
LIBRG_API void librg_entity_control_set(struct librg_ctx_t *ctx, librg_entity_id entity_id, librg_peer_t *peer);
/**
 * Get controller of the entity
 */
LIBRG_API librg_peer_t *librg_entity_control_get(struct librg_ctx_t *ctx, librg_entity_id entity_id);
/**
 * Remove some entity from stream ownership of the client
 */
LIBRG_API void librg_entity_control_remove(struct librg_ctx_t *ctx, librg_entity_id entity_id);
/**
 * Iterate over all the entities with a flag
 */
LIBRG_API void librg_entity_iterate(struct librg_ctx_t *ctx, u64 flags, librg_entity_cb callback);
/**
 * C based entity iteration macro
 */
#define librg_entity_iteratex(ctx, cflags, cname, code) do { \
    for (int _ent = 0, _valid = 0; _ent < ctx->max_entities && _valid < ctx->entity.count; ++_ent) { \
        if ((ctx->entity.list[_ent].flags & (LIBRG_ENTITY_ALIVE | cflags)) == (LIBRG_ENTITY_ALIVE | cflags)) { \
            _valid++; librg_entity_id cname = _ent; code; \
        } \
    } \
} while (0);
/**
 * Return entity type
 * @deprecated
 */
LIBRG_API u32 librg_entity_type(struct librg_ctx_t *ctx, librg_entity_id entity_id);

// =======================================================================//
// !
// ! Events
// !
// =======================================================================//

/**
 * Default built-in events
 * define your events likes this:
 *     enum {
 *         MY_NEW_EVENT_1 = LIBRG_LAST_EVENT,
 *         MY_NEW_EVENT_2,
 *         MY_NEW_EVENT_3,
 *     };
 */
enum librg_events {
    LIBRG_CONNECTION_INIT,
    LIBRG_CONNECTION_REQUEST,
    LIBRG_CONNECTION_REFUSE,
    LIBRG_CONNECTION_ACCEPT,
    LIBRG_CONNECTION_DISCONNECT,
    LIBRG_CONNECTION_TIMESYNC,

    LIBRG_ENTITY_CREATE,
    LIBRG_ENTITY_UPDATE,
    LIBRG_ENTITY_REMOVE,
    LIBRG_CLIENT_STREAMER_ADD,
    LIBRG_CLIENT_STREAMER_REMOVE,
    LIBRG_CLIENT_STREAMER_UPDATE,

    LIBRG_EVENT_LAST,
};

enum librg_event_flags {
    LIBRG_EVENT_NONE        = 0,        /* default empty user-created event */
    LIBRG_EVENT_REJECTED    = (1 << 0), /* whether or not this event was rejected */
    LIBRG_EVENT_REJECTABLE  = (1 << 1), /* can this event be rejected by user */
    LIBRG_EVENT_REMOTE      = (1 << 2), /* event was based on network message */
    LIBRG_EVENT_LOCAL       = (1 << 3), /* event was created locally */
};

typedef struct librg_event_t {
    struct librg_ctx_t *ctx;   /* librg context where event has been called */
    struct librg_data_t *data;  /* optional: data is used for built-in events */
    struct librg_entity_t *entity; /* optional: entity is used for built-in events */

    librg_peer_t *peer;  /* optional: peer is used for built-in events */

    u64 flags;  /* flags for that event */
    void *user_data; /* optional: user information */
} librg_event_t;

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
LIBRG_API u64 librg_event_add(struct librg_ctx_t *ctx, u64 id, librg_event_cb callback);
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
LIBRG_API void librg_event_trigger(struct librg_ctx_t *ctx, u64 id, struct librg_event_t *event);
/**
 * Used to remove particular callback from
 * event chain, so it wont be called ever again
 *
 * @param id usually you define event ids inside enum
 * @param index returned by librg_event_add
 */
LIBRG_API void librg_event_remove(struct librg_ctx_t *ctx, u64 id, u64 index);
/**
 * Used to reject some event from triggering from
 * inside of executing callback
 */
LIBRG_API void librg_event_reject(struct librg_event_t *event);
/**
 * Used to check if some event can be rejected
 */
LIBRG_API b32 librg_event_rejectable(struct librg_event_t *event);
/**
 * Checks if current event was not rejected
 * inside any of the callbacks
 */
LIBRG_API b32 librg_event_succeeded(struct librg_event_t *event);

// =======================================================================//
// !
// ! Binary data (bitstream)
// !
// =======================================================================//

typedef struct librg_data_t {
    usize capacity;
    usize read_pos;
    usize write_pos;

    void *rawptr;

    zpl_allocator_t allocator;
} librg_data_t;

/**
 * Initialize new bitstream with default mem size
 */
LIBRG_API void librg_data_init(struct librg_data_t *data);
/**
 * Initialize new bitstream with custom mem size
 */
LIBRG_API void librg_data_init_size(struct librg_data_t *data, usize size);
/**
 * Initialize new bitstream with custom mem size (bindings stuff)
 */
LIBRG_API struct librg_data_t *librg_data_init_new();
/**
 * Free initialized bitstream
 */
LIBRG_API void librg_data_free(struct librg_data_t *data);
/**
 * Reset initialized bitstream
 * NOTE: doesnt remove any data, just resets read and write pos to 0
 */
LIBRG_API void librg_data_reset(struct librg_data_t *data);
/**
 * Increase size of bitstream
 */
LIBRG_API void librg_data_grow(struct librg_data_t *data, usize min_size);
/**
 * Methods for getting various parameters of bitstream
 */
LIBRG_API usize librg_data_capacity(struct librg_data_t *data);
LIBRG_API usize librg_data_get_rpos(struct librg_data_t *data);
LIBRG_API usize librg_data_get_wpos(struct librg_data_t *data);
LIBRG_API void  librg_data_set_rpos(struct librg_data_t *data, usize position);
LIBRG_API void  librg_data_set_wpos(struct librg_data_t *data, usize position);
/**
 * Read and write methods for custom sized data
 */
LIBRG_API void librg_data_rptr(struct librg_data_t *data, void *ptr, usize size);
LIBRG_API void librg_data_wptr(struct librg_data_t *data, void *ptr, usize size);
/**
 * Read and write methods for custom sized data
 * at particular position in memory
 */
LIBRG_API void librg_data_rptr_at(struct librg_data_t *data, void *ptr, usize size, isize position);
LIBRG_API void librg_data_wptr_at(struct librg_data_t *data, void *ptr, usize size, isize position);
/**
 * A helprer macro for onliner methods
 */
#define LIBRG_GEN_DATA_READWRITE(TYPE) \
    LIBRG_API TYPE ZPL_JOIN2(librg_data_r,TYPE)(struct librg_data_t *data); \
    LIBRG_API void ZPL_JOIN2(librg_data_w,TYPE)(struct librg_data_t *data, TYPE value); \
    LIBRG_API TYPE ZPL_JOIN3(librg_data_r,TYPE,_at)(struct librg_data_t *data, isize position); \
    LIBRG_API void ZPL_JOIN3(librg_data_w,TYPE,_at)(struct librg_data_t *data, TYPE value, isize position);
/**
 * General one-line methods for reading/writing different types
 */
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
#define librg_data_went ZPL_JOIN2(librg_data_w, librg_entity_id)
#define librg_data_rent ZPL_JOIN2(librg_data_r, librg_entity_id)

// =======================================================================//
// !
// ! Network
// !
// =======================================================================//

#define librg_message_id u16

#define librg_data_wmid ZPL_JOIN2(librg_data_w, librg_message_id)
#define librg_data_rmid ZPL_JOIN2(librg_data_r, librg_message_id)

/**
 * Message structure
 * created inside network handler
 * and injected to each incoming message
 */
typedef struct librg_message_t {
    struct librg_ctx_t  *ctx;
    struct librg_data_t *data;

    librg_peer_t *peer;
    librg_packet_t *packet;

    void *user_data; /* optional: user information */
} librg_message_t;

/**
 * Check are we connected
 */
LIBRG_API b32 librg_is_connected(struct librg_ctx_t *ctx);
/**
 * Is librg instance is running
 * in the server mode
 */
LIBRG_API b32 librg_is_server(struct librg_ctx_t *ctx);
/**
 * Is librg instance is running
 * in the client mode
 */
LIBRG_API b32 librg_is_client(struct librg_ctx_t *ctx);
/**
 * Starts network connection
 * Requires you to provide .port (if running as server)
 * or both .port and .host (if running as client)
 *
 * For server mode - starts server
 * For client mode - starts client, and connects to provided host & port
 */
LIBRG_API void librg_network_start(struct librg_ctx_t *ctx, librg_address_t address);
/**
 * Disconnects (if connected), stops network
 * and releases resources
 */
LIBRG_API void librg_network_stop(struct librg_ctx_t *ctx);
/**
 * Forces disconnection for provided peer
 * @param ctx
 * @param peer
 */
LIBRG_API void librg_network_kick(struct librg_ctx_t *ctx, librg_peer_t *peer);
/**
 * Can be used to add handler
 * to a particular message id
 */
LIBRG_API void librg_network_add(struct librg_ctx_t *ctx, librg_message_id id, librg_message_cb callback);
/**
 * Can be used to remove a handler
 * from particular message id
 */
LIBRG_API void librg_network_remove(struct librg_ctx_t *ctx, librg_message_id id);
/**
 * Part of message API
 * Takes in initialized void of size pointer with written packet id
 * and sends data to all connected peers ( or to server if its client )
 */
LIBRG_API void librg_message_send_all(struct librg_ctx_t *ctx, librg_message_id id, void *data, usize size);
/**
 * Part of message API
 * Applies all from previous mehod
 * But data will be sent only to particular provided peer
 */
LIBRG_API void librg_message_send_to(struct librg_ctx_t *ctx, librg_message_id id, librg_peer_t *peer, void *data, usize size);
/**
 * Part of message API
 * Applies all from previous mehod
 * But data will be sent to all except provided peer
 */
LIBRG_API void librg_message_send_except(struct librg_ctx_t *ctx, librg_message_id id, librg_peer_t *peer, void *data, usize size);
/**
 * Part of message API
 * Applies all from previous mehod
 * Data will be sent only to entities, which are inside streamzone
 * for provided entity
 */
LIBRG_API void librg_message_send_instream(struct librg_ctx_t *ctx, librg_message_id id, librg_entity_id entity_id, void *data, usize size);
/**
 * Part of message API
 * Applies all from previous mehod
 * Data will be sent only to entities, which are inside streamzone
 * for provided entity except peer
 */
LIBRG_API void librg_message_send_instream_except(struct librg_ctx_t *ctx, librg_message_id id, librg_entity_id entity_id, librg_peer_t *peer, void *data, usize size);

// =======================================================================//
// !
// ! Helper methods
// !
// =======================================================================//

/**
 * Get current execution time
 * On server side returns time aprox since process is up and running
 * On client side returns synced time client time to server time.
 * Ideally calling this method on both connected client and server at the same time
 * will return pretty much similar time, or at least 2 that are quite close to one another
 *
 * Retuned time is a CLOCK_MONOTONIC, the format is second before comma,
 * and parts lesser than second after comma
 *
 * EXAMPLE:
 *     1.2424244 - 1 second 242 milliseconds
 *     255.4224244 - 255 seconds
 */
LIBRG_API f64 librg_time_now(struct librg_ctx_t *ctx);
/**
 * Calcualte standard devaiation for a set of values
 * Example:
 * f64 values[3] = { 1, 2, 3 };
 * f64 dev = librg_standard_deviation(&values, 3);
 */
LIBRG_API f64 librg_standard_deviation(f64 *values, usize size);

// =======================================================================//
// !
// ! Options
// !
// =======================================================================//

enum librg_options {
    LIBRG_PLATFORM_ID,
    LIBRG_PLATFORM_PROTOCOL,
    LIBRG_PLATFORM_BUILD,

    LIBRG_DEFAULT_CLIENT_TYPE,
    LIBRG_DEFAULT_STREAM_RANGE,
    LIBRG_DEFAULT_DATA_SIZE,

    LIBRG_NETWORK_CAPACITY,
    LIBRG_NETWORK_CHANNELS,
    LIBRG_NETWORK_PRIMARY_CHANNEL,
    LIBRG_NETWORK_SECONDARY_CHANNEL,
    LIBRG_NETWORK_MESSAGE_CHANNEL,
    LIBRG_NETWORK_BUFFER_SIZE,

    LIBRG_MAX_ENTITIES_PER_BRANCH,
    LIBRG_MAX_THREADS_PER_UPDATE,

    LIBRG_OPTIONS_SIZE,
};

/**
 * Get global cross-instance option for librg
 */
LIBRG_API u32 librg_option_get(u32 option);
/**
 * Set global cross-instance option for librg
 */
LIBRG_API void librg_option_set(u32 option, u32 value);

// =======================================================================//
// !
// ! Main context definition
// !
// =======================================================================//

typedef struct librg_snapshot {
    f64 time;
    void *data;
    usize size;
    librg_peer_t *peer;
} librg_snapshot;

ZPL_RING_DECLARE(librg_snapshot);

typedef struct librg_space_node_t {
    struct librg_entity_t *blob;
    b32 unused;
} librg_space_node_t;

typedef struct librg_space_t {
    zpl_allocator_t allocator;
    u32 max_nodes;
    isize dimensions;
    zplm_aabb3 boundary;
    zplm_vec3 min_bounds;
    b32 use_min_bounds;
    zpl_array(usize) free_nodes;
    zpl_array(struct librg_space_t) spaces;
    zpl_array(struct librg_space_node_t) nodes;
} librg_space_t;

#define LIBRG_TIMESYNC_SIZE 5
#define LIBRG_DATA_STREAMS_AMOUNT 4

/**
 * Context + config struct
 */
typedef struct librg_ctx_t {
    // core
    u16 mode;
    f64 tick_delay;

    // configuration
    u16 max_connections;
    u32 max_entities;

    zplm_vec3 world_size;
    zplm_vec3 min_branch_size;

    f64 last_update;
    void *user_data;

    struct {
        librg_peer_t *peer;
        librg_host_t *host;
        librg_table_t connected_peers;
        librg_address_t last_address;
    } network;

    struct {
        u32 count;
        u32 cursor;
        librg_table_t ignored;
        struct librg_entity_t *list;
        zpl_array(librg_entity_id) remove_queue;
        zpl_array(librg_message_t *) add_control_queue;
    } entity;

    union {
        struct {
            librg_data_t stream_input;
            librg_data_t stream_output;
            librg_data_t stream_upd_reliable;
            librg_data_t stream_upd_unreliable;
        };

        librg_data_t streams[LIBRG_DATA_STREAMS_AMOUNT];
    };

    #ifdef LIBRG_MULTITHREADED
    struct {
        zpl_atomic32_t signal;
        zpl_atomic32_t work_count;
        zpl_thread_t   *update_workers;
        zpl_mutex_t    *send_lock;
    } threading;
    #endif

    struct {
        f64 start_time;
        f64 offset_time;
        f64 median;
        f64 history[LIBRG_TIMESYNC_SIZE];
        f32 server_delay;
        zpl_timer_t *timer;
    } timesync;

    usize buffer_size;
    zpl_timer_t *buffer_timer;
    zpl_ring_librg_snapshot buffer;
    zpl_buffer_t(librg_message_cb *) messages;

    zpl_allocator_t     allocator;
    zpl_timer_pool      timers;
    librg_event_pool    events;
    librg_space_t       world;

} librg_ctx_t;

// =======================================================================//
// !
// ! Extensions
// !
// =======================================================================//

#if 1
    #define librg__send_internal(CTX, ID, NAME, CALLBACK_CODE, SEND_CODE) \
        librg_data_t NAME; \
        librg_data_init(&NAME); \
        CALLBACK_CODE; SEND_CODE; \
        librg_data_free(&NAME);

    #define librg_send_all(CTX, ID, NAME, CALLBACK_CODE) do { \
        librg__send_internal(CTX, ID, NAME, CALLBACK_CODE, { \
            librg_message_send_all(CTX, ID, NAME.rawptr, librg_data_get_wpos(&NAME)); \
        }); \
    } while(0);

    #define librg_send_to(CTX, ID, PEER, NAME, CALLBACK_CODE) do { \
        librg__send_internal(CTX, ID, NAME, CALLBACK_CODE, { \
            librg_message_send_to(CTX, ID, PEER, NAME.rawptr, librg_data_get_wpos(&NAME)); \
        }); \
    } while(0);

    #define librg_send_except(CTX, ID, PEER, NAME, CALLBACK_CODE) do { \
        librg__send_internal(CTX, ID, NAME, CALLBACK_CODE, { \
            librg_message_send_except(CTX, ID, PEER, NAME.rawptr, librg_data_get_wpos(&NAME)); \
        }); \
    } while(0);

    #define librg_send_instream(CTX, ID, ENTITY, NAME, CALLBACK_CODE) do { \
        librg__send_internal(CTX, ID, NAME, CALLBACK_CODE, { \
            librg_message_send_instream(CTX, ID, ENTITY, NAME.rawptr, librg_data_get_wpos(&NAME)); \
        }); \
    } while(0);

    #define librg_send_instream_except(CTX, ID, ENTITY, PEER, NAME, CALLBACK_CODE) do { \
        librg__send_internal(CTX, ID, NAME, CALLBACK_CODE, { \
            librg_message_send_instream(CTX, ID, ENTITY, PEER, NAME.rawptr, librg_data_get_wpos(&NAME)); \
        }); \
    } while(0);

    #define librg_send librg_send_all
#endif

#ifdef __cplusplus
}
#endif

#if defined(LIBRG_IMPLEMENTATION) && !defined(LIBRG_IMPLEMENTATION_DONE)
#define LIBRG_IMPLEMENTATION_DONE

#ifdef __cplusplus
extern "C" {
#endif

// =======================================================================//
// !
// ! Implementation part
// !
// =======================================================================//

    ZPL_TABLE_DEFINE(librg_event_pool, librg_event_pool_, librg_event_block);
    ZPL_TABLE_DEFINE(librg_table_t, librg_table_, u32);
    ZPL_RING_DEFINE(librg_snapshot);

    /* internal declarations */
    LIBRG_INTERNAL void librg__callback_connection_init(librg_message_t *msg);
    LIBRG_INTERNAL void librg__callback_connection_request(librg_message_t *msg);
    LIBRG_INTERNAL void librg__callback_connection_refuse(librg_message_t *msg);
    LIBRG_INTERNAL void librg__callback_connection_accept(librg_message_t *msg);
    LIBRG_INTERNAL void librg__callback_connection_disconnect(librg_message_t *msg);
    LIBRG_INTERNAL void librg__callback_connection_timesync(librg_message_t *msg);

    LIBRG_INTERNAL void librg__callback_entity_create(librg_message_t *msg);
    LIBRG_INTERNAL void librg__callback_entity_update(librg_message_t *msg);
    LIBRG_INTERNAL void librg__callback_entity_client_streamer_add(librg_message_t *msg);
    LIBRG_INTERNAL void librg__callback_entity_client_streamer_remove(librg_message_t *msg);
    LIBRG_INTERNAL void librg__callback_entity_client_streamer_update(librg_message_t *msg);

    LIBRG_INTERNAL void librg__timesync_start(librg_ctx_t *ctx);
    LIBRG_INTERNAL void librg__timesync_tick(void *usrptr);
    LIBRG_INTERNAL void librg__timesync_stop(librg_ctx_t *ctx);

    LIBRG_INTERNAL void librg__buffer_init(librg_ctx_t *ctx, usize size);
    LIBRG_INTERNAL void librg__buffer_free(librg_ctx_t *ctx);
    LIBRG_INTERNAL void librg__buffer_push(librg_ctx_t *ctx, f64 time, librg_peer_t *peer, void *data, usize size);
    LIBRG_INTERNAL void librg__buffer_tick(void *usrptr);

    // short internal helper macro
    #define LIBRG_MESSAGE_TO_EVENT(NAME, MSG) \
        librg_event_t NAME = {0}; \
        NAME.peer   = MSG->peer; \
        NAME.data   = MSG->data; \
        NAME.flags  = LIBRG_EVENT_REMOTE;


// =======================================================================//
// !
// ! Work in progress area
// ! TODO: move stuff below
// !
// =======================================================================//

    /* world stuff  */

    typedef struct {
        usize id;
        usize offset;
        usize count;
        struct librg_ctx_t *ctx;
    } librg_update_worker_si_t;


    LIBRG_INTERNAL void librg__world_update(void *);

    LIBRG_INTERNAL void librg__world_entity_query(librg_ctx_t *ctx, librg_entity_id entity, librg_space_t *c, zplm_aabb3 bounds, librg_entity_id **out_entities);
    LIBRG_INTERNAL b32 librg__world_entity_destroy(librg_ctx_t *ctx, librg_entity_id id);

    /* space stuff */

    LIBRG_INTERNAL void librg__space_init(librg_space_t *c, zpl_allocator_t a, isize dims, zplm_aabb3 bounds, zplm_vec3 min_bounds, u32 max_nodes);
    LIBRG_INTERNAL void librg__space_clear(librg_space_t *c);
    b32 librg__space_remove_node(librg_space_t *c, librg_entity_t *tag);

    LIBRG_INTERNAL b32 librg__space_intersects(isize dims, zplm_aabb3 a, zplm_aabb3 b) {
        for (i32 i = 0; i < dims; ++i) {
            if (zpl_abs(a.centre.e[i] - b.centre.e[i]) > (a.half_size.e[i] + b.half_size.e[i])) return false;
        }

        return true;
    }

    LIBRG_INTERNAL b32 librg__space_contains(isize dims, zplm_aabb3 a, f32 *point) {
        for (i32 i = 0; i < dims; ++i) {
            if (!( a.centre.e[i] - a.half_size.e[i] <= point[i]
                   && a.centre.e[i] + a.half_size.e[i] >= point[i])) {
                return false;
            }
        }

        return true;
    }

// =======================================================================//
// !
// ! Genaral methods
// !
// =======================================================================//

#if 1
    void *librg_allocate_ptr(enum librg_pointer_type type) {
        void *ptr = NULL;

        switch (type) {
            case LIBRG_POINTER_CTX:
                ptr = (void *)zpl_malloc(sizeof(librg_ctx_t));
                zpl_zero_size(ptr, sizeof(librg_ctx_t));
                break;

            case LIBRG_POINTER_DATA:
                ptr = (void *)zpl_malloc(sizeof(librg_data_t));
                zpl_zero_size(ptr, sizeof(librg_data_t));
                break;

            case LIBRG_POINTER_EVENT:
                ptr = (void *)zpl_malloc(sizeof(librg_event_t));
                zpl_zero_size(ptr, sizeof(librg_event_t));
                break;
        }

        return ptr;
    }

    void librg_release_ptr(void *ptr) {
        librg_assert(ptr);
        zpl_mfree(ptr);
    }

    void librg_init(librg_ctx_t *ctx) {
        librg_dbg("librg_init\n");

        #define librg_set_default(expr, value) if (!expr) expr = value

        // apply default settings (if no user provided)
        librg_set_default(ctx->tick_delay, 32.0);
        librg_set_default(ctx->max_connections, 16);
        librg_set_default(ctx->max_entities, 8192);
        librg_set_default(ctx->world_size.x, 5000.0f);
        librg_set_default(ctx->world_size.y, 5000.0f);
        librg_set_default(ctx->min_branch_size.x, 50.0f);
        librg_set_default(ctx->min_branch_size.y, 50.0f);
        librg_set_default(ctx->mode, LIBRG_MODE_SERVER);

        if (!ctx->allocator.proc && !ctx->allocator.data) {
            ctx->allocator = zpl_heap_allocator();
        }

        #undef librg_set_default

        // init entities system
        ctx->entity.list = (librg_entity_t*)zpl_alloc(ctx->allocator, sizeof(librg_entity_t) * ctx->max_entities);
        for (librg_entity_id i = 0; i < ctx->max_entities; i++) {
            librg_entity_t blob = { i, 0 };
            ctx->entity.list[i] = blob;
        }

        zpl_array_init(ctx->entity.remove_queue, ctx->allocator);
        zpl_array_init(ctx->entity.add_control_queue, ctx->allocator);

        for (i8 i = 0; i < LIBRG_DATA_STREAMS_AMOUNT; ++i) {
            librg_data_init(&ctx->streams[i]);
        }

        // streamer
        zplm_aabb3 world = {0};
        world.centre    = zplm_vec3f(0, 0, 0);
        world.half_size = zplm_vec3f(ctx->world_size.x, ctx->world_size.y, ctx->world_size.z);
        u32 dimension   = ctx->world_size.z == 0.0f ? LIBRG_SPACE_2D : LIBRG_SPACE_3D;

        if (ctx->min_branch_size.x == -1.0f &&
            ctx->min_branch_size.y == -1.0f &&
            ctx->min_branch_size.z == -1.0f) {
            zplm_vec3 no_min_bounds = { 0 };
            ctx->min_branch_size = no_min_bounds;
        }

        librg__space_init(&ctx->world, ctx->allocator, dimension, world, ctx->min_branch_size, librg_option_get(LIBRG_MAX_ENTITIES_PER_BRANCH));
        librg_table_init(&ctx->entity.ignored, ctx->allocator);

        #ifdef LIBRG_MULTITHREADED
        // threading
        usize thread_count = librg_option_get(LIBRG_MAX_THREADS_PER_UPDATE);
        if (thread_count > 0) {
            librg_dbg("librg: warning, LIBRG_MAX_THREADS_PER_UPDATE is experimental, and highly unstable!\n");

            ctx->threading.update_workers = (zpl_thread_t *)zpl_alloc(ctx->allocator, sizeof(zpl_thread_t)*thread_count);
            usize step = ctx->max_entities / thread_count;
            ctx->threading.send_lock = (zpl_mutex_t *)zpl_alloc(ctx->allocator, sizeof(zpl_mutex_t));
            zpl_mutex_init(ctx->threading.send_lock);

            usize offset = 0;
            for (usize i = 0; i < thread_count; ++i) {
                zpl_thread_init(ctx->threading.update_workers + i);

                librg_update_worker_si_t *si = (librg_update_worker_si_t *)zpl_alloc(ctx->allocator, sizeof(librg_update_worker_si_t));
                librg_update_worker_si_t si_ = { 0 };
                *si = si_;
                si->count   = step;
                si->offset  = offset;
                si->ctx     = ctx;
                si->id      = i;

                offset += step;

                zpl_thread_start(ctx->threading.update_workers + i, librg__execute_server_entity_update_worker, si);
            }
        }
        #endif

        // events
        librg_event_pool_init(&ctx->events, ctx->allocator);
        zpl_buffer_init(ctx->messages, ctx->allocator, librg_option_get(LIBRG_NETWORK_CAPACITY));

        // init timers
        zpl_array_init(ctx->timers, ctx->allocator);
        zpl_timer_t *tick_timer = zpl_timer_add(ctx->timers);
        tick_timer->user_data = (void *)ctx; /* provide ctx as a argument to timer */
        zpl_timer_set(tick_timer, ctx->tick_delay * 0.001, -1, librg__world_update);
        zpl_timer_start(tick_timer, 0.250);

        ctx->timesync.timer = zpl_timer_add(ctx->timers);
        ctx->timesync.timer->user_data = (void *)ctx; /* provide ctx as a argument to timer */

        ctx->buffer_timer = zpl_timer_add(ctx->timers);
        ctx->buffer_timer->user_data = (void *)ctx; /* provide ctx as a argument to timer */

        // network
        u8 enet_init = enet_initialize();
        librg_assert_msg(enet_init == 0, "cannot initialize enet");

        // add event handlers for our network stufz
        ctx->messages[LIBRG_CONNECTION_INIT]        = librg__callback_connection_init;
        ctx->messages[LIBRG_CONNECTION_REQUEST]     = librg__callback_connection_request;
        ctx->messages[LIBRG_CONNECTION_REFUSE]      = librg__callback_connection_refuse;
        ctx->messages[LIBRG_CONNECTION_ACCEPT]      = librg__callback_connection_accept;
        ctx->messages[LIBRG_CONNECTION_DISCONNECT]  = librg__callback_connection_disconnect;
        ctx->messages[LIBRG_CONNECTION_TIMESYNC]    = librg__callback_connection_timesync;
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

        for (isize i = 0; i < zpl_array_count(ctx->events.entries); ++i) {
            librg_event_block *block = &ctx->events.entries[i].value;

            if (block) {
                zpl_array_free(*block);
            }
        }

        librg_event_pool_destroy(&ctx->events);

        zpl_free(ctx->allocator, ctx->entity.list);
        zpl_array_free(ctx->entity.remove_queue);
        zpl_array_free(ctx->entity.add_control_queue);

        // streamer
        librg__space_clear(&ctx->world);
        librg_table_destroy(&ctx->entity.ignored);

        #ifdef LIBRG_MULTITHREADED
        // threading
        usize thread_count = librg_option_get(LIBRG_MAX_THREADS_PER_UPDATE);
        if (thread_count > 0) {
            zpl_atomic32_store(&ctx->threading.signal, LIBRG_THREAD_EXIT);

            for (usize i = 0; i < thread_count; ++i) {
                zpl_thread_join(ctx->threading.update_workers + i);
            }

            zpl_free(ctx->allocator, ctx->threading.update_workers);
            zpl_mutex_destroy(ctx->threading.send_lock);
            zpl_free(ctx->allocator, ctx->threading.send_lock);
        }
        #endif

        for (isize i = 0; i < LIBRG_DATA_STREAMS_AMOUNT; ++i) {
            librg_data_free(&ctx->streams[i]);
        }

        enet_deinitialize();
    }

    void librg_tick(librg_ctx_t *ctx) {
        zpl_timer_update(ctx->timers);

        if (!ctx->network.host) {
            return; /* occasion where we are not started network yet */
        }

        ENetEvent event;
        while (enet_host_service(ctx->network.host, &event, 0) > 0) {
            librg_message_t msg = {0}; {
                msg.ctx     = ctx;
                msg.data    = NULL;
                msg.peer    = event.peer;
                msg.packet  = event.packet;
            }

            switch (event.type) {
                case ENET_EVENT_TYPE_RECEIVE: {
                    librg_data_t data = {0};

                    data.rawptr = event.packet->data;
                    data.capacity = event.packet->dataLength;

                    // get curernt packet id
                    librg_message_id id = librg_data_rmid(&data);
                    f64 server_time = 0;

                    if (id == LIBRG_ENTITY_UPDATE) {
                        server_time = librg_data_rf64(&data);
                        // librg_log("server_time: %f, client_predicted: %f, diff: %f\n", server_time, librg_time_now(ctx), librg_time_now(ctx) - server_time);

                        if (librg_option_get(LIBRG_NETWORK_BUFFER_SIZE) > 1) {
                            librg__buffer_push(ctx, server_time, event.peer, data.rawptr, data.capacity);
                            enet_packet_destroy(event.packet);
                            return;
                        }
                    }

                    if (ctx->messages[id]) {
                        msg.data = &data;
                        ctx->messages[id](&msg);
                    }
                    else {
                        /* print unknown message id  */
                        librg_dbg("network: unknown message: %u\n", id);
                    }

                    enet_packet_destroy(event.packet);
                } break;
                case ENET_EVENT_TYPE_CONNECT:    ctx->messages[LIBRG_CONNECTION_INIT](&msg); break;
                case ENET_EVENT_TYPE_DISCONNECT: ctx->messages[LIBRG_CONNECTION_DISCONNECT](&msg); break;
                case ENET_EVENT_TYPE_NONE: break;
            }
        }
    }
#endif

// =======================================================================//
// !
// ! Entities
// !
// =======================================================================//

#if 1
    librg_entity_t *librg_entity_create(librg_ctx_t *ctx, u32 type) {
        librg_assert(ctx);
        librg_assert(librg_is_server(ctx));
        librg_assert_msg(ctx->entity.count < ctx->max_entities, "reached max_entities limit");

        ++ctx->entity.count;

        if (ctx->entity.cursor >= (ctx->max_entities - 1) || ctx->max_entities == 0) {
            ctx->entity.cursor = 0;
        }

        for (; ctx->entity.cursor < ctx->max_entities; ++ctx->entity.cursor) {
            librg_entity_t *entity = &ctx->entity.list[ctx->entity.cursor]; librg_assert(entity);

            if (entity->flags & LIBRG_ENTITY_ALIVE) continue;

            entity->type            = type;
            entity->flags           = LIBRG_ENTITY_ALIVE;
            entity->position        = zplm_vec3f_zero();
            entity->stream_range    = librg_option_get(LIBRG_DEFAULT_STREAM_RANGE) * 1.0f;
            entity->stream_branch   = NULL;

            return entity;
        }

        librg_assert_msg(false, "no entities to spawn");
        return NULL;
    }

    librg_entity_t *librg_entity_fetch(librg_ctx_t *ctx, librg_entity_id id) {
        if (librg_entity_valid(ctx, id))
            return &ctx->entity.list[id];

        return NULL;
    }

    void librg_entity_destroy(librg_ctx_t *ctx, librg_entity_id id) {
        librg_assert(librg_is_server(ctx));
        zpl_array_append(ctx->entity.remove_queue, id);
    }

    usize librg_entity_query(librg_ctx_t *ctx, librg_entity_id entity, librg_entity_id **out_entities) {
        librg_assert(ctx && out_entities);
        librg_assert(librg_is_server(ctx));
        librg_entity_t *blob = librg_entity_fetch(ctx, entity);

        if (!(blob->flags & LIBRG_ENTITY_QUERIED)) {
            blob->flags |= LIBRG_ENTITY_QUERIED;
            zpl_array_init(blob->last_query, ctx->allocator);
        }

        // reset array to 0
        zpl_array_count(blob->last_query) = 0;

        zplm_aabb3 search_bounds;
        search_bounds.centre    = blob->position;
        search_bounds.half_size = zplm_vec3f(blob->stream_range, blob->stream_range, blob->stream_range);
        librg__world_entity_query(ctx, entity, &ctx->world, search_bounds, &blob->last_query);
        *out_entities = blob->last_query;

        return zpl_array_count(blob->last_query);
    }

    b32 librg_entity_valid(librg_ctx_t *ctx, librg_entity_id id) {
        librg_assert(ctx);
        return (id < ctx->max_entities && (ctx->entity.list[id].flags & LIBRG_ENTITY_ALIVE));
    }

    librg_entity_t *librg_entity_find(librg_ctx_t *ctx, librg_peer_t *peer) {
        librg_assert(ctx && librg_is_server(ctx) && peer);
        librg_entity_id *id = librg_table_get(&ctx->network.connected_peers, (u64)peer);

        if (id) {
            return librg_entity_fetch(ctx, *id);
        }

        return NULL;
    }

    void librg_entity_visibility_set(librg_ctx_t *ctx, librg_entity_id entity, b32 state) {
        librg_assert(librg_is_server(ctx) && librg_entity_valid(ctx, entity));
        librg_table_set(&ctx->entity.ignored, entity, (u32)!state);
    }

    void librg_entity_visibility_set_for(librg_ctx_t *ctx, librg_entity_id entity, librg_entity_id target, b32 state) {
        librg_assert(librg_is_server(ctx) && librg_entity_valid(ctx, entity));
        librg_entity_t *blob = librg_entity_fetch(ctx, entity);

        if (!(blob->flags & LIBRG_ENTITY_IGNORING)) {
            blob->flags |= LIBRG_ENTITY_IGNORING;
            librg_table_init(&blob->ignored, ctx->allocator);
        }

        librg_table_set(&blob->ignored, target, (u32)!state);
    }

    b32 librg_entity_visibility_get(librg_ctx_t *ctx, librg_entity_id entity) {
        librg_assert(librg_is_server(ctx) && librg_entity_valid(ctx, entity));
        u32 *ignored = librg_table_get(&ctx->entity.ignored, entity);
        return !(ignored && *ignored);
    }

    b32 librg_entity_visibility_get_for(librg_ctx_t *ctx, librg_entity_id entity, librg_entity_id target) {
        librg_assert(librg_is_server(ctx));
        librg_entity_t *blob = librg_entity_fetch(ctx, entity);

        if (!(blob->flags & LIBRG_ENTITY_IGNORING)) {
            return true;
        }

        u32 *ignored = librg_table_get(&blob->ignored, target);
        return !(ignored && *ignored);
    }

    void librg_entity_control_set(librg_ctx_t *ctx, librg_entity_id entity, librg_peer_t *peer) {
        librg_assert(ctx && peer && librg_entity_valid(ctx, entity));
        librg_assert(librg_is_server(ctx));
        librg_entity_t *blob = librg_entity_fetch(ctx, entity);

        // replace current entity owner
        if (blob->flags & LIBRG_ENTITY_CONTROLLED) {
            if (blob->control_peer == peer) {
                return;
            }

            librg_send_to(ctx, LIBRG_CLIENT_STREAMER_REMOVE, blob->control_peer, librg_lambda(data), {
                librg_data_went(&data, entity);
            });

            blob->control_peer = peer;
        }
        // attach new entity owner
        else {
            blob->flags |= LIBRG_ENTITY_CONTROLLED;
            blob->control_peer = peer;
        }

        librg_message_t *msg = (librg_message_t *)zpl_alloc(ctx->allocator, sizeof(librg_message_t)); {
            librg_message_id id = LIBRG_CLIENT_STREAMER_ADD;

            msg->peer   = peer;
            msg->packet = enet_packet_create_offset(
                &entity, sizeof(librg_entity_id), sizeof(librg_message_id), ENET_PACKET_FLAG_RELIABLE
            );

            zpl_memcopy(msg->packet->data, &id, sizeof(librg_message_id));
        }

        zpl_array_append(ctx->entity.add_control_queue, msg);
    }

    librg_peer_t *librg_entity_control_get(librg_ctx_t *ctx, librg_entity_id entity) {
        librg_assert(ctx);
        librg_assert(librg_is_server(ctx));
        librg_entity_t *blob = librg_entity_fetch(ctx, entity);
        return (blob && blob->flags & LIBRG_ENTITY_CONTROLLED) ? blob->control_peer : NULL;
    }

    void librg_entity_control_remove(librg_ctx_t *ctx, librg_entity_id entity) {
        librg_assert(ctx && librg_entity_valid(ctx, entity));
        librg_assert(librg_is_server(ctx));
        librg_entity_t *blob = librg_entity_fetch(ctx, entity);

        if (!(blob->flags & LIBRG_ENTITY_CONTROLLED)) {
            return;
        }

        librg_send_to(ctx, LIBRG_CLIENT_STREAMER_REMOVE, blob->control_peer, librg_lambda(data), {
            librg_data_went(&data, entity);
        });

        blob->flags &= ~LIBRG_ENTITY_CONTROLLED;
        blob->control_peer = NULL;
    }

    void librg_entity_iterate(librg_ctx_t *ctx, u64 flags, librg_entity_cb callback) {
        librg_entity_iteratex(ctx, flags, librg_lambda(entity), { callback(ctx, librg_entity_fetch(ctx, entity)); });
    }

    // @deprected
    u32 librg_entity_type(librg_ctx_t *ctx, librg_entity_id id) {
        librg_assert(librg_entity_valid(ctx, id));
        return ctx->entity.list[id].type;
    }
#endif

// =======================================================================//
// !
// ! Events
// !
// =======================================================================//

#if 1
    u64 librg_event_add(librg_ctx_t *ctx, u64 id, librg_event_cb callback) {
        librg_assert(ctx);
        librg_event_block *block = librg_event_pool_get(&ctx->events, id);

        if (!block) {
            librg_event_block arr;
            zpl_array_init(arr, ctx->allocator);
            librg_event_pool_set(&ctx->events, id, arr);
            block = librg_event_pool_get(&ctx->events, id);
        }

        u64 offset = zpl_array_count(block);
        zpl_array_append(*block, callback);
        return offset;
    }

    void librg_event_trigger(librg_ctx_t *ctx, u64 id, librg_event_t *event) {
        librg_assert(event); event->ctx = ctx;
        librg_event_block *block = librg_event_pool_get(&ctx->events, id);
        if (!block) return;

        for (isize i = 0; i < zpl_array_count(*block) && !(event->flags & LIBRG_EVENT_REJECTED); ++i) {
            (*block)[i](event);
        }
    }

    void librg_event_remove(librg_ctx_t *ctx, u64 id, u64 index) {
        librg_assert(ctx);
        librg_event_block *block = librg_event_pool_get(&ctx->events, id);

        if (block) {
            zpl_array_remove_at(*block, (isize)index);
        }
    }

    void librg_event_reject(librg_event_t *event) {
        librg_assert(event);
        event->flags = (event->flags | LIBRG_EVENT_REJECTED);
    }

    b32 librg_event_rejectable(librg_event_t *event) {
        librg_assert(event);
        return (event->flags & LIBRG_EVENT_REJECTABLE);
    }

    b32 librg_event_succeeded(librg_event_t *event) {
        librg_assert(event);
        return !(event->flags & LIBRG_EVENT_REJECTED);
    }
#endif

// =======================================================================//
// !
// ! Binary data (bitstream)
// !
// =======================================================================//

#if 1
    void librg_data_init_size(librg_data_t *data, usize size) {
        librg_assert(data);

        data->capacity  = size;
        data->read_pos  = 0;
        data->write_pos = 0;
        data->allocator = zpl_heap_allocator();
        data->rawptr    = zpl_alloc(data->allocator, size);
    }

    void librg_data_init(librg_data_t *data) {
        librg_assert(data);
        librg_data_init_size(data, librg_option_get(LIBRG_DEFAULT_DATA_SIZE));
    }

    librg_data_t *librg_data_init_new() {
        librg_data_t *data = (librg_data_t *)zpl_malloc(sizeof(librg_data_t));
        librg_data_init(data);
        return data;
    }

    void librg_data_free(librg_data_t *data) {
        librg_assert(data && data->rawptr);
        zpl_free(data->allocator, data->rawptr);
    }

    void librg_data_reset(librg_data_t *data) {
        librg_data_set_wpos(data, 0);
        librg_data_set_rpos(data, 0);
    }

    void librg_data_grow(librg_data_t *data, usize min_size) {
        librg_assert(data && data->rawptr);

        usize new_capacity = LIBRG_DATA_GROW_FORMULA(data->capacity);
        if (new_capacity < (min_size))
            new_capacity = (min_size);

        void *newptr = zpl_alloc(data->allocator, new_capacity);

        zpl_memcopy(newptr, data->rawptr, data->write_pos);
        zpl_free(data->allocator, data->rawptr);

        data->capacity = new_capacity;
        data->rawptr = newptr;
    }

    usize librg_data_capacity(librg_data_t *data) {
        librg_assert(data); return data->capacity;
    }

    usize librg_data_get_rpos(librg_data_t *data) {
        librg_assert(data); return data->read_pos;
    }

    usize librg_data_get_wpos(librg_data_t *data) {
        librg_assert(data); return data->write_pos;
    }

    void librg_data_set_rpos(librg_data_t *data, usize position) {
        librg_assert(data); data->read_pos = position;
    }

    void librg_data_set_wpos(librg_data_t *data, usize position) {
        librg_assert(data); data->write_pos = position;
    }

    void librg_data_rptr_at(librg_data_t *data, void *ptr, usize size, isize position) {
        librg_assert(data && data->rawptr && ptr);

        librg_assert_msg(position + size <= librg_data_capacity(data),
            "librg_data: trying to read from outside of the bounds");

        zpl_memcopy(ptr, (char *)data->rawptr + position, size);
    }

    void librg_data_wptr_at(librg_data_t *data, void *ptr, usize size, isize position) {
        librg_assert(data && data->rawptr && ptr);

        if (position + size > librg_data_capacity(data)) {
            librg_data_grow(data, librg_data_capacity(data) + size + position);
        }

        zpl_memcopy((char *)data->rawptr + position, ptr, size);
    }

    void librg_data_rptr(librg_data_t *data, void *ptr, usize size) {
        librg_assert(data && data->rawptr && ptr);
        librg_data_rptr_at(data, ptr, size, librg_data_get_rpos(data));
        data->read_pos += size;
    }

    void librg_data_wptr(librg_data_t *data, void *ptr, usize size) {
        librg_assert(data && data->rawptr && ptr);
        librg_data_wptr_at(data, ptr, size, librg_data_get_wpos(data));
        data->write_pos += size;
    }

    /**
     * Value writers and readers
     */

    #define LIBRG_GEN_DATA_READWRITE(TYPE) \
        TYPE ZPL_JOIN2(librg_data_r,TYPE)(librg_data_t *data) { \
            TYPE value; librg_data_rptr(data, &value, sizeof(value)); return value; \
        } \
        void ZPL_JOIN2(librg_data_w,TYPE)(librg_data_t *data, TYPE value) { \
            librg_data_wptr(data, &value, sizeof(value)); \
        } \
        TYPE ZPL_JOIN3(librg_data_r,TYPE,_at)(librg_data_t *data, isize position) { \
            TYPE value; librg_data_rptr_at(data, &value, sizeof(value), position); return value; \
        } \
        void ZPL_JOIN3(librg_data_w,TYPE,_at)(librg_data_t *data, TYPE value, isize position) { \
            librg_data_wptr_at(data, &value, sizeof(value), position); \
        }
    /**
     * General one-line methods for reading/writing different types
     */
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
#endif

// =======================================================================//
// !
// ! Network
// !
// =======================================================================//

#if 1
    b32 librg_is_connected(librg_ctx_t *ctx) {
        return ctx->network.peer && ctx->network.peer->state == ENET_PEER_STATE_CONNECTED;
    }

    b32 librg_is_server(librg_ctx_t *ctx) {
        return ctx->mode == LIBRG_MODE_SERVER;
    }

    b32 librg_is_client(librg_ctx_t *ctx) {
        return ctx->mode == LIBRG_MODE_CLIENT;
    }

    void librg_network_start(librg_ctx_t *ctx, librg_address_t addr) {
        librg_dbg("librg_network_start\n");

        if (librg_is_server(ctx)) {
            librg_table_init(&ctx->network.connected_peers, ctx->allocator);
            ENetAddress address;

            if (addr.host && zpl_strcmp(addr.host, "localhost") == 0) {
                enet_address_set_host(&address, addr.host);
            } else {
                address.host = ENET_HOST_ANY;
            }

            address.port = addr.port;

            // setup server host
            ctx->network.host = enet_host_create(&address, ctx->max_connections, librg_option_get(LIBRG_NETWORK_CHANNELS), 0, 0);
            librg_assert_msg(ctx->network.host, "could not start server at provided port");
        }
        else {
            ENetAddress address;

            address.port = addr.port;
            enet_address_set_host(&address, addr.host);

            // setup client host
            // TODO: add override for bandwidth
            ctx->network.host = enet_host_create(NULL, 1, librg_option_get(LIBRG_NETWORK_CHANNELS), 0, 0);
            librg_assert_msg(ctx->network.host, "could not start client");

            // create peer connecting to server
            librg_dbg("connecting to server %s:%u\n", addr.host, addr.port);
            ctx->network.peer = enet_host_connect(ctx->network.host, &address, librg_option_get(LIBRG_NETWORK_CHANNELS), 0);
            librg_assert_msg(ctx->network.peer, "could not setup peer for provided address");
        }

        ctx->network.last_address = addr;
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

        // destroy all the entities that are currently created
        for (usize i = 0; i < ctx->max_entities; ++i) {
            if (librg_entity_valid(ctx, i)) {
                librg_event_t event = {0}; {
                    event.entity = librg_entity_fetch(ctx, i);
                    event.flags  = LIBRG_EVENT_LOCAL;
                }

                // skip local client entity
                if (event.entity->flags & LIBRG_ENTITY_CLIENT) continue;

                librg_event_trigger(ctx, LIBRG_ENTITY_REMOVE, &event);
                librg__world_entity_destroy(ctx, i);
            }
        }

        if (librg_is_server(ctx)) {
            librg_table_destroy(&ctx->network.connected_peers);
        }
    }

    void librg_network_kick(librg_ctx_t *ctx, librg_peer_t *peer) {
        librg_assert(ctx && peer);
        enet_peer_disconnect_later(peer, 0);
    }

    void librg_network_add(librg_ctx_t *ctx, librg_message_id id, librg_message_cb callback) {
        ctx->messages[id] = callback;
    }

    void librg_network_remove(librg_ctx_t *ctx, librg_message_id id) {
        ctx->messages[id] = NULL;
    }

    void librg_message_send_all(librg_ctx_t *ctx, librg_message_id id, void *data, usize size) {
        if (librg_is_client(ctx)) {
            librg_message_send_to(ctx, id, ctx->network.peer, data, size);
            return;
        }

        librg_message_send_except(ctx, id, NULL, data, size);
    }

    void librg_message_send_to(librg_ctx_t *ctx, librg_message_id id, librg_peer_t *peer, void *data, usize size) {
        zpl_unused(ctx);

        librg_packet_t *packet = enet_packet_create_offset(
            data, size, sizeof(librg_message_id), ENET_PACKET_FLAG_RELIABLE
        );

        // write id at the beginning
        zpl_memcopy(packet->data, &id, sizeof(librg_message_id));
        enet_peer_send(peer, librg_option_get(LIBRG_NETWORK_MESSAGE_CHANNEL), packet);
    }

    void librg_message_send_except(librg_ctx_t *ctx, librg_message_id id, librg_peer_t *peer, void *data, usize size) {
        librg_entity_iteratex(ctx, LIBRG_ENTITY_CLIENT, librg_lambda(entity), {
            librg_entity_t *blob = librg_entity_fetch(ctx, entity);

            if (blob->client_peer != peer) {
                librg_packet_t *packet = enet_packet_create_offset(
                    data, size, sizeof(librg_message_id), ENET_PACKET_FLAG_RELIABLE
                );

                // write id at the beginning
                zpl_memcopy(packet->data, &id, sizeof(librg_message_id));
                enet_peer_send(blob->client_peer, librg_option_get(LIBRG_NETWORK_MESSAGE_CHANNEL), packet);
            }
        });
    }

    void librg_message_send_instream(librg_ctx_t *ctx, librg_message_id id, librg_entity_id entity, void *data, usize size) {
        librg_message_send_instream_except(ctx, id, entity, NULL, data, size);
    }

    void librg_message_send_instream_except(librg_ctx_t *ctx, librg_message_id id, librg_entity_id entity, librg_peer_t * ignored, void *data, usize size) {
        zpl_array(librg_entity_id) queue;
        usize count = librg_entity_query(ctx, entity, &queue);

        for (isize i = 0; i < count; i++) {
            librg_entity_id target = queue[i];

            librg_entity_t *blob = librg_entity_fetch(ctx, target);
            if (!(blob->flags & LIBRG_ENTITY_CLIENT)) continue;

            librg_peer_t *peer = blob->client_peer;
            librg_assert(peer);

            if (peer == ignored) {
                continue;
            }

            librg_packet_t *packet = enet_packet_create_offset(
                data, size, sizeof(librg_message_id), ENET_PACKET_FLAG_RELIABLE
            );

            // write id at the beginning
            zpl_memcopy(packet->data, &id, sizeof(librg_message_id));
            enet_peer_send(peer, librg_option_get(LIBRG_NETWORK_MESSAGE_CHANNEL), packet);
        }
    }
#endif

// =======================================================================//
// !
// ! Helper methods
// !
// =======================================================================//

#if 1
    f64 librg_time_now(librg_ctx_t *ctx) {
        librg_assert(ctx);

        if (librg_is_server(ctx)) {
            return zpl_time_now();
        }

        return zpl_time_now() - ctx->timesync.start_time + ctx->timesync.offset_time;
    }

    f64 librg_standard_deviation(f64 *values, usize size) {
        f64 sum = 0.0, mean = 0.0, deviation = 0.0;
        for (isize i = 0; i < size; i++) { sum += values[i]; }

        mean = sum / (f64)size;

        for (isize i = 0; i < size; i++) {
            deviation += zplm_pow(values[i] - mean, 2);
        }

        return zplm_sqrt(deviation / (f64)size);
    }
#endif

// =======================================================================//
// !
// ! Time-syncer
// !
// =======================================================================//

#if 1
    void librg__timesync_tick(void *usrptr) {
        librg_send((librg_ctx_t *)usrptr, LIBRG_CONNECTION_TIMESYNC, data, {
            librg_data_wf64(&data, zpl_time_now());
        });
    }

    void librg__timesync_start(librg_ctx_t *ctx) {
        if (librg_is_client(ctx)) {
            zpl_zero_item(ctx->timesync.history);

            ctx->timesync.median        = 0.0;
            ctx->timesync.offset_time   = 0.0;
            ctx->timesync.start_time    = 0.0;

            zpl_timer_set(ctx->timesync.timer, 5.0, -1, librg__timesync_tick);
            zpl_timer_start(ctx->timesync.timer, 0);
        }
    }

    void librg__timesync_stop(librg_ctx_t *ctx) {
        if (librg_is_client(ctx)) {
            zpl_timer_stop(ctx->timesync.timer);
        }
    }
#endif

// =======================================================================//
// !
// ! Update buffer
// !
// =======================================================================//

#if 1
    void librg__buffer_init(librg_ctx_t *ctx, usize size) {
        if (librg_is_server(ctx)) return;

        zpl_ring_librg_snapshot_init(&ctx->buffer, ctx->allocator, size);
        ctx->buffer_size = size;

        zpl_timer_set(ctx->buffer_timer, ctx->timesync.server_delay, -1, librg__buffer_tick);
        zpl_timer_start(ctx->buffer_timer, 0);
    }

    void librg__buffer_free(librg_ctx_t *ctx) {
        if (librg_is_server(ctx)) return;
        zpl_timer_stop(ctx->buffer_timer);

        for (isize i = 0; i < ctx->buffer.capacity; ++i) {
            zpl_mfree(ctx->buffer.buf[i].data);
        }

        zpl_ring_librg_snapshot_free(&ctx->buffer);
    }

    void librg__buffer_push(librg_ctx_t *ctx, f64 time, librg_peer_t *peer, void *data, usize size) {
        librg_snapshot snap = { 0 };

        snap.time = time;
        snap.peer = peer;
        snap.size = size;

        snap.data = zpl_alloc_copy(zpl_heap(), data, size);
        librg_assert(data && snap.data);
        zpl_ring_librg_snapshot_append(&ctx->buffer, snap);
    }

    void librg__buffer_tick(void *usrptr) {
        librg_ctx_t *ctx = (librg_ctx_t *)usrptr;
        if (!zpl_ring_librg_snapshot_full(&ctx->buffer)) {
            return;
        }

        librg_snapshot *snap = zpl_ring_librg_snapshot_get(&ctx->buffer);
        f64 time_diff = ((ctx->buffer_size + 1) * ctx->timesync.server_delay);

        // if current update if too old, just skip it, and call next one
        if (snap->time < (librg_time_now(ctx) - time_diff)) {
            librg_dbg("librg__buffer_tick: dropping old update packet\n");
            zpl_mfree(snap->data);
            librg__buffer_tick((void *)ctx);
            return;
        }

        librg_data_t data = {0}; {
            data.rawptr   = snap->data;
            data.capacity = snap->size;
        }

        // skip our message id length and timestamp lenth
        // TODO: copy not all the stuff but only needed (skiping those two)
        librg_data_set_rpos(&data, sizeof(librg_message_id) + sizeof(f64));

        librg_message_t msg = {0}; {
            msg.ctx     = ctx;
            msg.data    = &data;
            msg.peer    = snap->peer;
            msg.packet  = NULL;
        }

        ctx->messages[LIBRG_ENTITY_UPDATE](&msg);
        zpl_mfree(snap->data);
    }
#endif

// =======================================================================//
// !
// ! Default network callbacks
// !
// =======================================================================//

#if 1
    /* Execution side: SHARED */
    LIBRG_INTERNAL void librg__callback_connection_init(librg_message_t *msg) {
        librg_dbg("librg__connection_init\n");

        #if defined(LIBRG_DEBUG)
        char my_host[16];

        enet_address_get_host_ip(&msg->peer->address, my_host, 16);
        librg_dbg("librg__connection_init: a new connection attempt at %s:%u.\n", my_host, msg->peer->address.port);
        #endif

        if (librg_is_client(msg->ctx)) {
            librg_data_t data;
            librg_data_init_size(&data, sizeof(u16) * 3);

            librg_data_wu32(&data, librg_option_get(LIBRG_PLATFORM_ID));
            librg_data_wu32(&data, librg_option_get(LIBRG_PLATFORM_BUILD));
            librg_data_wu32(&data, librg_option_get(LIBRG_PLATFORM_PROTOCOL));
            librg_data_wf64(&data, zpl_time_now());

            librg_event_t event = {0}; {
                event.peer  = msg->peer;
                event.data  = &data;
                event.flags = (LIBRG_EVENT_REJECTABLE | LIBRG_EVENT_REMOTE);
            }

            librg_event_trigger(msg->ctx, LIBRG_CONNECTION_REQUEST, &event);

            if (librg_event_succeeded(&event)) {
                librg_message_send_to(msg->ctx, LIBRG_CONNECTION_REQUEST,
                    msg->peer, data.rawptr, librg_data_get_wpos(&data)
                );
            }

            librg_data_free(&data);
        }
    }

    /* Execution side: SERVER */
    LIBRG_INTERNAL void librg__callback_connection_request(librg_message_t *msg) {
        librg_dbg("librg__connection_request\n");

        u32 platform_id       = librg_data_ru32(msg->data);
        u32 platform_build    = librg_data_ru32(msg->data);
        u32 platform_protocol = librg_data_ru32(msg->data);
        f64 client_time       = librg_data_rf64(msg->data);

        b32 blocked = (platform_id != librg_option_get(LIBRG_PLATFORM_ID) || platform_protocol != librg_option_get(LIBRG_PLATFORM_PROTOCOL));

        if (platform_build != librg_option_get(LIBRG_PLATFORM_BUILD)) {
            librg_dbg("NOTICE: librg platform build mismatch client %u, server: %u\n", platform_build, librg_option_get(LIBRG_PLATFORM_BUILD));
        }

        if (blocked) {
            librg_dbg("our platform: %d %d, their platform: %d %d\n",
                librg_option_get(LIBRG_PLATFORM_ID),
                librg_option_get(LIBRG_PLATFORM_PROTOCOL),
                platform_id, platform_protocol
            );
        }

        librg_event_t event = {0}; {
            event.peer  = msg->peer;
            event.data  = msg->data;
            event.flags = (LIBRG_EVENT_REJECTABLE | LIBRG_EVENT_REMOTE);
        }

        librg_event_trigger(msg->ctx, LIBRG_CONNECTION_REQUEST, &event);

        if (librg_event_succeeded(&event) && !blocked) {
            librg_entity_t *entity = librg_entity_create(msg->ctx, librg_option_get(LIBRG_DEFAULT_CLIENT_TYPE));

            // assign default
            entity->flags |= LIBRG_ENTITY_CLIENT;
            entity->client_peer = msg->peer;
            librg_table_init(&entity->last_snapshot, msg->ctx->allocator);

            // add client peer to storage
            librg_table_set(&msg->ctx->network.connected_peers, cast(u64)msg->peer, entity->id);

            // send accept
            librg_send_to(msg->ctx, LIBRG_CONNECTION_ACCEPT, msg->peer, librg_lambda(data), {
                librg_data_wf32(&data, msg->ctx->tick_delay / 1000.0f);
                librg_data_wf64(&data, client_time);
                librg_data_wf64(&data, zpl_time_now());
                librg_data_went(&data, entity->id);
            });

            event.data   = NULL;
            event.entity = entity;
            event.flags  = LIBRG_EVENT_LOCAL;

            librg_event_trigger(msg->ctx, LIBRG_CONNECTION_ACCEPT, &event);
        }
        else {
            librg_dbg("librg__connection_refuse\n");
            librg_message_send_to(msg->ctx, LIBRG_CONNECTION_REFUSE, msg->peer, NULL, 0);

            event.data   = NULL;
            event.flags  = LIBRG_EVENT_LOCAL;

            librg_event_trigger(msg->ctx, LIBRG_CONNECTION_REFUSE, &event);
        }
    }

    /* Execution side: CLIENT */
    LIBRG_INTERNAL void librg__callback_connection_refuse(librg_message_t *msg) {
        librg_dbg("librg__connection_refuse\n");
        LIBRG_MESSAGE_TO_EVENT(event, msg);
        librg_event_trigger(msg->ctx, LIBRG_CONNECTION_REFUSE, &event);
    }

    /* Execution side: CLIENT */
    LIBRG_INTERNAL void librg__callback_connection_accept(librg_message_t *msg) {
        librg_dbg("librg__connection_accept\n");
        librg_table_init(&msg->ctx->network.connected_peers, msg->ctx->allocator);

        f32 server_delay = librg_data_rf32(msg->data);
        f64 client_diff  = (zpl_time_now() - librg_data_rf64(msg->data)) / 2.0;
        f64 server_time  = librg_data_rf64(msg->data);

        librg_entity_id entity = librg_data_rent(msg->data);
        librg_entity_t *blob = &msg->ctx->entity.list[entity];

        msg->ctx->entity.count++;

        blob->type      = librg_option_get(LIBRG_DEFAULT_CLIENT_TYPE);
        blob->flags     = (LIBRG_ENTITY_ALIVE | LIBRG_ENTITY_CLIENT);
        blob->position  = zplm_vec3f_zero();

        // add server peer to storage
        librg_table_set(&msg->ctx->network.connected_peers, cast(u64)msg->peer, entity);

        // trigger damn events!
        LIBRG_MESSAGE_TO_EVENT(event, msg); event.entity = blob;
        librg_event_trigger(msg->ctx, LIBRG_CONNECTION_ACCEPT, &event);

        librg__timesync_start(msg->ctx);
        msg->ctx->timesync.start_time   = zpl_time_now();
        msg->ctx->timesync.offset_time  = server_time + client_diff;
        msg->ctx->timesync.server_delay = server_delay;

        if (librg_option_get(LIBRG_NETWORK_BUFFER_SIZE) > 1) {
            librg__buffer_init(msg->ctx, librg_option_get(LIBRG_NETWORK_BUFFER_SIZE));
        }
    }

    /* Execution side: SHARED */
    LIBRG_INTERNAL void librg__callback_connection_disconnect(librg_message_t *msg) {
        librg_dbg("librg__connection_disconnect\n");
        if (!msg->ctx->network.connected_peers.hashes) {
            librg_event_t event = {0}; {
                event.peer      = msg->peer;
                event.data      = NULL;
                event.entity    = NULL;
                event.flags     = (LIBRG_EVENT_REJECTABLE);
            }

            librg_event_trigger(msg->ctx, LIBRG_CONNECTION_DISCONNECT, &event);
            return;
        }

        librg_entity_id *entity = librg_table_get(&msg->ctx->network.connected_peers, cast(u64)msg->peer);
        if (entity && librg_entity_valid(msg->ctx, *entity)) {
            librg_entity_t *blob = librg_entity_fetch(msg->ctx, *entity);
            librg_event_t event = {0}; {
                event.peer      = msg->peer;
                event.data      = msg->data;
                event.entity    = blob;
                event.flags     = (LIBRG_EVENT_REJECTABLE | LIBRG_EVENT_REMOTE);
            }

            librg_event_trigger(msg->ctx, LIBRG_CONNECTION_DISCONNECT, &event);

            if (librg_is_server(msg->ctx)) {
                librg_entity_destroy(msg->ctx, *entity);
            } else {
                librg__world_entity_destroy(msg->ctx, *entity);
            }
        }

        if (librg_is_client(msg->ctx)) {
            librg_table_destroy(&msg->ctx->network.connected_peers);
            librg__timesync_stop(msg->ctx);

            if (librg_option_get(LIBRG_NETWORK_BUFFER_SIZE) > 1) {
                librg__buffer_free(msg->ctx);
            }
        }
    }

    /* Execution side: SHARED */
    /* Based on: http://www.mine-control.com/zack/timesync/timesync.html */
    void librg__callback_connection_timesync(librg_message_t *msg) {
        if (librg_is_server(msg->ctx)) {
            f64 client_time = librg_data_rf64(msg->data);
            f64 server_time = librg_time_now(msg->ctx);

            librg_send_to(msg->ctx, LIBRG_CONNECTION_TIMESYNC, msg->peer, librg_lambda(data), {
                librg_data_wf64(&data, client_time);
                librg_data_wf64(&data, server_time);
            });
        } else {
            librg_ctx_t *ctx = msg->ctx;

            f64 client_diff = (zpl_time_now() - librg_data_rf64(msg->data)) / 2.0;
            f64 server_time = librg_data_rf64(msg->data);

            // our first time sync packet, apply the value directly
            if (ctx->timesync.history[LIBRG_TIMESYNC_SIZE - 1] == 0.0) {
                ctx->timesync.start_time  = zpl_time_now();
                ctx->timesync.offset_time = server_time + client_diff;
            }

            // we are still collecting delta data
            if (ctx->timesync.history[0] == 0.0) {
                ctx->timesync.history[0] = client_diff;
                zpl_sort(&ctx->timesync.history, LIBRG_TIMESYNC_SIZE, sizeof(f64), zpl_f64_cmp(0));
            } else {
                // we just finished collecting, this is our median
                if (ctx->timesync.median == 0.0) {
                    ctx->timesync.median = ctx->timesync.history[LIBRG_TIMESYNC_SIZE / 2];
                    ctx->timesync.timer->duration = 60.0; /* slower our time update rate to 1 per minute */
                }

                // samples above approximately 1 standard-deviation from the median are discarded
                f64 sample[2] = { ctx->timesync.median, client_diff };
                if (librg_standard_deviation(sample, 2) > 1.0) {
                    return;
                }

                ctx->timesync.start_time  = zpl_time_now();
                ctx->timesync.offset_time = server_time + ctx->timesync.median;
            }
        }
    }

    /* Execution side: CLIENT */
    LIBRG_INTERNAL void librg__callback_entity_create(librg_message_t *msg) {
        u32 query_size = librg_data_ru32(msg->data);

        for (usize i = 0; i < query_size; ++i) {
            librg_entity_id entity = librg_data_rent(msg->data);
            u32 type               = librg_data_ru32(msg->data);

            zplm_vec3 position;
            librg_data_rptr(msg->data, &position, sizeof(zplm_vec3));

            // Create new entity on client side
            librg_entity_t *blob = &msg->ctx->entity.list[entity];
            librg_assert(blob);

            blob->type     = type;
            blob->flags    = LIBRG_ENTITY_ALIVE;
            blob->position = position;

            msg->ctx->entity.count++;

            LIBRG_MESSAGE_TO_EVENT(event, msg); event.entity = blob;
            librg_event_trigger(msg->ctx, LIBRG_ENTITY_CREATE, &event);
        }

        u32 remove_size = librg_data_ru32(msg->data);

        for (usize i = 0; i < remove_size; ++i) {
            librg_entity_id entity = librg_data_rent(msg->data);

            if (librg_entity_valid(msg->ctx, entity)) {
                LIBRG_MESSAGE_TO_EVENT(event, msg);
                event.entity = librg_entity_fetch(msg->ctx, entity);
                librg_event_trigger(msg->ctx, LIBRG_ENTITY_REMOVE, &event);
                librg__world_entity_destroy(msg->ctx, entity);
            }
            else {
                librg_dbg("unexpected entity %u on remove\n", entity);
            }
        }
    }

    /* Execution side: CLIENT */
    LIBRG_INTERNAL void librg__callback_entity_update(librg_message_t *msg) {
        u32 query_size = librg_data_ru32(msg->data);

        for (usize i = 0; i < query_size; ++i) {
            librg_entity_id entity = librg_data_rent(msg->data);

            zplm_vec3 position;
            librg_data_rptr(msg->data, &position, sizeof(position));

            if (!librg_entity_valid(msg->ctx, entity)) {
                continue;
            }

            librg_entity_t *blob = librg_entity_fetch(msg->ctx, entity);
            blob->position = position;

            LIBRG_MESSAGE_TO_EVENT(event, msg); event.entity = blob;
            librg_event_trigger(msg->ctx, LIBRG_ENTITY_UPDATE, &event);
        }
    }

    /* Execution side: CLIENT */
    LIBRG_INTERNAL void librg__callback_entity_client_streamer_add(librg_message_t *msg) {
        librg_entity_id entity = librg_data_rent(msg->data);

        if (!librg_entity_valid(msg->ctx, entity)) {
            librg_dbg("trying to add unknown entity to clientstream!\n");
            return;
        }

        librg_entity_t *blob = librg_entity_fetch(msg->ctx, entity);

        if (!(blob->flags & LIBRG_ENTITY_CONTROLLED)) {
            blob->flags |= LIBRG_ENTITY_CONTROLLED;

            librg_entity_t *blob = librg_entity_fetch(msg->ctx, entity);

            LIBRG_MESSAGE_TO_EVENT(event, msg); event.entity = blob;
            librg_event_trigger(msg->ctx, LIBRG_CLIENT_STREAMER_ADD, &event);
        }
    }

    /* Execution side: SERVER */
    LIBRG_INTERNAL void librg__callback_entity_client_streamer_update(librg_message_t *msg) {
        u32 amount = librg_data_ru32(msg->data);

        for (usize i = 0; i < amount; i++) {
            librg_entity_id entity = librg_data_rent(msg->data);
            u32 size = librg_data_ru32(msg->data);

            if (!librg_entity_valid(msg->ctx, entity)) {
                librg_dbg("invalid entity on client streamer update\n");
                librg_data_set_rpos(msg->data, librg_data_get_rpos(msg->data) + size);
                continue;
            }

            librg_entity_t *blob = librg_entity_fetch(msg->ctx, entity);

            if (!(blob->flags & LIBRG_ENTITY_CONTROLLED) || blob->control_peer != msg->peer) {
                librg_dbg("no component, or peer is different\n");
                librg_data_set_rpos(msg->data, librg_data_get_rpos(msg->data) + size);
                continue;
            }

            LIBRG_MESSAGE_TO_EVENT(event, msg); event.entity = blob;
            librg_event_trigger(msg->ctx, LIBRG_CLIENT_STREAMER_UPDATE, &event);
            librg_data_rptr(msg->data, &blob->position, sizeof(blob->position));
        }
    }

    /* Execution side: CLIENT */
    LIBRG_INTERNAL void librg__callback_entity_client_streamer_remove(librg_message_t *msg) {
        librg_entity_id entity = librg_data_rent(msg->data);

        if (!librg_entity_valid(msg->ctx, entity)) {
            librg_dbg("trying to remove unknown entity from clientstream!\n");
            return;
        }

        librg_entity_t *blob = librg_entity_fetch(msg->ctx, entity);

        if (blob->flags & LIBRG_ENTITY_CONTROLLED) {
            blob->flags &= ~LIBRG_ENTITY_CONTROLLED;

            LIBRG_MESSAGE_TO_EVENT(event, msg); event.entity = blob;
            librg_event_trigger(msg->ctx, LIBRG_CLIENT_STREAMER_REMOVE, &event);
        }
    }
#endif

// =======================================================================//
// !
// ! World methods (TODO)
// !
// =======================================================================//

#if 1
    /**
     * CLIENT-SIDE
     *
     * Responsive for updating the client side streamer
     */
    librg_internal void librg__execute_client_update(librg_ctx_t *ctx) {
        u32 amount = 0;
        librg_data_t data;
        librg_data_init(&data);

        librg_data_wmid(&data, LIBRG_CLIENT_STREAMER_UPDATE);
        librg_data_wu32(&data, 0); // amount of entities to be sent (updates)

        librg_entity_iteratex(ctx, LIBRG_ENTITY_CONTROLLED, librg_lambda(entity), {
            librg_entity_t *blob = librg_entity_fetch(ctx, entity);

            librg_data_t subdata;
            librg_data_init(&subdata);

            librg_event_t event = {0}; {
                event.entity = blob;
                event.data  = &subdata;
                event.flags = (LIBRG_EVENT_REJECTABLE | LIBRG_EVENT_LOCAL);
            }

            librg_event_trigger(ctx, LIBRG_CLIENT_STREAMER_UPDATE, &event);

            // check if user rejected the event
            if (!(event.flags & LIBRG_EVENT_REJECTED)) {
                librg_data_wptr(&subdata, &blob->position, sizeof(zplm_vec3));
                librg_data_went(&data, entity);
                librg_data_wu32(&data, librg_data_get_wpos(&subdata));

                // write sub-bitstream to main bitstream
                librg_data_wptr(&data, subdata.rawptr, librg_data_get_wpos(&subdata));
                librg_data_free(&subdata);

                amount++;
            }
        });

        if (amount < 1) {
            librg_data_free(&data);
            return;
        }

        // write amount after packet id
        librg_data_wu32_at(&data, amount, sizeof(librg_message_id));

        enet_peer_send(ctx->network.peer, librg_option_get(LIBRG_NETWORK_SECONDARY_CHANNEL),
            enet_packet_create(data.rawptr, librg_data_get_wpos(&data), 0)
        );

        librg_data_free(&data);
    }

    /**
     * SERVER-SIDE
     *
     * Responsive for updating the server-side streamer
     */
    void librg__execute_server_entity_update_proc(librg_ctx_t *ctx, librg_data_t *reliable, librg_data_t *unreliable, usize offset, usize count) {
        for (isize j = offset; j < offset+count; j++) {
            librg_entity_t *blob = &ctx->entity.list[j];

            if (!(blob->flags & LIBRG_ENTITY_CLIENT)) continue;

            // assume that entity is valid, having the client
            librg_entity_id player = j;

            // get old, and preapre new snapshot handlers
            librg_table_t *last_snapshot = &blob->last_snapshot;
            librg_table_t next_snapshot = { 0 };
            librg_table_init(&next_snapshot, ctx->allocator);

            // fetch entities in the steram zone
            zpl_array(librg_entity_id) queue;
            usize queue_count = librg_entity_query(ctx, player, &queue);

            u32 created_entities = 0;
            u32 updated_entities = cast(u32)queue_count;
            u32 removed_entities = 0;

            // write packet headers
            librg_data_wmid(reliable, LIBRG_ENTITY_CREATE);
            librg_data_wu32(reliable, created_entities);

            librg_data_wmid(unreliable, LIBRG_ENTITY_UPDATE);
            librg_data_wf64(unreliable, zpl_time_now());
            librg_data_wu32(unreliable, updated_entities);

            // add entity creates and updates
            for (isize i = 0; i < queue_count; ++i) {

                librg_entity_id entity = cast(librg_entity_id)queue[i];

                // fetch value of entity in the last snapshot
                u32 *existed_in_last = librg_table_get(last_snapshot, entity);

                librg_entity_t *eblob = librg_entity_fetch(ctx, entity);

                // write create
                if (!existed_in_last) {
                    updated_entities--;

                    // skip entity create if this is player's entity
                    if (entity == player) continue;

                    // increase write amount for create counter
                    created_entities++;

                    // save write size before writing stuff
                    // (in case we will need reject the event)
                    u32 curr_wsize = librg_data_get_wpos(reliable);

                    // write all basic data
                    librg_data_went(reliable, entity);
                    librg_data_wu32(reliable, eblob->type);
                    librg_data_wptr(reliable, &eblob->position, sizeof(eblob->position));

                    // request custom data from user
                    librg_event_t event = {0}; {
                        event.peer   = blob->client_peer;
                        event.data   = reliable;
                        event.entity = eblob;
                        event.flags  = (LIBRG_EVENT_REJECTABLE | LIBRG_EVENT_LOCAL);
                    }

                    librg_event_trigger(ctx, LIBRG_ENTITY_CREATE, &event);

                    // check if event was rejected
                    if (event.flags & LIBRG_EVENT_REJECTED) {
                        created_entities--;
                        librg_data_set_wpos(reliable, curr_wsize);
                    }
                }
                else {
                    // mark entity as still alive, for the remove cycle
                    librg_table_set(last_snapshot, entity, 0);

                    // if this entity is client streamable and this client is owner
                    if ((eblob->flags & LIBRG_ENTITY_CONTROLLED) && eblob->control_peer == blob->client_peer) {
                        updated_entities--;
                    }
                    // write update
                    else {

                        // save write size before writing stuff
                        // (in case we will need reject the event)
                        u32 curr_wsize = librg_data_get_wpos(unreliable);

                        librg_data_went(unreliable, entity);
                        librg_data_wptr(unreliable, &eblob->position, sizeof(eblob->position));

                        // request custom data from user
                        librg_event_t event = { 0 }; {
                            event.peer   = blob->client_peer;
                            event.data   = unreliable;
                            event.entity = eblob;
                            event.flags  = (LIBRG_EVENT_REJECTABLE | LIBRG_EVENT_LOCAL);
                        }

                        librg_event_trigger(ctx, LIBRG_ENTITY_UPDATE, &event);

                        // check if event was rejected
                        if (event.flags & LIBRG_EVENT_REJECTED) {
                            updated_entities--;
                            librg_data_set_wpos(unreliable, curr_wsize);
                        }
                    }
                }

                // mark entity as existed for the next update
                librg_table_set(&next_snapshot, entity, 1);
            }

            // write our calcualted amounts right after packet id (from the beginning)
            librg_data_wu32_at(reliable, created_entities, sizeof(librg_message_id));
            librg_data_wu32_at(unreliable, updated_entities, sizeof(librg_message_id) + sizeof(f64));

            // save pos for remove data counter
            usize write_pos = librg_data_get_wpos(reliable);
            librg_data_wu32(reliable, 0);

            // add entity removes
            for (isize i = 0; i < zpl_array_count(last_snapshot->entries); ++i) {
                librg_entity_id entity = (librg_entity_id)last_snapshot->entries[i].key;

                // check if entity existed before
                b32 not_existed = last_snapshot->entries[i].value;
                if (not_existed == 0) continue;

                // skip entity delete if this is player's entity
                if (entity == player) continue;

                // save write size before writing stuff
                // (in case we will need reject the event)
                u32 curr_wsize = librg_data_get_wpos(reliable);

                // write id
                librg_data_went(reliable, entity);
                removed_entities++;

                // write the rest
                librg_event_t event = { 0 }; {
                    event.peer   = blob->client_peer;
                    event.data   = reliable;
                    event.entity = &ctx->entity.list[entity];
                    event.flags  = (LIBRG_EVENT_REJECTABLE | LIBRG_EVENT_LOCAL);
                }

                librg_event_trigger(ctx, LIBRG_ENTITY_REMOVE, &event);

                // check if even was rejected
                if (event.flags & LIBRG_EVENT_REJECTED) {
                    removed_entities--;
                    librg_data_set_wpos(reliable, curr_wsize);
                }
            }

            // write remove amount
            librg_data_wu32_at(reliable, removed_entities, write_pos);

            // swap snapshot tables
            librg_table_destroy(&blob->last_snapshot);
            *last_snapshot = next_snapshot;

            #ifdef LIBRG_MULTITHREADED
            if (librg_option_get(LIBRG_MAX_THREADS_PER_UPDATE) > 0) zpl_mutex_lock(ctx->threading.send_lock);
            #endif

            // send the data, via differnt channels and reliability setting
            if (librg_data_get_wpos(reliable) > (sizeof(librg_message_id) + sizeof(u32) * 2)) {
                enet_peer_send(blob->client_peer, librg_option_get(LIBRG_NETWORK_PRIMARY_CHANNEL),
                    enet_packet_create(reliable->rawptr, librg_data_get_wpos(reliable), ENET_PACKET_FLAG_RELIABLE)
                );
            }

            enet_peer_send(blob->client_peer, librg_option_get(LIBRG_NETWORK_SECONDARY_CHANNEL),
                enet_packet_create(unreliable->rawptr, librg_data_get_wpos(unreliable), 0)
            );

            #ifdef LIBRG_MULTITHREADED
            if (librg_option_get(LIBRG_MAX_THREADS_PER_UPDATE) > 0) zpl_mutex_unlock(ctx->threading.send_lock);
            #endif

            // and cleanup
            librg_data_reset(reliable);
            librg_data_reset(unreliable);
        }
    }

    #ifdef LIBRG_MULTITHREADED
    isize librg__execute_server_entity_update_worker(zpl_thread_t *thread) {
        librg_update_worker_si_t *si = cast(librg_update_worker_si_t *)thread->user_data;
        librg_ctx_t *ctx = si->ctx;

        librg_data_t reliable, unreliable;
        librg_data_init(&reliable);
        librg_data_init(&unreliable);

        while (true) {
            i32 signal = zpl_atomic32_load(&ctx->threading.signal);

            while (signal == LIBRG_THREAD_IDLE) {
                zpl_sleep_ms(1);
                signal = zpl_atomic32_load(&ctx->threading.signal);
                zpl_mfence();
            };

            if (signal == LIBRG_THREAD_EXIT) break;

            librg__execute_server_entity_update_proc(ctx, &reliable, &unreliable, si->offset, si->count);
            zpl_atomic32_fetch_add(&ctx->threading.work_count, -1);
        }

        librg_data_free(&reliable);
        librg_data_free(&unreliable);

        zpl_free(ctx->allocator, si);
        thread->return_value = 0;

        return 0;
    }
    #endif

    librg_internal void librg__execute_server_entity_update(librg_ctx_t *ctx) {
        librg_assert(ctx);

        if (librg_option_get(LIBRG_MAX_THREADS_PER_UPDATE) == 0) {
            librg__execute_server_entity_update_proc(ctx, &ctx->stream_upd_reliable, &ctx->stream_upd_unreliable, 0, ctx->max_entities);
            return;
        }

        #ifdef LIBRG_MULTITHREADED
        zpl_atomic32_store(&ctx->threading.signal, LIBRG_THREAD_WORK);
        zpl_atomic32_store(&ctx->threading.work_count, librg_option_get(LIBRG_MAX_THREADS_PER_UPDATE));

        i32 work_count = zpl_atomic32_load(&ctx->threading.work_count);
        while (work_count > 0) {
            zpl_sleep_ms(1);
            work_count = zpl_atomic32_load(&ctx->threading.work_count);
            zpl_mfence();
        }

        zpl_atomic32_store(&ctx->threading.signal, LIBRG_THREAD_IDLE);
        #endif
    }
#endif

// =======================================================================//
// !
// ! World Spaces (TODO)
// !
// =======================================================================//

#if 1
    b32 librg__space_bounds_small_enough(zplm_aabb3 a, zplm_vec3 b) {
        //TODO(zaklaus): Is this the best way we can determine bounds for k-d ?
        return a.half_size.x <= b.x && a.half_size.y <= b.y && a.half_size.z <= b.z;
    }

    zpl_global f32 librg__space_tpl[][3] = {
        {-1.0f, -1.0f, -1.0f},
        {+1.0f, -1.0f, -1.0f},
        {-1.0f, +1.0f, -1.0f},
        {+1.0f, +1.0f, -1.0f},
        {-1.0f, -1.0f, +1.0f},
        {+1.0f, -1.0f, +1.0f},
        {-1.0f, +1.0f, +1.0f},
        {+1.0f, +1.0f, +1.0f},
    };

    void librg__space_split(librg_space_t *c) {
        zplm_aabb3 hd = c->boundary;
        for (i32 i = 0; i < c->dimensions; ++i) {
            hd.half_size.e[i] /= 2.0f;
        }

        i32 loops = 4;
        if (c->dimensions == LIBRG_SPACE_3D)
            loops = 8;

        f32 p[3] = {0};
        for (i32 i = 0; i < loops; ++i) {
            librg_space_t space = {0};
            zplm_aabb3 bounds = {0};
            p[0] = c->boundary.centre.e[0] + hd.half_size.e[0]*librg__space_tpl[i][0];
            p[1] = c->boundary.centre.e[1] + hd.half_size.e[1]*librg__space_tpl[i][1];
            p[2] = c->boundary.centre.e[2] + hd.half_size.e[2]*librg__space_tpl[i][2];
            zpl_memcopy(bounds.centre.e, p, 3 * sizeof(f32));
            bounds.half_size = hd.half_size;

            space.boundary   = bounds;
            space.min_bounds = c->min_bounds;
            space.use_min_bounds = c->use_min_bounds;
            space.max_nodes  = c->max_nodes;
            space.dimensions = c->dimensions;
            space.allocator  = c->allocator;

            zpl_array_append(c->spaces, space);
        }
    }

    b32 librg__space_remove_node(librg_space_t *c, librg_entity_t *tag) {
        if (c->nodes == NULL) return false;
        for (i32 i = 0; i < zpl_array_count(c->nodes); ++i) {
            librg_space_node_t *node = &c->nodes[i];
            if (node->blob == tag) {
                if (node->unused) return false;
                if (c->free_nodes == NULL) {
                    zpl_array_init_reserve(c->free_nodes, c->allocator, c->max_nodes);
                }
                zpl_array_append(c->free_nodes, i);
                node->unused = true;
                return true;
            }
        }

        return false;
    }

    void librg__space_init(librg_space_t *c, zpl_allocator_t a, isize dims, zplm_aabb3 bounds, zplm_vec3 min_bounds, u32 max_nodes) {
        librg_space_t c_ = {0};
        *c            = c_;
        c->allocator  = a;
        c->boundary   = bounds;
        c->min_bounds = min_bounds;
        c->max_nodes  = max_nodes;
        c->dimensions = dims;
        c->use_min_bounds = zplm_vec3_mag(min_bounds) > 0.0f;
    }

    void librg__space_clear(librg_space_t *c) {
        // TODO(ZaKlaus): Support more allocators?
        if (c->allocator.proc == zpl_arena_allocator_proc) {
            zpl_free_all(c->allocator);
            c->nodes = NULL;
            c->spaces = NULL;
            return;
        }

        if (c->nodes) {
            zpl_array_free(c->nodes);
            c->nodes = NULL;
        }

        if (!c->spaces) return;
        isize spaces_count = zpl_array_count(c->spaces);
        for (i32 i = 0; i < spaces_count; ++i) {
            librg__space_clear((c->spaces+i));
        }

        zpl_array_free(c->spaces);
        if (c->free_nodes) zpl_array_free(c->free_nodes);
        c->spaces = NULL;
        c->free_nodes = NULL;
    }

    librg_inline librg_space_t *librg__space_insert(librg_ctx_t *ctx, librg_space_t *space, librg_space_node_t node) {
        if (!librg__space_contains(space->dimensions, space->boundary, node.blob->position.e)) return NULL;

        if (space->nodes == NULL) {
            zpl_array_init(space->nodes, space->allocator);
        }

        if (space->free_nodes && zpl_array_count(space->free_nodes) > 0) {
            node.unused = false;
            space->nodes[space->free_nodes[zpl_array_count(space->free_nodes)-1]] = node;
            zpl_array_pop(space->free_nodes);
            return space;
        }

        if ((usize)zpl_array_count(space->nodes) < space->max_nodes) {
            insert:
            zpl_array_append(space->nodes, node);
            return space;
        }

        if (space->use_min_bounds && librg__space_bounds_small_enough(space->boundary, space->min_bounds)) {
            goto insert;
        }

        if (space->spaces == NULL) {
            zpl_array_init(space->spaces, space->allocator);
        }

        isize spaces_count = zpl_array_count(space->spaces);
        if (spaces_count == 0) {
            librg__space_split(space);
        }

        spaces_count = zpl_array_count(space->spaces);
        for (i32 i = 0; i < spaces_count; ++i) {
            librg_space_t *sub = librg__space_insert(ctx, (space->spaces+i), node);
            if (sub) return sub;
        }

        return NULL;
    }

    librg_inline void librg__execute_server_entity_insert(librg_ctx_t *ctx) {
        librg_assert(ctx);

        // fill up
        librg_entity_iteratex(ctx, LIBRG_ENTITY_ALIVE, entity, {
            librg_entity_t *blob = &ctx->entity.list[entity];

            librg_space_node_t node = { 0 };

            node.blob        = blob;

            if (blob->stream_branch == NULL) {
                blob->stream_branch = librg__space_insert(ctx, &ctx->world, node);
            }
            else {
                librg_space_t *branch = blob->stream_branch;
                b32 contains = librg__space_contains(branch->dimensions, branch->boundary, blob->position.e);
                if (!contains) {
                    librg__space_remove_node(branch, blob);
                    blob->stream_branch = librg__space_insert(ctx, &ctx->world, node);
                }
            }
        });
    }

    librg_inline void librg__execute_server_entity_destroy(librg_ctx_t *ctx) {
        for (isize i = 0; i < zpl_array_count(ctx->entity.remove_queue); i++) {
            librg__world_entity_destroy(ctx, ctx->entity.remove_queue[i]);
        }

        zpl_array_clear(ctx->entity.remove_queue);
    }

    librg_inline void librg__execture_server_entity_control(librg_ctx_t *ctx) {
        for (isize i = 0; i < zpl_array_count(ctx->entity.add_control_queue); i++) {
            librg_message_t *msg = ctx->entity.add_control_queue[i];
            enet_peer_send(msg->peer, librg_option_get(LIBRG_NETWORK_MESSAGE_CHANNEL), msg->packet);
            zpl_free(ctx->allocator, msg);
        }

        zpl_array_clear(ctx->entity.add_control_queue);
    }

    librg_internal void librg__world_update(void *data) {
        f64 start  = zpl_time_now();
        librg_ctx_t *ctx = (librg_ctx_t *)data;
        librg_assert(ctx);

        if (librg_is_server(ctx)) {
            librg__execute_server_entity_insert(ctx); /* create the server cull tree */
            librg__execute_server_entity_update(ctx); /* create and send updates to all clients */
            librg__execute_server_entity_destroy(ctx); /* destroy queued entities */
            librg__execture_server_entity_control(ctx); /* send controll add for created entities */
        } else {
            librg__execute_client_update(ctx); /* send information about client updates */
        }

        ctx->last_update = zpl_time_now() - start;
    }







    void librg__world_entity_query(librg_ctx_t *ctx, librg_entity_id entity, librg_space_t *c, zplm_aabb3 bounds, librg_entity_id **out_entities) {
        if (c->nodes == NULL) return;
        if (!librg__space_intersects(c->dimensions, c->boundary, bounds)) return;

        isize nodes_count = zpl_array_count(c->nodes);
        for (i32 i = 0; i < nodes_count; ++i) {
            if (c->nodes[i].unused) continue;

            librg_entity_id target = c->nodes[i].blob->id;

            if (librg_entity_valid(ctx, target)) {
                librg_entity_t *blob = c->nodes[i].blob;
                b32 inside = librg__space_contains(c->dimensions, bounds, blob->position.e);

                if (inside) {
                    if (!librg_entity_visibility_get(ctx, target)) continue;
                    if (!librg_entity_visibility_get_for(ctx, target, entity)) continue;

                    zpl_array_append(*out_entities, target);
                }
            }
        }

        if(c->spaces == NULL) return;

        isize spaces_count = zpl_array_count(c->spaces);
        if (spaces_count == 0) return;

        for (i32 i = 0; i < spaces_count; ++i) {
            librg__world_entity_query(ctx, entity, (c->spaces+i), bounds, out_entities);
        }
    }

    b32 librg__world_entity_destroy(librg_ctx_t *ctx, librg_entity_id id) {
        librg_assert(ctx);
        librg_assert(librg_entity_valid(ctx, id));
        librg_assert(ctx->entity.count > 0);

        ctx->entity.count--;
        librg_entity_t *entity = &ctx->entity.list[id];

        if (entity->flags & LIBRG_ENTITY_CLIENT) {
            entity->client_peer = NULL;
            librg_table_destroy(&entity->last_snapshot);
        }

        // remove entity from the streamer
        if (entity->stream_branch) {
            librg__space_remove_node(entity->stream_branch, entity);
        }

        if (entity->flags & LIBRG_ENTITY_QUERIED) {
            zpl_array_free(entity->last_query);
        }

        if (entity->flags & LIBRG_ENTITY_IGNORING) {
            librg_table_destroy(&entity->ignored);
        }

        if (librg_is_server(ctx)) {
            librg_entity_visibility_set(ctx, entity->id, true);
        }

        entity->flags         = LIBRG_ENTITY_NONE;
        entity->position      = zplm_vec3f_zero();
        entity->type          = 0;
        entity->stream_branch = NULL;

        return true;
    }
#endif

// =======================================================================//
// !
// ! Options
// !
// =======================================================================//

    /* Global option storage */
    librg_global u32 librg_options[LIBRG_OPTIONS_SIZE] = {
        /*LIBRG_PLATFORM_ID*/               1,
        /*LIBRG_PLATFORM_PROTOCOL*/         1,
        /*LIBRG_PLATFORM_BUILD*/            1,
        /*LIBRG_DEFAULT_CLIENT_TYPE*/       0,
        /*LIBRG_DEFAULT_STREAM_RANGE*/      250,
        /*LIBRG_DEFAULT_DATA_SIZE*/         1024,
        /*LIBRG_NETWORK_CAPACITY*/          2048,
        /*LIBRG_NETWORK_CHANNELS*/          4,
        /*LIBRG_NETWORK_PRIMARY_CHANNEL*/   1,
        /*LIBRG_NETWORK_SECONDARY_CHANNEL*/ 2,
        /*LIBRG_NETWORK_MESSAGE_CHANNEL*/   3,
        /*LIBRG_NETWORK_BUFFER_SIZE*/       0,
        /*LIBRG_MAX_ENTITIES_PER_BRANCH*/   4,
        /*LIBRG_MAX_THREADS_PER_UPDATE*/    0, /* MT is disabled by default = 0 */
    };

    void librg_option_set(u32 option, u32 value) {
        librg_options[option] = value;
    }

    u32 librg_option_get(u32 option) {
        return librg_options[option];
    }

#undef LIBRG_MESSAGE_TO_EVENT
#ifdef __cplusplus
}
#endif
#endif // LIBRG_IMPLEMENTATION
#endif // LIBRG_INCLUDE_H
