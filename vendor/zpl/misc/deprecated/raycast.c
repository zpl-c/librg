/*

Simple raycaster idea (Wolfenstein3D-esque)

Author: Dominik "ZaKlaus" Madarasz

Dependencies:
zpl.h + ZPL_PLATFORM
zpl_math.h
zpl_gif.h
stb_image.h
stb_image_write.h

Based on:
http://lodev.org/cgtutor/raycasting.html

*/


#define ZPL_IMPLEMENTATION
#define ZPL_PLATFORM
#include "zpl.h"

#define ZPL_MATH_IMPLEMENTATION
#include "zpl_math.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"

#define ZPL_IMAGE_IMPLEMENTATION
#include "zpl_image.h"

#define WIN_WIDTH 800
#define WIN_HEIGHT 600

#define MAP_WIDTH 24
#define MAP_HEIGHT 24
int world_map[MAP_WIDTH][MAP_HEIGHT] =
{
    {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,7,7,7,7,7,7,7,7},
    {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,7},
    {4,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7},
    {4,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7},
    {4,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,7},
    {4,0,4,0,0,0,0,5,5,5,5,5,5,5,5,5,7,7,0,7,7,7,7,7},
    {4,0,5,0,0,0,0,5,0,5,0,5,0,5,0,5,7,0,0,0,7,7,7,1},
    {4,0,6,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,0,0,0,8},
    {4,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,7,7,1},
    {4,0,8,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,0,0,0,8},
    {4,0,0,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,7,7,7,1},
    {4,0,0,0,0,0,0,5,5,5,5,0,5,5,5,5,7,7,7,7,7,7,7,1},
    {6,6,6,6,6,6,6,6,6,6,6,0,6,6,6,6,6,6,6,6,6,6,6,6},
    {8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
    {6,6,6,6,6,6,0,6,6,6,6,0,6,6,6,6,6,6,6,6,6,6,6,6},
    {4,4,4,4,4,4,0,4,4,4,6,0,6,2,2,2,2,2,2,2,3,3,3,3},
    {4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,0,0,0,2},
    {4,0,0,0,0,0,0,0,0,0,0,0,6,2,0,0,5,0,0,2,0,0,0,2},
    {4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,2,0,2,2},
    {4,0,6,0,6,0,0,0,0,4,6,0,0,0,0,0,5,0,0,0,0,0,0,2},
    {4,0,0,5,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,2,0,2,2},
    {4,0,6,0,6,0,0,0,0,4,6,0,6,2,0,0,5,0,0,2,0,0,0,2},
    {4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,0,0,0,2},
    {4,4,4,4,4,4,4,4,4,4,1,1,1,2,2,2,2,2,2,3,3,3,3,3}
};

typedef struct Rect { i32 x, y, w, h; } Rect;

typedef struct Camera {
    f64 pos_x, pos_y;
    f64 dir_x, dir_y;
    f64 plane_x, plane_y;
    
    f64 mov_speed;
    f64 rot_speed;
} Camera;

void
fill_rect(u32 *pixels, u32 colour, Rect rect) {
    ZPL_ASSERT_NOT_NULL(pixels);
    
    for (i32 row = 0; row < rect.h; ++row) {
        for (i32 col = 0; col < rect.w; ++col) {
            pixels[(WIN_HEIGHT - row - rect.y - 1)*WIN_WIDTH + col + rect.x] = colour;
        }
    }
}

void
fill_stripe(u32 *pixels, u32 colour, Rect rect) {
    ZPL_ASSERT_NOT_NULL(pixels);
    
    for (i32 row = rect.y; row < rect.h; ++row) {
        for (i32 col = 0; col < 1; ++col) {
            pixels[(WIN_HEIGHT - row - 1)*WIN_WIDTH + col + rect.x] = colour;
        }
    }
}

zpl_global i32 default_colours[] = {
    0xFF000000,
    0x00FF0000,
    0x0000FF00,
    0xFFFFFF00,
    0xFFFF0000
};

zpl_global f64 identity_filter[3][3] = {
    {0, 0, 0},
    {0, 1, 0},
    {0, 0, 0}
};

int
main(void) {
    zplPlatform p_ = {0};
    zplPlatform*p  = &p_;
    
    zpl_platform_init_with_software(p, "Raycaster", WIN_WIDTH, WIN_HEIGHT, 0);
    
    b32 is_running = true;
    u32 *pixels = p->sw_framebuffer.memory;
    
    Camera player = {0};
    player.pos_x = 22;
    player.pos_y = 12;
    player.dir_x = -1;
    player.dir_y = 0;
    player.plane_x = 0;
    player.plane_y = 0.66;
    
    Rect plane = {0};
    
    Rect window = {0};
    window.w = WIN_WIDTH;
    window.h = WIN_HEIGHT;
    
    Rect test_rect = {
        .x = 0, .y = 20,
        .w = 64, .h = 64
    };
    
#define TEX_DIM 64
    u32 tex_data[8][TEX_DIM*TEX_DIM];
    for(i32 x = 0; x < TEX_DIM; ++x) {
        for (i32 y = 0; y < TEX_DIM; ++y) {
            i32 xor = (x * 256 / TEX_DIM) ^ (y * 256 / TEX_DIM);
            i32 yc = y * 256 / TEX_DIM;
            i32 xyc= y * 128 / TEX_DIM + x * 128 / TEX_DIM;
            
            tex_data[0][TEX_DIM * y + x] = 65536 * 254 * (x != y && x != TEX_DIM - y); //flat red tex_data with black cross
            tex_data[1][TEX_DIM * y + x] = xyc + 256 * xyc + 65536 * xyc; //sloped greyscale
            tex_data[2][TEX_DIM * y + x] = 256 * xyc + 65536 * xyc; //sloped yellow gradient
            tex_data[3][TEX_DIM * y + x] = xor + 256 * xor + 65536 * xor; //xor greyscale
            tex_data[4][TEX_DIM * y + x] = 256 * xor; //xor green
            //tex_data[5][TEX_DIM * y + x] = 65536 * 192 * (x % 16 && y % 16); //red bricks
            tex_data[6][TEX_DIM * y + x] = 65536 * yc; //red gradient
            tex_data[7][TEX_DIM * y + x] = 128 + 256 * 128 + 65536 * 128; //flat grey tex_data
        }
    }
    
    i32 gif_x, gif_y, gif_frames, gif_cursor = 0, gif_cd = 0;
    zplGifResult *gif = zpl_image_gif_load("test2.gif", &gif_x, &gif_y, &gif_frames);
    zplGifResult *gif_ptr = gif;
    
    zplGifResult *frame = gif;
    for (; frame; frame = frame->next) {
        u32 *new_data = zpl_malloc(TEX_DIM * TEX_DIM * 4);
        u32 *old_data = cast(u32 *)frame->data;
        
        zpl_image_rgb_resize(old_data, gif_x, gif_y,
                             new_data, TEX_DIM, TEX_DIM);
        
        zpl_image_rgb_filter(old_data, gif_x, gif_y,
                             new_data, TEX_DIM, TEX_DIM,
                             &identity_filter[0][0], 3, 3);
        
        //zpl_memcopy(new_data, old_data, gif_x * gif_y * 4);
        STBI_FREE(frame->data);
        frame->data = cast(u8 *)new_data;
    }
    
    while(is_running) {
        zpl_platform_update(p);
        
        player.mov_speed = p->dt_for_frame * 5.0;
        player.rot_speed = p->dt_for_frame * 3.0;
        
        // NOTE(ZaKlaus): Handle exit
        if (p->keys[zplKey_Escape] == true ||
            p->keys[zplKey_Space]  == true) {
            is_running = false;
        }
        else if (p->window_is_closed) {
            is_running = false;
        }
        
        // NOTE(ZaKlaus): gif playback
        if (gif_cursor == gif_frames-1) {
            gif_cursor = 0;
            gif_ptr = gif;
        }
        else {
            if (!gif_cd) {
                ++gif_cursor;
                gif_ptr = gif_ptr->next;
            }
        }
        
        if (!gif_cd) {
            gif_cd  = gif_ptr->delay;
        }
        else {
            gif_cd -= 2;
        }
        
        // NOTE(ZaKlaus): controls
        if(p->keys[zplKey_W]) {
            if (world_map[cast(i32)(player.pos_x + player.dir_x*player.mov_speed)][cast(i32)player.pos_y] == false) player.pos_x += player.dir_x * player.mov_speed;
            if (world_map[cast(i32)player.pos_x][cast(i32)(player.pos_y + player.dir_y*player.mov_speed)] == false) player.pos_y += player.dir_y * player.mov_speed;
        }
        if(p->keys[zplKey_S]) {
            if (world_map[cast(i32)(player.pos_x - player.dir_x*player.mov_speed)][cast(i32)player.pos_y] == false) player.pos_x -= player.dir_x * player.mov_speed;
            if (world_map[cast(i32)player.pos_x][cast(i32)(player.pos_y - player.dir_y*player.mov_speed)] == false) player.pos_y -= player.dir_y * player.mov_speed;
        }
        if(p->keys[zplKey_D]) {
            f64 old_dir_x = player.dir_x;
            player.dir_x = player.dir_x * zpl_cos(-player.rot_speed) - player.dir_y * zpl_sin(-player.rot_speed);
            player.dir_y = old_dir_x * zpl_sin(-player.rot_speed) + player.dir_y * zpl_cos(-player.rot_speed);
            
            f64 old_plane_x = player.plane_x;
            player.plane_x = player.plane_x * zpl_cos(-player.rot_speed) - player.plane_y * zpl_sin(-player.rot_speed);
            player.plane_y = old_plane_x * zpl_sin(-player.rot_speed) + player.plane_y * zpl_cos(-player.rot_speed);
        }
        if (p->keys[zplKey_A]) {
            f64 old_dir_x = player.dir_x;
            player.dir_x = player.dir_x * zpl_cos(player.rot_speed) - player.dir_y * zpl_sin(player.rot_speed);
            player.dir_y = old_dir_x * zpl_sin(player.rot_speed) + player.dir_y * zpl_cos(player.rot_speed);
            
            f64 old_plane_x = player.plane_x;
            player.plane_x = player.plane_x * zpl_cos(player.rot_speed) - player.plane_y * zpl_sin(player.rot_speed);
            player.plane_y = old_plane_x * zpl_sin(player.rot_speed) + player.plane_y * zpl_cos(player.rot_speed);
        }
        
        // NOTE(ZaKlaus): clear the screen
        fill_rect(pixels, 0, window);
        
        // NOTE(ZaKlaus): perform raycasting
        for (i32 x = 0; x < WIN_WIDTH; ++x) {
            f64 camera_x = 2 * x / cast(f64)WIN_WIDTH - 1;
            f64 raypos_x = player.pos_x;
            f64 raypos_y = player.pos_y;
            f64 raydir_x = player.dir_x + player.plane_x*camera_x;
            f64 raydir_y = player.dir_y + player.plane_y*camera_x;
            
            i32 map_x = cast(i32)raypos_x;
            i32 map_y = cast(i32)raypos_y;
            
            f64 side_dist_x, side_dist_y;
            
            f64 delta_dist_x = zpl_sqrt(1 + (raydir_y * raydir_y) / (raydir_x * raydir_x));
            f64 delta_dist_y = zpl_sqrt(1 + (raydir_x * raydir_x) / (raydir_y * raydir_y));
            f64 wall_dist;
            
            i32 step_x, step_y;
            
            i32 hit = 0;
            i32 side = 0;
            
            if (raydir_x < 0) {
                step_x = -1;
                side_dist_x = (raypos_x - map_x) * delta_dist_x;
            }
            else {
                step_x = 1;
                side_dist_x = (map_x + 1.0 - raypos_x) * delta_dist_x;
            }
            
            if (raydir_y < 0) {
                step_y = -1;
                side_dist_y = (raypos_y - map_y) * delta_dist_y;
            }
            else {
                step_y = 1;
                side_dist_y = (map_y + 1.0 - raypos_y) * delta_dist_y;
            }
            
            while (!hit) {
                if (side_dist_x < side_dist_y) {
                    side_dist_x += delta_dist_x;
                    map_x += step_x;
                    side = 0;
                }
                else {
                    side_dist_y += delta_dist_y;
                    map_y += step_y;
                    side = 1;
                }
                
                if (world_map[map_x][map_y] > 0) {
                    hit = 1;
                }
            }
            
            if (side == 0) {
                wall_dist = (map_x - raypos_x + (1.0 - step_x) / 2.0) / raydir_x;
            }
            else {
                wall_dist = (map_y - raypos_y + (1.0 - step_y) / 2.0) / raydir_y;
            }
            
            i32 line_h = cast(i32)(WIN_HEIGHT / wall_dist);
            
            i32 draw_y = -line_h / 2 + WIN_HEIGHT / 2;
            if (draw_y < 0) draw_y = 0;
            i32 draw_h =  line_h / 2 + WIN_HEIGHT / 2;
            if (draw_h >= WIN_HEIGHT) draw_h = WIN_HEIGHT - 1;
            
            i32 col_index = world_map[map_x][map_y] - 1;
            
            f32 wall_x;
            if (side == 0) wall_x = raypos_y + wall_dist * raydir_y;
            else           wall_x = raypos_x + wall_dist * raydir_x;
            wall_x -= zpl_floor(wall_x);
            
            i32 tex_x = cast(i32)(wall_x * cast(f64)TEX_DIM);
            if (side == 0 && raydir_x > 0) tex_x = TEX_DIM - tex_x - 1;
            if (side == 1 && raydir_y < 0) tex_x = TEX_DIM - tex_x - 1;
            
            plane.x = x;
            plane.w = 1;
            plane.y = draw_y;// + (WIN_HEIGHT / 2);
            plane.h = draw_h;// - (WIN_HEIGHT / 3);
            
            for (i32 y = draw_y; y < draw_h; ++y) {
                i32 d = y * 256 - WIN_HEIGHT * 128 + line_h * 128;
                i32 tex_y = ((d * TEX_DIM) / line_h) / 256;
                
                u32 colour = 0x00000000;
                if (col_index != 5) {
                    
                    colour = tex_data[col_index][TEX_DIM * tex_x + tex_y];
                }
                else {
                    u32 *data = cast(u32 *)gif_ptr->data;
                    colour = data[(tex_y) * TEX_DIM + tex_x];
                    
                    u8 *blue = cast(u8 *)(&colour);
                    u8 *red  = cast(u8 *)zpl_pointer_add(&colour, 2);
                    zpl_memswap(red, blue, zpl_size_of(u8));
                }
                //if (side == 1) colour = (colour >> 1) & 8355711;
                pixels[(WIN_HEIGHT - y - 1)*WIN_WIDTH + x] = colour;
            }
            
            // NOTE(ZaKlaus): floor
            f64 floor_x_wall, floor_y_wall;
            
            /**/ if (side == 0 && raydir_x > 0) {
                floor_x_wall = map_x;
                floor_y_wall = map_y + wall_x;
            }
            else if (side == 0 && raydir_x < 0) {
                floor_x_wall = map_x + 1.0;
                floor_y_wall = map_y + wall_x;
            }
            else if (side == 1 && raydir_y > 0) {
                floor_x_wall = map_x + wall_x;
                floor_y_wall = map_y;
            }
            else {
                floor_x_wall = map_x + wall_x;
                floor_y_wall = map_y + 1.0;
            }
            
            f64 dist_wall, dist_player, curr_dist;
            
            dist_wall   = wall_dist;
            dist_player = 0.0;
            
            if (draw_h < 0) draw_h = WIN_HEIGHT;
            
            for (int y = draw_h + 1; y < WIN_HEIGHT; ++y) {
                curr_dist = WIN_HEIGHT / (2.0 * y - WIN_HEIGHT);
                
                f64 weight = (curr_dist - dist_player) / (dist_wall - dist_player);
                
                f64 curr_floor_x = weight * floor_x_wall + (1.0 - weight) * player.pos_x;
                f64 curr_floor_y = weight * floor_y_wall + (1.0 - weight) * player.pos_y;
                
                i32 floor_tex_x, floor_tex_y;
                floor_tex_x = cast(i32)(curr_floor_x * TEX_DIM) % TEX_DIM;
                floor_tex_y = cast(i32)(curr_floor_y * TEX_DIM) % TEX_DIM;
                
                u32 colour;
                
                // NOTE(ZaKlaus): floor
                u32 *data = cast(u32 *)gif_ptr->data;
                colour = (data[(floor_tex_y) * TEX_DIM + floor_tex_x] >> 1) & 8355711;
                
                u8 *blue = cast(u8 *)(&colour);
                u8 *red  = cast(u8 *)zpl_pointer_add(&colour, 2);
                zpl_memswap(red, blue, zpl_size_of(u8));
                
                pixels[(WIN_HEIGHT - y - 1)*WIN_WIDTH + x] = colour;
                
                // NOTE(ZaKlaus): ceil
                colour = tex_data[3][floor_tex_y * TEX_DIM + floor_tex_x];
                pixels[(y)*WIN_WIDTH + x] = colour;
            }
        }
        
        zpl_platform_display(p);
        zpl_sleep_ms(10);
    }
}