#define ZPL_IMPLEMENTATION
#define ZPLE_IMPLEMENTATION
#include <zpl.h>
#include <zpl_ent.h>

typedef struct {
    f32 x,y,z;
} ZPLE_COMP_DECLARE(transform);

void transform_print_cb(zple_id_t handle, transform_t *tran) {
    zpl_printf("%f, %f, %f\n", tran->x, tran->y, tran->z);
}

// NOTE(ZaKlaus): Put this EXACTLY to one source file!
ZPLE_COMP_DEFINE(transform);

int main(void) {

    // NOTE(ZaKlaus): Initialize our entity pool
    zple_pool pool = {0};
    zple_init(&pool, zpl_heap_allocator(), 100);

    // NOTE(ZaKlaus): Initialize our component pool
    // WARN(ZaKlaus): Component pool of type X can be used EXACTLY by ONE entity pool EXCLUSIVELY!!!
    transform_pool tranpool = {0};
    transform_init(&tranpool, &pool, zpl_heap_allocator());

    // NOTE(ZaKlaus): Generate some entities.
    for (isize i = 0; i < 15; ++i) {
        zple_id_t h = zple_create(&pool);
        transform_attach(&tranpool, h, (transform_t){i, i*2, i*i});
    }

    // NOTE(ZaKlaus): Do foreach on our component pool
    zple_foreach((&tranpool), transform, transform_print_cb);

    zple_id_t t = zple_create(&pool);
    ZPL_ASSERT(t.id == 15); // NOTE(ZaKlaus): This entity should be 16th.

    // TODO(ZaKlaus): Add component to entity
    transform_t *data = transform_attach(&tranpool, t, (transform_t) { 1, 2, 3 });

    // NOTE(ZaKlaus): Check on the data.
    transform_t *d = transform_fetch(&tranpool, t);
    ZPL_ASSERT(d->x == 1 && d->y == 2 && d->z == 3);

    // NOTE(ZaKlaus): Detach component from entity.
    // WARN(ZaKlaus): Do this each time when you destroy the entity!
    transform_detach(&tranpool, t);

    // TODO(ZaKlaus): Remove entity
    zple_destroy(&pool, t);

    // NOTE(ZaKlaus): Add new entity
    t = zple_create(&pool);
    ZPL_ASSERT(t.id == 15 && t.generation == 1); // NOTE(ZaKlaus): This should be re-used entity.

    // NOTE(ZaKlaus): Release our resources
    transform_free(&tranpool);
    zple_free(&pool);

    return 0;
}
