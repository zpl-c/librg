## Introduction

**librg** is a light-weight "mid-level" library, that can be used to serve as a middleware between data-transferring libraries (networking, file-streaming, etc.) and core application/game logic.
Main responsibilities of the library include:

 * entity tracking (tracks which entity belongs to what world, and what states do they have)
 * owner tracking (tracks which participant owns what entity)
 * area of interest management (controls what can and cannot be seen a participant)
 * world replication (re-creates a limited representation in a destination world of what is considered visible to a participant in the source world)

The library was born to solve complexities of setting up and managing the flow of multi-player games and dedicated game servers.
It came a long way of stripping out things that were non-essential, slowly sculpting in its current form, which you are able to see and use today.

## Features

 * cross-platform support
 * light-weight, single-header
 * compile- and run-time configurable
 * written in C99 (portability reasons)
 * no external dependencies
 * built-in unit test coverage

## F.A.Q.

> **Question**: Is this a networking library?

**Answer**: Not really, no. It is inteded to be used with netwoking in mind, but it does not have any networking capabilities on its own.

> **Question**: Can I use any networking library with it?

**Anwser**: Yes. All you need is an ability to read data to and from the buffer, and most libraries do support that.

> **Question**: The repository contains a bunch of `*.h` and `*.c` files, and yet you suggest it is a single-header library, how is that possible?

**Answer**: The library is spread into multiple files so it is easier to work with it while developing, however each time a new release is created, a "bundled" version of the header file is created and pushed directly to the [releases](https://github.com/zpl-c/librg/releases) page.

> **Question**: You've mentioned entities in the description above, does it mean I would need to use another entity system?

**Answer**: No, the library does not manage nor actually create it's own entities, it rather expects you to provide your entity/object id to attach some internal data onto it,
which in context of the library we call "tracking".
