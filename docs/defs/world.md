# World manipulation methods

World can be considered a container of sorts, responsible for tracking a set of entities and their relationships.
Single application can have a theoretically unlimited amout of worlds created. Tracked entities can share multiple worlds.

## librg_world_create

This function is responsible for creating a [world](defs/types.md#world) instance.
Internally it allocates all needed memory for the main world structure, and returns an opaque pointer.
Developer can choose to create as many worlds as he needs to. All [world](defs/types.md#world) pointers will need to be freed (or destroyed) by [librg_world_destroy](#librg_world_destroy) method.

> Note: All other library calls require this pointer to be provided.

##### Signature
```c
librg_world * librg_world_create();
```

##### Returns

* In case of success this function returns an opaque pointer, which you will need to later use in other calls
* In case of error - the return will be a `NULL` value

-------------------------------

## librg_world_destroy

This function is responsible for destroying a [world](defs/types.md#world) instance.
Internally it frees all needed memory for the main world structure, and returns a result indicating if the call succeeded.

> Note: after you've destroyed a world, it is recommended to manually set it's pointer to `NULL`, to prevent it from being considered as valid by other methods.

##### Signature
```c
int8_t librg_world_destroy(
    librg_world *world
)
```

##### Returns

* In case of success return code is `LIBRG_OK` (defined as `0`)
* In case of error return code is `LIBRG_WORLD_INVALID`

-------------------------------

## librg_world_valid

Method allows you to check whether provided world is a valid one.

##### Signature
```c
int8_t librg_world_valid(
    librg_world *world
)
```

##### Returns

* In case of success return code is `LIBRG_TRUE` (defined as `1`)
* In all other cases return code is `LIBRG_FALSE` (defined as `0`)

-------------------------------

## librg_world_userdata_set

Method allows you to store a custom peice of data (pointer to data) in the current world container.
As soon as it is set, the data will be kept, until the world will be destroyed, or data will be overwritten by another call to this method.

Existing data can be fetched using [librg_world_userdata_get](#librg_world_userdata_get) method.

##### Signature
```c
int8_t librg_world_userdata_set(
    librg_world *world,
    void *data
)
```

##### Returns

* In case of success return code is `LIBRG_OK` (defined as `0`)
* In case of error return code is `LIBRG_WORLD_INVALID`

-------------------------------

## librg_world_userdata_get

Method can be used to fetch currently stored data in the world, that was previously pushed there by [librg_world_userdata_set](#librg_world_userdata_set) method.
If no data was ever pushed, the default value will be `NULL`.

##### Signature
```c
void * librg_world_userdata_get(
    librg_world *world
)
```

##### Returns

* In case of success returned value is the data
* In all other cases returned value is `NULL`

-------------------------------

## librg_version

Method returns current library version in form of an integer. Can be used to compare against different librg versions.

##### Signature
```c
int32_t librg_version()
```

##### Returns

* Integer representing current version

-------------------------------

## **Examples**

A general example that shows how all methods above can be used together

[example-world.c](https://raw.githubusercontent.com/zpl-c/librg/master/code/apps/example-world.c ':include :type=code')

