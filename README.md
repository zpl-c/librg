<div align="center">
    <a href="https://github.com/librg/librg"><img src="https://user-images.githubusercontent.com/2182108/29322933-0be3cb06-81e8-11e7-9cef-aa6be82c9faa.png" alt="librg" /></a>
</div>

<div align="center">
    <a href="https://travis-ci.org/librg/librg"><img src="https://travis-ci.org/librg/librg.svg?branch=master" alt="Build status" /></a>
    <a href="https://www.npmjs.com/package/librg.c"><img src="https://img.shields.io/npm/v/librg.c.svg?maxAge=3600" alt="NPM version" /></a>
    <a href="https://david-dm.org/librg/librg"><img src="https://img.shields.io/david/librg/librg.svg?maxAge=3600" alt="Dependencies" /></a>
    <a href="https://kiwiirc.com/client/irc.handmade.network:+7777/?nick=guest|?#librg"><img src="https://img.shields.io/badge/IRC-%23librg-blue.svg" alt="IRC server"/></a>
    <a href="https://discord.gg/2fZVEym"><img src="https://discordapp.com/api/guilds/354670964400848898/embed.png" alt="Discord server" /></a>
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

## Desciption

Have you ever wondered why is it always so hard to make a multi-player game, even the most basic one?
And the complexity even increases with bigger amounts of players and things you need to sync between your clients.
This library is designed exactly to solve that problem. It has a simple interface with highly flexible base which allows you to build great things!

Many people think that implementing networking solution for your game project is the most complicated and time consuming task.
We believe many people are afraid to even try, which results in the fact, that you almost never see small games made by indie developers having any type of networking involved.

Thus we hope that with this library, which is just a small step in the direction, we might help anyone and everyone who wants to add a multi-player capabilities inside one's game.

## Features

* high performance
* minimalistic interface
* server-side network culling
* client-side entity streaming
* fast and performant event system
* highly configurable, both at compile- and run-time
* lightweight, minimal dependencies, single-header
* written in C99 (portability reasons)
* npm support, cmake support

## How it works

#### General

As we've mentioned before, librg has a simple interface. Like many C libraries, it has some sort of application context, which makes it  thread-safe and offers the ability to run multiple instances under the same process. Here is sample server code:

```c
int main(int argc, char const *argv[]) {
    librg_ctx_t ctx = { 0 };
    librg_init(&ctx);

    librg_address_t addr = { 27010 };
    librg_network_start(&ctx, addr);

    bool running = true;
    while (running) {
        librg_tick(&ctx);
    }

    librg_network_stop(&ctx);
    librg_free(&ctx);
    return 0;
}
```

#### Events

Everything is built around events, something gets created - the related event gets triggered. Your task as a developer is to use these events and write appropriate handlers.

Let's look at the example, client connects to the server, spawns on the map, and librg triggers `LIBRG_ENTIY_CREATE` event for every entity in the player's range:

```c
void mygame_entity_create(librg_event_t *event) {
    int entity_id   = event->entity->id;
    int entity_type = event->entity->type;
    vec3 position   = event->entity->position;

    // and call your game method to spawn entity in the game world
    MyEntity *entity = MyGame_CreateEntity(entity_type, position);
    myEntities[entity_id] = entity;
}

    // ...
    // register the handler
    librg_event_add(&ctx, LIBRG_ENTITY_CREATE, mygame_entity_create);
    // ...
```

This way, the client on his side creates all entities that server tells him to create.
And the best part is that, server will tell only about those entities that are in the specified range.
This allows you to have bigger amounts of synced data, using the same amount of bandwidth.

Same concept applies when your client's entity needs to be removed, you just register the `LIBRG_ENTITY_REMOVE` handler, and unspawn the entity from the game world.

#### Server sync

What if you need, (and most likely you do), to send some data, on every server tick, like object position or rotation, or maybe even object color.

You need to register a handler for the `LIBRG_ENTITY_UDPATE` event, but this time you need to do it on both sides!

