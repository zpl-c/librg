/*
 * File: main.c
 * Author: MarkAtk
 * Date: 03.07.2019
 */

#define LIBRG_DEBUG
#define LIBRG_IMPLEMENTATION
#include <stdio.h>
#include <librg.h>

void on_connect_request(librg_event *event) {
    enet_peer_timeout(event->peer, 10, 5000, 5000);
    printf("client connection requested\n");
}

void on_connect_accept(librg_event *event) {
    printf("client connection accepted\n");
}

void on_disconnect(librg_event *event) {
    printf("client disconnected\n");
}

void on_timeout(librg_event *event) {
    printf("client timed out\n");
}

int main(int argc, char **argv) {
    // setup librg
    librg_ctx ctx = {0};

    ctx.tick_delay = 64;
    ctx.mode = LIBRG_MODE_SERVER;

    librg_init(&ctx);

    // register events
    librg_event_add(&ctx, LIBRG_CONNECTION_REQUEST, on_connect_request);
    librg_event_add(&ctx, LIBRG_CONNECTION_ACCEPT, on_connect_accept);
    librg_event_add(&ctx, LIBRG_CONNECTION_DISCONNECT, on_disconnect);
    librg_event_add(&ctx, LIBRG_CONNECTION_DISCONNECT, on_timeout);

    librg_network_start(&ctx, (librg_address) { .port = 7999 });

    printf("Server started\n");

    // main loop
    // for (int i = 0; i < 5000; ++i) {
    while (true) {
        librg_tick(&ctx);
        zpl_sleep_ms(1);
    }

    // cleanup
    librg_network_stop(&ctx);
    librg_free(&ctx);

    return 0;
}
