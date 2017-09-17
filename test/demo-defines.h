enum {
	DEMO_SPAWN_BLOCK = LIBRG_LAST_ENUM_NUMBER,
};

enum {
	DEMO_TYPE_PLAYER,
	DEMO_TYPE_NPC,
};


typedef struct {
	zplm_vec3_t accel;
	f32 walk_time;
	f32 cooldown;
	i32 max_hp;
	i32 cur_hp;
} librg_component(hero);