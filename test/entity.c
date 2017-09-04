#define LIBRG_IMPLEMENTATION
#include <librg.h>

typedef struct { i32 x; i32 y; i32 z; } librg_component(position);
typedef struct { i32 x; i32 y; i32 z; } librg_component(waffle);
typedef struct { b32 cool; } librg_component(zaklaus);

void entitycb(librg_entity_t entity) {
    transform_t *tran = librg_fetch_transform(entity);
    // librg_log("entitycb: %d, %d, %d\n", tran->x, tran->y, tran->z);
    // librg_log("calling entitycb\n");
}

int main() {

    librg_init((librg_config_t) {
        .tick_delay     = 32,
        .mode           = LIBRG_MODE_SERVER,
        .world_size     = zplm_vec2(5000.0f, 5000.0f),
        .entity_limit   = 2048,
    });

    for (isize i = 0; i < 15; ++i) {
        librg_entity_t h = librg_entity_create(0);
        librg_attach_transform(h, (transform_t){i, i*2, i*i});
        if (i % 2 == 0) {
            librg_attach_position(h, (position_t){1,2,3});
        }

        if (i % 10 == 0) {
            librg_attach_zaklaus(h, (zaklaus_t){true});

        }
    }

    librg_entity_t t = librg_entity_create(0);
    ZPL_ASSERT(t.id == 15);

    transform_t *data = librg_attach_transform(t, (transform_t) { 1, 2, 3 });

    librg_attach_zaklaus(t, (zaklaus_t) { true });

    transform_t *d = librg_fetch_transform(t);

    librg_entity_t a = librg_entity_create_shared(128, 0);
    printf("Remote: %lu, Local: %lu, Remote (from Local): %lu, Local (from Remote): %lu, Remote (from Local from Remote): %lu\n", a.id, librg__entity_get(a).id, librg__entity_get(librg__entity_get(a)).id,
           librg__entity_get(librg__entity_get(librg__entity_get(a))).id, librg__entity_get(librg__entity_get(librg__entity_get(librg__entity_get(a)))).id);

    // librg_detach_transform(t);

    librg_entity_destroy(t);

    t = librg_entity_create(0);
    ZPL_ASSERT(t.id == 15 && t.generation == 1); // NOTE(ZaKlaus): This should be re-used entity.

    librg_entity_t ent = librg_entity_create(0);
    librg_entity_t ent2 = librg_entity_create(0);

    librg_attach_position(ent, (position_t) { 1, 2, 3 });
    librg_attach_position(ent2, (position_t) { 1, 2, 3 });
    // librg_attach_rotation(ent, (rotation_t) { 1, 2, 3 });
    librg_attach_zaklaus(ent, (zaklaus_t) { true });

    // librg_detach_zaklaus(ent);

    librg_entity_each((librg_entity_filter_t){
        librg_index_transform(),
        librg_index_position(),
        .excludes1 = librg_index_zaklaus(),
    }, entitycb);

    librg_log("indexes: %d %d %d\n", librg_index_transform(), librg_index_position(), librg_index_zaklaus());


    librg_free();

    return 0;
}
