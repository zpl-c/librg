#include <librg.h>

librg_component_declare(waffle, { i32 x; i32 y; i32 z; });
// librg_component_define(waffle, { i32 x; i32 y; i32 z; });

int bar() {
    librg_entity_t ent2 = librg_create();
    librg_attach_waffle(ent2, (waffle_t) { 1, 2, 3 });
}
