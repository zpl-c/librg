#ifdef LIBRG_IMPLEMENTATION
#define ZPL_IMPLEMENTATION
#endif

#include <zpl.h>

void librg_init();
void librg_tick();
void librg_free();
void librg_log();

void librg_is_server();
void librg_is_client();

void librg_connect();
void librg_disconnect();

void librg_add();
void librg_send();
void librg_send_to();
void librg_send_all();
void librg_send_except();
void librg_send_stream();

void librg_evt_add();
void librg_evt_trigger();

void librg_streamer_set_visible();
void librg_streamer_set_visible_for();

void librg_streamer_client_add();
void librg_streamer_client_remove();
