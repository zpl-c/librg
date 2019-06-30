/**
 * librg - a library for building simple and elegant cross-platform multiplayer client-server solutions.
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
 * enet.h
 *
 * For the demo:
 * sdl2.h
 *
 * Version History:
 * 4.1.1
 * - Added compile-time 'features':
 *     - Ability to enable/disable some librg compile-time features
 *     - Entity igore tables are now optional, and can be disabled
 *     - Implmented simple optional Virtual world feature for entities
 *     - Multiple features can be combined
 * - Added 'generation' to entity control lists:
 *     Setting, removing and setting control to the same entity again with same owner
 *     will now distinct between old and new controllers, and messages still coming
 *     from old control generation will be rejected in favor of new ones.
 * - Added guard to minimum sized packet in receive for both sides
 * - Added sperical culler handler, and ability to do runtime switch
 * - Streamed entities are now gonna be always returned in query for controlling peer
 * - Fixed issue with host setting on the server side
 * - Fixed nullptr crash on empty host string for client on connect
 * - Removed experimental multithreading code
 *
 * 4.1.0
 * - Added new, extended message methods and sending options
 *
 * 4.0.0
 * - Coding style changes and bug fixes
 *
 * 3.3.1
 * - Updated zpl dependencies
 * - Removed zpl_math dependency (replaced by internal module in zpl)
 *
 * 3.3.0
 * - Added ipv6 support
 * - Added safe bitstream reads for internal methods
 * - Updated enet to latest version (2.0.1, ipv6 support)
 * - Updated zpl to latest version
 *
 * 3.2.0
 * - Fixed minor memory client-side memory leak with empty control list
 * - Fixed issue with client stream update and removed entity on server
 * - Updated zpl to new major version, watch out for possible incompatibilities
 * - Added method for alloc/dealloc the librg_ctx, librg_data, librg_event for the bindings
 * - Added experimental support for update buffering, disabled by default, and not recommended to use
 * - Added built-in timesyncer, working on top of monotonic time, syncing client clock to server one
 * - Added helper methods: librg_time_now, librg_standard_deviation
 * - Changed ctx->tick_delay from u16 to f64 (slightly more precision)
 *
 * Version History:
 * 3.1.0
 * - Removed zpl_cull and zpl_event dependencies
 * - added librg_network_kick()
 * - saving current librg_address to ctx->network
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
 * 2.2.1 - Fixed cpp issues with librg_data pointers
 * 2.2.0 - Inner message system rafactor
 * 2.1.0 - Inner bitstream refactors, with slight interface changes
 * 2.0.2 - C++ and MSVC related fixes
 * 2.0.0 - Initial C version rewrite
 *
 * Things TODO:
 *     find a nice way to decrease size on client for peer struct
 *     try to remember stuff about bistram safety
 *     fix disconnection code issues
 * - Add method to check if entity is in stream of other entity
 * - Add DEBUG packet size validation (FEATURE)
 * - refactoring librg_table (FEATURE)
 * - remove entity ignore for target entity that was disconnected/deleted (BUG)
 * - remove controller peer for entity, on owner disconnect (BUG)
 * - possibly adding stream_range to the query, to make it bi-sided (FEATURE)
 * - tree/space node flattening (FEATURE)
 *
 * Useful links:
 * - https://antriel.com/post/online-platformer-5/#server-update-data
 * - https://github.com/fnuecke/Space/blob/5ddfec5fc8139c4b0def8fdfc16b5f5357a8cd38/EngineController/AbstractTssClient.cs#L325
 *
 * License notice:
 *
 * Copyright 2017-2018 Vladyslav Hrytsenko
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

#define LIBRG_VERSION_MAJOR 4
#define LIBRG_VERSION_MINOR 1
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
    #ifdef LIBRG_IMPLEMENTATION
        #define ZPL_IMPLEMENTATION
        #define ENET_IMPLEMENTATION
    #endif
    #include "zpl.h"

    #ifdef ZPL_SYSTEM_WINDOWS
        #define _WINSOCK_DEPRECATED_NO_WARNINGS
    #endif

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

/* keeping enabled by default depecrecated */
/* naming support till next major release */
#ifndef LIBRG_NO_DEPRECATIONS
#define librg_ctx_t     librg_ctx
#define librg_data_t    librg_data
#define librg_entity_t  librg_entity
#define librg_address_t librg_address
#define librg_message_t librg_message
#define librg_event_t   librg_event
#define librg_peer_t    librg_peer
#define librg_host_t    librg_host
#define librg_packet_t  librg_packet
#endif

#ifdef __cplusplus
extern "C" {
#endif


// =======================================================================//
// !
// ! Library compile-time configure features
// !
// =======================================================================//

#define LIBRG_FEATURE_VIRTUAL_WORLDS 1      // enabled by default (define LIBRG_DISABLE_FEATURE_VIRTUAL_WORLDS before including to disable)
#define LIBRG_FEATURE_ENTITY_VISIBILITY 1   // enabled by default (define LIBRG_DISABLE_FEATURE_ENTITY_VISIBILITY before including to disable)
// #define LIBRG_FEATURE_OCTREE_CULLER 1    // disabled by default (define before including to enable)

#if defined(LIBRG_FEATURE_VIRTUAL_WORLDS) && defined(LIBRG_DISABLE_FEATURE_VIRTUAL_WORLDS)
#undef LIBRG_FEATURE_VIRTUAL_WORLDS
#endif

#if defined(LIBRG_FEATURE_ENTITY_VISIBILITY) && defined(LIBRG_DISABLE_FEATURE_ENTITY_VISIBILITY)
#undef LIBRG_FEATURE_ENTITY_VISIBILITY
#endif

#if defined(LIBRG_FEATURE_OCTREE_CULLER) && defined(LIBRG_DISABLE_FEATURE_OCTREE_CULLER)
#undef LIBRG_FEATURE_OCTREE_CULLER
#endif

// =======================================================================//
// !
// ! Public API
// !
// =======================================================================//

struct librg_ctx;
struct librg_data;
struct librg_entity;
struct librg_address;
struct librg_message;
struct librg_event;

typedef ENetPeer   librg_peer;
typedef ENetHost   librg_host;
typedef ENetPacket librg_packet;

enum librg_mode         { LIBRG_MODE_SERVER, LIBRG_MODE_CLIENT };
enum librg_space_type   { LIBRG_SPACE_2D = 2, LIBRG_SPACE_3D = 3 };
enum librg_pointer_type { LIBRG_POINTER_CTX, LIBRG_POINTER_DATA, LIBRG_POINTER_EVENT };
enum librg_thread_state { LIBRG_THREAD_IDLE, LIBRG_THREAD_WORK, LIBRG_THREAD_EXIT };

/**
 * Simple host address
 * used to configure network on start
 */
typedef struct librg_address { i32 port; char *host; } librg_address;

typedef void (librg_entity_cb)(struct librg_ctx *ctx, struct librg_entity *entity);
typedef void (librg_message_cb)(struct librg_message *msg);
typedef void (librg_event_cb)(struct librg_event *event);
typedef librg_event_cb **librg_event_block; ///< zpl_array

ZPL_TABLE_DECLARE(extern, librg_table, librg_table_, u32);
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
LIBRG_API void librg_init(struct librg_ctx *ctx);
/**
 * Should be called at the end of
 * execution of the program
 */
LIBRG_API void librg_free(struct librg_ctx *ctx);
/**
 * Main tick method
 * MUST BE called in your loop
 * preferably w/o delays
 *
 * Calling tick ensures that packets will be sent and received,
 * world state will be recalculated and new update data will be created/used
 */
LIBRG_API void librg_tick(struct librg_ctx *ctx);

// =======================================================================//
// !
// ! Entities
// !
// =======================================================================//

#define librg_entity_id u32

enum librg_entity_flag {
    LIBRG_ENTITY_NONE       = 0,        /* general flag, all destroyed/non-created entities have it */
    LIBRG_ENTITY_ALIVE      = (1 << 0), /* general flag, all created entities have it */
    LIBRG_ENTITY_CLIENT     = (1 << 1), /* flag describing entities created for client peer */
    LIBRG_ENTITY_IGNORING   = (1 << 2), /* flag showing that entity has ignore overrides */
    LIBRG_ENTITY_QUERIED    = (1 << 3), /* flag showing that entity has a cached culler query */
    LIBRG_ENTITY_CONTROLLED = (1 << 4), /* flag showing if the entity is controlled(streamed) by some peer */
    LIBRG_ENTITY_UNUSED     = (1 << 5), /* flag showing whether the entity's space is unused */
    LIBRG_ENTITY_CONTROL_REQUESTED = (1 << 5), /* flag showing whether or not an entity control has been requested, but not yet sent */
};

typedef struct librg_entity {
    u32 id;
    u32 type;
    u64 flags;

    zpl_vec3 position;
    f32 stream_range;

    void *user_data;
    struct librg_space *stream_branch;

    #ifdef LIBRG_FEATURE_ENTITY_VISIBILITY
    librg_table ignored;
    #endif

    #ifdef LIBRG_FEATURE_VIRTUAL_WORLDS
    u32 virual_world;
    #endif

    librg_table last_snapshot;

    librg_peer *client_peer;
    librg_peer *control_peer;

    u8 control_generation;

    librg_entity_id *last_query; ///< zpl_array
} librg_entity;

/**
 * Create entity and return handle
 */
LIBRG_API struct librg_entity *librg_entity_create(struct librg_ctx *ctx, u32 type);
/**
 * Try to find an existing entity by a provided id, or NULL
 */
LIBRG_API struct librg_entity *librg_entity_fetch(struct librg_ctx *ctx, librg_entity_id entity_id);
/**
 * Reuqest an entity destruction (will be marked as destroyed, and send to be destroyed on clients on the next tick)
 */
LIBRG_API void librg_entity_destroy(struct librg_ctx *ctx, librg_entity_id entity_id);
/**
 * Query for entities that are in stream zone
 * for current entity, and are visible to this entity
 * an uninitialized pointer to the pointer of librg_entity_id should be provided:
 *
 * EXAMPLE:
 * librg_entity_id *result;
 * usize amount = librg_entity_query(&ctx, 15, &result);
 */
LIBRG_API usize librg_entity_query(struct librg_ctx *ctx, librg_entity_id entity_id, librg_entity_id **result);
/**
 * Check if provided entity is a valid entity
 */
LIBRG_API b32 librg_entity_valid(struct librg_ctx *ctx, librg_entity_id id);
/**
 * Try to find an entity that is assossiated with a particular peer, or NULL
 */

LIBRG_API struct librg_entity *librg_entity_find(struct librg_ctx *ctx, librg_peer *peer);

/**
 * Concept of entity control describes an entity behavior, data of which is constantly sent to the server from a particular peer (controller).
 * And, updates about that entity are later sent to other peers, except the original controller.
 *
 * New data will arrive, accordingly to configured client tick delay, and the LIBRG_CLIENT_STREAMER_UPDATE event will be triggered.
 * librg_entity_control_set is an async method, it will send a message to the client, notifiyng him of becoming a controller.
 * librg_entity_control_remove is a hybrid sync+async method. Behaves similar wat to _set, described above, however,
 * even thought client will still be sending control messages for some time, server will reject them if method has been called.
 *
 * For occasions, where some sort of an authoritative pause is needed for the server, in the controlled data stream,
 * for example you want to change some property data of the entity, authoritatively from the server, like position.
 * Calling librg_entity_control_remove(ctx, id), making the neccessary changes, and calling librg_entity_control_set(ctx, id, peer)
 * will make sure all current streamed data from the client will be ignored, and new data will be force-delivered to client
 */

LIBRG_API librg_peer *librg_entity_control_get(struct librg_ctx *ctx, librg_entity_id entity_id);
LIBRG_API void librg_entity_control_set(struct librg_ctx *ctx, librg_entity_id entity_id, librg_peer *peer);
LIBRG_API void librg_entity_control_remove(struct librg_ctx *ctx, librg_entity_id entity_id);
LIBRG_API void librg_entity_control_ignore_next_update(struct librg_ctx *ctx, librg_entity_id entity_id);

/**
 * Iterate over all the entities with a flag
 */
LIBRG_API void librg_entity_iterate(struct librg_ctx *ctx, u64 flags, librg_entity_cb callback);
/**
 * C based entity iteration macro
 */
#define librg_entity_iteratex(ctx, cflags, cname, code) do { \
    for (u32 _ent = 0, _valid = 0; _ent < ctx->max_entities && _valid < ctx->entity.count; ++_ent) { \
        if ((ctx->entity.list[_ent].flags & (LIBRG_ENTITY_ALIVE | cflags)) == (LIBRG_ENTITY_ALIVE | cflags)) { \
            _valid++; librg_entity_id cname = _ent; code; \
        } \
    } \
} while (0);

