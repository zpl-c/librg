# World configuration methods

All world configuration methods can be called at runtime. There will be no re-allocations happening, since methods only change world properties,
and mostly only influece how chunk id will be calculated.

## librg_config_chunkamount_set

Method allows you to set current amount of chunks in each axis. Minimal value for each axis is `1`. The best alternative name for this property is **world size**.
As soon as it is set, the data will be kept, until the world will be destroyed, or data will be overwritten by another call to this method.

Existing set of values can be fetched using [librg_config_chunkamount_get](#librg_config_chunkamount_get) method.

* Setting values of `[1, 1, 1]` is an equialent of having a chunk-less world, where all the entities will always be in the same chunk
* Setting values of `[16, 1, 1]` is an equialent of having a 1D world, consisting of 16 chunks (might be useful for side-scroller kind of things)
* Setting values of `[16, 16, 1]` is an equialent of having a 2D world, consisting of a grid 16x16 chunks (might be useful for top-down or isometric things)
* Setting values of `[16, 16, 16]` is an equialent of having a 3D world, consisting of a cube 16x16x16 chunks (might be useful for big 3d worlds)

##### Signature
```c
int8_t librg_config_chunkamount_set(
    librg_world *world,
    uint16_t x,
    uint16_t y,
    uint16_t z
)
```

##### Returns

* In case of success return code is `LIBRG_OK` (defined as `0`)
* In case of error return code is `LIBRG_WORLD_INVALID`

-------------------------------

## librg_config_chunkamount_get

Method allows you to get current amounts of chunks in each axis, that was previously pushed there by [librg_config_chunkamount_set](#librg_config_chunkamount_set) method.
If no data was ever pushed, the default set of values will be as following: `[256, 256, 256]`.

> Note: in case of success, the returned values will be put into variables you've provided by the reference

##### Signature
```c
int8_t librg_config_chunkamount_get(
    librg_world *world,
    uint16_t *x, /* out */
    uint16_t *y, /* out */
    uint16_t *z  /* out */
)
```

##### Returns

* In case of success return code is `LIBRG_OK` (defined as `0`)
* In case of error return code is `LIBRG_WORLD_INVALID`

-------------------------------

## librg_config_chunksize_set

Method allows you to set current size of a chunk in each axis. The best alternative name for this property is **chunk unit size**.
As soon as it is set, the data will be kept, until the world will be destroyed, or data will be overwritten by another call to this method.

Unit, in context of your application, can represent any arbitrary value of any scale (`pixel`, `meter`, `inch`, etc.)

Existing set of values can be fetched using [librg_config_chunksize_get](#librg_config_chunksize_get) method.

##### Signature
```c
int8_t librg_config_chunksize_set(
    librg_world *world,
    uint16_t x,
    uint16_t y,
    uint16_t z
)
```

##### Returns

* In case of success return code is `LIBRG_OK` (defined as `0`)
* In case of error return code is `LIBRG_WORLD_INVALID`

-------------------------------

## librg_config_chunksize_get

Method can be used to fetch currently stored size of a chunk in each axis, that was previously pushed there by [librg_config_chunksize_set](#librg_config_chunksize_set) method.
If no data was ever pushed, the default set of values will be as following: `[16, 16, 16]`.

> Note: in case of success, the returned values will be put into variables you've provided by the reference

##### Signature
```c
int8_t librg_config_chunksize_get(
    librg_world *world,
    uint16_t *x, /* out */
    uint16_t *y, /* out */
    uint16_t *z  /* out */
)
```

##### Returns

* In case of success return code is `LIBRG_OK` (defined as `0`)
* In case of error return code is `LIBRG_WORLD_INVALID`

-------------------------------

## librg_config_chunkoffset_set

Method allows you set rules for how much the each chunk axis will be offset from the original value when calculating the chunk ids.
As soon as it is set, the data will be kept, until the world will be destroyed, or data will be overwritten by another call to this method.

Possible values for a single axis:
* `LIBRG_OFFSET_MID` - the **default** value, chunk id will be calculated based on the half offset of the chunk amount.
    So in case chunk amount is `256`, and target position is `0`, the relative chunk position will be resolved to `256/2=128`
* `LIBRG_OFFSET_BEG` - chunk id will be calculated without offset.
    So in case chunk amount is `256`, and target position is `0`, the relative chunk position will be resolved to `0`
* `LIBRG_OFFSET_END` - chunk id will be calculated based on full offset of the chunk amount.
    So in case chunk amount is `256`, and target position is `0`, the relative chunk position will be resolved to `256`
* Any arbitrary value (positive/negative) can be used as well. It will be used in the caclulations directly
    So target position is `0`, the relative chunk position will be resolved to `0+value`

Existing set of values can be fetched using [librg_config_chunkoffset_get](#librg_config_chunkoffset_get) method.

##### Signature
```c
int8_t librg_config_chunkoffset_set(
    librg_world *world,
    int16_t x,
    int16_t y,
    int16_t z
)
```

##### Returns

* In case of success return code is `LIBRG_OK` (defined as `0`)
* In case of error return code is `LIBRG_WORLD_INVALID`

-------------------------------

## librg_config_chunkoffset_get

Method can be used to fetch currently stored chunk offsets in the world, that was previously pushed there by [librg_config_chunkoffset_set](#librg_config_chunkoffset_set) method.
If no data was ever pushed, the default set of values will be as following: `[LIBRG_OFFSET_MID, LIBRG_OFFSET_MID, LIBRG_OFFSET_MID]`.

> Note: in case of success, the returned values will be put into variables you've provided by the reference

##### Signature
```c
int8_t librg_config_chunkoffset_get(
    librg_world *world,
    int16_t *x, /* out */
    int16_t *y, /* out */
    int16_t *z  /* out */
)
```

##### Returns

* In case of success return code is `LIBRG_OK` (defined as `0`)
* In case of error return code is `LIBRG_WORLD_INVALID`