```c
/* server side */
void myserver_entity_update(librg_event_t *event) {
    // change the position, it will be sent automatically
    event->entity->position.x += 5.0f;

    // send some u32 numbers by manually writing to the stream
    librg_data_wu32(event->data, 0); /* black color */
    librg_data_wu32(event->data, 300); /* speed is 300 */
}

/* client side */
void myclient_entity_update(librg_event_t *event) {
    int entity_id    = event->entity->id;
    MyEntity *entity = myEntities[entity_id];

    // position has been updated, we can set the ingame object's position
    MyGame_SetPosition(entity, event->entity->position);

    // read u32 numbers manually from the stream
    MyGame_SetColor(entity, librg_data_ru32(event->data));
    MyGame_SetSpeed(entity, librg_data_ru32(event->data));
}

```

Now, when you write data on the server, you just read it back on the client in the same order as it was written. That's it, you have a server streamed sync!

#### Messages

Considering you've already synced your server and clients after reading the previous section, you might want to trigger some actions/commands on the server, so it could become interactive.
And the way you can do it is quite simple, it is similar to events you are familiar already, so it should be an easy task:

```c
/* server side */
void myserver_onmessage1(librg_message_t *msg) {
    printf("we got message 1\n");
}

void myserver_onmessage2(librg_message_t *msg) {
    printf("we got message 2\n");

    YourData yourData; /* read data back */
    librg_data_rptr(msg->data, &yourData, sizeof(YourData));
}

    // ...
    librg_message_add(&ctx, MESSAGE_1, myserver_onmessage1);
    librg_message_add(&ctx, MESSAGE_2, myserver_onmessage2);
    // ...


/* client side */
    // ...
    librg_messsage_send_all(&ctx, MESSAGE_1, NULL, 0); /* send an empty message */
    librg_messsage_send_all(&ctx, MESSAGE_2, &yourData, sizeof(yourData)); /* send some clean data */
```

#### Client sync

If you want your client to send updates about one or more entities to the server, and you need a simple and fast solution, you can stream your entities right from your clients.
For example, you have a huge game world (e.g. open world sandbox game), where there are some entities which are not players, however you need someone to control them.
Considering the fact that you probably don't have any game logic on the server, you need one of your clients to send updates about ingame entities to other clients.

This can be achieved quite easily, you can just call method `librg_entity_control_set` on the server, for specific entity.
It will make provided client responsive for streaming this entity to the server.

Now, what you need to do is just to update that entity data from your local client's game world. And that can also be achieved quite easily:

```c
/* client side */
void mygame_client_stream(librg_event_t *event) {
    // write new entity position (will be sent automatically)
    event->entity->position = MyGame_GetPosition(event->entity->id);

    // write some data
    librg_data_wf32(&data, MyGame_GetDotVelocity(event->entity->id));
}

    // ...
    librg_event_add(&ctx, LIBRG_CLIENT_STREAMER_UPDATE, mygame_client_stream);
    // ...
```

Now on the server side, you pretty much do the same, it is very similar to what we did for entity update, with only difference being that we use `LIBRG_CLIENT_STREAMER_UPDATE` event id.

#### Additional

You can and **MUST** (not really, but we really advise you to) look into the source code ([librg.h](include/librg.h)).
There we have many helpful (and not really) comments that will guide you or at very least give you explanation what is this or that, why it's needed, and how to use it.

Also you can look inside our [test](test/) folder, there are many different things. It's usually used for the development purposes, but I guarantee you can find something interesting in there.

## Installation

There are multiple way of how you can "download" the library:

