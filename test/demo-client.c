#define LIBRG_DEBUG
#define LIBRG_IMPLEMENTATION
#include <librg.h>
#include <librg_limiter.h>
#include <SDL.h>

#define DEMO_CLIENT

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


#define SIZE_X 800
#define SIZE_Y 600

zpl_global f64 last_delta;
zpl_global f64 last_time;

/**
 * SDL PART
 */
SDL_Window *sdl_window;
SDL_Renderer *sdl_renderer;

int init_sdl() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        return 1;
    }

    if (!(sdl_window = SDL_CreateWindow("client", 100, 100, SIZE_X, SIZE_Y, 0))) {
        SDL_Quit();
        return 1;
    }

    if (!(sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC))) {
        SDL_DestroyWindow(sdl_window);
        SDL_Quit();
        return 1;
    }

    // Set size of renderer to the same as window
    SDL_RenderSetLogicalSize(sdl_renderer, SIZE_X, SIZE_Y);
    SDL_SetRenderDrawBlendMode(sdl_renderer, SDL_BLENDMODE_BLEND);

    // Set color of renderer to green
    SDL_SetRenderDrawColor(sdl_renderer, 39, 40, 34, 150);

    return 0;
}

void free_sdl() {
    SDL_DestroyRenderer(sdl_renderer);
    SDL_DestroyWindow(sdl_window);
    SDL_Quit();
}


/**
 * LIBRG PART
 */
SDL_Rect camera;
librg_entity_id player;

void on_connect_request(librg_event_t *event) {
    librg_log("on_connect_request\n");
    librg_data_wu32(event->data, 42);
}

void on_connect_accepted(librg_event_t *event) {
    librg_log("on_connect_accepted\n");
    player = event->entity->id;

    librg_log("spawned me with id: %u\n", player);
}

void on_connect_refused(librg_event_t *event) {
    librg_log("on_connect_refused\n");
}

void on_entity_create(librg_event_t *event) {
    switch (librg_entity_type(event->ctx, event->entity->id)) {
        case DEMO_TYPE_PLAYER:
            break;
        case DEMO_TYPE_NPC: {
            event->entity->user_data = zpl_malloc(sizeof(hero_t));
            hero_t *hero = (hero_t *)event->entity->user_data;
            librg_data_rptr(event->data, event->entity->user_data, sizeof(hero->stream));

            hero->curr_pos = event->entity->position;
            hero->last_pos = event->entity->position;
            hero->target_pos = event->entity->position;
            hero->delta = 0.0f;
        } break;
    }
}

void on_entity_update(librg_event_t *event) {
    hero_t *hero = (hero_t *)event->entity->user_data;
    if (!hero) return;

    hero->last_pos = hero->target_pos;
    hero->target_pos = event->entity->position;
    hero->delta = .0f;
}

void on_client_entity_update(librg_event_t *event) {
    // ..
}

/**
 * GAMEPLAY PART
 */

SDL_Rect default_position() {
    SDL_Rect position;

    position.x = SIZE_X / 2 - camera.x;
    position.y = SIZE_Y / 2 - camera.y;

    return position;
}

void render_entity(librg_ctx_t *ctx, librg_entity_t *blob) {
    librg_entity_id entity = blob->id;

    // set render color
    if (entity == player) {
        SDL_SetRenderDrawColor( sdl_renderer, 150, 250, 150, 255 );
    }
    else if (librg_entity_type(ctx, entity) == DEMO_TYPE_NPC) {
        SDL_SetRenderDrawColor( sdl_renderer, 150, 25, 25, 255 );
    }
    else if (librg_entity_type(ctx, entity) == DEMO_TYPE_NPC) {
        SDL_SetRenderDrawColor( sdl_renderer, 25, 25, 150, 255 );
    }
    else {
        SDL_SetRenderDrawColor( sdl_renderer, 150, 150, 150, 255 );
    }

    SDL_Rect position = default_position();

    hero_t *hero = (hero_t *)blob->user_data;

    if (entity == player || !hero) {
        position.x += blob->position.x - 10;
        position.y += blob->position.y - 10;
    }
    else {
        position.x += hero->curr_pos.x - 10;
        position.y += hero->curr_pos.y - 10;
    }

    position.w = 20;
    position.h = 20;

    // render
    SDL_RenderFillRect( sdl_renderer, &position );

    if (entity == player) {
        SDL_Rect zone = default_position();

        zone.x = position.x - 250;
        zone.y = position.y - 250;
        zone.w = 500;
        zone.h = 500;

        SDL_SetRenderDrawColor( sdl_renderer, 133, 133, 133, 75 );
        SDL_RenderFillRect( sdl_renderer, &zone );
    }

    position.h = 5;
    SDL_SetRenderDrawColor(sdl_renderer, 255, 0, 0, 150);
    SDL_RenderFillRect(sdl_renderer, &position);
}