/**
 * Return entity type
 * @deprecated
 */
LIBRG_API u32 librg_entity_type(struct librg_ctx *ctx, librg_entity_id entity_id);

#ifdef LIBRG_FEATURE_ENTITY_VISIBILITY

    /**
     * A complex api allowing to change visibility of the entities via specific set of rules.
     *
     * Calling librg_entity_visibility_set, will change global visibility of partiulcar entity for others.
     * Meaning, the entity is still gonna be able to see other entities, while others won't be able to see it back.
     *
     * Calling librg_entity_visibility_set_for, has a similar effect to method mentioned above,
     * however extends also to specify entity inivisibilty for other specific entity.
     */

    LIBRG_API void librg_entity_visibility_set(struct librg_ctx *ctx, librg_entity_id entity_id, b32 state);
    LIBRG_API b32 librg_entity_visibility_get(struct librg_ctx *ctx, librg_entity_id entity_id);

    LIBRG_API void librg_entity_visibility_set_for(struct librg_ctx *ctx, librg_entity_id entity_id, librg_entity_id target, b32 state);
    LIBRG_API b32 librg_entity_visibility_get_for(struct librg_ctx *ctx, librg_entity_id entity_id, librg_entity_id target);
#endif

#ifdef LIBRG_FEATURE_VIRTUAL_WORLDS

    /**
     * A simple thin api allowing to change virtual world for specific entity
     * "Virtual world" is a concept where entities can see other entities near
     * each other only when they share the virtual world.
     *
     * If entity is being removed from the virtual world, it's, LIBRG_ENTITY_REMOVED
     * will be called for all the clients that were sharing same world with target entity.
     */

    LIBRG_API u32  librg_entity_world_get(struct librg_ctx *ctx, librg_entity_id entity_id);
    LIBRG_API void librg_entity_world_set(struct librg_ctx *ctx, librg_entity_id entity_id, u32 world);
#endif

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

typedef struct librg_event {
    u32 id; /* librg event id, that is being triggered */
    struct librg_ctx *ctx;   /* librg context where event has been called */
    struct librg_data *data;  /* optional: data is used for built-in events */
    struct librg_entity *entity; /* optional: entity is used for built-in events */

    librg_peer *peer;  /* optional: peer is used for built-in events */

    u64 flags;  /* flags for that event */
    void *user_data; /* optional: user information */
} librg_event;

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
LIBRG_API u64 librg_event_add(struct librg_ctx *ctx, u64 id, librg_event_cb callback);
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
LIBRG_API void librg_event_trigger(struct librg_ctx *ctx, u64 id, struct librg_event *event);
/**
 * Used to remove particular callback from
 * event chain, so it wont be called ever again
 *
 * @param id usually you define event ids inside enum
 * @param index returned by librg_event_add
 */
LIBRG_API void librg_event_remove(struct librg_ctx *ctx, u64 id, u64 index);
/**
 * Used to reject some event from triggering from
 * inside of executing callback
 */
LIBRG_API void librg_event_reject(struct librg_event *event);
/**
 * Used to check if some event can be rejected
 */
LIBRG_API b32 librg_event_rejectable(struct librg_event *event);
/**
 * Checks if current event was not rejected
 * inside any of the callbacks
 */
LIBRG_API b32 librg_event_succeeded(struct librg_event *event);

// =======================================================================//
// !
// ! Binary data (bitstream)
// !
// =======================================================================//

typedef struct librg_data {
    usize capacity;
    usize read_pos;
    usize write_pos;

    void *rawptr;

    zpl_allocator allocator;
} librg_data;

/**
 * Initialize new bitstream with default mem size
 */
LIBRG_API void librg_data_init(struct librg_data *data);
/**
 * Initialize new bitstream with custom mem size
 */
LIBRG_API void librg_data_init_size(struct librg_data *data, usize size);
/**
 * Initialize new bitstream with custom mem size (bindings stuff)
 */
LIBRG_API struct librg_data *librg_data_init_new();
/**
 * Free initialized bitstream
 */
LIBRG_API void librg_data_free(struct librg_data *data);
/**
 * Reset initialized bitstream
 * NOTE: doesnt remove any data, just resets read and write pos to 0
 */
LIBRG_API void librg_data_reset(struct librg_data *data);
/**
 * Increase size of bitstream
 */
LIBRG_API void librg_data_grow(struct librg_data *data, usize min_size);
/**
 * Methods for getting various parameters of bitstream
 */
LIBRG_API usize librg_data_capacity(struct librg_data *data);
LIBRG_API usize librg_data_get_rpos(struct librg_data *data);
LIBRG_API usize librg_data_get_wpos(struct librg_data *data);
LIBRG_API void  librg_data_set_rpos(struct librg_data *data, usize position);
LIBRG_API void  librg_data_set_wpos(struct librg_data *data, usize position);
/**
 * Read and write methods for custom sized data
 */
LIBRG_API void librg_data_rptr(struct librg_data *data, void *ptr, usize size);
LIBRG_API void librg_data_wptr(struct librg_data *data, void *ptr, usize size);
/**
 * Read and write methods for custom sized data
 * at particular position in memory
 */
LIBRG_API void librg_data_rptr_at(struct librg_data *data, void *ptr, usize size, isize position);
LIBRG_API void librg_data_wptr_at(struct librg_data *data, void *ptr, usize size, isize position);
/**
 * A helprer macro for onliner methods
 */
#define LIBRG_GEN_DATA_READWRITE(TYPE) \
    LIBRG_API TYPE ZPL_JOIN2(librg_data_r,TYPE)(struct librg_data *data); \
    LIBRG_API void ZPL_JOIN2(librg_data_w,TYPE)(struct librg_data *data, TYPE value); \
    LIBRG_API TYPE ZPL_JOIN3(librg_data_r,TYPE,_at)(struct librg_data *data, isize position); \
    LIBRG_API void ZPL_JOIN3(librg_data_w,TYPE,_at)(struct librg_data *data, TYPE value, isize position);
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
typedef struct librg_message {
    u32 id;
    struct librg_ctx  *ctx;
    struct librg_data *data;

    librg_peer *peer;
    librg_packet *packet;

    void *user_data; /* optional: user information */
} librg_message;

/**
 * Check are we connected
 */
LIBRG_API b32 librg_is_connected(struct librg_ctx *ctx);
/**
 * Is librg instance is running
 * in the server mode
 */
LIBRG_API b32 librg_is_server(struct librg_ctx *ctx);
/**
 * Is librg instance is running
 * in the client mode
 */
LIBRG_API b32 librg_is_client(struct librg_ctx *ctx);
/**
 * Starts network connection
 * Requires you to provide .port (if running as server)
 * or both .port and .host (if running as client)
 *
 * For server mode - starts server
 * For client mode - starts client, and connects to provided host & port
 */
LIBRG_API void librg_network_start(struct librg_ctx *ctx, librg_address address);
/**
 * Disconnects (if connected), stops network
 * and releases resources
 */
LIBRG_API void librg_network_stop(struct librg_ctx *ctx);
/**
 * Forces disconnection for provided peer
 * @param ctx
 * @param peer
 */
LIBRG_API void librg_network_kick(struct librg_ctx *ctx, librg_peer *peer);
/**
 * Can be used to add handler
 * to a particular message id
 */
LIBRG_API void librg_network_add(struct librg_ctx *ctx, librg_message_id id, librg_message_cb callback);
/**
 * Can be used to remove a handler
 * from particular message id
 */
LIBRG_API void librg_network_remove(struct librg_ctx *ctx, librg_message_id id);

/**
 * General reliable message/data sending API
 * (original message api version since v2.0.0, might be subject for changes in the future)
 *
 * Message id - is an identifier which librg will use to route the message from
 * (to particlar handlers which are added via librg_network_add)
 *
 * method _all - sends the data to all connected peers
 * method _to - sends the data to particular peer
 * method _except - sends the data to all but particular peer
 * method _instream - sends the data to all connected peers in the particular range (based off entity's stream_range)
 * method _instream_except - similar to method above, however excludes particular peer
 *
 * The data which is provided inside the method will be copied inside packet struct, so it can be safely freed after you've called a method.
 */

