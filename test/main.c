#define LIBRG_IMPLEMENTATION
#include <librg.h>

typedef struct { i32 x; i32 y; i32 z; } librg_component(transform);
typedef struct { i32 x; i32 y; i32 z; } librg_component(position);
typedef struct { i32 x; i32 y; i32 z; } librg_component(waffle);
typedef struct { b32 cool; } librg_component(zaklaus);

void transform_print_cb(zple_id_t handle, transform_t *tran) {
    zpl_printf("%d, %d, %d\n", tran->x, tran->y, tran->z);
}

int main() {

    // dis is inside librg_init
    zple_init(&librg__entity_pool, zpl_heap_allocator(), 100);

    // librg_entity_t ent = librg_create();
    // librg_entity_t ent2 = librg_create();

    // librg_attach_position(ent, (position_t) { 1, 2, 3 });
    // librg_attach_position(ent2, (position_t) { 1, 2, 3 });
    // librg_attach_rotation(ent, (rotation_t) { 1, 2, 3 });
    // librg_attach_zaklaus(ent, (zaklaus_t) { true });

    // librg_detach_zaklaus(ent);


    // NOTE(ZaKlaus): Generate some entities.
    for (isize i = 0; i < 15; ++i) {
        zple_id_t h = librg_create();
        librg_attach_transform(h, (transform_t){i, i*2, i*i});
    }

    // NOTE(ZaKlaus): Do foreach on our component pool
    librg_foreach(transform, transform_print_cb);

    zple_id_t t = zple_create();
    ZPL_ASSERT(t.id == 15); // NOTE(ZaKlaus): This entity should be 16th.

    // // TODO(ZaKlaus): Add component to entity
    // transform_t *data = transform_attach(&tranpool, t, (transform_t) { 1, 2, 3 });

    // // NOTE(ZaKlaus): Check on the data.
    // transform_t *d = transform_fetch(&tranpool, t);
    // ZPL_ASSERT(d->x == 1 && d->y == 2 && d->z == 3);

    // // NOTE(ZaKlaus): Detach component from entity.
    // // WARN(ZaKlaus): Do this each time when you destroy the entity!
    // transform_detach(&tranpool, t);

    // // TODO(ZaKlaus): Remove entity
    // zple_destroy(&pool, t);

    // // NOTE(ZaKlaus): Add new entity
    // t = zple_create(&pool);
    // ZPL_ASSERT(t.id == 15 && t.generation == 1); // NOTE(ZaKlaus): This should be re-used entity.

    // NOTE(ZaKlaus): Release our resources
    // transform_free(&tranpool);
    // zple_free(&pool);


    // bar();

    // transform_t *foo = librg_fetch_transform(ent);

    // librg_log("the result is: %d %d %d\n", foo->x, foo->y, foo->z);

    return 0;
}
