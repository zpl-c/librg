![](https://user-images.githubusercontent.com/2182108/29322933-0be3cb06-81e8-11e7-9cef-aa6be82c9faa.png)

Pure C library for building simple and elegant cross-platform mmo client-server solutions.

## Status

Currently under active development. It is not ready to be used.

## Structure
Library has 2 basic dependencies:

* [zpl](https://github.com/zpl-c) - ZPL is a collection of single-file public domain header-only libraries for C and C++.
* [enet](https://github.com/zpl-c/enet-c) - ENet reliable UDP networking library.

While `zpl` is header only library, which integrates into librg implicitly, 
`enet` is a standalone dependency that has to be compiled separately and then linked with.

## What is new
(Comparing to the previous, obsolete version of librg)

- Single header library format, pure c implementation, and reduced number of dependencies.
- Changes in method naming. All types, functions, defines, etc always start with prefix `librg_`.
- Shared entity ids for all server-created entities.
- New, more robust event system design, with wrapping event struct and event rejection.
- Changes in message sending interface. Methods for cleaning up entity pools after disconnection.
- Improved interface for handling basic user interaction events.


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
    librg_init((librg_config_t) {
        .tick_delay     = 32,
        .mode           = LIBRG_MODE_SERVER,
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