LIBRG_API void librg_message_send_all             (struct librg_ctx *ctx, librg_message_id id, void *data, usize size);
LIBRG_API void librg_message_send_to              (struct librg_ctx *ctx, librg_message_id id, librg_peer *target, void *data, usize size);
LIBRG_API void librg_message_send_except          (struct librg_ctx *ctx, librg_message_id id, librg_peer *target, void *data, usize size);
LIBRG_API void librg_message_send_instream        (struct librg_ctx *ctx, librg_message_id id, librg_entity_id entity_id, void *data, usize size);
LIBRG_API void librg_message_send_instream_except (struct librg_ctx *ctx, librg_message_id id, librg_entity_id entity_id, librg_peer *target, void *data, usize size);

/**
 * The basic message/data sending API
 * Supports sending both reliable and unreliable messages, and ability to set which channel to use for message sending.
 *
 * All previous message sending methods are using these extended variants internally.
 *
 * method sendex:
 *     if target is NULL - will send the data to all connected peers, else - to a particular peer
 *     if except is NULL - argument does nothing, else - prevents data from being sent to a particlar peer
 *     channel - provides which network channel is supposed to be used for message sending
 *         (make that channel id is always < librg_option_get(LIBRG_NETWORK_CHANNELS), or use librg_option_set to change the default value)
 *     if reliable is 1 (true) - message will be sent using reliable methods, else using unreliable
 */

LIBRG_API void librg_message_sendex               (struct librg_ctx *ctx, librg_message_id id, librg_peer *target, librg_peer *except, u16 channel, b8 reliable, void *data, usize size);
LIBRG_API void librg_message_sendex_instream      (struct librg_ctx *ctx, librg_message_id id, librg_entity_id entity_id, librg_peer *except, u16 channel, b8 reliable, void *data, usize size);

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
LIBRG_API f64 librg_time_now(struct librg_ctx *ctx);
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
    LIBRG_USE_RADIUS_CULLING,

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
    librg_peer *peer;
} librg_snapshot;

ZPL_RING_DECLARE(librg_snapshot);

typedef struct librg_space_node {
    struct librg_entity *blob;
    b32 unused;
} librg_space_node;

typedef struct librg_space {
    zpl_allocator allocator;
    u32 max_nodes;
    isize dimensions;
    zpl_aabb3 boundary;
    zpl_vec3 min_bounds;
    b32 use_min_bounds;
    usize *free_nodes; ///< zpl_array
    struct librg_space *spaces; ///< zpl_array
    struct librg_space_node *nodes; ///< zpl_array
} librg_space;

#define LIBRG_TIMESYNC_SIZE 5
#define LIBRG_DATA_STREAMS_AMOUNT 4

/**
 * Context + config struct
 */
typedef struct librg_ctx {
    // core
    u16 mode;
    f64 tick_delay;

    // configuration
    u16 max_connections;
    u32 max_entities;

    zpl_vec3 world_size;
    zpl_vec3 min_branch_size;

    f64 last_update;
    void *user_data;

    struct {
        librg_peer *peer;
        librg_host *host;
        librg_table connected_peers;
        librg_address last_address;
    } network;

    struct {
        u32 count;
        u32 cursor;

        #ifdef LIBRG_FEATURE_ENTITY_VISIBILITY
        librg_table ignored;
        #endif

        struct librg_entity *list;
        librg_entity_id *remove_queue; ///< zpl_array
        librg_message **add_control_queue; ///< zpl_array
    } entity;

    union {
        struct {
            librg_data stream_input;
            librg_data stream_output;
            librg_data stream_upd_reliable;
            librg_data stream_upd_unreliable;
        };

        librg_data streams[LIBRG_DATA_STREAMS_AMOUNT];
    };

    struct {
        f64 start_time;
        f64 offset_time;
        f64 median;
        f64 history[LIBRG_TIMESYNC_SIZE];
        f32 server_delay;
        zpl_timer *timer;
    } timesync;

    usize buffer_size;
    zpl_timer *buffer_timer;
    zpl_ring_librg_snapshot buffer;
    librg_message_cb **messages; ///< zpl_buffer

    zpl_allocator     allocator;
    zpl_timer_pool    timers;
    librg_event_pool  events;
    librg_space       world;

} librg_ctx;

// =======================================================================//
// !
// ! Extensions
// !
// =======================================================================//

