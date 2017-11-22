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


int main() {
    librg_ctx_t ctx = {0};
    ctx.max_entities = 100;

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

    librg_free(&ctx);
    return 0;
}
