# General library methods

## World manipulation methods

World can be considered a container of sorts, responsible for tracking a set of entities and their relationships.
Single application can have a theoretically unlimited amout of worlds created. Tracked entities can share multiple worlds.

### `fn` librg_world_create

This function is responsible for creating a world instance.
Internally it allocates all needed memory for the main world structure, and returns an opaque pointer.
Developer can choose to create as many worlds as he needs to. All `world` pointers will need to be freed (or destroyed) by [librg_world_destroy](#fn-librg_world_destroy) method.

> Note: All other library calls require this pointer to be provided.

#### Signature
```c
librg_world * librg_world_create();
```

#### Returns

* In case of success this function returns an opaque pointer, which you will need to later use in other calls
* In case of error - the return will be a `NULL` value

-------------------------------

### `fn` librg_world_destroy

This function is responsible for destroying a world instance.
Internally it frees all needed memory for the main world structure, and returns a result indicating if the call succeeded.

> Note: after you've destroyed a world, it is recommended to manually set it's pointer to `NULL`, to prevent it from being considered as valid by other methods.

#### Signature
```c
int8_t librg_world_destroy(librg_world *world);
```

#### Returns

* In case of success return code is `LIBRG_OK` (defined as `0`)
* In case of error return code is `LIBRG_WORLD_INVALID`

-------------------------------

### `fn` librg_world_valid

Method allows you to check whether provided world is a valid one.

#### Signature
```c
int8_t librg_world_valid(librg_world *world);
```

#### Returns

* In case of success return code is `LIBRG_TRUE` (defined as `1`)
* In all other cases return code is `LIBRG_FALSE` (defined as `0`)

-------------------------------

### `fn` librg_world_userdata_set

Method allows you to store a custom peice of data (pointer to data) in the current world container.
As soon as it is set, the data will be kept, until the world will be destroyed, or data will be overwritten by another call to this method.

Existing data can be fetched using [librg_world_userdata_get](#fn-librg_world_userdata_get) method.

#### Signature
```c
int8_t librg_world_userdata_set(librg_world *world, void *data);
```

#### Returns

* In case of success return code is `LIBRG_OK` (defined as `0`)
* In case of error return code is `LIBRG_WORLD_INVALID`

-------------------------------

### `fn` librg_world_userdata_get

Method can be used to fetch currently stored data in the world, that was previously pushed there by [librg_world_userdata_set](#fn-librg_world_userdata_set) method.
If no data was ever pushed, the default value will be `NULL`.

#### Signature
```c
void * librg_world_userdata_get(librg_world *world);
```

#### Returns

* In case of success returned value is the data
* In all other cases returned value is `NULL`

-------------------------------

### **Examples**

A general example that shows how all methods above can be used together

[example-world.c](https://raw.githubusercontent.com/zpl-c/librg/next/code/apps/example-world.c ':include :type=code')

-------------------------------














## World configuration methods

All world configuration methods can be called at runtime. There will be no re-allocations happening, since methods only change world properties,
and mostly only influece how chunk id will be calculated.

### `fn` librg_config_chunkamount_set

Method allows you to set current amount of chunks in each axis. Minimal value for each axis is `1`. The best alternative name for this property is **world size**.
As soon as it is set, the data will be kept, until the world will be destroyed, or data will be overwritten by another call to this method.

Existing set of values can be fetched using [librg_config_chunkamount_get](#fn-librg_config_chunkamount_get) method.

* Setting values of `[1, 1, 1]` is an equialent of having a chunk-less world, where all the entities will always be in the same chunk
* Setting values of `[16, 1, 1]` is an equialent of having a 1D world, consisting of 16 chunks (might be useful for side-scroller kind of things)
* Setting values of `[16, 16, 1]` is an equialent of having a 2D world, consisting of a grid 16x16 chunks (might be useful for top-down or isometric things)
* Setting values of `[16, 16, 16]` is an equialent of having a 3D world, consisting of a cube 16x16x16 chunks (might be useful for big 3d worlds)

#### Signature
```c
int8_t librg_config_chunkamount_set(librg_world *world, uint16_t x, uint16_t y, uint16_t z);
```

#### Returns

* In case of success return code is `LIBRG_OK` (defined as `0`)
* In case of error return code is `LIBRG_WORLD_INVALID`

-------------------------------

### `fn` librg_config_chunkamount_get

Method allows you to get current amounts of chunks in each axis, that was previously pushed there by [librg_config_chunkamount_set](#fn-librg_config_chunkamount_set) method.
If no data was ever pushed, the default set of values will be as following: `[256, 256, 256]`.

> Note: in case of success, the returned values will be put into variables you've provided by the reference

#### Signature
```c
int8_t librg_config_chunkamount_get(librg_world *world, uint16_t *x, uint16_t *y, uint16_t *z);
```

#### Returns

* In case of success return code is `LIBRG_OK` (defined as `0`)
* In case of error return code is `LIBRG_WORLD_INVALID`

-------------------------------

### `fn` librg_config_chunksize_set

Method allows you to store a custom peice of data (pointer to data) in the current world container.
As soon as it is set, the data will be kept, until the world will be destroyed, or data will be overwritten by another call to this method.

Existing set of values can be fetched using [librg_config_chunksize_get](#fn-librg_config_chunksize_get) method.

#### Signature
```c
int8_t librg_config_chunksize_set(librg_world *world, uint16_t x, uint16_t y, uint16_t z);
```

#### Returns

* In case of success return code is `LIBRG_OK` (defined as `0`)
* In case of error return code is `LIBRG_WORLD_INVALID`

-------------------------------

### `fn` librg_config_chunksize_get

Method can be used to fetch currently stored data in the world, that was previously pushed there by [librg_config_chunksize_set](#fn-librg_config_chunksize_set) method.
If no data was ever pushed, the default set of values will be as following: `[16, 16, 16]`.

> Note: in case of success, the returned values will be put into variables you've provided by the reference

#### Signature
```c
int8_t librg_config_chunksize_get(librg_world *world, uint16_t *x, uint16_t *y, uint16_t *z);
```

#### Returns

* In case of success return code is `LIBRG_OK` (defined as `0`)
* In case of error return code is `LIBRG_WORLD_INVALID`

-------------------------------

### `fn` librg_config_chunkoffset_set

Method allows you to store a custom peice of data (pointer to data) in the current world container.
As soon as it is set, the data will be kept, until the world will be destroyed, or data will be overwritten by another call to this method.

Existing set of values can be fetched using [librg_config_chunkoffset_get](#fn-librg_config_chunkoffset_get) method.

#### Signature
```c
int8_t librg_config_chunkoffset_set(librg_world *world, int16_t x, int16_t y, int16_t z);
```

#### Returns

* In case of success return code is `LIBRG_OK` (defined as `0`)
* In case of error return code is `LIBRG_WORLD_INVALID`

-------------------------------

### `fn` librg_config_chunkoffset_get

Method can be used to fetch currently stored data in the world, that was previously pushed there by [librg_config_chunkoffset_set](#fn-librg_config_chunkoffset_set) method.
If no data was ever pushed, the default set of values will be as following: `[0, 0, 0]`.

> Note: in case of success, the returned values will be put into variables you've provided by the reference

#### Signature
```c
int8_t librg_config_chunkoffset_get(librg_world *world, int16_t *x, int16_t *y, int16_t *z);
```

#### Returns

* In case of success return code is `LIBRG_OK` (defined as `0`)
* In case of error return code is `LIBRG_WORLD_INVALID`

-------------------------------

### **Examples**


## Event methods

### **Examples**


## Utility methods

### **Examples**


```c

// =======================================================================//
// !
// ! Events
// !
// =======================================================================//

typedef int32_t (*librg_event_fn)(librg_world *world, librg_event *event);

LIBRG_API int8_t            librg_event_set(librg_world *world, librg_event_type, librg_event_fn);
LIBRG_API int8_t            librg_event_remove(librg_world *world, librg_event_type);
LIBRG_API librg_event_type  librg_event_type_get(librg_world *world, librg_event *event);
LIBRG_API int64_t           librg_event_owner_get(librg_world *world, librg_event *event);
LIBRG_API int64_t           librg_event_entity_get(librg_world *world, librg_event *event);
LIBRG_API char *            librg_event_buffer_get(librg_world *world, librg_event *event);
LIBRG_API size_t            librg_event_size_get(librg_world *world, librg_event *event);
LIBRG_API int8_t            librg_event_userdata_set(librg_world *world, librg_event *event, void *userdata);
LIBRG_API void *            librg_event_userdata_get(librg_world *world, librg_event *event);

// =======================================================================//
// !
// ! Utility methods
// !
// =======================================================================//

LIBRG_API librg_chunk librg_chunk_from_realpos(librg_world *world, double x, double y, double z);
LIBRG_API librg_chunk librg_chunk_from_chunkpos(librg_world *world, int16_t chunk_x, int16_t chunk_y, int16_t chunk_z);
LIBRG_API int8_t      librg_chunk_to_chunkpos(librg_world *world, librg_chunk id, int16_t *chunk_x, int16_t *chunk_y, int16_t *chunk_z);
```
