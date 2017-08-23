#define ZPL_IMPLEMENTATION
#include <zpl.h>

ZPL_TABLE(extern, librg__entbool_t, librg__entbool_, u32);

int main() {
    librg__entbool_t foo;
    librg__entbool_init(&foo, zpl_heap_allocator());

    librg__entbool_set(&foo, 131, 1);
    librg__entbool_set(&foo, 398, 1);
    librg__entbool_set(&foo, 1000, 1);
    librg__entbool_set(&foo, 479, 1);
    librg__entbool_set(&foo, 543, 1);
    librg__entbool_set(&foo, 741, 1);
    librg__entbool_set(&foo, 220, 1);
    librg__entbool_set(&foo, 333, 1);
    librg__entbool_set(&foo, 379, 1);
    librg__entbool_set(&foo, 948, 1);
    librg__entbool_set(&foo, 576, 1);
    librg__entbool_set(&foo, 503, 1);

    zpl_printf("%u\n", *librg__entbool_get(&foo, 131));
    zpl_printf("%u\n", *librg__entbool_get(&foo, 398));
    zpl_printf("%u\n", *librg__entbool_get(&foo, 1000));
    zpl_printf("%u\n", *librg__entbool_get(&foo, 479));
    zpl_printf("%u\n", *librg__entbool_get(&foo, 543));
    zpl_printf("%u\n", *librg__entbool_get(&foo, 741));
    zpl_printf("%u\n", *librg__entbool_get(&foo, 220));
    zpl_printf("%u\n", *librg__entbool_get(&foo, 333));
    zpl_printf("%u\n", *librg__entbool_get(&foo, 379));
    zpl_printf("%u\n", *librg__entbool_get(&foo, 948));
    zpl_printf("%u\n", *librg__entbool_get(&foo, 576));
    zpl_printf("%u\n", *librg__entbool_get(&foo, 503));

    librg__entbool_destroy(&foo);

    return 0;
}
