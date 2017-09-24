#define LIBRG_IMPLEMENTATION
#define LIBRG_DEBUG
#include <librg.h>

typedef struct { u32 bar; } foo;

void custom_components(librg_ctx_t *ctx) {
    librg_component_register(ctx, librg_component_last, sizeof(foo));
}

int main() {
    librg_ctx_t ctx = {0};
    ctx.components.register_cb = custom_components;
    librg_init(&ctx);

    librg_entity_t entity = librg_entity_create(&ctx, 0);

    foo z = {15};
    librg_component_attach(&ctx, librg_component_last, entity, &z);
    // librg_component_detach(&ctx, librg_component_last, entity);

    // detach all components
    for (usize i = 0; i < ctx.components.count; i++) {
        ctx.components.headers[i].used[entity] = false;
    }

    librg_assert(librg_component_fetch(&ctx, librg_component_last, entity) == NULL);

    librg_free(&ctx);
    return 0;
}
