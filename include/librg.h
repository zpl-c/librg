#ifndef LIBRG_INCLUDE_H
#define LIBRG_INCLUDE_H

#define LIBRG_VERSION_MAJOR 1
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
    #define ZPL_MATH_IMPLEMENTATION
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
        zpl_string_t password;

        // backend network
        u8 platform_id;
        u8 proto_version;
        u8 build_version;
    } librg_cfg_t;

    LIBRG_API void librg_init(librg_cfg_t config);
    LIBRG_API void librg_tick();
    LIBRG_API void librg_free();

    LIBRG_API void librg_mode_set(i32 mode);
    LIBRG_API i32 librg_mode_get();


    /**
     * ENTITIES
     */
    typedef void *librg_ent_t;

    LIBRG_API librg_ent_t librg_ent_create();
    LIBRG_API void librg_ent_destroy(librg_ent_t entity);


    /**
     * NETWORK
     */

    LIBRG_API void librg_connect(char *host, int port);
    LIBRG_API void librg_disconnect();

    LIBRG_API b32 librg_is_server();
    LIBRG_API b32 librg_is_client();
    LIBRG_API b32 librg_is_connected();

    typedef void *librg_peer_t;
    typedef void *librg_net_cb;

    LIBRG_API void librg_send_to      (u64 id, librg_ent_t entity, librg_net_cb callback);
    LIBRG_API void librg_send_except  (u64 id, librg_ent_t entity, librg_net_cb callback);
    LIBRG_API void librg_send_all     (u64 id, librg_net_cb callback);
    LIBRG_API void librg_send_instream(u64 id, librg_net_cb callback);

    #define librg_send librg_send_all

    LIBRG_API u64 librg_net_add(u64 id, librg_net_cb callback);
    LIBRG_API void librg_net_remove(u64 id, u64 index);


    /**
     * EVENTS
     */

    typedef zplev_data_t librg_evt_t;
    typedef zplev_cb librg_evt_cb;

    LIBRG_API u64 librg_evt_add(u64 id, librg_evt_cb callback);
    LIBRG_API void librg_evt_trigger(u64 id, librg_evt_t event);
    LIBRG_API void librg_evt_remove(u64 id, u64 index);


    /**
     * STREAMER
     */

    LIBRG_API void librg_streamer_remove          (librg_ent_t entity);
    LIBRG_API void librg_streamer_set_visible     (librg_ent_t entity, b32 state);
    LIBRG_API void librg_streamer_set_visible_for (librg_ent_t entity, librg_ent_t target, b32 state);
    LIBRG_API void librg_streamer_client_set      (librg_ent_t entity, librg_peer_t peer);
    LIBRG_API void librg_streamer_client_remove   (librg_ent_t entity);

#ifdef __cplusplus
}
#endif

#if defined(LIBRG_IMPLEMENTATION) && !defined(LIBRG_IMPLEMENTATION_DONE)
#define LIBRG_IMPLEMENTATION_DONE

#ifdef __cplusplus
extern "C" {
#endif


    // IMPLEMENTATION


#ifdef __cplusplus
}
#endif

#endif // LIBRG_IMPLEMENTATION
#endif // LIBRG_INCLUDE_H
