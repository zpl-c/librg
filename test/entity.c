#define LIBRG_IMPLEMENTATION
#define LIBRG_DEBUG
#include <librg.h>

enum {
    LIBRG_ENTITY_NONE   = 0,
    LIBRG_ENTITY_ALIVE  = (1 << 0),
};

typedef struct {
    u32 id;
    u32 type;
    u64 flags;

    zplm_vec3_t position;
    f32 stream_range;

    librg_peer_t *client_peer;
    librg_peer_t *control_peer;

    zpl_array_t(librg_entity_t) last_query;

    librg_table_t last_snapshot;
    librg_table_t ignored;

    zplc_t *branch;
    void *user_data;
} librg_entity_tx;

// entity_create - server
// entity_valid - both
// entity_destroy - server
// entity_visibility_set - server
// entity_visibility_get - server
// entity_visibility_set_for - server
// entity_visibility_get_for - server
// entity_control_set - server
// entity_control_get - server
// entity_iterate_all - both
// entity_iterate_having - both

typedef struct {
    u8 *entity_list;
    u8 *client_list; // ?
} librg_ctx_ents;

#define librg_u8bit_set(A,k)     ( A[(k/8)] |= (1 << (k%8)) )
#define librg_u8bit_get(A,k)     ( A[(k/8)] & (1 << (k%8)) )
#define librg_u8bit_clear(A,k)   ( A[(k/8)] &= ~(1 << (k%8)) )

int main() {
    librg_ctx_t ctx = {0};

    ctx.max_entities = 16000;

    librg_ctx_ents zzzz = {0};
    // ctx.components.register_cb = custom_components;
    librg_init(&ctx, NULL);

    // librg_entity_t entity = librg_entity_create(&ctx, 0);

    // init
    {
        zzzz.entity_list = zpl_alloc(ctx.allocator, sizeof(u8) * (ctx.max_entities / 8 + 1));
        for (usize i = 0; i < ctx.max_entities; i++) librg_u8bit_clear(zzzz.entity_list, i);
    }

    librg_u8bit_set(zzzz.entity_list, 322);

    librg_log("%d\n", (b32)librg_u8bit_get(zzzz.entity_list, 322));

    // librg_entity_tx a;
    // librg_entity_tx *b;

    // librg_log("size is: %u mb\n", sizeof(a));
    // librg_log("size is: %u b\n", sizeof(b));

    // char str[32];
    // librg_log("%x\n", str);

    librg_free(&ctx);
    return 0;
}
