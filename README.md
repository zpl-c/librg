<div align="center">
    <a href="https://github.com/zpl-c/librg"><img src="https://user-images.githubusercontent.com/2182108/83804073-40d23c80-a6b6-11ea-8bee-52a42ed45bed.png" alt="librg" /></a>
</div>

<div align="center">
    <a href="https://github.com/zpl-c/librg/actions"><img src="https://img.shields.io/github/workflow/status/zpl-c/librg/tests?label=Tests&style=for-the-badge" alt="build status" /></a>
    <img src="https://img.shields.io/github/package-json/v/zpl-c/librg?style=for-the-badge" alt="version" /></a>
    <a href="https://discord.gg/2fZVEym"><img src="https://img.shields.io/discord/354670964400848898?style=for-the-badge" alt="discord" /></a>
    <a href="LICENSE"><img src="https://img.shields.io/github/license/zpl-c/librg?style=for-the-badge" alt="license" /></a>
</div>

<br />

<div align="center">
  Making multi-player simpler since 2017. Single-header cross-platform world replication library in pure C99.
</div>

<div align="center">
  <sub>
    Built with love using <a href="https://github.com/zpl-c/zpl"><strong>zpl</strong></a>
    &bull; Brought to you by <a href="https://github.com/inlife">@inlife</a>,
    <a href="https://github.com/zaklaus">@zaklaus</a>
    and <a href="https://github.com/zpl-c/librg/graphs/contributors">contributors</a>
  </sub>
</div>

## Introduction

**librg** is a light-weight "mid-level" library, to serve as a middleware between data-transferring libraries (networking, file-streaming, etc.) and core application/game logic.
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
If you have tested it, and it compiles, or it perhaps stopped compiling, please feel free to describe the issue in the [issues](https://github.com/zpl-c/librg/issues).

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

| *Platform*         | ✅ - compiles/runs | ☑️ - compiles with limited features | ❔ - not tested |
|:-------------------|:---:|:---:|:---:|
| **Windows**        | msvc, mingw | | |
| **macOS**          | gcc, clang | | |
| **OpenBSD**        | | | gcc, clang |
| **FreeBSD**        | | | |
| **Linux**          | gcc, clang | | |
| **iOS**            |  
| **Android**        |  
| **Raspberry Pi**   |  
| **Emscripten**     |  

|
|---|---|---|---|---|---|---|---|---|---|
| *clang* | :grey_question: | :ballot_box_with_check: | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark: | :grey_question: | :grey_question: |
| *gcc* | :white_check_mark: | :grey_question: | :white_check_mark: | :white_check_mark: | :white_check_mark: |  |  | :grey_question: | :ballot_box_with_check: |
| *msvc C* | :white_check_mark: |   |   |  |  |  |  |  |  |
| *mingw* | :white_check_mark: |   |   |  |  |  |  |  |  |
| *clang++* | :grey_question: | :ballot_box_with_check: | :white_check_mark: | :white_check_mark: | :white_check_mark: | :grey_question: | :white_check_mark: | :grey_question: | :grey_question: |
| *g++* | :white_check_mark: | :grey_question: | :white_check_mark: | :white_check_mark: | :grey_question: |  |  | :grey_question: | :ballot_box_with_check: |
| *msvc C++* | :white_check_mark: |   |   |  |  |  |  |  |  |
| *mingw++* | :white_check_mark: |   |   |  |  |  |  |  |  |


<sub>
:white_check_mark: - compiles/runs without any errors.  
:grey_question: - not tested.  
:red_circle: - compilation/runtime faults.  
</sub>

