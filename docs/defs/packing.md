# Packing

## librg_world_write

Method is used to pack a set of visible entities to a given owner.

In some way method is similar to [librg_world_query](defs/query.md#librg_world_query), and actually is using it inside.
However, instead of returning set of results directly to you, it rather builds a snapshot of what is currently visible to the given owner,
and compares it with a previous snapshot (from previous call to this method).
This way method is able to keep track which entities are considered to be new, existing or forgotten for each owner within his view radius (based on entities that he owns).

Additionally, an event is fired for each entity within owner's view radius, allowing you to write any additional information specific for this owner or this entity.
For more details on events, please refer to the [events](defs/events.md) page.
In case you wish to write any additional information, you would need to return length of the data you copied to the buffer. More details in the [example](#example) below.

All of that information is then written to the buffer that provided as an argument, and is ready to be transferred/saved, and then later on read by [librg_world_read](#librg_world_read) method.

Important: the [librg_world_query](defs/query.md#librg_world_query) will use a **temporary allocated** buffer of size [LIBRG_WORLDWRITE_MAXQUERY](compiletime.md#LIBRG_WORLDWRITE_MAXQUERY) elements.
If the provided space will not be enough, you need to redefine the macro to increase the limit.

> Note:
> * pre-last argument tells method maximum length of your buffer, and the method will respect that length
> * pre-last argument is in-out reference value, the resulting length will be written back to that variable
> * please make sure to always provide a buffer of appropriately sized length
> * last argument allows you to pass a custom user pointer, it will be injected to the event structure, allowing you to use that in the event callback

##### Signature
```c
int32_t librg_world_write(
    librg_world *world,
    int64_t owner_id,
    char *buffer,       /* out */
    size_t *size,       /* in-out */
    void *userdata
)
```

##### Returns

* In case of success: `LIBRG_OK`
* Alternatively, in case of success: positive aproximated length by which your buffer should be increased
* In case of invalid world: `LIBRG_WORLD_INVALID`
* In case of invalid owner: `LIBRG_OWNER_INVALID`


### **Example**

[example-packing.c](https://raw.githubusercontent.com/zpl-c/librg/master/code/apps/example-packing.c ':include :type=code')

------------------------------

## librg_world_read

Method is used to unpack a previously packed buffer with data, containing a snapshot of the world for a specific owner.

This method will read the buffer, and apply all the "instructions" from a source world to the current (destination) world.
It will create entities that were not there before, update existing entities, and remove ones that are not considered visible by the source world,
calling all appopriate events alonside, allowing you to read any additional data that you've passed from the source world.

Check out [example](#example-1) below, for more details and demo.

> Note:
> * pre-last argument tells method maximum length of your buffer, and the method will respect that length
> * last argument allows you to pass a custom user pointer, it will be injected to the event structure, allowing you to use that in the event callback

##### Signature
```c
int32_t librg_world_read(
    librg_world *world,
    int64_t owner_id,
    char *buffer,       /* out */
    size_t size,
    void *userdata
)
```

##### Returns

* In case of success: `LIBRG_OK`
* Alternatively, in case of success: positive aproximated length of how much data it was not able to read
* In case of invalid/mismatching data size: `LIBRG_READ_INVALID`
* In case of invalid world: `LIBRG_WORLD_INVALID`
* In case of invalid owner: `LIBRG_OWNER_INVALID`

### **Example**

[example-unpacking.c](https://raw.githubusercontent.com/zpl-c/librg/master/code/apps/example-unpacking.c ':include :type=code')
