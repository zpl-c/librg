#define ZPL_IMPLEMENTATION
#define ZPLE_IMPLEMENTATION
#include <zpl.h>
#include <zpl_ent.h>

typedef struct {
    zpl_string_t name;
    i32 age;
} ZPLE_COMP_DECLARE(persona);

typedef struct {
    i32 x,y,z;
} ZPLE_COMP_DECLARE(position);

typedef struct {
    f32 strength;
    f32 wisdom;
    f32 agility;
} ZPLE_COMP_DECLARE(stats);

// NOTE(ZaKlaus): Demonstration purposes
typedef void armor_piece_t;

typedef struct {
    armor_piece_t *Head;
    armor_piece_t *Torso;
    armor_piece_t *Legs;
} ZPLE_COMP_DECLARE(armor);

typedef struct {
    zple_pool entities;

    // NOTE(ZaKlaus): All possible components
    persona_pool  persona;
    position_pool position;
    stats_pool    stats;
    armor_pool    armor;
} world_t;

//// NOTE(ZaKlaus): These are usually placed in exactly ONE source file
ZPLE_COMP_DEFINE(persona);
ZPLE_COMP_DEFINE(position);
ZPLE_COMP_DEFINE(stats);
ZPLE_COMP_DEFINE(armor);
////

void world_init(world_t *w, zpl_allocator_t alloc, isize count) {
    zple_init(&w->entities, alloc, count);

    persona_init(&w->persona, &w->entities, alloc);
    position_init(&w->position, &w->entities, alloc);
    stats_init(&w->stats, &w->entities, alloc);
    armor_init(&w->armor, &w->entities, alloc);
}

void world_free(world_t *w) {
    zple_free(&w->entities);

    persona_free(&w->persona);
    position_free(&w->position);
    stats_free(&w->stats);
    armor_free(&w->armor);
}


zple_entity_t hero_spawn(world_t *world, char *const name, i32 age, position_t pos, f32 strength_factor) {
    zple_entity_t hero = zple_create(&world->entities);

    persona_attach(&world->persona, hero, (persona_t){ zpl_string_make(zpl_heap_allocator(), name), age, });
    position_attach(&world->position, hero, pos);
    armor_attach(&world->armor, hero, (armor_t){0});

    stats_t stats = { strength: 2*strength_factor, wisdom: 7*(age/cast(f32)12), agility: 6*(8.0/age) };
    stats_attach(&world->stats, hero, stats);

    return hero;
}

void hero_kill(world_t *world, zple_entity_t hero) {
    persona_detach(&world->persona, hero);
    position_detach(&world->position, hero);
    stats_detach(&world->stats, hero);
    armor_detach(&world->armor, hero);

    zple_destroy(&world->entities, hero);
}

void hero_dostuff(world_t *world, zple_entity_t hero) {
    zpl_when (persona_fetch(&world->persona, hero), persona_t*, persona) {
        stats_t *stats = stats_fetch(&world->stats, hero);

        zpl_printf("Hero called %s and aged %d with strength: %d, wisdom: %d and agility: %d was here!\n",
                   persona->name, persona->age, stats->strength, stats->wisdom, stats->agility);
    }
}

char *names[] = {
    "Jamie",
    "Ollyria",
    "Jane",
    "Wolfbung",
    "Alizar"
};

int main(void) {

    world_t world = {0};
    world_init(&world, zpl_heap_allocator(), 5);

    // NOTE(ZaKlaus): Our heroes will do stuff
    {
        for (isize i = 0; i < 5; ++i) {
            zple_entity_t hero = hero_spawn(&world, names[i], (25 + (i%3)*4), (position_t){ i, i*2, i*i }, i % 4);

            hero_dostuff(&world, hero);

            hero_kill(&world, hero);
        }
    }

    world_free(&world);

    return 0;
}
