# Event handling

There 3 general event types that are shared between both reading and writing, they represent 3 states of the entity within a world: `created`, `updated`, `removed`.
Following that model, events then are separated onto 2 distinct cateogries:

* write-related (events that are triggered by [librg_world_write](defs/packing.md#librg_world_write) method)
* read-related (events that are triggered by [librg_world_read](defs/packing.md#librg_world_read) method)

### Write events

Write events can be considered as a simple dialogue interface, that asks you (developer), which additional data would you like to pack in, to the buffer,
so that it could be unpacked read and easily assosiated with a particular entity in the destination world.

Important note, write events are optional, and you don't have to handle them.
Additionally, every write event can be rejected, and that influece what will happen to it.
More details about rejection in each type below:

* `LIBRG_WRITE_CREATE` - called when an entity has become visible for the owner.
Might happen for a few reasons, for example it was just created, or it "walked" in his view radius, or one of the entities owned by owner was moved/teleported to a different place.
Both entity and owner might have existed in the world for quite long time, however, they never have intersected their view radius before.
Rejecting this event will prevent entity from being marked as created for that owner, which will cause this event to be called again in the next write method call.
* `LIBRG_WRITE_UPDATE` - called when an entity is considered known to the owner.
This event will be called on constant basis.
Rejecting this event will not do anything except skipping data portion for that entity.
This can be used as a with "update-only-when-changed" apporach, packing updates only when changes indeed occured, or creating some sort of throttling to control update frequency.
* `LIBRG_WRITE_REMOVE` - called when an entity disappeared from owner's view radius.
It might have been removed or just changed its position.
Rejecting this event will cause it to be called on the next call.
Note, however, this even is considered **dangerous**, since underlying entity might have been already untracked, use with caution.

### Read events

Read events can be considered as a set of instructions coming from source world to the destination world, telling which entities should be created/updated/removedº there,
passing all additional data that was written in the data buffers by inside custom event callbacks.

Each event contains a buffer, and a size, that tells how much extra data was written there.

* `LIBRG_READ_CREATE` - an instruction for destination world to create provided entity.
If a given entity id was already taken (a collision occured, with existing entity in that world), `LIBRG_ERROR_CREATE` will be called instead.
* `LIBRG_READ_UPDATE` - an instruction for destination world to update provided entity.
IF a given entity id is not tracked, or owner of the entity does not much the `owner_id` argument of the read method call,
or there is an ownership token mismatch, `LIBRRG_ERROR_UPDATE` will be called instead.
* `LIBRG_READ_REMOVE` - an instruction for destination world to remove provided entity.
IF a given entity id is not tracked, or it is not considered to be a foreign entity, `LIBRG_ERROR_REMOVE` will be called instead.

## librg_event_set

Sets a [callback/handler](defs/types.md#librg_event_fn) for a provided event type/id.

##### Signature
```c
int8_t librg_event_set(
    librg_world *world,
    librg_event_type id,
    librg_evetn_fn callback
)
```

##### Returns

* In case of success return code is `LIBRG_OK`
* In case an existing event was replaced: `LIBRG_HANDLER_REPLACED`
* In case of invalid world: `LIBRG_WORLD_INVALID`

-------------------------------

## librg_event_remove

Removes a [callback/handler](defs/types.md#librg_event_fn) for a provided event type/id.

##### Signature
```c
int8_t librg_event_remove(
    librg_world *world,
    librg_event_type id
)
```

##### Returns

* In case of success return code is `LIBRG_OK`
* In case a non-existing event: `LIBRG_HANDLER_EMPTY`
* In case of invalid world: `LIBRG_WORLD_INVALID`

-------------------------------

## librg_event_type_get

Returns current event type of the provided event.
Usually this method is called within the event callback itself.

##### Signature
```c
librg_event_type librg_event_type_get(
    librg_world *world,
    librg_event *event
)
```

##### Returns

* In case of success: current event type
* In case of invalid event: `LIBRG_EVENT_INVALID`

-------------------------------

## librg_event_owner_get

Returns owner id, which the event is called in context of.
The owner id is provided from a parent [librg_world_write](defs/packing.md#librg_world_write) or [librg_world_read](defs/packing.md#librg_world_read) call.
Usually this method is called within the event callback itself.

##### Signature
```c
int64_t librg_event_owner_get(
    librg_world *world,
    librg_event *event
)
```

##### Returns

* In case of success: valid id
* In case of invalid event: `LIBRG_EVENT_INVALID`

-------------------------------

## librg_event_entity_get

Returns entity id, which the event is called for.
Usually this method is called within the event callback itself.

##### Signature
```c
int64_t librg_event_entity_get(
    librg_world *world,
    librg_event *event
)
```

##### Returns

* In case of success: valid id
* In case of invalid event: `LIBRG_EVENT_INVALID`

-------------------------------

## librg_event_buffer_get

Returns pointer to the data buffer, where data can be written to, or read from (depending on the event type).
Usually this method is called within the event callback itself.

##### Signature
```c
char * librg_event_buffer_get(
    librg_world *world,
    librg_event *event
)
```

##### Returns

* In case of success: valid pointer
* In case of error: `NULL`

-------------------------------

## librg_event_size_get

For read events, returns current size of the data stored in the buffer.
For write events, returns maximum allowed length of the data that can be written to the buffer (based on the buffer passed to [librg_world_write](defs/packing.md#librg_world_write) call).
Usually this method is called within the event callback itself.

##### Signature
```c
int64_t librg_event_size_get(
    librg_world *world,
    librg_event *event
)
```

##### Returns

* In case of success: positive number represening length
* In case of invalid event: `LIBRG_EVENT_INVALID`

-------------------------------

## librg_event_userdata_get

Returns pointer with userdata that might be stored within the event.
The userdata is provided from a parent [librg_world_write](defs/packing.md#librg_world_write) or [librg_world_read](defs/packing.md#librg_world_read) call.
Usually this method is called within the event callback itself.

##### Signature
```c
char * librg_event_userdata_get(
    librg_world *world,
    librg_event *event
)
```

##### Returns

* In case of success: passed pointer
* In case of error: `NULL`