* automatic
    * using **[npm](https://www.npmjs.com/get-npm)**. Just run `npm install librg.c` in folder with your project, that's it!   
        <sub><i>(maybe you will not like the `node_modules` folder, however you can move it or rename it, if you are *not planning* to fetch any udpates via npm)</i></sub>

* manual:
    * downloading/copying only [librg.h](https://raw.githubusercontent.com/librg/librg/master/include/librg.h) file, and doing same for each dependency.
    * downloading/cloning repos for the librg and for each other dependency.

Essentially, what you will need to have in the result is a set of single-header libraries. Which you can just put in the same folder and point the compiler include ther.

## Structure

The library is built with **single header-only file design**. So everything librg provides is located in [includes/librg.h](https://github.com/librg/librg/blob/master/include/librg.h) file.  
However it has **dependencies**. All of them are also single header-only libraries.

### List
Current list and description of dependencies looks like this:

| Library | Version | Description |
|:-:|:-:|---|
| [enet](https://github.com/zpl-c/enet) | ![](https://img.shields.io/npm/v/enet.c.svg?maxAge=3600) | **enet** is a quite popular high performant low-level network library. A core for the librg. |
| [zpl](https://github.com/zpl-c/zpl) | ![](https://img.shields.io/npm/v/zpl.c.svg?maxAge=3600) | **zpl** is a basic library containing many functions you would use in everyday programming.|
| [zpl-cull](https://github.com/zpl-c/zpl-cull) | ![](https://img.shields.io/npm/v/zpl_cull.c.svg?maxAge=3600) | **zpl-cull** is culling library. Used to create an entity tree, and query objects based on that. |
| [zpl-math](https://github.com/zpl-c/zpl-math) | ![](https://img.shields.io/npm/v/zpl_math.c.svg?maxAge=3600) | **zpl-math** is obviously a math library. Used mostly in the streamer part. |
| [zpl-event](https://github.com/zpl-c/zpl-event) | ![](https://img.shields.io/npm/v/zpl_event.c.svg?maxAge=3600) | **zpl-event** is simple and yet powerful event library. Most of the public interfaces of librg are build using zpl-event. |

### Build instructions
librg comes with a **[CMakeLists.txt](CMakeLists.txt)** file. You can use it to integrate the library inside your project. Or use it as a boilerplate for new project.
There is also the **[build.sh](build.sh)** shell script, which is usually used for the development purposes, however it also can be used as an example of instructions you need to provide to your compiler.


## Use-cases

* As "built-in" server:  
    Server can be merged with the client and run even from the same thread. It will result in a quite common concept of client-hosted game sessions for **friends**.
    However it requires user, or you, to deal with stuff like public ip/visibility.

* As proxy server:  
    Would you like to have a **dedicated server** solution? Server will be used as a proxy to store and send accumulated player data.
    Considering the fact that you don't have (almost) any game logic on the server side, it can handle big amounts of clients.
    Similar example can be something like [San-Andreas Multiplayer](http://www.sa-mp.com/).

* As thin client (advanced):  
    For client side, you will use thin-client "pattern". All user actions will be sent directly to the server.
    Requires you to write all the server side systems, like physics handling, game world management, and **gameloop on the server side**.

* Any other possible way that was not mentioned.

## Testing

We started testing the library for different platforms. This table provides some sort of description for compatibility.
If you have tested it, and it compiles, or it perhaps stopped compiling, please feel free to describe the issue in the [issues](https://github.com/librg/librg/issues).

| *Compiler* / Target   | Windows               | Linux                 | macOS                 | iOS                   | Android               |
|:-:|:-:|:-:|:-:|:-:|:-:    |
| *clang C*             | :grey_question:       | :white_check_mark:    | :white_check_mark:    | :white_check_mark:    | :grey_question:       |
| *gcc C*               |                       | :white_check_mark:    | :white_check_mark:    |                       |                       |
| *msvc C*              | :white_check_mark:    |                       |                       |                       |                       |
| *mingw C*             | :white_check_mark:    |                       |                       |                       |                       |
| *clang C++*           | :grey_question:       | :white_check_mark:    | :white_check_mark:    | :grey_question:       | :white_check_mark:    |
| *gcc C++*             |                       | :white_check_mark:    | :grey_question:       |                       |                       |
| *msvc C++*            | :white_check_mark:    |                       |                       |                       |                       |
| *mingw C++*           | :white_check_mark:    |                       |                       |                       |                       |

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
