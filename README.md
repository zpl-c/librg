# librg

Pure C library for building simple and elegant cross-platform mmo client-server solutions.

## Status

Currently under active development. Not ready to be used.

## Structure
Library has 2 basic dependencies:

* [zpl](https://github.com/zaklaus/zpl) - ZPL is a collection of single-file public domain header-only libraries for C and C++.
* [enet](https://github.com/lsalzman/enet) - ENet reliable UDP networking library.

While `zpl` is header only library, `enet` is compilable library, with multiple source files, so it requires you to compile, and link it.

## Whats new
(Comparing to previous version of librg)

1. Single header library format, pure c imlementation, and reduced number of depdendencies.
2. Changes in method naming. All types, functions, defines, etc always starts with prefix `librg_`.
3. Shared entitiy ids for all server-created entities.
4. New, more robust event system design, with wrapping event sturct, event rejection.
5. Changes in sending message interface.
6. Methods for cleaning up entity pools after disconnection.
7. Improved interface for handling basic user interaction events.


## Example

```c
// server
#define LIBRG_IMPLEMENTATION
#include <librg.h>

typedef struct { bool a; } librg_component(foo);

void on_connect_request(librg_event_t *event) {
    u32 secret = zpl_bs_read_u32(event->bs);

    if (secret != 42) {
        return librg_event_reject(event);
    }

    librg_log("user connection successfully approved!");
}

// we are sending some entity to the client
// with some custom information
void on_entity_create(librg_event_t *event) {
    zpl_bs_write_u64(event->bs, 11223344);
}

int main() {
    librg_init((librg_cfg_t) {
        .tick_delay     = 32,
        .mode           = librg_server_ev,
        .world_size     = zplm_vec2(5000.0f, 5000.0f),
        .entity_limit   = 2048,
    });

    librg_event_add(LIBRG_CONNECTION_REQUEST, on_connect_request);
    librg_event_add(LIBRG_ENTITY_CREATE, on_entity_create);

    // start server
    librg_network_start((librg_address_t) { .port = 27010 });

    while (true) {
        librg_tick();
    }

     // stop server
    librg_network_stop();
    librg_free();

    return 0;
}
```
