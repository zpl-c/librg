<div align="center">
    <a href="https://lass.js.org"><img src="https://user-images.githubusercontent.com/2182108/29322933-0be3cb06-81e8-11e7-9cef-aa6be82c9faa.png" alt="Lass" /></a>
</div>

<div align="center">
    <a href="https://travis-ci.org/librg/librg"><img src="https://travis-ci.org/librg/librg.svg" alt="Build status" /></a>
    <a href="https://www.npmjs.com/package/librg.c"><img src="https://img.shields.io/npm/v/librg.c.svg?maxAge=3600" alt="NPM version" /></a>
    <a href="https://david-dm.org/librg/librg"><img src="https://img.shields.io/david/librg/librg.svg?maxAge=3600" alt="Dependencies" /></a>
    <a href="https://kiwiirc.com/client/irc.handmade.network:+7777/?nick=guest|?#librg"><img src="https://img.shields.io/badge/IRC-%23librg-blue.svg" alt="IRC server"/></a>
    <a href="https://discord.gg/2fZVEym"><img src="https://discordapp.com/api/guilds/219565308007022592/embed.png" alt="Discord server" /></a>
    <a href="LICENSE"><img src="https://img.shields.io/github/license/librg/librg.svg" alt="license" /></a>
</div>

<br />
<div align="center">
  Pure C99 library for building simple and elegant cross-platform mmo client-server solutions.
</div>

<div align="center">
  <sub>
    Built with love using <a href="https://github.com/zpl-c/zpl"><strong>zpl</strong></a>
    &bull; Brought to you by <a href="https://github.com/inlife">@inlife</a>,
    <a href="https://github.com/zaklaus">@zaklaus</a>
    and <a href="#contributors">contributors</a>
  </sub>
</div>

## Table of Contents

