# Querying

## librg_world_fetch_all

Method is used to fetch all tracked entities across all dimensions within the world.

> Note:
> * last argument tells method maximum number of elements of your array, and the method will respect that count
> * last argument is in-out reference value, the resulting count will be written back to that variable
> * please make sure to always provide a buffer of appropriately sized length, since you might not read all of the existing data

##### Signature
```c
int32_t librg_world_fetch_all(
    librg_world *world,
    int64_t *entity_ids,    /* out */
    size_t *entity_amount   /* in-out */
)
```

##### Returns

* In case of success: `LIBRG_OK`
* Alternatively, in case of success: positive aproximated amount by which your buffer should be increased
* In case of invalid world: `LIBRG_WORLD_INVALID`

------------------------------

## librg_world_fetch_chunk

Method is used to fetch all tracked entities across all dimensions within a single chunk.

> Note:
> * last argument tells method maximum number of elements of your array, and the method will respect that count
> * last argument is in-out reference value, the resulting count will be written back to that variable
> * please make sure to always provide a buffer of appropriately sized length, since you might not read all of the existing data

##### Signature
```c
int32_t librg_world_fetch_chunk(
    librg_world *world,
    librg_chunk chunk,
    int64_t *entity_ids,    /* out */
    size_t *entity_amount   /* in-out */
)
```

##### Returns

* In case of success: `LIBRG_OK`
* Alternatively, in case of success: positive aproximated amount by which your buffer should be increased
* In case of invalid world: `LIBRG_WORLD_INVALID`

------------------------------

## librg_world_fetch_chunkarray

Method is used to fetch all tracked entities across all dimensions within an array of chunks.

> Note:
> * last argument tells method maximum number of elements of your array, and the method will respect that count
> * last argument is in-out reference value, the resulting count will be written back to that variable
> * please make sure to always provide a buffer of appropriately sized length, since you might not read all of the existing data

##### Signature
```c
int32_t librg_world_fetch_chunkarray(
    librg_world *world,
    const librg_chunk *chunks,  /* in */
    size_t chunk_amount,
    int64_t *entity_ids,        /* out */
    size_t *entity_amount       /* in-out */
)
```

##### Returns

* In case of success: `LIBRG_OK`
* Alternatively, in case of success: positive aproximated amount by which your buffer should be increased
* In case of invalid world: `LIBRG_WORLD_INVALID`

------------------------------

## librg_world_fetch_owner

Method is used to fetch all tracked entities across all dimensions owned by a given owner.

> Note:
> * last argument tells method maximum number of elements of your array, and the method will respect that count
> * last argument is in-out reference value, the resulting count will be written back to that variable
> * please make sure to always provide a buffer of appropriately sized length, since you might not read all of the existing data

##### Signature
```c
int32_t librg_world_fetch_owner(
    librg_world *world,
    int64_t owner_id,
    int64_t *entity_ids,        /* out */
    size_t *entity_amount       /* in-out */
)
```

##### Returns

* In case of success: `LIBRG_OK`
* Alternatively, in case of success: positive aproximated amount by which your buffer should be increased
* In case of invalid world: `LIBRG_WORLD_INVALID`

------------------------------

## librg_world_fetch_ownerarray

Method is used to fetch all tracked entities across all dimensions owned by any of the given owners.

> Note:
> * last argument tells method maximum number of elements of your array, and the method will respect that count
> * last argument is in-out reference value, the resulting count will be written back to that variable
> * please make sure to always provide a buffer of appropriately sized length, since you might not read all of the existing data

##### Signature
```c
int32_t librg_world_fetch_ownerarray(
    librg_world *world,
    const int64_t *owner_ids,   /* in */
    size_t owner_amount,
    int64_t *entity_ids,        /* out */
    size_t *entity_amount       /* in-out */
)
```

##### Returns

* In case of success: `LIBRG_OK`
* Alternatively, in case of success: positive aproximated amount by which your buffer should be increased
* In case of invalid world: `LIBRG_WORLD_INVALID`

------------------------------

## librg_world_query

Method is used for spatial entity filtering.
It returns all entities that are "visible" to a provided `owner_id`.

The visibility is calculated based on visibility "radius" of each entity that is owned by that owner, and returned as a single array of unique entries.
If entity was not properly placed onto a **valid chunk**, it will be filtered out from the query.
Additionally any visibility overrides are applied on per-entity basis, filtering out those entities that should be (in)visible for the given owner.

**Important**: owned entities will **always** be included in the query, even if they are located in the invalid chunk.

> Note:
> * last argument tells method maximum number of elements of your array, and the method will respect that count
> * last argument is in-out reference value, the resulting count will be written back to that variable
> * please make sure to always provide a buffer of appropriately sized length, since you might not read all of the existing data

##### Signature
```c
int32_t librg_world_query(
    librg_world *world,
    int64_t owner_id,
    int64_t *entity_ids,        /* out */
    size_t *entity_amount       /* in-out */
)
```

##### Returns

* In case of success: `LIBRG_OK`
* Alternatively, in case of success: positive aproximated amount by which your buffer should be increased
* In case of invalid world: `LIBRG_WORLD_INVALID`
