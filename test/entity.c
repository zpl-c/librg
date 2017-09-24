#define LIBRG_IMPLEMENTATION
#define LIBRG_DEBUG
#include <librg.h>

#define LIBRG_COMPONENTS_MAX 64
#define LIBRG_COMPONENTS_SIZE (ctx->max_entities * LIBRG_COMPONENTS_MAX * 128)

typedef struct {
    u32 x;
    u32 y;
} mycmp_t;

#define somemem zplm_quat_t x[1024];

typedef struct { somemem } comp1;
typedef struct { somemem } comp2;
typedef struct { somemem } comp3;
typedef struct { somemem } comp4;
typedef struct { somemem } comp5;
typedef struct { somemem } comp6;
typedef struct { somemem } comp7;
typedef struct { somemem } comp8;


enum {
    index_mycmp,
    index_comp1,
    index_comp2,
    index_comp3,
    index_comp4,
    index_comp5,
    index_comp6,
    index_comp7,
    index_comp8,
    total_components,
};


zpl_inline void librg__component_finish(librg_ctx_t *ctx) {
    ctx->components.data = zpl_malloc(ctx->components.size);
}

void librg_component_register(librg_ctx_t *ctx, u32 index, usize component_size) {
    librg_assert(ctx);
    librg_assert_msg(ctx->components.count == index, "you should register components in order");

    librg_component_meta *header = &ctx->components.headers[index]; librg_assert(header);
    usize size = component_size * ctx->max_entities;

    ctx->components.size += size;
    ctx->components.count++;

    zpl_buffer_init(header->used, ctx->allocator, ctx->max_entities);

    header->offset = size;
    header->size   = component_size;
}

librg_void *librg_component_attach(librg_ctx_t *ctx, u32 index, librg_entity_t entity, librg_void *data) {
    librg_component_meta *header = &ctx->components.headers[index];
    librg_assert(header && header->size);
    header->used[entity] = true;
    librg_void *cdata = ctx->components.data + header->offset;
    zpl_memcopy(&cdata[entity * header->size], data, (usize)header->size);
    return &cdata[entity * header->size];
}

librg_void *librg_component_fetch(librg_ctx_t *ctx, u32 index, librg_entity_t entity) {
    librg_component_meta *header = &ctx->components.headers[index]; librg_assert(header);
    librg_void *cdata = ctx->components.data + header->offset;
    return header->used[entity] ? &cdata[entity * header->size] : NULL;
}

void librg_component_detach(librg_ctx_t *ctx, u32 index, librg_entity_t entity) {
    librg_component_meta *header = &ctx->components.headers[index]; librg_assert(header);
    header->used[entity] = false;
}

int main() {
    librg_ctx_t ctx = {0};
    librg_init(&ctx);

    librg_component_register(&ctx, index_mycmp, sizeof(mycmp_t));
    librg_component_register(&ctx, index_comp1, sizeof(comp1));
    librg_component_register(&ctx, index_comp2, sizeof(comp2));
    librg_component_register(&ctx, index_comp3, sizeof(comp3));
    librg_component_register(&ctx, index_comp4, sizeof(comp4));
    librg_component_register(&ctx, index_comp5, sizeof(comp5));
    librg_component_register(&ctx, index_comp6, sizeof(comp6));
    librg_component_register(&ctx, index_comp7, sizeof(comp7));
    librg_component_register(&ctx, index_comp8, sizeof(comp8));

    librg__component_finish(&ctx);

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


    for (int i = 0; i < 10000; ++i) {
        comp8 cmp = {0};
        librg_component_attach(&ctx, index_comp8, i, &cmp);
    }

    mycmp_t bar = {15, 25};
    mycmp_t *foo = librg_component_attach(&ctx, 0, 1, &bar);

    librg_assert(bar.x == foo->x);
    librg_assert(((mycmp_t *)librg_component_fetch(&ctx, 0, 1))->y == bar.y);

    librg_component_detach(&ctx, 0, 0);
    librg_component_detach(&ctx, 0, 1);

    librg_assert(!librg_component_fetch(&ctx, 0, 1));

    while(1) {
        zpl_sleep_ms(1);
    }

    librg_free(&ctx);
    return 0;
}
