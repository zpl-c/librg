<div align="center">
    <a href="https://lass.js.org"><img src="https://user-images.githubusercontent.com/2182108/29322933-0be3cb06-81e8-11e7-9cef-aa6be82c9faa.png" alt="Lass" /></a>
</div>

<div align="center">
    <a href="https://travis-ci.org/librg/librg"><img src="https://travis-ci.org/librg/librg.svg" alt="Build status" /></a>
    <a href="https://github.com/librg/librg"><img src="https://img.shields.io/npm/v/librg.c.svg?maxAge=3600" alt="NPM version" /></a>
    <a href="https://david-dm.org/librg/librg"><img src="https://img.shields.io/david/librg/librg.svg?maxAge=3600" alt="Dependencies" /></a>
    <a href="https://discord.gg/2fZVEym"><img src="https://discordapp.com/api/guilds/219565308007022592/embed.png" alt="Discord server" /></a>
    <a href="LICENSE"><img src="https://img.shields.io/github/license/librg/librg.svg" alt="license" /></a>
</div>

<br />
<div align="center">
  Pure C library for building simple and elegant cross-platform mmo client-server solutions.
</div>

<div align="center">
  <sub>
    Built with love using <a href="https://github.com/zpl-c/zpl"><strong>zpl</strong></a>
    &bull; Brought to you by <a href="https://github.com/inlife">@inlife</a>,
    <a href="https://github.com/zaklaus">@zaklaus</a>
    and <a href="#contributors">contributors</a>
  </sub>
</div>


## Purpose

Many people think that implementing networking solution for your game project is the most complicated and time consuming task.  
We believe many people are afraid to even try, which results in the fact, that you almost never see small games made by indie developers having any type of networking involved.

Thus we hope that with this library, which is just a small step in the direction, we might help anyone and everyone who wants to add a multi-player capabilities inside one's game.

## Use-cases

* As "built-in" server:  
    Server can be attached to the client and run as a subprocess. It will result in a quite common concept of client-hosted game sessions for **friends**.
    However it requires user, or you, to deal with stuff like public ip/visibility.

