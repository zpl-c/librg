#define LIBRG_DEBUG
#define LIBRG_IMPLEMENTATION
#define LIBRG_PLATFORM_BUILD 3
#include <librg.h>
#include <SDL.h>

#define SIZE_X 800
#define SIZE_Y 600

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
librg_entity_t player;

void on_connect_request(librg_event_t *event) {
    librg_log("on_connect_request\n");
    librg_data_wu32(event->data, 42);
}

void on_connect_accepted(librg_event_t *event) {
    librg_log("on_connect_accepted\n");
    player = event->entity;

    librg_log("spawned me with id: %u\n", player);
}

void on_connect_refused(librg_event_t *event) {
    librg_log("on_connect_refused\n");
}

void on_entity_create(librg_event_t *event) {
    // librg_log("on_entity_create: %u\n", event->entity);

    // librg_transform_t *transform = librg_fetch_transform(event->entity);

    // librg_log("spawning entity %u at: %f %f %f\n",
    //     event->entity,
    //     transform->position.x,
    //     transform->position.y,
    //     transform->position.z
    // );
}

void on_entity_update(librg_event_t *event) {
    // librg_transform_t *transform = librg_fetch_transform(event->entity);

    // librg_log("moving entity %u at: %f %f %f\n",
    //     event->entity,
    //     transform->position.x,
    //     transform->position.y,
    //     transform->position.z
    // );
}

void on_client_entity_update(librg_event_t *event) {
//     librg_transform_t *transform = librg_fetch_transform(player);
//     if (!transform) return;

//     librg_log("sending pos: %f %f %f\n",
//         transform->position.x,
//         transform->position.y,
//         transform->position.z
//     );
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

void render_entity(librg_entity_t entity) {
    // set render color
    if (entity == player) {
        SDL_SetRenderDrawColor( sdl_renderer, 150, 250, 150, 255 );
    }
    else if (librg_entity_type(entity) == 1) {
        SDL_SetRenderDrawColor( sdl_renderer, 150, 25, 25, 255 );
    }
    else if (librg_entity_type(entity) == 2) {
        SDL_SetRenderDrawColor( sdl_renderer, 25, 25, 150, 255 );
    }
    else {
        SDL_SetRenderDrawColor( sdl_renderer, 150, 150, 150, 255 );
    }

    SDL_Rect position = default_position();
    librg_transform_t *transform = librg_fetch_transform(entity);

    position.x += transform->position.x;
    position.y += transform->position.y;

    position.w = 20;
    position.h = 20;

    // render
    SDL_RenderFillRect( sdl_renderer, &position );
}

void render()
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
    librg_entity_each((librg_entity_filter_t) {librg_index_transform() }, render_entity);

    // render the changes above
    SDL_SetRenderDrawColor( sdl_renderer, 75, 75, 76, 10 );
    SDL_RenderPresent( sdl_renderer );
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

    librg_init((librg_config_t) {
        .tick_delay     = 32,
        .mode           = LIBRG_MODE_CLIENT,
        .world_size     = zplm_vec2(5000.0f, 5000.0f),
    });

    librg_event_add(LIBRG_CONNECTION_REQUEST, on_connect_request);
    librg_event_add(LIBRG_CONNECTION_ACCEPT, on_connect_accepted);
    librg_event_add(LIBRG_CONNECTION_REFUSE, on_connect_refused);
    librg_event_add(LIBRG_ENTITY_CREATE, on_entity_create);
    librg_event_add(LIBRG_ENTITY_UPDATE, on_entity_update);
    librg_event_add(LIBRG_CLIENT_STREAMER_UPDATE, on_client_entity_update);

    librg_network_start((librg_address_t) { .host = "localhost", .port = 27010 });

    bool loop = true;

    while (loop) {
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

        librg_transform_t *transform = librg_fetch_transform(player);

        if (keys_held[SDLK_t] && transform) {
            zpl_printf("triggering 1 entity spawn server-side.\n");

            librg_send_all(42, librg_lambda(data), {
               librg_data_wptr(&data, transform, sizeof(librg_transform_t));
            });

            // keys_held[SDLK_t] = false;
        }

        if (transform) {
            transform->position.x = (f32)camera.x;
            transform->position.y = (f32)camera.y;
        }

        librg_tick();
        render();
    }

    librg_network_stop();
    librg_free();
    free_sdl();

    return 0;
}
