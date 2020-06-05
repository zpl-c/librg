# Migration guide

There were multiple major changes, to the library coming from version `5.*.*` to version `6.0.0`.
This page attempts to summirize most of them so that existing programs could be migrated much more quickly.

Biggest change coming from an older version, is obvious lack of any methods related to the networking.
Decoupling the library from the netwokring part allows it to have a much better focus on what it was and is supposed to do.
Additionally, that change now makes it possible to use any other networking library, stack or even approach, since it is working with much simpler primitives.

## Naming

1. `librg_ctx` -> `librg_world` What used to be known as context, now is named a world.
2. `librg_client` -> `owner` Since library does not handle networking, a client was an inappropriate name for it.
3. `entity controller` -> `entity owner` That name better represents the relation of the owner to an entity
4. `stream range` -> `view radius` Still describes a radius around an entity, but under a more appropriate name.
5. `librg_entity_create` -> `librg_entity_track` A changed name, that attempts to show that entities are coming from the host application, and the library is actually only "tracking" them.
5. `librg_entity_destroy` -> `librg_entity_untrack` Same point as in explanation above.
5. All entity run-time events where essentially split onto 2 main categories, `write`-related and `read`-related:
    * `LIBRG_WRITE_CREATE`
    * `LIBRG_WRITE_UPDATE`
    * `LIBRG_WRITE_REMOVE`
    * `LIBRG_READ_CREATE`
    * `LIBRG_READ_UPDATE`
    * `LIBRG_READ_REMOVE`

## Structures

After version `6.0.0` all "public" structures were moved to an hidden internal part of the code, and outside those are provided only as a set of opaque pointers.
Everything else is provided via funcitons/methods, which use only primitive standart types, with an exception of user-provided buffers for arrays.
That approach should make it much easier to use the library with bindings for different languages, since no structures need to be "re-created" on the binding side.

## Positioning

Older version of the library havily relied on a vector based real positions to manage the visibility and distance checking.
Additionally internally it operated based on a dynamically allocated, variable-sized sets of structures that were used for culling.

Current version simplifies this approach a lot, by getting rid of actual positions, and rather relying on the integer based chunk identifiers.
And all distance checks are replaced with a simple integer comparation.
That approach reduces complexity, and should result in much less posibilities for edge cases and bugs occuring in the run-time.

## Querying

Previous version relied on `iteration` based methods to go over all list of the entities.
Current version is based on user-provided buffers where the entities are copied to,
alongside the total amount of data that was written.

## Buffers and data

Older version relied heavily on `librg_data` structure, and methods for writing and reading various types of data primitives.
This was dropped to reduce overall complexity of the library, and allowing users to actually rely on existing set of libraries that do that specific job and do it very well:

 * [protobuf](https://developers.google.com/protocol-buffers/)
 * [flatbuffers](https://google.github.io/flatbuffers/)
 * [msgpack](https://msgpack.org/)
 * [cap'n'proto](https://capnproto.org/)
 * any other serialization library not in that list
 * or good old `memcpy` :)

## Networking integration

Since networking part of the library was dropped, the new overall interface of the library was made in such a way that majority of the networking libraries could be supported.

All you would need to have from a library is:

1. Ability to send and receive a `char *` buffer
2. Ability to read or set that buffer size
3. Ability to indentify who is the receiver or sender of the data with an integer id

And that is pretty much it!

Just a theoretical list of what kind of libraries should be supported:

* `ENet`
* `yojimbo`
* `Raknet/SLikeNet`
* `Websocket`
* `WebRTC`
* Any other `UDP` or `TCP` based library

> Note: you can check an example for network [integration for enet](https://github.com/zpl-c/librg/blob/master/code/apps/example-enet.c).

## Other

This section will be expanded with additional questions and answers coming from the users.

Feel free to do the same, by opening an [issue](https://github.com/zpl-c/librg/issues) or asking us directly in our [discord](https://discord.gg/2fZVEym).
