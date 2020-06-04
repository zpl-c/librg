# General types

## world

An opaque pointer used by public api, that points to an internal persisted, allocated world structure

```c
typedef void librg_world;
```

## event

An opaque pointer used by public api, that points to a temporary event structure on the stack

```c
typedef void librg_event;
```

## chunk

A number that reprensents a single chunk, all negative values are considered invalid

```c
typedef int64_t librg_chunk;
```

## LIBRG_OK

```c
#define LIBRG_OK 0
```

## LIBRG_TRUE

```c
#define LIBRG_TRUE 1
```

## LIBRG_FALSE

```c
#define LIBRG_FALSE 0
```

## librg_event_fn

```c
typedef int32_t (*librg_event_fn)(
    librg_world *world,
    librg_event *event
);
```
