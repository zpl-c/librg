#define LIBRG_IMPLEMENTATION
#include <librg.h>

typedef struct { i32 x; i32 y; i32 z; } librg_component(rotation);
typedef struct { i32 x; i32 y; i32 z; } librg_component(position);
typedef struct { i32 x; i32 y; i32 z; } librg_component(waffle);
typedef struct { b32 cool; } librg_component(zaklaus);

// int bar();

int main() {

    // dis is inside librg_init
        zple_init(&librg__entity_pool, zpl_heap_allocator(), 100);

    librg_entity_t ent = librg_create();
    librg_entity_t ent2 = librg_create();

    librg_attach_position(ent, (position_t) { 1, 2, 3 });
    librg_attach_position(ent2, (position_t) { 1, 2, 3 });
    librg_attach_rotation(ent, (rotation_t) { 1, 2, 3 });
    librg_attach_zaklaus(ent, (zaklaus_t) { true });

    // bar();

    // transform_t *foo = librg_fetch_transform(ent);

    // librg_log("the result is: %d %d %d\n", foo->x, foo->y, foo->z);

    return 0;
}
