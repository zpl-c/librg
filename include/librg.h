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

#define librg_log zpl_printf
typedef zple_entity_t librg_ent_t;

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

void librg_init(librg_cfg_t config);
void librg_tick();
void librg_free();

void librg_mode_set(i32 mode);
i32 librg_mode_get();

b32 librg_is_server();
b32 librg_is_client();

void librg_connect(char *host, int port);
void librg_disconnect();

librg_ent_t librg_ent_create();
void librg_ent_free(librg_ent_t entity);

typedef void *librg_net_cb;

void librg_send_to      (u64 id, librg_ent_t entity, librg_net_cb callback);
void librg_send_except  (u64 id, librg_ent_t entity, librg_net_cb callback);
void librg_send_all     (u64 id, librg_net_cb callback);
void librg_send_instream(u64 id, librg_net_cb callback);

#define librg_send librg_send_all

u64 librg_net_add(u64 id, librg_net_cb callback);
void librg_net_remove(u64 id, u64 index);

typedef zplev_data_t librg_evt_t;
typedef zplev_cb librg_evt_cb;

u64 librg_evt_add(u64 id, librg_evt_cb callback);
void librg_evt_trigger(u64 id, librg_evt_t event);
void librg_evt_remove(u64 id, u64 index);

void librg_streamer_remove();
void librg_streamer_set_visible();
void librg_streamer_set_visible_for();
void librg_streamer_client_set();
void librg_streamer_client_remove();
