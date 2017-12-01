enum {
	DEMO_SPAWN_BLOCK = LIBRG_EVENT_LAST,
};

enum {
	DEMO_TYPE_PLAYER,
	DEMO_TYPE_NPC,
};

// enum {
//     component_hero = librg_component_last,
// };

typedef struct {

    struct {
        zplm_vec3_t accel;
        f32 walk_time;
        f32 cooldown;
        i32 max_hp;
        i32 cur_hp;
        librg_limiter_t limiter;
    } stream;

#ifdef DEMO_CLIENT
    // interpolation
    f32 delta;
    zplm_vec3_t curr_pos, last_pos, target_pos;
#endif
} hero_t;

// generate methods for components
// librg_component(hero, component_hero, hero_t);