#if 1
    #define librg__send_internal(CTX, ID, NAME, CALLBACK_CODE, SEND_CODE) \
        librg_data NAME; \
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
    ZPL_TABLE_DEFINE(librg_table, librg_table_, u32);
    ZPL_RING_DEFINE(librg_snapshot);

    /* internal declarations */
    LIBRG_INTERNAL void librg__callback_connection_init(librg_message *msg);
    LIBRG_INTERNAL void librg__callback_connection_request(librg_message *msg);
    LIBRG_INTERNAL void librg__callback_connection_refuse(librg_message *msg);
    LIBRG_INTERNAL void librg__callback_connection_accept(librg_message *msg);
    LIBRG_INTERNAL void librg__callback_connection_disconnect(librg_message *msg);
    LIBRG_INTERNAL void librg__callback_connection_timesync(librg_message *msg);

    LIBRG_INTERNAL void librg__callback_entity_create(librg_message *msg);
    LIBRG_INTERNAL void librg__callback_entity_update(librg_message *msg);
    LIBRG_INTERNAL void librg__callback_entity_client_streamer_add(librg_message *msg);
    LIBRG_INTERNAL void librg__callback_entity_client_streamer_remove(librg_message *msg);
    LIBRG_INTERNAL void librg__callback_entity_client_streamer_update(librg_message *msg);

    LIBRG_INTERNAL void librg__timesync_start(librg_ctx *ctx);
    LIBRG_INTERNAL void librg__timesync_tick(void *usrptr);
    LIBRG_INTERNAL void librg__timesync_stop(librg_ctx *ctx);

    LIBRG_INTERNAL void librg__buffer_init(librg_ctx *ctx, usize size);
    LIBRG_INTERNAL void librg__buffer_free(librg_ctx *ctx);
    LIBRG_INTERNAL void librg__buffer_push(librg_ctx *ctx, f64 time, librg_peer *peer, void *data, usize size);
    LIBRG_INTERNAL void librg__buffer_tick(void *usrptr);

    // short internal helper macro
    #define LIBRG_MESSAGE_TO_EVENT(NAME, MSG) \
        librg_event NAME = {0}; \
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
        struct librg_ctx *ctx;
    } librg_update_worker_si_t;


    LIBRG_INTERNAL void librg__world_update(void *);

    LIBRG_INTERNAL void librg__world_entity_query(librg_ctx *ctx, librg_entity_id entity, librg_space *c, zpl_aabb3 bounds, usize controlled_amount, librg_entity_id **out_entities);
    LIBRG_INTERNAL b32 librg__world_entity_destroy(librg_ctx *ctx, librg_entity_id id);

    /* space stuff */

    LIBRG_INTERNAL void librg__space_init(librg_space *c, zpl_allocator a, isize dims, zpl_aabb3 bounds, zpl_vec3 min_bounds, u32 max_nodes);
    LIBRG_INTERNAL void librg__space_clear(librg_space *c);
    b32 librg__space_remove_node(librg_space *c, librg_entity *tag);

    LIBRG_INTERNAL b32 librg__space_intersects(isize dims, zpl_aabb3 a, zpl_aabb3 b) {
        for (i32 i = 0; i < dims; ++i) {
            if (zpl_abs(a.centre.e[i] - b.centre.e[i]) > (a.half_size.e[i] + b.half_size.e[i])) return false;
        }

        return true;
    }

    LIBRG_INTERNAL b32 librg__space_contains(isize dims, zpl_aabb3 a, f32 *point) {
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
                ptr = (void *)zpl_malloc(sizeof(librg_ctx));
                zpl_zero_size(ptr, sizeof(librg_ctx));
                break;

            case LIBRG_POINTER_DATA:
                ptr = (void *)zpl_malloc(sizeof(librg_data));
                zpl_zero_size(ptr, sizeof(librg_data));
                break;

            case LIBRG_POINTER_EVENT:
                ptr = (void *)zpl_malloc(sizeof(librg_event));
                zpl_zero_size(ptr, sizeof(librg_event));
                break;
        }

        return ptr;
    }

    void librg_release_ptr(void *ptr) {
        librg_assert(ptr);
        zpl_mfree(ptr);
    }

    void librg_init(librg_ctx *ctx) {
        librg_dbg("[dbg] librg_init\n");

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
        ctx->entity.list = (librg_entity*)zpl_alloc(ctx->allocator, sizeof(librg_entity) * ctx->max_entities);
        for (librg_entity_id i = 0; i < ctx->max_entities; i++) {
            librg_entity blob = { i, 0 };
            ctx->entity.list[i] = blob;
        }

        zpl_array_init(ctx->entity.remove_queue, ctx->allocator);
        zpl_array_init(ctx->entity.add_control_queue, ctx->allocator);

        for (i8 i = 0; i < LIBRG_DATA_STREAMS_AMOUNT; ++i) {
            librg_data_init(&ctx->streams[i]);
        }

        // streamer
        zpl_aabb3 world = {0};
        world.centre    = zpl_vec3f(0, 0, 0);
        world.half_size = zpl_vec3f(ctx->world_size.x, ctx->world_size.y, ctx->world_size.z);
        u32 dimension   = ctx->world_size.z == 0.0f ? LIBRG_SPACE_2D : LIBRG_SPACE_3D;

        if (ctx->min_branch_size.x == -1.0f &&
            ctx->min_branch_size.y == -1.0f &&
            ctx->min_branch_size.z == -1.0f) {
            zpl_vec3 no_min_bounds = { 0 };
            ctx->min_branch_size = no_min_bounds;
        }

        librg__space_init(&ctx->world, ctx->allocator, dimension, world, ctx->min_branch_size, librg_option_get(LIBRG_MAX_ENTITIES_PER_BRANCH));

        #ifdef LIBRG_FEATURE_ENTITY_VISIBILITY
        librg_table_init(&ctx->entity.ignored, ctx->allocator);
        #endif

        // events
        librg_event_pool_init(&ctx->events, ctx->allocator);
        zpl_buffer_init(ctx->messages, ctx->allocator, librg_option_get(LIBRG_NETWORK_CAPACITY));

        // init timers
        zpl_array_init(ctx->timers, ctx->allocator);
        zpl_timer *tick_timer = zpl_timer_add(ctx->timers);
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

    void librg_free(librg_ctx *ctx) {
        librg_dbg("[dbg] librg_free\n");

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

        #ifdef LIBRG_FEATURE_ENTITY_VISIBILITY
        librg_table_destroy(&ctx->entity.ignored);
        #endif

        for (isize i = 0; i < LIBRG_DATA_STREAMS_AMOUNT; ++i) {
            librg_data_free(&ctx->streams[i]);
        }

        enet_deinitialize();
    }

    void librg_tick(librg_ctx *ctx) {
        zpl_timer_update(ctx->timers);

        if (!ctx->network.host) {
            return; /* occasion where we are not started network yet */
        }

        ENetEvent event;
        while (enet_host_service(ctx->network.host, &event, 0) > 0) {
            librg_message msg = {0}; {
                msg.ctx     = ctx;
                msg.data    = NULL;
                msg.peer    = event.peer;
                msg.packet  = event.packet;
            }

            switch (event.type) {
                case ENET_EVENT_TYPE_RECEIVE: {
                    librg_data data = {0};

                    data.rawptr = event.packet->data;
                    data.capacity = event.packet->dataLength;

                    if (!data.rawptr || data.capacity < sizeof(librg_message_id)) {
                        librg_assert(false);
                        librg_dbg("[dbg] corrupted packet in librg_tick, on receive\n");
                        continue;
                    }

                    // get curernt packet id
                    librg_message_id id = librg_data_rmid(&data);
                    f64 server_time = 0;

                    msg.id = id;

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
                        librg_dbg("[dbg] unknown message: %u\n", id);
                    }

                    enet_packet_destroy(event.packet);
                } break;
                case ENET_EVENT_TYPE_CONNECT:    ctx->messages[LIBRG_CONNECTION_INIT](&msg); break;
                case ENET_EVENT_TYPE_DISCONNECT: ctx->messages[LIBRG_CONNECTION_DISCONNECT](&msg); break;
                case ENET_EVENT_TYPE_DISCONNECT_TIMEOUT: ctx->messages[LIBRG_CONNECTION_DISCONNECT](&msg); break;
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
    librg_entity *librg_entity_create(librg_ctx *ctx, u32 type) {
        librg_assert(ctx);
        librg_assert(librg_is_server(ctx));
        librg_assert_msg(ctx->entity.count < ctx->max_entities, "reached max_entities limit");

        ++ctx->entity.count;

        if (ctx->entity.cursor >= (ctx->max_entities - 1) || ctx->max_entities == 0) {
            ctx->entity.cursor = 0;
        }

        for (; ctx->entity.cursor < ctx->max_entities; ++ctx->entity.cursor) {
            librg_entity *entity = &ctx->entity.list[ctx->entity.cursor]; librg_assert(entity);

            if (entity->flags & LIBRG_ENTITY_ALIVE) continue;

            entity->type               = type;
            entity->flags              = LIBRG_ENTITY_ALIVE;
            entity->position           = zpl_vec3f_zero();
            entity->stream_range       = librg_option_get(LIBRG_DEFAULT_STREAM_RANGE) * 1.0f;
            entity->stream_branch      = NULL;
            entity->control_generation = 0;

            return entity;
        }

        librg_assert_msg(false, "no entities to spawn");
        return NULL;
    }

    librg_entity *librg_entity_fetch(librg_ctx *ctx, librg_entity_id id) {
        if (librg_entity_valid(ctx, id))
            return &ctx->entity.list[id];

        return NULL;
    }

    void librg_entity_destroy(librg_ctx *ctx, librg_entity_id id) {
        librg_assert(librg_is_server(ctx));
        zpl_array_append(ctx->entity.remove_queue, id);
    }

    usize librg_entity_query(librg_ctx *ctx, librg_entity_id entity, librg_entity_id **out_entities) {
        librg_assert(ctx && out_entities);
        librg_assert(librg_is_server(ctx));
        librg_entity *blob = librg_entity_fetch(ctx, entity);

        if (!(blob->flags & LIBRG_ENTITY_QUERIED)) {
            blob->flags |= LIBRG_ENTITY_QUERIED;
            zpl_array_init(blob->last_query, ctx->allocator);
        }

        // reset array to 0
        zpl_array_count(blob->last_query) = 0;

        // add all currently streamed entities automatically
        librg_entity_iteratex(ctx, LIBRG_ENTITY_CONTROLLED, librg_lambda(controlled), {
            if (blob->client_peer && librg_entity_control_get(ctx, controlled) == blob->client_peer) {
                zpl_array_append(blob->last_query, controlled);
            }
        });

        zpl_aabb3 search_bounds; {
            search_bounds.centre    = blob->position;
            search_bounds.half_size = zpl_vec3f(blob->stream_range, blob->stream_range, blob->stream_range);
        };

        librg__world_entity_query(ctx, entity, &ctx->world, search_bounds, zpl_array_count(blob->last_query), &blob->last_query);
        *out_entities = blob->last_query;

        return zpl_array_count(blob->last_query);
    }

    b32 librg_entity_valid(librg_ctx *ctx, librg_entity_id id) {
        librg_assert(ctx);
        return (id < ctx->max_entities && (ctx->entity.list[id].flags & LIBRG_ENTITY_ALIVE));
    }

    librg_entity *librg_entity_find(librg_ctx *ctx, librg_peer *peer) {
        librg_assert(ctx && librg_is_server(ctx) && peer);
        librg_entity_id *id = librg_table_get(&ctx->network.connected_peers, (u64)peer);

        if (id) {
            return librg_entity_fetch(ctx, *id);
        }

        return NULL;
    }

    void librg_entity_iterate(librg_ctx *ctx, u64 flags, librg_entity_cb callback) {
        librg_entity_iteratex(ctx, flags, librg_lambda(entity), { callback(ctx, librg_entity_fetch(ctx, entity)); });
    }

    // @deprected
    u32 librg_entity_type(librg_ctx *ctx, librg_entity_id id) {
        librg_assert(librg_entity_valid(ctx, id));
        return ctx->entity.list[id].type;
    }

    #ifdef LIBRG_FEATURE_ENTITY_VISIBILITY
        void librg_entity_visibility_set(librg_ctx *ctx, librg_entity_id entity, b32 state) {
            librg_assert(librg_is_server(ctx) && librg_entity_valid(ctx, entity));
            librg_table_set(&ctx->entity.ignored, entity, (u32)!state);
        }

        void librg_entity_visibility_set_for(librg_ctx *ctx, librg_entity_id entity, librg_entity_id target, b32 state) {
            librg_assert(librg_is_server(ctx) && librg_entity_valid(ctx, entity));
            librg_entity *blob = librg_entity_fetch(ctx, entity);

            if (!(blob->flags & LIBRG_ENTITY_IGNORING)) {
                blob->flags |= LIBRG_ENTITY_IGNORING;
                librg_table_init(&blob->ignored, ctx->allocator);
            }

            librg_table_set(&blob->ignored, target, (u32)!state);
        }

        b32 librg_entity_visibility_get(librg_ctx *ctx, librg_entity_id entity) {
            librg_assert(librg_is_server(ctx) && librg_entity_valid(ctx, entity));
            u32 *ignored = librg_table_get(&ctx->entity.ignored, entity);
            return !(ignored && *ignored);
        }

        b32 librg_entity_visibility_get_for(librg_ctx *ctx, librg_entity_id entity, librg_entity_id target) {
            librg_assert(librg_is_server(ctx));
            librg_entity *blob = librg_entity_fetch(ctx, entity);

            if (!(blob->flags & LIBRG_ENTITY_IGNORING)) {
                return true;
            }

            u32 *ignored = librg_table_get(&blob->ignored, target);
            return !(ignored && *ignored);
        }
    #endif

    #ifdef LIBRG_FEATURE_VIRTUAL_WORLDS
        u32 librg_entity_world_get(struct librg_ctx *ctx, librg_entity_id entity_id) {
            librg_assert(ctx);
            librg_entity *entity = librg_entity_fetch(ctx, entity_id);

            if (entity) {
                return entity->virual_world;
            }

            return 0;
        }

        void librg_entity_world_set(struct librg_ctx *ctx, librg_entity_id entity_id, u32 world) {
            librg_assert(ctx);
            librg_entity *entity = librg_entity_fetch(ctx, entity_id);

            if (entity) {
                entity->virual_world = world;
            }
        }
    #endif

    void librg_entity_control_set(librg_ctx *ctx, librg_entity_id entity, librg_peer *peer) {
        librg_assert(ctx && peer && librg_entity_valid(ctx, entity));
        librg_assert(librg_is_server(ctx));
        librg_entity *blob = librg_entity_fetch(ctx, entity);

        // replace current entity owner
        if (blob->flags & LIBRG_ENTITY_CONTROLLED) {
            if (blob->control_peer == peer) {
                return;
            }

            librg_send_to(ctx, LIBRG_CLIENT_STREAMER_REMOVE, blob->control_peer, librg_lambda(data), {
                librg_data_went(&data, entity);
            });
        }

        // attach new entity owner
        blob->flags |= (LIBRG_ENTITY_CONTROLLED | LIBRG_ENTITY_CONTROL_REQUESTED);
        blob->control_peer = peer;

        // main compare/validation thingy
        blob->control_generation++;

        librg_message *msg = (librg_message *)zpl_alloc(ctx->allocator, sizeof(librg_message)); {
            librg_message_id id = LIBRG_CLIENT_STREAMER_ADD;

            msg->id     = id;
            msg->peer   = peer;
            msg->packet = enet_packet_create_offset(
                &entity, sizeof(librg_entity_id), sizeof(librg_message_id), ENET_PACKET_FLAG_RELIABLE
            );

            zpl_memcopy(msg->packet->data, &id, sizeof(librg_message_id));
        }

        zpl_array_append(ctx->entity.add_control_queue, msg);
    }

    librg_peer *librg_entity_control_get(librg_ctx *ctx, librg_entity_id entity) {
        librg_assert(ctx);
        librg_assert(librg_is_server(ctx));
        librg_entity *blob = librg_entity_fetch(ctx, entity);
        return (blob && blob->flags & LIBRG_ENTITY_CONTROLLED) ? blob->control_peer : NULL;
    }

    void librg_entity_control_remove(librg_ctx *ctx, librg_entity_id entity) {
        librg_assert(ctx && librg_entity_valid(ctx, entity));
        librg_assert(librg_is_server(ctx));
        librg_entity *blob = librg_entity_fetch(ctx, entity);

        // some minor validations
        if (!(blob->flags & LIBRG_ENTITY_CONTROLLED)) { return; }

        librg_send_to(ctx, LIBRG_CLIENT_STREAMER_REMOVE, blob->control_peer, librg_lambda(data), {
            librg_data_went(&data, entity);
        });

        blob->flags &= ~LIBRG_ENTITY_CONTROLLED;
        blob->control_peer = NULL;
    }

    void librg_entity_control_ignore_next_update(librg_ctx *ctx, librg_entity_id entity) {
        librg_assert(ctx && librg_entity_valid(ctx, entity));
        librg_assert(librg_is_server(ctx));
        librg_entity *blob = librg_entity_fetch(ctx, entity);

        // some minor validations
        if (!(blob->flags & LIBRG_ENTITY_CONTROLLED)) { return; }
        if (blob->flags & LIBRG_ENTITY_CONTROL_REQUESTED) { return; }

        // and super complex algo to reset the peer, and update the control_generation
        librg_peer *client_peer = librg_entity_control_get(ctx, entity);
        librg_entity_control_remove(ctx, entity);
        librg_entity_control_set(ctx, entity, client_peer);
    }

#endif

// =======================================================================//
// !
// ! Events
// !
// =======================================================================//

#if 1
    u64 librg_event_add(librg_ctx *ctx, u64 id, librg_event_cb callback) {
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

    void librg_event_trigger(librg_ctx *ctx, u64 id, librg_event *event) {
        librg_assert(event);

        event->id = id;
        event->ctx = ctx;

        librg_event_block *block = librg_event_pool_get(&ctx->events, id);
        if (!block) return;

        for (isize i = 0; i < zpl_array_count(*block) && !(event->flags & LIBRG_EVENT_REJECTED); ++i) {
            (*block)[i](event);
        }
    }

    void librg_event_remove(librg_ctx *ctx, u64 id, u64 index) {
        librg_assert(ctx);
        librg_event_block *block = librg_event_pool_get(&ctx->events, id);

        if (block) {
            zpl_array_remove_at(*block, (isize)index);
        }
    }

    void librg_event_reject(librg_event *event) {
        librg_assert(event);
        event->flags = (event->flags | LIBRG_EVENT_REJECTED);
    }

    b32 librg_event_rejectable(librg_event *event) {
        librg_assert(event);
        return (event->flags & LIBRG_EVENT_REJECTABLE);
    }

    b32 librg_event_succeeded(librg_event *event) {
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
    void librg_data_init_size(librg_data *data, usize size) {
        librg_assert(data);

        data->capacity  = size;
        data->read_pos  = 0;
        data->write_pos = 0;
        data->allocator = zpl_heap_allocator();
        data->rawptr    = zpl_alloc(data->allocator, size);
    }

    void librg_data_init(librg_data *data) {
        librg_assert(data);
        librg_data_init_size(data, librg_option_get(LIBRG_DEFAULT_DATA_SIZE));
    }

    librg_data *librg_data_init_new() {
        librg_data *data = (librg_data *)zpl_malloc(sizeof(librg_data));
        librg_data_init(data);
        return data;
    }

    void librg_data_free(librg_data *data) {
        librg_assert(data && data->rawptr);
        zpl_free(data->allocator, data->rawptr);
    }

    void librg_data_reset(librg_data *data) {
        librg_data_set_wpos(data, 0);
        librg_data_set_rpos(data, 0);
    }

    void librg_data_grow(librg_data *data, usize min_size) {
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

    usize librg_data_capacity(librg_data *data) {
        librg_assert(data); return data->capacity;
    }

    usize librg_data_get_rpos(librg_data *data) {
        librg_assert(data); return data->read_pos;
    }

    usize librg_data_get_wpos(librg_data *data) {
        librg_assert(data); return data->write_pos;
    }

    void librg_data_set_rpos(librg_data *data, usize position) {
        librg_assert(data); data->read_pos = position;
    }

    void librg_data_set_wpos(librg_data *data, usize position) {
        librg_assert(data); data->write_pos = position;
    }

    void librg_data_rptr_at(librg_data *data, void *ptr, usize size, isize position) {
        librg_assert(data && data->rawptr && ptr);

        librg_assert_msg(position + size <= librg_data_capacity(data),
            "librg_data: trying to read from outside of the bounds");

        zpl_memcopy(ptr, (char *)data->rawptr + position, size);
    }

    void librg_data_wptr_at(librg_data *data, void *ptr, usize size, isize position) {
        librg_assert(data && data->rawptr && ptr);

        if (position + size > librg_data_capacity(data)) {
            librg_data_grow(data, librg_data_capacity(data) + size + position);
        }

        zpl_memcopy((char *)data->rawptr + position, ptr, size);
    }

    void librg_data_rptr(librg_data *data, void *ptr, usize size) {
        librg_assert(data && data->rawptr && ptr);
        librg_data_rptr_at(data, ptr, size, librg_data_get_rpos(data));
        data->read_pos += size;
    }

    void librg_data_wptr(librg_data *data, void *ptr, usize size) {
        librg_assert(data && data->rawptr && ptr);
        librg_data_wptr_at(data, ptr, size, librg_data_get_wpos(data));
        data->write_pos += size;
    }

    /**
     * Value writers and readers
     */

    #define LIBRG_GEN_DATA_READWRITE(TYPE) \
        TYPE ZPL_JOIN2(librg_data_r,TYPE)(librg_data *data) { \
            TYPE value; librg_data_rptr(data, &value, sizeof(value)); return value; \
        } \
        void ZPL_JOIN2(librg_data_w,TYPE)(librg_data *data, TYPE value) { \
            librg_data_wptr(data, &value, sizeof(value)); \
        } \
        TYPE ZPL_JOIN3(librg_data_r,TYPE,_at)(librg_data *data, isize position) { \
            TYPE value; librg_data_rptr_at(data, &value, sizeof(value), position); return value; \
        } \
        void ZPL_JOIN3(librg_data_w,TYPE,_at)(librg_data *data, TYPE value, isize position) { \
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

    /**
     * Special method for internal packet-safe reading
     * @param  TYPE variable type
     * @param  VAR  variable name
     * @param  DATA librg_data ptr
     */
    #define librg_data_read_safe(TYPE, VAR, DATA) \
        TYPE VAR = 0; \
        if ((librg_data_get_rpos(DATA) + sizeof(TYPE)) <= librg_data_capacity(DATA)) { \
            VAR = ZPL_JOIN2(librg_data_r,TYPE)(DATA); \
        } else { \
            librg_dbg("[dbg] corrupted packet in method (%s::%s) at line: %d\n", _LIBRG_METHOD, "librg_data_r"#TYPE, __LINE__); \
            return; \
        }

#endif

// =======================================================================//
// !
// ! Network
// !
// =======================================================================//

#if 1
    b32 librg_is_connected(librg_ctx *ctx) {
        return ctx->network.peer && ctx->network.peer->state == ENET_PEER_STATE_CONNECTED;
    }

    b32 librg_is_server(librg_ctx *ctx) {
        return ctx->mode == LIBRG_MODE_SERVER;
    }

    b32 librg_is_client(librg_ctx *ctx) {
        return ctx->mode == LIBRG_MODE_CLIENT;
    }

    void librg_network_start(librg_ctx *ctx, librg_address addr) {
        librg_dbg("[dbg] librg_network_start\n");
        librg_table_init(&ctx->network.connected_peers, ctx->allocator);

        ENetAddress address = {0};
        if (librg_is_server(ctx)) {

            if (addr.host && zpl_strcmp(addr.host, "localhost") != 0) {
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
            const char *ipv6lclhst = "::1";

            if (!addr.host || zpl_strcmp(addr.host, "localhost") == 0) {
                addr.host = (char *)ipv6lclhst;
            }

            address.port = addr.port;
            enet_address_set_host(&address, addr.host);

            // setup client host
            // TODO: add override for bandwidth
            ctx->network.host = enet_host_create(NULL, 1, librg_option_get(LIBRG_NETWORK_CHANNELS), 0, 0);
            librg_assert_msg(ctx->network.host, "could not start client");

            // create peer connecting to server
            librg_dbg("[dbg] connecting to server %s:%u\n", addr.host, addr.port);
            ctx->network.peer = enet_host_connect(ctx->network.host, &address, librg_option_get(LIBRG_NETWORK_CHANNELS), 0);
            librg_assert_msg(ctx->network.peer, "could not setup peer for provided address");
        }

        ctx->network.last_address = addr;
    }

    void librg_network_stop(librg_ctx *ctx) {
        librg_dbg("[dbg] librg_network_stop\n");

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
                librg_event event = {0}; {
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

    void librg_network_kick(librg_ctx *ctx, librg_peer *peer) {
        librg_assert(ctx && peer);
        enet_peer_disconnect_later(peer, 0);
    }

    void librg_network_add(librg_ctx *ctx, librg_message_id id, librg_message_cb callback) {
        ctx->messages[id] = callback;
    }

    void librg_network_remove(librg_ctx *ctx, librg_message_id id) {
        ctx->messages[id] = NULL;
    }

    void librg_message_sendex(struct librg_ctx *ctx, librg_message_id id, librg_peer *target, librg_peer *except, u16 channel, b8 reliable, void *data, usize size) {
        librg_packet *packet = enet_packet_create_offset(
            data, size, sizeof(librg_message_id), reliable ? ENET_PACKET_FLAG_RELIABLE : 0
        );

        // write id at the beginning
        zpl_memcopy(packet->data, &id, sizeof(librg_message_id));
        librg_assert(channel <= librg_option_get(LIBRG_NETWORK_CHANNELS));

        if (target) {
            enet_peer_send(target, channel, packet);
        } else {
            ENetPeer *currentPeer;

            for (currentPeer = ctx->network.host->peers; currentPeer < &ctx->network.host->peers[ctx->network.host->peerCount]; ++currentPeer) {
                if (currentPeer->state != ENET_PEER_STATE_CONNECTED) {
                    continue;
                }

                if (currentPeer == except) {
                    continue;
                }

                /* TODO: add a skip if not peer's entity is not 'alive' */

                enet_peer_send(currentPeer, channel, packet);
            }
        }

        if (packet->referenceCount == 0) {
            enet_packet_destroy(packet);
        }
    }

    void librg_message_sendex_instream(struct librg_ctx *ctx, librg_message_id id, librg_entity_id entity_id, librg_peer *except, u16 channel, b8 reliable, void *data, usize size) {
        librg_packet *packet = enet_packet_create_offset(
            data, size, sizeof(librg_message_id), reliable ? ENET_PACKET_FLAG_RELIABLE : 0
        );

        // write id at the beginning
        zpl_memcopy(packet->data, &id, sizeof(librg_message_id));
        librg_assert(channel <= librg_option_get(LIBRG_NETWORK_CHANNELS));

        zpl_array(librg_entity_id) queue;
        usize count = librg_entity_query(ctx, entity_id, &queue);

        for (isize i = 0; i < count; i++) {
            librg_entity_id target = queue[i];

            librg_entity *blob = librg_entity_fetch(ctx, target);
            if (!(blob->flags & LIBRG_ENTITY_CLIENT)) continue;

            librg_peer *peer = blob->client_peer;
            librg_assert(peer);

            if (peer == except) {
                continue;
            }

            enet_peer_send(peer, channel, packet);
        }

        if (packet->referenceCount == 0) {
            enet_packet_destroy(packet);
        }
    }

    librg_inline void librg_message_send_all(librg_ctx *ctx, librg_message_id id, void *data, usize size) {
        librg_message_sendex(ctx, id, NULL, NULL, librg_option_get(LIBRG_NETWORK_MESSAGE_CHANNEL), true, data, size);
    }

    librg_inline void librg_message_send_to(librg_ctx *ctx, librg_message_id id, librg_peer *target, void *data, usize size) {
        librg_message_sendex(ctx, id, target, NULL, librg_option_get(LIBRG_NETWORK_MESSAGE_CHANNEL), true, data, size);
    }

    librg_inline void librg_message_send_except(librg_ctx *ctx, librg_message_id id, librg_peer *except, void *data, usize size) {
        librg_message_sendex(ctx, id, NULL, except, librg_option_get(LIBRG_NETWORK_MESSAGE_CHANNEL), true, data, size);
    }

    librg_inline void librg_message_send_instream(librg_ctx *ctx, librg_message_id id, librg_entity_id entity_id, void *data, usize size) {
        librg_message_sendex_instream(ctx, id, entity_id, NULL, librg_option_get(LIBRG_NETWORK_MESSAGE_CHANNEL), true, data, size);
    }

    librg_inline void librg_message_send_instream_except(librg_ctx *ctx, librg_message_id id, librg_entity_id entity_id, librg_peer *except, void *data, usize size) {
        librg_message_sendex_instream(ctx, id, entity_id, except, librg_option_get(LIBRG_NETWORK_MESSAGE_CHANNEL), true, data, size);
    }
#endif

// =======================================================================//
// !
// ! Helper methods
// !
// =======================================================================//

#if 1
    f64 librg_time_now(librg_ctx *ctx) {
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
            deviation += zpl_pow(values[i] - mean, 2);
        }

        return zpl_sqrt(deviation / (f64)size);
    }
#endif

// =======================================================================//
// !
// ! Time-syncer
// !
// =======================================================================//

#if 1
    void librg__timesync_tick(void *usrptr) {
        librg_send((librg_ctx *)usrptr, LIBRG_CONNECTION_TIMESYNC, data, {
            librg_data_wf64(&data, zpl_time_now());
        });
    }

    void librg__timesync_start(librg_ctx *ctx) {
        if (librg_is_client(ctx)) {
            zpl_zero_item(ctx->timesync.history);

            ctx->timesync.median        = 0.0;
            ctx->timesync.offset_time   = 0.0;
            ctx->timesync.start_time    = 0.0;

            zpl_timer_set(ctx->timesync.timer, 5.0, -1, librg__timesync_tick);
            zpl_timer_start(ctx->timesync.timer, 0);
        }
    }

    void librg__timesync_stop(librg_ctx *ctx) {
        if (librg_is_client(ctx) && ctx->timesync.timer && ctx->timesync.timer->enabled) {
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
    void librg__buffer_init(librg_ctx *ctx, usize size) {
        if (librg_is_server(ctx)) return;

        zpl_ring_librg_snapshot_init(&ctx->buffer, ctx->allocator, size);
        ctx->buffer_size = size;

        zpl_timer_set(ctx->buffer_timer, ctx->timesync.server_delay, -1, librg__buffer_tick);
        zpl_timer_start(ctx->buffer_timer, 0);
    }

    void librg__buffer_free(librg_ctx *ctx) {
        if (librg_is_server(ctx)) return;
        if (!ctx->buffer_timer || !ctx->buffer_timer->enabled) return;
        zpl_timer_stop(ctx->buffer_timer);

        for (isize i = 0; i < ctx->buffer.capacity; ++i) {
            zpl_mfree(ctx->buffer.buf[i].data);
        }

        zpl_ring_librg_snapshot_free(&ctx->buffer);
    }

    void librg__buffer_push(librg_ctx *ctx, f64 time, librg_peer *peer, void *data, usize size) {
        librg_snapshot snap = { 0 };

        snap.time = time;
        snap.peer = peer;
        snap.size = size;

        snap.data = zpl_alloc_copy(zpl_heap(), data, size);
        librg_assert(data && snap.data);
        zpl_ring_librg_snapshot_append(&ctx->buffer, snap);
    }

    void librg__buffer_tick(void *usrptr) {
        librg_ctx *ctx = (librg_ctx *)usrptr;
        if (!zpl_ring_librg_snapshot_full(&ctx->buffer)) {
            return;
        }

        librg_snapshot *snap = zpl_ring_librg_snapshot_get(&ctx->buffer);
        f64 time_diff = ((ctx->buffer_size + 1) * ctx->timesync.server_delay);

        // if current update if too old, just skip it, and call next one
        if (snap->time < (librg_time_now(ctx) - time_diff)) {
            librg_dbg("[dbg] librg__buffer_tick: dropping old update packet\n");
            zpl_mfree(snap->data);
            librg__buffer_tick((void *)ctx);
            return;
        }

        librg_data data = {0}; {
            data.rawptr   = snap->data;
            data.capacity = snap->size;
        }

        // skip our message id length and timestamp lenth
        // TODO: copy not all the stuff but only needed (skiping those two)
        librg_data_set_rpos(&data, sizeof(librg_message_id) + sizeof(f64));

        librg_message msg = {0}; {
            msg.id      = LIBRG_ENTITY_UPDATE;
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
    LIBRG_INTERNAL void librg__callback_connection_init(librg_message *msg) {
        #define _LIBRG_METHOD "librg__callback_connection_init"
        librg_dbg("[dbg] %s\n", _LIBRG_METHOD);

        #if defined(LIBRG_DEBUG)
        char my_host[16];

        enet_address_get_host_ip(&msg->peer->address, my_host, 16);
        librg_dbg("[dbg] %s: a new connection attempt at %s:%u.\n", _LIBRG_METHOD, my_host, msg->peer->address.port);
        #endif

        if (librg_is_client(msg->ctx)) {
            librg_data data;
            librg_data_init_size(&data, sizeof(u16) * 3);

            librg_data_wu32(&data, librg_option_get(LIBRG_PLATFORM_ID));
            librg_data_wu32(&data, librg_option_get(LIBRG_PLATFORM_BUILD));
            librg_data_wu32(&data, librg_option_get(LIBRG_PLATFORM_PROTOCOL));
            librg_data_wf64(&data, zpl_time_now());

            librg_event event = {0}; {
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

        #undef _LIBRG_METHOD
    }

    /* Execution side: SERVER */
    LIBRG_INTERNAL void librg__callback_connection_request(librg_message *msg) {
        #define _LIBRG_METHOD "librg__callback_connection_request"
        librg_dbg("[dbg] %s\n", _LIBRG_METHOD);

        librg_data_read_safe(u32, platform_id, msg->data);
        librg_data_read_safe(u32, platform_build, msg->data);
        librg_data_read_safe(u32, platform_protocol, msg->data);
        librg_data_read_safe(f64, client_time, msg->data);

        b32 blocked = (platform_id != librg_option_get(LIBRG_PLATFORM_ID) || platform_protocol != librg_option_get(LIBRG_PLATFORM_PROTOCOL));

        if (platform_build != librg_option_get(LIBRG_PLATFORM_BUILD)) {
            librg_dbg("[dbg] NOTICE: librg platform build mismatch client %u, server: %u\n", platform_build, librg_option_get(LIBRG_PLATFORM_BUILD));
        }

        if (blocked) {
            librg_dbg("[dbg] BLOCKED: our platform: %d %d, their platform: %d %d\n",
                librg_option_get(LIBRG_PLATFORM_ID),
                librg_option_get(LIBRG_PLATFORM_PROTOCOL),
                platform_id, platform_protocol
            );
        }

        librg_event event = {0}; {
            event.peer  = msg->peer;
            event.data  = msg->data;
            event.flags = (LIBRG_EVENT_REJECTABLE | LIBRG_EVENT_REMOTE);
        }

        librg_event_trigger(msg->ctx, LIBRG_CONNECTION_REQUEST, &event);

        if (librg_event_succeeded(&event) && !blocked) {
            librg_entity *entity = librg_entity_create(msg->ctx, librg_option_get(LIBRG_DEFAULT_CLIENT_TYPE));

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
            librg_dbg("[dbg] librg__connection_refuse\n");
            librg_message_send_to(msg->ctx, LIBRG_CONNECTION_REFUSE, msg->peer, NULL, 0);

            event.data   = NULL;
            event.flags  = LIBRG_EVENT_LOCAL;

            librg_event_trigger(msg->ctx, LIBRG_CONNECTION_REFUSE, &event);
        }

        #undef _LIBRG_METHOD
    }

    /* Execution side: CLIENT */
    LIBRG_INTERNAL void librg__callback_connection_refuse(librg_message *msg) {
        librg_dbg("[dbg] librg__connection_refuse\n");
        LIBRG_MESSAGE_TO_EVENT(event, msg);
        librg_event_trigger(msg->ctx, LIBRG_CONNECTION_REFUSE, &event);
    }

    /* Execution side: CLIENT */
    LIBRG_INTERNAL void librg__callback_connection_accept(librg_message *msg) {
        librg_dbg("[dbg] librg__connection_accept\n");

        f32 server_delay = librg_data_rf32(msg->data);
        f64 client_diff  = (zpl_time_now() - librg_data_rf64(msg->data)) / 2.0;
        f64 server_time  = librg_data_rf64(msg->data);

        librg_entity_id entity = librg_data_rent(msg->data);
        librg_entity *blob = &msg->ctx->entity.list[entity];

        msg->ctx->entity.count++;

        blob->type      = librg_option_get(LIBRG_DEFAULT_CLIENT_TYPE);
        blob->flags     = (LIBRG_ENTITY_ALIVE | LIBRG_ENTITY_CLIENT);
        blob->position  = zpl_vec3f_zero();

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
    LIBRG_INTERNAL void librg__callback_connection_disconnect(librg_message *msg) {
        librg_dbg("[dbg] librg__connection_disconnect\n");

        if (!msg->ctx->network.connected_peers.hashes) {
            librg_event event = {0}; {
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
            librg_entity *blob = librg_entity_fetch(msg->ctx, *entity);
            librg_event event = {0}; {
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
    void librg__callback_connection_timesync(librg_message *msg) {
        #define _LIBRG_METHOD "librg__callback_connection_timesync"

        if (librg_is_server(msg->ctx)) {
            librg_data_read_safe(f64, client_time, msg->data);
            f64 server_time = librg_time_now(msg->ctx);

            librg_send_to(msg->ctx, LIBRG_CONNECTION_TIMESYNC, msg->peer, librg_lambda(data), {
                librg_data_wf64(&data, client_time);
                librg_data_wf64(&data, server_time);
            });
        } else {
            librg_ctx *ctx = msg->ctx;

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

        #undef _LIBRG_METHOD
    }

    /* Execution side: CLIENT */
    LIBRG_INTERNAL void librg__callback_entity_create(librg_message *msg) {
        u32 query_size = librg_data_ru32(msg->data);

        for (usize i = 0; i < query_size; ++i) {
            librg_entity_id entity = librg_data_rent(msg->data);
            u32 entity_type        = librg_data_ru32(msg->data);
            u8 control_generation  = librg_data_ru8(msg->data);

            zpl_vec3 position;
            librg_data_rptr(msg->data, &position, sizeof(zpl_vec3));

            // Create new entity on client side
            librg_entity *blob = &msg->ctx->entity.list[entity]; librg_assert(blob); {
                blob->type               = entity_type;
                blob->flags              = LIBRG_ENTITY_ALIVE;
                blob->position           = position;
                blob->control_generation = control_generation;
            };

            msg->ctx->entity.count++;

            LIBRG_MESSAGE_TO_EVENT(event, msg); event.entity = blob;
            librg_event_trigger(msg->ctx, LIBRG_ENTITY_CREATE, &event);
        }

        u32 remove_size = librg_data_ru32(msg->data);

        for (usize i = 0; i < remove_size; ++i) {
            librg_entity_id entity = librg_data_rent(msg->data);

            if (librg_entity_valid(msg->ctx, entity)) {
                LIBRG_MESSAGE_TO_EVENT(event, msg); {
                    event.entity = librg_entity_fetch(msg->ctx, entity);
                };

                librg_event_trigger(msg->ctx, LIBRG_ENTITY_REMOVE, &event);
                librg__world_entity_destroy(msg->ctx, entity);
            }
            else {
                librg_dbg("[dbg] unexpected entity %u on remove\n", entity);
            }
        }
    }

    /* Execution side: CLIENT */
    LIBRG_INTERNAL void librg__callback_entity_update(librg_message *msg) {
        u32 query_size = librg_data_ru32(msg->data);

        for (usize i = 0; i < query_size; ++i) {
            librg_entity_id entity = librg_data_rent(msg->data);

            zpl_vec3 position;
            librg_data_rptr(msg->data, &position, sizeof(position));

            if (!librg_entity_valid(msg->ctx, entity)) {
                continue;
            }

            librg_entity *blob = librg_entity_fetch(msg->ctx, entity);
            blob->position = position;

            LIBRG_MESSAGE_TO_EVENT(event, msg); event.entity = blob;
            librg_event_trigger(msg->ctx, LIBRG_ENTITY_UPDATE, &event);
        }
    }

    /* Execution side: CLIENT */
    LIBRG_INTERNAL void librg__callback_entity_client_streamer_add(librg_message *msg) {
        librg_entity_id entity = librg_data_rent(msg->data);

        if (!librg_entity_valid(msg->ctx, entity)) {
            librg_dbg("[dbg] trying to add unknown entity to clientstream!\n");
            return;
        }

        librg_entity *blob = librg_entity_fetch(msg->ctx, entity);

        if (!(blob->flags & LIBRG_ENTITY_CONTROLLED)) {
            blob->flags |= LIBRG_ENTITY_CONTROLLED;
            blob->control_generation++;

            LIBRG_MESSAGE_TO_EVENT(event, msg); event.entity = blob;
            librg_event_trigger(msg->ctx, LIBRG_CLIENT_STREAMER_ADD, &event);
        }
    }

    /* Execution side: SERVER */
    LIBRG_INTERNAL void librg__callback_entity_client_streamer_update(librg_message *msg) {
        #define _LIBRG_METHOD "librg__callback_entity_client_streamer_update"
        librg_data_read_safe(u32, amount, msg->data);

        for (usize i = 0; i < amount; i++) {
            librg_data_read_safe(u32, entity, msg->data);
            librg_data_read_safe(u8, control_generation, msg->data);
            librg_data_read_safe(u32, size, msg->data);

            if (librg_data_capacity(msg->data) < librg_data_get_rpos(msg->data) + size ||
                librg_data_capacity(msg->data) < librg_data_get_rpos(msg->data) + sizeof(zpl_vec3)) {
                librg_dbg("[dbg] invalid packet size on client streamer update\n");
                return;
            }

            #define LIBRG_LOCAL_ASSERT(cond, alert) \
                if (!(cond)) { \
                    librg_data_set_rpos(msg->data, librg_data_get_rpos(msg->data) + size); \
                    librg_dbg(alert); continue; \
                }

            if (!librg_entity_valid(msg->ctx, entity)) {
                librg_dbg("[dbg] invalid entity on client streamer update\n");
                librg_data_set_rpos(msg->data, librg_data_get_rpos(msg->data) + size);
                continue;
            }

            LIBRG_LOCAL_ASSERT(librg_entity_valid(msg->ctx, entity), "[dbg] client_streamer_update: invalid entity\n");
            librg_entity *blob = librg_entity_fetch(msg->ctx, entity);

            LIBRG_LOCAL_ASSERT(blob->flags & LIBRG_ENTITY_CONTROLLED, "[dbg] client_streamer_update: entity has no LIBRG_ENTITY_CONTROLLED flag\n");
            LIBRG_LOCAL_ASSERT(blob->control_peer == msg->peer, "[dbg] client_streamer_update: entity controller peer and msg->peer are different\n");
            LIBRG_LOCAL_ASSERT(!(blob->flags & LIBRG_ENTITY_CONTROL_REQUESTED), "[dbg] client_streamer_update: entity still has LIBRG_ENTITY_CONTROL_REQUESTED flag\n");
            LIBRG_LOCAL_ASSERT(blob->control_generation == control_generation, "[dbg] client_streamer_update: control_generation is different\n");

            #undef LIBRG_LOCAL_ASSERT

            LIBRG_MESSAGE_TO_EVENT(event, msg); event.entity = blob;
            librg_event_trigger(msg->ctx, LIBRG_CLIENT_STREAMER_UPDATE, &event);
            librg_data_rptr(msg->data, &blob->position, sizeof(blob->position));
        }

        #undef _LIBRG_METHOD
    }

    /* Execution side: CLIENT */
    LIBRG_INTERNAL void librg__callback_entity_client_streamer_remove(librg_message *msg) {
        librg_entity_id entity = librg_data_rent(msg->data);

        if (!librg_entity_valid(msg->ctx, entity)) {
            librg_dbg("[dbg] trying to remove unknown entity from clientstream!\n");
            return;
        }

        librg_entity *blob = librg_entity_fetch(msg->ctx, entity);

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
    librg_internal void librg__execute_client_update(librg_ctx *ctx) {
        u32 amount = 0;
        librg_data data;
        librg_data_init(&data);

        librg_data_wmid(&data, LIBRG_CLIENT_STREAMER_UPDATE);
        librg_data_wu32(&data, 0); // amount of entities to be sent (updates)

        librg_entity_iteratex(ctx, LIBRG_ENTITY_CONTROLLED, librg_lambda(entity), {
            librg_entity *blob = librg_entity_fetch(ctx, entity);

            librg_data subdata = {0};
            librg_data_init(&subdata);

            librg_event event = {0}; {
                event.entity = blob;
                event.data  = &subdata;
                event.flags = (LIBRG_EVENT_REJECTABLE | LIBRG_EVENT_LOCAL);
            }

            librg_event_trigger(ctx, LIBRG_CLIENT_STREAMER_UPDATE, &event);

            // check if user rejected the event
            if (!(event.flags & LIBRG_EVENT_REJECTED)) {
                librg_data_wptr(&subdata, &blob->position, sizeof(zpl_vec3));
                librg_data_went(&data, entity);
                librg_data_wu8(&data, blob->control_generation);
                librg_data_wu32(&data, librg_data_get_wpos(&subdata));

                // write sub-bitstream to main bitstream
                librg_data_wptr(&data, subdata.rawptr, librg_data_get_wpos(&subdata));

                amount++;
            }

            librg_data_free(&subdata);
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
    librg_internal void librg__execute_server_entity_update(librg_ctx *ctx) {
        librg_assert(ctx);

        // use preallocated data stream objects
        librg_data *reliable   = &ctx->stream_upd_reliable;
        librg_data *unreliable = &ctx->stream_upd_unreliable;

        for (usize j = 0; j < ctx->max_entities; j++) {
            librg_entity *blob = &ctx->entity.list[j];
            if (!(blob->flags & LIBRG_ENTITY_CLIENT)) continue;

            // assume that entity is valid, having the client
            librg_entity_id player = j;

            // get old, and preapre new snapshot handlers
            librg_table *last_snapshot = &blob->last_snapshot;
            librg_table next_snapshot = { 0 };
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
            for (usize i = 0; i < queue_count; ++i) {
                librg_entity_id entity = cast(librg_entity_id)queue[i];

                // fetch value of entity in the last snapshot
                u32 *existed_in_last = librg_table_get(last_snapshot, entity);
                librg_entity *eblob  = librg_entity_fetch(ctx, entity);

                // write create
                if (entity != player && !existed_in_last) {
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
                    librg_data_wu8(reliable,  eblob->control_generation);
                    librg_data_wptr(reliable, &eblob->position, sizeof(eblob->position));

                    // request custom data from user
                    librg_event event = {0}; {
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
                    if (eblob && (eblob->flags & LIBRG_ENTITY_CONTROLLED) && (!(eblob->flags & LIBRG_ENTITY_CONTROL_REQUESTED)) && eblob->control_peer == blob->client_peer) {
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
                        librg_event event = { 0 }; {
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
                librg_event event = { 0 }; {
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

            // send the data, via differnt channels and reliability setting
            if (librg_data_get_wpos(reliable) > (sizeof(librg_message_id) + sizeof(u32) * 2)) {
                enet_peer_send(blob->client_peer, librg_option_get(LIBRG_NETWORK_PRIMARY_CHANNEL),
                    enet_packet_create(reliable->rawptr, librg_data_get_wpos(reliable), ENET_PACKET_FLAG_RELIABLE)
                );
            }

            enet_peer_send(blob->client_peer, librg_option_get(LIBRG_NETWORK_SECONDARY_CHANNEL),
                enet_packet_create(unreliable->rawptr, librg_data_get_wpos(unreliable), 0)
            );

            // and cleanup
            librg_data_reset(reliable);
            librg_data_reset(unreliable);
        }
    }
#endif

// =======================================================================//
// !
// ! World Spaces (TODO)
// !
// =======================================================================//

#if 1
    b32 librg__space_bounds_small_enough(zpl_aabb3 a, zpl_vec3 b) {
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

    void librg__space_split(librg_space *c) {
        zpl_aabb3 hd = c->boundary;
        for (i32 i = 0; i < c->dimensions; ++i) {
            hd.half_size.e[i] /= 2.0f;
        }

        i32 loops = 4;
        if (c->dimensions == LIBRG_SPACE_3D)
            loops = 8;

        f32 p[3] = {0};
        for (i32 i = 0; i < loops; ++i) {
            librg_space space = {0};
            zpl_aabb3 bounds = {0};
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

    b32 librg__space_remove_node(librg_space *c, librg_entity *tag) {
        if (c->nodes == NULL) return false;
        for (i32 i = 0; i < zpl_array_count(c->nodes); ++i) {
            librg_space_node *node = &c->nodes[i];
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

    void librg__space_init(librg_space *c, zpl_allocator a, isize dims, zpl_aabb3 bounds, zpl_vec3 min_bounds, u32 max_nodes) {
        librg_space c_ = {0};
        *c            = c_;
        c->allocator  = a;
        c->boundary   = bounds;
        c->min_bounds = min_bounds;
        c->max_nodes  = max_nodes;
        c->dimensions = dims;
        c->use_min_bounds = zpl_vec3_mag(min_bounds) > 0.0f;
    }

    void librg__space_clear(librg_space *c) {
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

    librg_inline librg_space *librg__space_insert(librg_ctx *ctx, librg_space *space, librg_space_node node) {
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
            librg_space *sub = librg__space_insert(ctx, (space->spaces+i), node);
            if (sub) return sub;
        }

        return NULL;
    }

    librg_inline void librg__execute_server_entity_insert(librg_ctx *ctx) {
        librg_assert(ctx);

        // fill up
        librg_entity_iteratex(ctx, LIBRG_ENTITY_ALIVE, entity, {
            librg_entity *blob = &ctx->entity.list[entity];
            librg_space_node node = {0}; {
                node.blob = blob;
            }

            if (blob->stream_branch == NULL) {
                blob->stream_branch = librg__space_insert(ctx, &ctx->world, node);
            }
            else {
                librg_space *branch = blob->stream_branch;
                b32 contains = librg__space_contains(branch->dimensions, branch->boundary, blob->position.e);
                if (!contains) {
                    librg__space_remove_node(branch, blob);
                    blob->stream_branch = librg__space_insert(ctx, &ctx->world, node);
                }
            }
        });
    }

    librg_inline void librg__execute_server_entity_destroy(librg_ctx *ctx) {
        for (isize i = 0; i < zpl_array_count(ctx->entity.remove_queue); i++) {
            librg__world_entity_destroy(ctx, ctx->entity.remove_queue[i]);
        }

        zpl_array_clear(ctx->entity.remove_queue);
    }

    librg_inline void librg__execture_server_entity_control(librg_ctx *ctx) {
        librg_entity_iteratex(ctx, LIBRG_ENTITY_CONTROL_REQUESTED, (entity), {
            librg_entity_fetch(ctx, entity)->flags &= ~LIBRG_ENTITY_CONTROL_REQUESTED;
        });

        for (isize i = 0; i < zpl_array_count(ctx->entity.add_control_queue); i++) {
            librg_message *msg = ctx->entity.add_control_queue[i];
            enet_peer_send(msg->peer, librg_option_get(LIBRG_NETWORK_MESSAGE_CHANNEL), msg->packet);
            zpl_free(ctx->allocator, msg);
        }

        zpl_array_clear(ctx->entity.add_control_queue);
    }

    librg_internal void librg__world_update(void *data) {
        f64 start  = zpl_time_now();
        librg_ctx *ctx = (librg_ctx *)data;
        librg_assert(ctx);

        if (librg_is_server(ctx)) {
            #if defined(LIBRG_FEATURE_OCTREE_CULLER)
            librg__execute_server_entity_insert(ctx); /* create the server cull tree */
            #endif

            librg__execute_server_entity_update(ctx); /* create and send updates to all clients */
            librg__execute_server_entity_destroy(ctx); /* destroy queued entities */
            librg__execture_server_entity_control(ctx); /* send controll add for created entities */
        } else {
            librg__execute_client_update(ctx); /* send information about client updates */
        }

        ctx->last_update = zpl_time_now() - start;
    }







    void librg__world_entity_query(librg_ctx *ctx, librg_entity_id entity, librg_space *c, zpl_aabb3 bounds, usize controlled_amount, librg_entity_id **out_entities) {
    #if defined(LIBRG_FEATURE_OCTREE_CULLER)
        if (c->nodes == NULL) return;
        if (!librg__space_intersects(c->dimensions, c->boundary, bounds)) return;
    #endif

        b32 use_radius = librg_option_get(LIBRG_USE_RADIUS_CULLING);
        librg_entity *ent_blob = librg_entity_fetch(ctx, entity);
        librg_assert(ent_blob);

    #if defined(LIBRG_FEATURE_OCTREE_CULLER)
        isize nodes_count = zpl_array_count(c->nodes);
        for (i32 i = 0; i < nodes_count; ++i) {
            if (c->nodes[i].unused) continue;
            librg_entity_id target = c->nodes[i].blob->id;
    #else
        for (u32 target = 0; target < ctx->max_entities; ++target) {
            if (!librg_entity_valid(ctx, target)) { continue; }
            if (target == entity) { continue; }
    #endif
            // iterate over pre-added controlled entities, to prevent duplications
            for (int j = 0; j < controlled_amount; ++j) {
                if (target == (*out_entities)[j]) { continue; }
            }

            if (librg_entity_valid(ctx, target)) {
                #if defined(LIBRG_FEATURE_OCTREE_CULLER)
                librg_entity *blob = c->nodes[i].blob;
                #else
                librg_entity *blob = librg_entity_fetch(ctx, target);
                #endif

                b32 inside = false; if (!use_radius) {
                    inside = librg__space_contains(c->dimensions, bounds, blob->position.e);
                } else {
                    zpl_vec3 diff;
                    zpl_vec3_sub(&diff, ent_blob->position, blob->position);
                    inside = zpl_vec3_mag2(diff) < zpl_square(ent_blob->stream_range);
                }

                if (inside) {
                    #ifdef LIBRG_FEATURE_ENTITY_VISIBILITY
                    if (!librg_entity_visibility_get(ctx, target)) continue;
                    if (!librg_entity_visibility_get_for(ctx, target, entity)) continue;
                    #endif

                    #ifdef LIBRG_FEATURE_VIRTUAL_WORLDS
                    if (librg_entity_world_get(ctx, target) != librg_entity_world_get(ctx, entity)) continue;
                    #endif

                    zpl_array_append(*out_entities, target);
                }
            }
        }

    #if defined(LIBRG_FEATURE_OCTREE_CULLER)
        if(c->spaces == NULL) return;

        isize spaces_count = zpl_array_count(c->spaces);
        if (spaces_count == 0) return;

        for (i32 i = 0; i < spaces_count; ++i) {
            librg__world_entity_query(ctx, entity, (c->spaces+i), bounds, controlled_amount, out_entities);
        }
    #endif
    }

    b32 librg__world_entity_destroy(librg_ctx *ctx, librg_entity_id id) {
        librg_assert(ctx);
        librg_assert(librg_entity_valid(ctx, id));
        librg_assert(ctx->entity.count > 0);

        ctx->entity.count--;
        librg_entity *entity = &ctx->entity.list[id];

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

        #ifdef LIBRG_FEATURE_ENTITY_VISIBILITY
        if (entity->flags & LIBRG_ENTITY_IGNORING) {
            librg_table_destroy(&entity->ignored);
        }

        if (librg_is_server(ctx)) {
            librg_entity_visibility_set(ctx, entity->id, true);
        }
        #endif

        entity->flags         = LIBRG_ENTITY_NONE;
        entity->position      = zpl_vec3f_zero();
        entity->type          = 0;
        entity->stream_branch = NULL;

        #ifdef LIBRG_FEATURE_VIRTUAL_WORLDS
        entity->virual_world  = 0;
        #endif

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
        /*LIBRG_USE_RADIUS_CULLING*/        0,
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