* [Purpose](#purpose)
* [Use-cases](#use-cases)
* [Features](#features)
* [Plans](#plans)
* [How it works](#how-it-works)
    * [General](#general)
    * [Server streaming](#server-streaming)
    * [Client streaming](#client-streaming)
    * [Custom data](#custom-data)
* [Structure](#structure)
    * [List](#list)
    * [Information](#information)
    * [Build instructions](#build-instructions)
* [Installation](#installation)
* [Examples](#examples)
    * [Server](#server)
    * [Client](#client)
    * [Additional](#additional)
* [Testing](#testing)
* [Contributors](#contributors)
* [License](#license)

## Purpose

Many people think that implementing **networking** solution for your **game project** is the most **complicated** and time consuming **task**.  
We **believe** many people are afraid to even try, which results in the fact, that you **almost** never see small games made by **indie developers** having any type of networking involved.

Thus we hope that with this **library**, which is just a **small step in the direction**, we might help anyone and everyone who wants to add a **multi-player** capabilities inside one's game.

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

## Features

* simple to use **entity-component** system
* fast and performant **event** system
* convenient interface for binary data reading/writing (**bitstream**)
* highly configurable, both at compile- and run-time
* high performant **server-side network-culling**, or in other words **"streaming"**
* single header-only file format (however it has some [dependencies](#structure))
* written in C99 (portability reasons)
* small (only 1160LOC)
* ready for C/C++ projects
* CMake support
* npm support (for dependency management)

## Plans

We are currently fixing issues on different platforms and possibly adding small features. However, the main idea for library and it's interface is done.
In the future, we would like to see this library being used in different platforms and environments.

For example, creating bindings for different platforms and languages (e.g `librg-love2d` or `librg-csharp`) would be useful.
If you want to get involved - please contact us, we would gladly answer your questions and provide you support with a repository reserved in our organisation.


## How it works

#### General

General concept is very simple. Both server and client operate on an **array of entities**. Entities have attached **components**.
User can create his **own components** and **attach** them onto entities.

librg entities have few **default components**, like: `transform`, `streamable`, `client` (for client) and `clientstream` (for entities which are controlled by client).
You can find more information about those by checking out the source header file.

#### Server streaming

Server **always has all the entities in the game world**. Clients will have only **snapshot** of the current world, which is **limited** by the **stream-range**.
Client will receive **periodic updates from the server**, containing information about the world (snaphot).

When entity receives an entity which **was not in his stream zone** for the first time, the `LIBRG_ENTITY_CREATE` event will be called,
there you can **create your in-game object**, attach a component with custom data, etc.
You should **consider sending as much data** as possible about the entity on it's creation, for example: color, hair-style, vehicle model id, etc.

If entity already exists in the client's local game world, `LIBRG_ENTITY_UPDATE` event will be called. (Note: it will be called **each time** server sends update, and entity is still in the stream-zone).
It's recommended that you send as **less data** as possible in the update, otherwise you will be **polluting the network**. Entity **transform** will be always sent.

At last, if entity is **no longer in the client's stream-zone** the `LIBRG_ENTITY_REMOVE` event will be triggered. There you should remove previously created in-game object,
detach all components, and de-allocate all your game data related to that entity (or cache it, it's your call).

#### Client streaming

If you want your client to send updates about **one or more** entities to the server.  
For example, you have a huge game world (e.g. open world sandbox game), where there are some entities which are not players, however you need someone to control them.
Considering the fact that you probably don't have any game logic on the server, you need **one of your clients to send updates about ingame entities** to other clients.

This can be achieved quite easily, you can just call method `librg_streamer_client_set` on the server, for specific entity.
It will make provided **client responsive** for streaming this entity to the server. 

Now, what you need to do is just **update that entity** components like `transform` and so on from your local client's game world. And that `transform` will be automatically sent to the server.


#### Custom data

Now, you probably have a question, `how do i send custom data?`

That is also quite simple indeed. Before triggering, let's say `LIBRG_ENTITY_UPDATE` event on the client,
same event will be triggered on the server, allowing you to write something in the `data` parameter.

For example:

```c
void on_entity_update(librg_event_t *event) {
    librg_data_wu32(event->data, 11223345); // write unsigned long
    librg_data_wptr(event->data, &mycomponent, sizeof(mycomponent)); // write data right from the struct
}
```

Now on the other side, you are required to read the data with in the same exact sequence:

```c
void on_entity_update(librg_event_t *event) {
    u32 mynumber = librg_data_ru32(&event->data); // read unsigned long

    mycompoonent_t mycomponent;
    librg_data_rptr(event->data, &mycomponent, sizeof(mycompoonent_t)); // read data right to the struct
}
```

This concept can be applied to all events that have been described above: `LIBRG_ENTITY_CREATE`, `LIBRG_ENTITY_UPDATE`, `LIBRG_ENTITY_REMOVE`, `LIBRG_CLIENT_STREAMER_REMOVE`, and some others.

## Structure

The library is built with **single header-only file design**. So everything librg provides is located in [includes/librg.h](https://github.com/librg/librg/blob/master/include/librg.h) file.  
However it has **dependencies**. Most of them are also single header-only libraries.

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
librg comes with a **[CMakeLists.txt](CMakeLists.txt)** file. You can use it to integrate the library inside your project. Or use it as a boilerplate for new project.
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

Simple server, which will behave like a **proxy**, creating entity for each joining client, and showing him his **network-culled zone of view**. Some people call it **streaming**.
Updates will be sent to the client each `config.tick_delay` ms. You can add your own logic of moving objects on the server,
and all **changes** of the position and other parameters will be **automatically sent to all clients**.

```c
#define LIBRG_DEBUG
#define LIBRG_IMPLEMENTATION
#include <librg.h>

void on_connect_accepted(librg_event_t *event) {
    librg_log("someone connected to the server!\n");
}

int main() {
    // initialization
    librg_config_t config = {0};

    config.tick_delay   = 64;
    config.mode         = LIBRG_MODE_SERVER;
    config.world_size   = zplm_vec2(5000.0f, 5000.0f);

    librg_init(config);

    // adding event handlers
    librg_event_add(LIBRG_CONNECTION_ACCEPT, on_connect_accepted);

    // starting server
    librg_address_t address = {0}; address.port = 22331;
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

Client receives a **snapshot of network-culled game world** and calls methods for **creating entities according to what server tells them**.
Moving objects (on the server) may go out of "stream-zone" and client will trigger **entity remove event**, if object is still visible for the player, **update event** will be triggered,
it will contain **current server's information** about the object.

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
    librg_config_t config = {0};

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


## Testing

We started testing the library for different platforms. This table provides some sort of description for compatibility.
If you have tested it, and it compiles, or it perhaps stopped compiling, please feel free to describe the issue in the [issues](https://github.com/librg/librg/issues).

| *Compiler* / Target   | Windows               | Linux                 | macOS                 | iOS               | Android           |
|:-:|:-:|:-:|:-:|:-:|:-:|
| *clang C*             | :grey_question:       | :white_check_mark:    | :white_check_mark:    | :grey_question:   | :grey_question:   |
| *clang C++*           | :grey_question:       | :grey_question:       | :white_check_mark:    | :grey_question:   | :grey_question:   |
| *gcc C*               |                       | :white_check_mark:    | :white_check_mark:    |                   | :grey_question:   |
| *gcc C++*             |                       | :grey_question:       | :grey_question:       |                   | :grey_question:   |
| *msvc C++*            | :white_check_mark:    |                       |                       |                   |                   |
| *msvc C*              | :white_check_mark:    |                       |                       |                   |                   |
| *mingw C++*           | :white_check_mark:    |                       |                       |                   |                   |
| *mingw C*             | :white_check_mark:    |                       |                       |                   |                   |

<sub>
:white_check_mark: - compiles/runs without any errors.  
:grey_question: - not tested.  
:red_circle: - compilation/runtime faults.  
</sub>

## Contributors

Current ~~list~~ table of contributors looks like this:

| [Inlife](https://github.com/inlife) | [ZaKlaus](https://github.com/zaklaus) | You |
|:-:|:-:|:-:|
|[![](https://avatars3.githubusercontent.com/u/2182108?v=4&s=64)](https://github.com/inlife)|[![](https://avatars0.githubusercontent.com/u/9026786?v=4&s=64)](https://github.com/zaklaus)|![](https://avatars0.githubusercontent.com/u/19527937?v=4&s=64)|

If you want to help us - you are very welcome. New features, performance optimizations, fixes for bugs and errors are waiting for you!

## License
Last time we checked it was **Apache License 2.0**. However we suggest you to [check it again](LICENSE), what if it has been changed!
