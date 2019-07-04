/*
 * File: main.c
 * Author: MarkAtk
 * Date: 03.07.2019
 */

#define LIBRG_DEBUG
#define LIBRG_IMPLEMENTATION
#include <stdio.h>
#include <librg.h>
#include <stdbool.h>

static bool run;

void on_connect_request(librg_event *event) {
    printf("connection requested\n");
}

void on_connect_accept(librg_event *event) {
    printf("connection accepted\n");

    run = false;
}

void on_disconnect(librg_event *event) {
    printf("disconnected\n");

    run = false;
}

void on_timeout(librg_event *event) {
    printf("timeout\n");

    run = false;
}

void connect_to_host(librg_ctx *ctx, char *host) {
    run = true;

    librg_network_start(ctx, (librg_address) { .host = host, .port = 7999 });
    enet_peer_timeout(ctx->network.peer, 10, 5000, 10000);

    printf("Client connecting to %s\n", host);

    // main loop
    while (run) {
        librg_tick(ctx);

        zpl_sleep_ms(1);
    }

    if (librg_is_connected(ctx)) {
        librg_network_stop(ctx);
    }
}

int main(int argc, char **argv) {
    // setup librg
    librg_ctx ctx = {0};

    ctx.tick_delay = 64;
    ctx.mode = LIBRG_MODE_CLIENT;

    librg_init(&ctx);

    // register events
    librg_event_add(&ctx, LIBRG_CONNECTION_REQUEST, on_connect_request);
    librg_event_add(&ctx, LIBRG_CONNECTION_ACCEPT, on_connect_accept);
    librg_event_add(&ctx, LIBRG_CONNECTION_DISCONNECT, on_disconnect);
    librg_event_add(&ctx, LIBRG_CONNECTION_TIMEOUT, on_timeout);

    // connect to server
    connect_to_host(&ctx, "::2");
    connect_to_host(&ctx, "::1");
    connect_to_host(&ctx, "::2");

    // cleanup
    librg_free(&ctx);

    return 0;
}
