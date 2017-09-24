#define LIBRG_IMPLEMENTATION
#define LIBRG_DEBUG
#include <librg.h>

int main() {
    librg_ctx_t ctx = {0};
    librg_init(&ctx);

    // librg_assert(0xffffffff == (u32)-1);

    // librg_component_register(&ctx, index_mycmp, sizeof(mycmp_t));
    // librg_component_register(&ctx, index_comp1, sizeof(comp1));
    // librg_component_register(&ctx, index_comp2, sizeof(comp2));
    // librg_component_register(&ctx, index_comp3, sizeof(comp3));
    // librg_component_register(&ctx, index_comp4, sizeof(comp4));
    // librg_component_register(&ctx, index_comp5, sizeof(comp5));
    // librg_component_register(&ctx, index_comp6, sizeof(comp6));
    // librg_component_register(&ctx, index_comp7, sizeof(comp7));
    // librg_component_register(&ctx, index_comp8, sizeof(comp8));

    // librg__component_finish(&ctx);

    // i32 index = 0;
    // for (int i = 0; i < 10000; ++i) {
    //     if (i % 1000) index++;
    //     if (i < 1000) {
    //         mycmp_t cmp = {0};
    //         librg_component_attach(&ctx, index, i, &cmp);
    //     }
    //     else {
    //         comp1 cmp = {0};
    //         librg_component_attach(&ctx, index, i, &cmp);
    //     }
    // }


    // for (int i = 0; i < 10000; ++i) {
    //     comp8 cmp = {0};
    //     librg_component_attach(&ctx, index_comp8, i, &cmp);
    // }

    // mycmp_t bar = {15, 25};
    // mycmp_t *foo = librg_component_attach(&ctx, 0, 1, &bar);

    // librg_assert(bar.x == foo->x);
    // librg_assert(((mycmp_t *)librg_component_fetch(&ctx, 0, 1))->y == bar.y);

    // librg_component_detach(&ctx, 0, 0);
    // librg_component_detach(&ctx, 0, 1);

    // librg_assert(!librg_component_fetch(&ctx, 0, 1));

    // while(1) {
    //     zpl_sleep_ms(1);
    // }

    librg_free(&ctx);
    return 0;
}
