# Compile-time configuration

There are some options of the library that can be changed at compile time.
All of them need to be defined only for the implementation part of the code (the one that has `LIBRG_IMPL` or `LIBRG_IMPLEMENTATION` defined).

## LIBRG_DEBUG

A macro that defines [LIBRG_ASSERT](#librg_assert) macro. Automatically defined if `_DEBUG` macro is defined as well.

## LIBRG_ASSERT(x)

A helper macro that can be used for debugging purposes, mainly to catch `NULL` pointers in the flow of the code.
Can be redefined to any other function or macro.

### Example:

```c
#define LIBRG_IMPL
#define LIBRG_ASSERT(x) assert(x)
#include "librg.h"
#include <assert.h>
```

## LIBRG_ENTITY_MAXCHUNKS

Defines how many max chunks an entity can be located in simultaneously. Default value is `8`.
Can be redefined to increase the amount:

```c
#define LIBRG_IMPL
#define LIBRG_ENTITY_MAXCHUNKS 16
#include "librg.h"
```

## LIBRG_WORLDWRITE_MAXQUERY

Defines how many max entity ids could be used inside of the [librg_world_write](defs/packing.md#librg_world_write) call. Default value is `8192`.
Can be redefined to increase the amount:

```c
#define LIBRG_IMPL
#define LIBRG_WORLDWRITE_MAXQUERY 16000
#include "librg.h"
```
