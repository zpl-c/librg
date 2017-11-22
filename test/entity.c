#define LIBRG_IMPLEMENTATION
#define LIBRG_DEBUG
#include <librg.h>

enum {
    LIBRG_ENTITY_NONE       = 0,
    LIBRG_ENTITY_ALIVE      = (1 << 0),

    LIBRG_ENTITY_CLIENT     = (1 << 4),
    LIBRG_ENTITY_IGNORING   = (1 << 5),
    LIBRG_ENTITY_QUERIED    = (1 << 6),

    LIBRG_ENTITY_CONTROLLED = (1 << 10),
};

typedef struct librg_entity_blob_t {
    u32 id;
    u32 type;
    u64 flags;

    zplm_vec3_t position;
    f32 stream_range;

    zplc_t *stream_branch;
    void *user_data;

    u8 *ignored;
    u8 *last_snapshot;
    librg_peer_t *client_peer;
    librg_peer_t *control_peer;

    zpl_array_t(librg_entity_t) last_query;
} librg_entity_blob_t;

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

    librg_entity_t librg_ex_entity_create(librg_ctx_t *ctx, u32 type) {
        librg_assert(ctx);
        librg_assert(librg_is_server(ctx));
        librg_assert_msg(ctx->entity.count < ctx->max_entities, "reached max_entities limit");

        ++ctx->entity.count;

        if (ctx->entity.cursor == ctx->max_entities || ctx->max_entities == 0) {
            ctx->entity.cursor = 0;
        }

        for (; ctx->entity.cursor < ctx->max_entities; ++ctx->entity.cursor) {
            librg_entity_blob_t *entity = &ctx->entity.list[ctx->entity.cursor]; librg_assert(entity);

            if (entity->flags & LIBRG_ENTITY_ALIVE) continue;

            entity->type            = type;
            entity->flags           = LIBRG_ENTITY_ALIVE;
            entity->position        = zplm_vec3_zero();
            entity->stream_range    = 250.0f; // TODO: move to an default option

            return entity->id;
        }

        librg_assert_msg(false, "no entities to spawn");
        return 0;
    }

    b32 librg_ex_entity_destroy(librg_ctx_t *ctx, librg_entity_t id) {
        librg_assert(ctx);
        librg_assert(librg_is_server(ctx));
        librg_assert(librg_entity_valid(ctx, id));
        librg_entity_blob_t *entity = &ctx->entity.list[id];

        if (entity->flags & LIBRG_ENTITY_CLIENT) {
            entity->client_peer     = NULL;
            entity->last_snapshot   = NULL; // TODO: free dat badboi
        }

        if (entity->flags & LIBRG_ENTITY_QUERIED) {
            zpl_array_free(entity->last_query);
        }

        if (entity->flags & LIBRG_ENTITY_IGNORING) {
            // TODO: free ignored
        }

        entity->flags = LIBRG_ENTITY_NONE;
        return true;
    }

    librg_inline b32 librg_ex_entity_valid(librg_ctx_t *ctx, librg_entity_t id) {
        librg_assert(ctx && id < ctx->max_entities);
        return (ctx->entity.list[id].flags & LIBRG_ENTITY_ALIVE);
    }

    librg_inline u32 librg_ex_entity_type(librg_ctx_t *ctx, librg_entity_t id) {
        librg_assert(librg_ex_entity_valid(ctx, id));
        return ctx->entity.list[id].type;
    }

    librg_inline librg_entity_blob_t *librg_ex_entity_blob(librg_ctx_t *ctx, librg_entity_t id) {
        librg_assert(librg_ex_entity_valid(ctx, id));
        return &ctx->entity.list[id];
    }

    // librg_inline void librg_ex_entity_iterate(librg_ctx_t *ctx, u64 flags, )


int main() {
    librg_ctx_t ctx = {0};
    ctx.max_entities = 100;

    librg_init(&ctx, NULL);

    {
        ctx.entity.list = zpl_alloc(ctx.allocator, sizeof(librg_entity_blob_t) * ctx.max_entities);
        for (usize i = 0; i < ctx.max_entities; i++) {
            librg_entity_blob_t blob = { i, 0 };
            ctx.entity.list[i] = blob;
        }
    }

    for (int i = 0; i < 100; ++i) {
        librg_log("created entity %d\n", librg_ex_entity_create(&ctx, 0));
    }

    librg_log("created entity %d\n", librg_ex_entity_create(&ctx, 0));
    librg_log("created entity %d\n", librg_ex_entity_create(&ctx, 0));
    librg_log("created entity %d\n", librg_ex_entity_create(&ctx, 0));
    librg_log("created entity %d\n", librg_ex_entity_create(&ctx, 0));

    librg_free(&ctx);
    return 0;
}
