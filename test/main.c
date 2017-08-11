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
    zpl_array_init(librg__components, zpl_heap_allocator());

    // librg_entity_t ent = librg_create();
    // librg_entity_t ent2 = librg_create();

    // librg_attach_position(ent, (position_t) { 1, 2, 3 });
    // librg_attach_position(ent2, (position_t) { 1, 2, 3 });
    // librg_attach_rotation(ent, (rotation_t) { 1, 2, 3 });
    // librg_attach_zaklaus(ent, (zaklaus_t) { true });

    // librg_detach_zaklaus(ent);


    for (isize i = 0; i < 15; ++i) {
        zple_id_t h = librg_create();
        librg_attach_transform(h, (transform_t){i, i*2, i*i});
    }

    librg_foreach(transform, transform_print_cb);

    zple_id_t t = librg_create();
    ZPL_ASSERT(t.id == 15);

    transform_t *data = librg_attach_transform(t, (transform_t) { 1, 2, 3 });

    librg_attach_zaklaus(t, (zaklaus_t) { true });

    transform_t *d = librg_fetch_transform(t);
    ZPL_ASSERT(d->x == 1 && d->y == 2 && d->z == 3);

    // librg_detach_transform(t);

    librg_destroy(t);

    t = librg_create();
    ZPL_ASSERT(t.id == 15 && t.generation == 1); // NOTE(ZaKlaus): This should be re-used entity.

    return 0;
}
