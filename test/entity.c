#define LIBRG_IMPLEMENTATION
#define LIBRG_DEBUG
#include <librg.h>

#define LIBRG_COMPONENTS_MAX 64
#define LIBRG_COMPONENTS_SIZE (pctx->max_entities * LIBRG_COMPONENTS_MAX * 128)

typedef struct {
    u32 x;
    u32 y;
} mycmp_t;

typedef struct {
    librg_void *data;
    usize       size;
    zpl_buffer_t(b32) used;
} librg_component_meta;

typedef struct {

    struct {
        librg_void *data;
        usize size;
        usize count;
        zpl_buffer_t(librg_component_meta) headers;
    } components;

    u32 max_entities;
} librg_ctx_t;


void librg_component_register(librg_ctx_t *ctx, u32 index, usize size) {
    librg_component_meta *header = &ctx->components.headers[index]; librg_assert(header);

    if (ctx->components.data == NULL) {
#ifdef LIBRG_CUSTOM_COMPONENT_SIZE
        ctx->components.data = zpl_alloc(zpl_heap_allocator(), LIBRG_CUSTOM_COMPONENT_SIZE);
        ctx->components.size = LIBRG_CUSTOM_COMPONENT_SIZE;
#else
        ctx->components.data = zpl_alloc(zpl_heap_allocator(), size * index + 2 * size);
        ctx->components.size = size * index + 2 * size;
#endif
    }
    else if (index * size > ctx->components.size) {
        ctx->components.data = zpl_resize(zpl_heap_allocator(), ctx->components.data, ctx->components.size, index * size);
        ctx->components.size = index * size;
    }

    header->data = ctx->components.data + (index * size);
    zpl_buffer_init(header->used, zpl_heap_allocator(), ctx->max_entities);
    header->size = size;
}

librg_void *librg_component_attach(librg_ctx_t *ctx, u32 index, librg_entity_t entity, librg_void *data) {
    librg_component_meta *header = &ctx->components.headers[index]; librg_assert(header);
    header->used[entity] = true;
    zpl_memcopy(&header->data[entity * header->size], data, (usize)header->size);
    return &header->data[entity * header->size];
}

librg_void *librg_component_fetch(librg_ctx_t *ctx, u32 index, librg_entity_t entity) {
    librg_component_meta *header = &ctx->components.headers[index]; librg_assert(header);
    return header->used[entity] ? &header->data[entity * header->size] : NULL;
}

void librg_component_detach(librg_ctx_t *ctx, u32 index, librg_entity_t entity) {
    librg_component_meta *header = &ctx->components.headers[index]; librg_assert(header);
    header->used[entity] = false;
}

int main() {
    librg_init((librg_config_t) {
        .tick_delay     = 32,
        .mode           = LIBRG_MODE_SERVER,
        .world_size     = zplm_vec2(5000.0f, 5000.0f),
    });

    librg_ctx_t ctx = {0};
    ctx.max_entities = 150;

    // librg_init
    {
        librg_ctx_t *pctx = &ctx;
        zpl_buffer_init(pctx->components.headers, zpl_heap_allocator(), LIBRG_COMPONENTS_MAX);
    }

    librg_component_register(&ctx, 0, sizeof(mycmp_t));

    mycmp_t bar = {15, 25};
    mycmp_t *foo = librg_component_attach(&ctx, 0, 1, &bar);

    librg_assert(bar.x == foo->x);
    librg_assert(((mycmp_t *)librg_component_fetch(&ctx, 0, 1))->y == bar.y);

    librg_component_detach(&ctx, 0, 0);
    librg_component_detach(&ctx, 0, 1);

    librg_assert(!librg_component_fetch(&ctx, 0, 1));


    // librg_free
    {
        librg_ctx_t *pctx = &ctx;
        for (int i = 0; i < pctx->components.count; ++i) {
            librg_component_meta *header = &pctx->components.headers[i]; librg_assert(header);
            zpl_buffer_free(header->used, zpl_heap_allocator());
        }

        zpl_free(zpl_heap_allocator(), pctx->components.data);
        zpl_buffer_free(pctx->components.headers, zpl_heap_allocator());
    }


    librg_free();
    return 0;
}
