#define LIBRG_IMPLEMENTATION
#define LIBRG_DEBUG
#include <librg.h>



// entity_create - server
// entity_destroy - server
// entity_valid - both
// entity_type - both
// entity_blob - both
// entity_visibility_set - server
// entity_visibility_get - server
// entity_visibility_set_for - server
// entity_visibility_get_for - server
// entity_control_set - server
// entity_control_get - server
// entity_iterate - both



    // librg_inline void librg_ex_entity_iterate(librg_ctx_t *ctx, u64 flags, )

zpl_array_t(int) z;

void zer(int **a) {
    for (int i = 0; i < 15000; i++) {
        zpl_array_append(*a, i);
    }
}

int bar(int **a) {
    zer(a);
    return 0;
}

int query(int **a) {
    static bool foo = false;
    if (!foo) {
        zpl_array_init(z, zpl_heap_allocator());
        foo = true;
    }

    zpl_array_count(z) = 0;

    bar(&z);
    *a = z;

    return 0;
}


int main() {
    librg_ctx_t ctx = {0};
    ctx.max_entities = 100;

    zpl_array_t(int) a;

    for (int i = 0; i < 10000; ++i){
        query(&a);
    }

    for (int i = 0; i < 15000; ++i) {
        librg_assert(i == a[i]);
    }

    librg_log("%td\n", zpl_array_count(a));


    librg_init(&ctx);

    for (int i = 0; i < 100; ++i) {
        librg_log("created entity %d\n", librg_entity_create(&ctx, 0));
    }

    librg__entity_destroy(&ctx, 1);
    librg__entity_destroy(&ctx, 2);
    librg__entity_destroy(&ctx, 3);
    librg__entity_destroy(&ctx, 5);

    librg_log("created entity %d\n", librg_entity_create(&ctx, 0));
    librg_log("created entity %d\n", librg_entity_create(&ctx, 0));
    librg_log("created entity %d\n", librg_entity_create(&ctx, 0));
    librg_log("created entity %d\n", librg_entity_create(&ctx, 0));

    librg_network_start(&ctx, (librg_address_t) { 7777 });
    librg_tick(&ctx);
    librg_tick(&ctx);
    librg_tick(&ctx);
    librg_network_stop(&ctx);

    librg_free(&ctx);
    return 0;
}
