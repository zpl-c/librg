# Utility methods

Generally used on different places of the application


## librg_chunk_from_chunkpos

A helper function to convert a set of chunk axis coordinates into a single [chunk](defs/types.md#chunk) id, which can be used to represent same coordinates.
Resulting value depends not only on the provided set of arguments, but also on the world configuration,
more specifically on the values that were set by [librg_config_chunkamount_set](defs/config.md#librg_config_chunkamount_set) method.

If resulting value goes outside of the valid bounds (being less than `0`, or bigger than a calculated world size `chunkamount(x * y * z)`), a `LIBRG_CHUNK_INVALID` will be returned instead.

##### Signature
```c
librg_chunk librg_chunk_from_chunkpos(
    librg_world *world,
    int16_t chunk_x,
    int16_t chunk_y,
    int16_t chunk_z
)
```

##### Returns

* In case of success returns a valid [chunk](defs/types.md#chunk) id
* In case of error returns `LIBRG_CHUNK_INVALID`

-------------------------------

## librg_chunk_from_realpos

A helper function to convert a set of "real" axis coordinates into a single [chunk](defs/types.md#chunk) id. Very similar to the [librg_chunk_from_chunkpos](#librg_chunk_from_chunkpos) method.
Resulting value depends not only on the provided set of arguments, but also on the world configuration,
more specifically on the values that were set by both [librg_config_chunkamount_set](defs/config.md#librg_config_chunkamount_set) and [librg_config_chunksize_set](defs/config.md#librg_config_chunksize_set) methods.

If resulting value goes outside of the valid bounds (being less than `0`, or bigger than a calculated world size `chunkamount(x * y * z)`), a `LIBRG_CHUNK_INVALID` will be returned instead.

##### Signature
```c
librg_chunk librg_chunk_from_realpos(
    librg_world *world,
    double x,
    double y,
    double z
);
```

##### Returns

* In case of success returns a valid [chunk](defs/types.md#chunk) id
* In case of error returns `LIBRG_CHUNK_INVALID`

-------------------------------

## librg_chunk_to_chunkpos

A helper function, that does opposite of what [librg_chunk_from_chunkpos](#librg_chunk_from_chunkpos) is supposed to do.
It "coverts" a single [chunk](defs/types.md#chunk) id, into a set of values (chunk coordinates) that correspond to the specified chunk.
Relies on both [librg_config_chunkamount_set](defs/config.md#librg_config_chunkamount_set) and [librg_config_chunksize_set](defs/config.md#librg_config_chunksize_set) methods to calculate the values.

> Note: resulting values will be written to the arguments that are passed by reference.

##### Signature
```c
int8_t librg_chunk_to_chunkpos(
    librg_world *world,
    librg_chunk id,
    int16_t *chunk_x, /* out */
    int16_t *chunk_y, /* out */
    int16_t *chunk_z  /* out */
)
```

##### Returns

* In case of success: `LIBRG_OK`
* In case of invalid world: `LIBRG_WORLD_INVLAID`
* In case of invalid chunk id: `LIBRG_CHUNK_INVALID`