void render(librg_ctx_t *ctx)
{
    // clear the window and make it all green
    SDL_RenderClear( sdl_renderer );
    SDL_SetRenderDrawColor( sdl_renderer, 90, 90, 90, 255 );

    // render world
    i32 size = 1024;
    i32 half = size / 2;
    i32 cell = 72;

    for (isize i = 0; i < size; ++i) {
        SDL_RenderDrawLine(sdl_renderer, -half * cell, -camera.y + i * cell, half * cell, -camera.y + i * cell);
    }

    for (isize i = 0; i < size; ++i) {
        SDL_RenderDrawLine(sdl_renderer, -camera.x + i * cell, -half * cell, -camera.x + i * cell, half * cell);
    }

    // render entities
    librg_entity_iterate(ctx, LIBRG_ENTITY_ALIVE, render_entity);

    // render the changes above
    SDL_SetRenderDrawColor( sdl_renderer, 75, 75, 76, 10 );
    SDL_RenderPresent( sdl_renderer );
}

void on_entity_remove(librg_event_t *event) {
    if (event->entity->type == DEMO_TYPE_NPC) {
        zpl_mfree(event->entity->user_data);
    }
}

void interpolate_npcs(librg_ctx_t *ctx) {
    for (u32 i = 0; i < ctx->max_entities; i++) {
        if (i == player) continue;

        librg_entity_t *entity = librg_entity_fetch(ctx, i);

        if (!entity) continue;

        hero_t *hero = (hero_t *)entity->user_data;
        if (!hero) continue;

        hero->delta += (last_delta /(f32) (ctx->tick_delay));

        zplm_vec3_t delta_pos;
        zplm_vec3_lerp(&delta_pos, hero->last_pos, hero->target_pos, zpl_clamp01(hero->delta));

        hero->curr_pos = delta_pos;
    }
}


bool shooting = false;
bool keys_held[323] = { false };

#ifdef main
#undef main
#endif /* main */

int main(int argc, char *argv[]) {
    if (init_sdl() != 0) {
        return librg_log("error creating sdl\n");
    }

#ifdef ZPL_SYSTEM_WINDOWS
    AllocConsole();
    freopen("conin$","r",stdin);
    freopen("conout$","w",stdout);
    freopen("conout$","w",stderr);
#endif

    librg_log("%s\n\n", "===============      CLIENT      =================\n" \
                        "==                                              ==\n" \
                        "==                 ¯\\_(ツ)_/¯                   ==\n" \
                        "==                                              ==\n" \
                        "==================================================\n");

    librg_ctx_t ctx     = {0};
    ctx.tick_delay      = 64;
    ctx.mode            = LIBRG_MODE_CLIENT;
    ctx.world_size      = zplm_vec3(5000.0f, 5000.0f, 0.f);
    ctx.max_entities    = 2000;

    librg_init(&ctx);

    librg_event_add(&ctx, LIBRG_CONNECTION_REQUEST, on_connect_request);
    librg_event_add(&ctx, LIBRG_CONNECTION_ACCEPT, on_connect_accepted);
    librg_event_add(&ctx, LIBRG_CONNECTION_REFUSE, on_connect_refused);
    librg_event_add(&ctx, LIBRG_ENTITY_CREATE, on_entity_create);
    librg_event_add(&ctx, LIBRG_ENTITY_UPDATE, on_entity_update);
    librg_event_add(&ctx, LIBRG_ENTITY_REMOVE, on_entity_remove);
    librg_event_add(&ctx, LIBRG_CLIENT_STREAMER_UPDATE, on_client_entity_update);

    librg_network_start(&ctx, (librg_address_t) { .host = "localhost", .port = 7777 });

    bool loop = true;

    while (loop) {
        f64 curr_time = zpl_utc_time_now();
        last_delta = (curr_time - last_time) / 1000.f;
        last_time = curr_time;

        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                loop = false;

            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym < 323) keys_held[event.key.keysym.sym] = true;

                switch (event.key.keysym.sym) {
                    case SDLK_SPACE:
                        shooting = true;
                        break;
                }
            }
            else if (event.type == SDL_KEYUP) {
                if (event.key.keysym.sym < 323) keys_held[event.key.keysym.sym] = false;
            }
        }

        i32 speed = 5;

        if (keys_held[SDLK_a]) {
            camera.x -= speed;
        }
        if (keys_held[SDLK_d]) {
            camera.x += speed;
        }
        if (keys_held[SDLK_w]) {
            camera.y -= speed;
        }
        if (keys_held[SDLK_s]) {
            camera.y += speed;
        }
        if (keys_held[SDLK_f]) {
            loop = false;
        }

        if (librg_entity_valid(&ctx, player)) {
            librg_entity_t *blob = librg_entity_fetch(&ctx, player);

            blob->position.x = (f32)camera.x;
            blob->position.y = (f32)camera.y;
        }

        librg_tick(&ctx);
        interpolate_npcs(&ctx);
        render(&ctx);
    }

    librg_network_stop(&ctx);
    librg_free(&ctx);
    free_sdl();

    return 0;
}