* As proxy server:  
    Would you like to have a **dedicated server** solution? Server will be used as a proxy to store and send accumulated player data.
    Considering the fact that you don't have (almost) any game logic on the server side, it can handle big amounts of clients.
    Great example can be something like [San-Andreas Multiplayer](http://www.sa-mp.com/).

* As thin client (advanced):  
    For client side, you will use thin-client "pattern". All user actions will be sent directly to the server.
    Requires you to write all the server side systems, like physics handling, game world management, and **gameloop on the server side**.

* Any other possible way that was not mentioned.


## Structure

### General
The library is built with single header-only file design. So everything librg provides is located in `includes/librg.h` file.  
However it has dependencies. Most of them are also single header-only libraries.

### List
Current list and description of dependencies looks like this:

| Library | Version | Description |
|:-:|:-:|---|
| [zpl](https://github.com/zpl-c/zpl) | ![](https://img.shields.io/npm/v/zpl.c.svg?maxAge=3600) | **zpl** is a basic library containing many functions you would use in everyday programming.|
| [zpl-cull](https://github.com/zpl-c/zpl-cull) | ![](https://img.shields.io/npm/v/zpl_cull.c.svg?maxAge=3600) | **zpl-cull** is culling library. Used to create an entity tree, and query objects based on that. |
| [zpl-math](https://github.com/zpl-c/zpl-math) | ![](https://img.shields.io/npm/v/zpl_math.c.svg?maxAge=3600) | **zpl-math** is obviously a math library. Used mostly in the streamer part. |
| [zpl-event](https://github.com/zpl-c/zpl-event) | ![](https://img.shields.io/npm/v/zpl_event.c.svg?maxAge=3600) | **zpl-event** is simple and yet powerful event library. Most of the public interfaces of librg are build using zpl-event. |
| [enet](https://github.com/zpl-c/enet) | ![](https://img.shields.io/npm/v/enet.c.svg?maxAge=3600) | **enet** is a quite popular high performant low-level network library. A core for the librg. |

### Information
**enet** is the only dependency which has a multi-file structure and source parts (*.c files). So it should be compiled separately and linked together with your project.

### Build instructions
librg comes with a **CMakeLists.txt** file. You can use it to integrate the library inside your project. Or use it as a boilerplate for new project.
There is also the **[build.sh](build.sh)** shell script, which is usually used for the development purposes, however it also can be used as an example of instructions you need to provide to your compiler.

## Installation

There are multiple way of how you can "install" the library:

* automatic
    * using **[npm](https://www.npmjs.com/get-npm)**. Just run `npm install librg.c` in folder with your project, that's it!   
        <sub><i>(maybe you will not like the `node_modules` folder, however you can move it or rename it, if you are *not planning* to fetch any udpates via npm)</i></sub>

* manual:
    * downloading/copying only [librg.h](https://raw.githubusercontent.com/librg/librg/master/include/librg.h) file, and doing same for each dependency.
    * downloading/cloning repos for the librg and for each other dependency.


## Examples

### Server

Simple server, which will behave like a proxy, creating entity for each joining client, and showing him his network-culled zone of view. Some people call it streaming.
Updates will be sent to the client each `config.tick_delay` ms. You can add your own logic of moving objects on the server,
and all changes of the position and other parameters will be automatically sent to all clients.

```c
#define LIBRG_DEBUG
#define LIBRG_IMPLEMENTATION
#include <librg.h>

void on_connect_accepted(librg_event_t *event) {
    librg_log("someone connected to the server!\n");
}

int main() {
    // initialization
    librg_config_t config;

    config.tick_delay   = 64;
    config.mode         = LIBRG_MODE_SERVER;
    config.world_size   = zplm_vec2(5000.0f, 5000.0f);

    librg_init(config);

    // adding event handlers
    librg_event_add(LIBRG_CONNECTION_ACCEPT, on_connect_accepted);

    // starting server
    librg_address_t address; address.port = 22331;
    librg_network_start(address);

    // starting main loop
    while (true) {
        librg_tick();
        zpl_sleep_ms(1);
    }

    // stopping network and resources disposal
    librg_network_stop();
    librg_free();
    return 0;
}
```

### Client

Client receives a snapshot of network-culled game world and calls methods for creating entities according to what server tells them.
Moving objects (on the server) may go out of "stream-zone" and client will trigger entity remove event, if object is still visible for the player, update event will be triggered,
it will contain current server's information about the object.

```c
#define LIBRG_DEBUG
#define LIBRG_IMPLEMENTATION
#include <librg.h>

void on_entity_create(librg_event_t *event) {
    zplm_vec3_t position = librg_fetch_transform(event->entity)->position;

    // call to ingame method of creating object
    librg_log("creating entity with id: %u, of type: %u, on position: %f %f %f \n",
        event->entity, librg_entity_type(event->entity),
        position.x, position.y, position.z
    );
}

void on_entity_update(librg_event_t *event) {
    zplm_vec3_t position = librg_fetch_transform(event->entity)->position;

    // call to ingame method of updating object
    librg_log("updating position for entity with id: %u, of type: %u, to position: %f %f %f \n",
        event->entity, librg_entity_type(event->entity),
        position.x, position.y, position.z
    );
}

void on_entity_remove(librg_event_t *event) {
    // call to ingame method of destroying object
    librg_log("destroying entity with id: %u\n", event->entity);
}

int main() {
    // initialization
    librg_config_t config;

    config.tick_delay   = 64;
    config.mode         = LIBRG_MODE_CLIENT;
    config.world_size   = zplm_vec2(5000.0f, 5000.0f);

    librg_init(config);

    // adding event handlers
    librg_event_add(LIBRG_ENTITY_CREATE, on_entity_create);
    librg_event_add(LIBRG_ENTITY_UPDATE, on_entity_update);
    librg_event_add(LIBRG_ENTITY_REMOVE, on_entity_remove);

    // connect to the server
    librg_address_t address = { "localhost", 22331 };
    librg_network_start(address);

    // starting main loop
    while (true) {
        librg_tick();
        zpl_sleep_ms(1);
    }

    // disconnection from the server
    // and resource disposal
    librg_network_stop();
    librg_free();
    return 0;
}
```

### Additional

You can and **MUST** (not really, but i really advise you to) look into the source code ([librg.h](include/librg.h)).
There we have many helpful (and not really) comments that will guide you or at very least give you explanation what is this or that, why it's needed, and how to use it.

Also you can look inside our [test](test/) folder, there are many different things. It's usually used for the development purposes, but I guarantee you can find something interesting in there.

## Features

* single header-only file format (however it has some [dependencies](#Structure))
* written in C99 (portability reasons)
* highly configurable
* small (only 1160LOC)
* ready for C/C++ projects
* CMake support
* npm support (for dependency management)

## Contributors

| [Inlife](https://github.com/inlife) | [ZaKlaus](https://github.com/zaklaus) |
|:-:|:-:|
|[![](https://avatars3.githubusercontent.com/u/2182108?v=4&s=64)](https://github.com/inlife)|[![](https://avatars0.githubusercontent.com/u/9026786?v=4&s=64)](https://github.com/zaklaus)|

## License
Last time we checked it was **Apache License 2.0**.  
However we suggest you to check out [License](LICENSE).
