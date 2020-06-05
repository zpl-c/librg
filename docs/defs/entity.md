# Entities

The library provides a way for developer to attach (track) his own entities, and deal with issues of ownership and visibility.

## librg_entity_track

Method takes a number that represents a unique entity identifier inside of your own system/application. The entity_id is considered valid if it is in range of `[0 ... MAX_SIGNED_INT64]`.
Internally it uses a hash-table to store pairs of id <-> entity data struct. There is an allocation happening inside of the method, mainly in cases when the hashtable needs to grow.

##### Signature
```c
int8_t librg_entity_track(
    librg_world *world,
    int64_t entity_id
)
```

##### Returns

* In case of success: `LIBRG_OK`
* In case of invalid world: `LIBRG_WORLD_INVALID`
* In case of invalid entity id: `LIBRG_ENTITY_INVALID`
* In case of collision (`entity_id` is taken): `LIBRG_ENTITY_ALREADY_TRACKED`

------------------------------

## librg_entity_untrack

Method removes provided entity from internal storage, cleaning up anything that was allocated within the context of that entity during time it was being tracked.
Cannot be called on entities that were tracked in the world, different from current (check [librg_entity_foreign](#librg_entity_foreign) for more info).

If entity is owned at the moment when it is being untracked, additional routine will be launched, that will scan all entities owned by the same owner,
and in case there are no more found, any references about the owner will be removed from internal storage.

Additionally, if [librg_entity_visibility_owner_set](#librg_entity_visibility_owner_set) was ever used on this entity, it also will launch a routine to cleanup the internal storage for that.

##### Signature
```c
int8_t librg_entity_untrack(
    librg_world *world,
    int64_t entity_id
)
```

##### Returns

* In case of success: `LIBRG_OK`
* In case of invalid world: `LIBRG_WORLD_INVALID`
* In case of unknown entity: `LIBRG_ENTITY_UNTRACKED`
* In case of foreign entity: `LIBRG_ENTITY_FOREIGN`

------------------------------

## librg_entity_tracked

Checks if particular entity is being tracked

##### Signature
```c
int8_t librg_entity_tracked(
    librg_world *world,
    int64_t entity_id
)
```

##### Returns

* In case of truth: `LIBRG_TRUE`
* In all other cases: `LIBRG_FALSE`

------------------------------

## librg_entity_foreign

Checks if a particular entity is actually a foreign entity.

Foreign entity is an entity that was tracked in a different [world](defs/types.md#world),
and was replicated in this world as an instance via [librg_world_read](defs/query.md#librg_world_read) method.
Such entity cannot change it's current owner, nor it cannot be manually untracked in this world.
The only way it can disappear from this world is by remove "instruction" from world where it has originated from also via [librg_world_read](defs/query.md#librg_world_read) call.

##### Signature
```c
int8_t librg_entity_foreign(
    librg_world *world,
    int64_t entity_id
)
```

##### Returns

* In case of truth: `LIBRG_TRUE`
* In all other cases: `LIBRG_FALSE`

------------------------------

## librg_entity_count

A helper method that returns current amount of entities that are being tracked in the world

##### Signature
```c
int32_t librg_entity_count(
    librg_world *world
)
```

##### Returns

* In case of success: a positive number represeting current count
* In case of invalid world: `LIBRG_WORLD_INVALID`

-------------------------------

## librg_entity_userdata_set

Method allows you to store a custom peice of data (pointer to data) in the internal entity structure.
As soon as it is set, the data will be kept, until the entity get untracked, or data overwritten by another call to this method.

Existing data can be fetched using [librg_entity_userdata_get](#librg_entity_userdata_get) method.

##### Signature
```c
int8_t librg_entity_userdata_set(
    librg_world *world,
    int64_t entity_id,
    void *data
)
```

##### Returns

* In case of success: `LIBRG_OK`
* In case of invalid world: `LIBRG_WORLD_INVALID`
* In case of unknown entity: `LIBRG_ENTITY_UNTRACKED`

-------------------------------

## librg_entity_userdata_get

Method can be used to fetch currently stored data in the entity, that was previously pushed there by [librg_entity_userdata_set](#librg_entity_userdata_set) method.
If no data was ever pushed, the default value will be `NULL`.

##### Signature
```c
void * librg_entity_userdata_get(
    librg_world *world,
    int64_t entity_id
)
```

##### Returns

* In case of success: pointer with the data
* In all other cases: `NULL`

------------------------------

## librg_entity_chunk_set

Sets current "chunk" location of the entity.
Since entity can simultaneously be located in up to `8` chunks by default ([compile-time configuration](compiletime.md)),
setting this value overwrites any existing values for this entity and entity is placed strictly in a single chunk.

To set entity location to multiple chunks at the same time, you can use [librg_entity_chunkarray_set](#librg_entity_chunkarray_set) method.

##### Signature
```c
int8_t librg_entity_chunk_set(
    librg_world *world,
    int64_t entity_id,
    librg_chunk chunk
)
```

##### Returns

* In case of success: `LIBRG_OK`
* In case of invalid world: `LIBRG_WORLD_INVALID`
* In case of unknown entity: `LIBRG_ENTITY_UNTRACKED`

------------------------------

## librg_entity_chunk_get

Gets current "chunk" location of the entity.
Since entity can simultaneously be located in up to `8` chunks by default ([compile-time configuration](compiletime.md)),
the returned value will be the first chunk id element in the array of chunks that is entity currently in.

> Note: Unless you are sure that entity is located only in a single chunk, it might be better to use method provided below

To get entity location within multiple chunks at the same time, you can use [librg_entity_chunkarray_get](#librg_entity_chunkarray_get) method.

##### Signature
```c
librg_chunk librg_entity_chunk_get(
    librg_world *world,
    int64_t entity_id
)
```

##### Returns

* In case of success: a valid [chunk](defs/types.md#chunk) id
* In case of invalid world: `LIBRG_WORLD_INVALID`
* In case of unknown entity: `LIBRG_ENTITY_UNTRACKED`
* In all other cases: `LIBRG_CHUNK_INVALID`

------------------------------

## librg_entity_chunkarray_set

Sets current "chunk" location of the entity in form of an array.
Since entity can simultaneously be located in up to `8` chunks by default ([compile-time configuration](compiletime.md)),
setting this value overwrites any existing values for this entity and entity is placed in all of the chunks provided in argugments.
The chunk ids will be copied from the buffer that you provide into an internal storage, so you do not need to keep that buffer allocated after the call.

> Note: last argument tells method how many elemets are in the array, however no more than `8` (by default) will be used

##### Signature
```c
int8_t librg_entity_chunkarray_set(
    librg_world *world,
    int64_t entity_id,
    const librg_chunk *values, /* in */
    size_t chunk_amount
)
```

##### Returns

* In case of success: `LIBRG_OK`
* In case of invalid world: `LIBRG_WORLD_INVALID`
* In case of unknown entity: `LIBRG_ENTITY_UNTRACKED`

------------------------------

## librg_entity_chunkarray_get

Gets current chunk positions of the entity.
Since entity can simultaneously be located in up to `8` chunks by default ([compile-time configuration](compiletime.md)),
the returned value will be copied from an internal buffer to the array you provide.

> Note:
> * last argument tells method maximum number of elements of your array, and the method will respect that count
> * last argument is in-out reference value, the resulting count will be written back to that variable
> * please make sure to always provide a buffer of appropriately sized length, since you might not read all of the existing data

##### Signature
```c
int8_t librg_entity_chunkarray_get(
    librg_world *world,
    int64_t entity_id,
    librg_chunk *values, /* out */
    size_t *chunk_amount /* in-out */
)
```

##### Returns

* In case of success: `LIBRG_OK`
* Alternatively, in case of success: positive aproximated amount by which your buffer should be increased
* In case of invalid world: `LIBRG_WORLD_INVALID`
* In case of unknown entity: `LIBRG_ENTITY_UNTRACKED`

------------------------------

## librg_entity_dimension_set

Sets current entity dimension.
Any given entity can be located only inside a single given dimension at any given time.
There is no limit on how many entities can share a given dimension.

##### Signature
```c
int8_t librg_entity_dimension_set(
    librg_world *world,
    int64_t entity_id,
    int32_t dimension
)
```

##### Returns

* In case of success: `LIBRG_OK`
* In case of invalid world: `LIBRG_WORLD_INVALID`
* In case of unknown entity: `LIBRG_ENTITY_UNTRACKED`

------------------------------

## librg_entity_dimension_get

Gets current entity dimension.

##### Signature
```c
int32_t librg_entity_dimension_get(
    librg_world *world,
    int64_t entity_id
)
```

##### Returns

* In case of success: number representing a dimension
* In case of invalid world: `LIBRG_WORLD_INVALID`
* In case of unknown entity: `LIBRG_ENTITY_UNTRACKED`

------------------------------

## librg_entity_owner_set

Sets current entity owner.
Owner, is an artificial concept that represents a relation of the entity to someone or something else.

Making an entity owned allows to do multiple things:

* Fetching entities owned by specific user via [librg_world_fetch_owner](def/query.md#librg_world_fetch_owner)
* Querying nearby entities based of current [radius](librg_entity_radius_set) via [librg_world_query](def/query.md#librg_world_query)
* Setting up global and relational visibility via [librg_entity_visibility_owner_set](#librg_entity_visibility_owner_set) that will be used by query methods
* Writing and reading world (serialization and replication) via [librg_world_write](def/query.md#librg_world_write) methods

Each call to the method will generate and set a new ownership token, which is in turn will be used in the next [librg_world_write](def/query.md#librg_world_write) call.
Ownership token is used to validate control settings for an entity, and with outdated token and updates coming into
[librg_world_read](def/query.md#librg_world_read) of local world that are targeting the entity will be discarded.
For more details please refer to the documentation for those methods.

##### Signature
```c
int8_t librg_entity_owner_set(
    librg_world *world,
    int64_t entity_id,
    int64_t owner_id
)
```

##### Returns

* In case of success: `LIBRG_OK`
* In case of invalid world: `LIBRG_WORLD_INVALID`
* In case of unknown entity: `LIBRG_ENTITY_UNTRACKED`

------------------------------

## librg_entity_owner_get

Returns current entity owner.

##### Signature
```c
int64_t librg_entity_owner_get(
    librg_world *world,
    int64_t entity_id
)
```

##### Returns

* In case of success: number representing an owner
* In case of invalid world: `LIBRG_WORLD_INVALID`
* In case of unknown entity: `LIBRG_ENTITY_UNTRACKED`

------------------------------

## librg_entity_radius_set

Sets current entity visibility radius.

Visibility radius influences only entities that are owned.
It represents a linear/circular/spherical (depending on world configuration) radius of entity visiblity in terms of nearby chunks,
and used whilist general visibility calculations in the [librg_world_query](def/query.md#librg_world_query) method.
If property set for an entity that is not owned, but the owner is later on changed, it will be still applied, since the value is stored in the internal storage.

##### Signature
```c
int8_t librg_entity_radius_set(
    librg_world *world,
    int64_t entity_id,
    int8_t observed_chunk_radius
)
```

##### Returns

* In case of success: `LIBRG_OK`
* In case of invalid world: `LIBRG_WORLD_INVALID`
* In case of unknown entity: `LIBRG_ENTITY_UNTRACKED`

------------------------------

## librg_entity_radius_get

Returns current entity visibility radius.

##### Signature
```c
int8_t librg_entity_radius_get(
    librg_world *world,
    int64_t entity_id
)
```

##### Returns

* In case of success: number representing current radius
* In case of invalid world: `LIBRG_WORLD_INVALID`
* In case of unknown entity: `LIBRG_ENTITY_UNTRACKED`

------------------------------

## librg_entity_visibility_global_set

Set entity global visibility value.
It affects how this entity will be viewed by other owners.

Global visibility has lower priority than relation (owner) visibility,
meaning globally invisible entity, will still be visibile to an owner if it has `LIBRG_VISIBLITY_ALWAYS` set for that owner and vice verca.

Possible values:
 * `LIBRG_VISIBLITY_DEFAULT` - the default value of the visibility setting
 * `LIBRG_VISIBLITY_NEVER` - specified entity will be always invisible, regardless of proximity
 * `LIBRG_VISIBLITY_ALWAYS` - specified entity will be always visible, regardless of proximity

> Note: For the owner of the entity the entity will remain visible regardless of the visibility setting.

##### Signature
```c
int8_t librg_entity_visibility_global_set(
    librg_world *world,
    int64_t entity_id,
    librg_visibility value
)
```

##### Returns

* In case of success: `LIBRG_OK`
* In case of invalid world: `LIBRG_WORLD_INVALID`
* In case of unknown entity: `LIBRG_ENTITY_UNTRACKED`

------------------------------

## librg_entity_visibility_global_get

Get entity global visibility value.
It affects how this entity will be viewed by other owners.

Possible values:
 * `LIBRG_VISIBLITY_DEFAULT` - the default value of the visibility setting
 * `LIBRG_VISIBLITY_NEVER` - specified entity will be always invisible, regardless of proximity
 * `LIBRG_VISIBLITY_ALWAYS` - specified entity will be always visible, regardless of proximity

> Note: For the owner of the entity the entity will remain visible regardless of the visibility setting.

##### Signature
```c
int8_t librg_entity_visibility_global_get(
    librg_world *world,
    int64_t entity_id
)
```

##### Returns

* In case of success: current visibility value
* In case of invalid world: `LIBRG_WORLD_INVALID`
* In case of unknown entity: `LIBRG_ENTITY_UNTRACKED`

------------------------------

## librg_entity_visibility_owner_set

Set entity relational visibility value.
It affects how this entity will be viewed by provided owner.

Relational (owner) visibility has higher priority than global visibility,
meaning globally invisible entity, will still be visibile to an owner if it has `LIBRG_VISIBLITY_ALWAYS` set for that owner and vice verca.


Possible values:
 * `LIBRG_VISIBLITY_DEFAULT` - the default value of the visibility setting
 * `LIBRG_VISIBLITY_NEVER` - specified entity will be always invisible, regardless of proximity
 * `LIBRG_VISIBLITY_ALWAYS` - specified entity will be always visible, regardless of proximity

> Note: For the owner of the entity the entity will remain visible regardless of the visibility setting.

##### Signature
```c
int8_t librg_entity_visibility_owner_set(
    librg_world *world,
    int64_t entity_id,
    int64_t owner_id,
    librg_visibility value
)
```

##### Returns

* In case of success: `LIBRG_OK`
* In case of invalid world: `LIBRG_WORLD_INVALID`
* In case of unknown entity: `LIBRG_ENTITY_UNTRACKED`

------------------------------

## librg_entity_visibility_owner_get

Get entity relational visibility value.
It affects how this entity will be viewed by provided owner.

Possible values:
 * `LIBRG_VISIBLITY_DEFAULT` - the default value of the visibility setting
 * `LIBRG_VISIBLITY_NEVER` - specified entity will be always invisible, regardless of proximity
 * `LIBRG_VISIBLITY_ALWAYS` - specified entity will be always visible, regardless of proximity

> Note: For the owner of the entity the entity will remain visible regardless of the visibility setting.

##### Signature
```c
int8_t librg_entity_visibility_owner_get(
    librg_world *world,
    int64_t entity_id,
    int64_t owner_id
)
```

##### Returns

* In case of success: current visibility value
* In case of invalid world: `LIBRG_WORLD_INVALID`
* In case of unknown entity: `LIBRG_ENTITY_UNTRACKED`
