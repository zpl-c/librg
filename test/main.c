#define LIBRG_IMPLEMENTATION
#include <librg.h>

librg_component(waffle, { i32 x; i32 y; i32 z; });

librg_component(position, { i32 x; i32 y; i32 z; });
librg_component(rotation, { i32 x; i32 y; i32 z; });
librg_component(zaklaus,  { b32 cool; });

int bar();

int main() {

    // dis is inside librg_init
        zple_init(&librg__entity_pool, zpl_heap_allocator(), 100);

    librg_entity_t ent = librg_create();
    librg_entity_t ent2 = librg_create();

    librg_attach_position(ent, (position_t) { 1, 2, 3 });
    librg_attach_position(ent2, (position_t) { 1, 2, 3 });
    librg_attach_rotation(ent, (rotation_t) { 1, 2, 3 });
    librg_attach_zaklaus(ent, (zaklaus_t) { true });

    bar();

    // transform_t *foo = librg_fetch_transform(ent);

    // librg_log("the result is: %d %d %d\n", foo->x, foo->y, foo->z);

    return 0;
}
