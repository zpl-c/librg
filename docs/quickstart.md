# Quick start

A simple guide with insturction on how to setup, configure, compile and use the library.

## Downloading

First thing you need to do is to get/install the library, and the easiest way of doing that would be downloading it from our releases section

```sh
$ wget https://github.com/zpl-c/librg/releases/latest/download/librg.h
```

or via shorter link:

```sh
$ wget http://zpl.pw/fetch/librg
```

Alternatively, you can just visit [releases](https://github.com/zpl-c/librg/releases/) page, and grab a release manually.

## Installation

To add `librg` to your project, just include it.

```c
#include "librg.h"
```

This, however, will include only a **header** section of the library.
And if you try to compile your project, linker will eventually tell you that you lack implemntation methods for bunch of functions.
That happens because the implemntation part of the library is never compiled.

To fix that, choose one of your source files (any `*.c` or `*.cpp` file), and define there a macro `LIBRG_IMPL` before including the library, just like so:

```c
#define LIBRG_IMPL
#include "librg.h"
```

That will cause your compiler to include the source part of the library and compile it alonside your application.
Additionally, to keep library code and your code fully separated at compile time, you can create a special source file, consisting only of 2 lines of code shown above.


## First world

This small example shows how can you create your first world, confgure it, add an entity, and use a querying and buffer packing methods.

[example-simple.c](https://raw.githubusercontent.com/zpl-c/librg/master/code/apps/example-simple.c ':include :type=code')
