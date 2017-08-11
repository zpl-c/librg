

#define ZPL_IMPLEMENTATION
#define ZPL_PLATFORM
#include "zpl.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"

#define ZPL_IMAGE_IMPLEMENTATION
#include "zpl_image.h"

#define WIN_WIDTH 1280
#define WIN_HEIGHT 720

typedef struct Rect {
    i32 x;
    i32 y;
    i32 w;
    i32 h;
} Rect;

void
fill_rect(u32 *pixels, u32 colour, Rect rect)
{
    ZPL_ASSERT_NOT_NULL(pixels);
    
    for (i32 row = 0; row < rect.h; ++row) {
        for (i32 col = 0; col < rect.w; ++col) {
            pixels[(WIN_HEIGHT-row-rect.y-1)*WIN_WIDTH + rect.x + col] = colour;
        }
    }
}

zpl_global u8 slider = 0;

void
paint_rect(u32 *pixels, u32 *image_data, i32 image_width, Rect rect)
{
    ZPL_ASSERT_NOT_NULL(pixels);
    ZPL_ASSERT_NOT_NULL(image_data);
    
    for (i32 row = 0; row < rect.h; ++row) {
        for (i32 col = 0; col < rect.w; ++col) {
            u32 colour = image_data[(row)*image_width + col];
            u8 *blue = cast(u8 *)(&colour);
            u8 *red  = cast(u8 *)zpl_pointer_add(&colour, 2);
            zpl_memswap(red, blue, zpl_size_of(u8));
            
            zplHsvColour hsv_colour = zpl_image_rgb_to_hsv(hard_cast(zplRgbColour)colour);
            
            //hsv_colour.h = slider;
            //hsv_colour.s += slider; hsv_colour.s = zpl_clamp(hsv_colour.s, 0, 255);
            //hsv_colour.v = slider;
            
            zplRgbColour rgb_colour = zpl_image_hsv_to_rgb(hsv_colour);
            colour = hard_cast(u32)rgb_colour;
            
            pixels[(WIN_HEIGHT - row-rect.y-1)*WIN_WIDTH + rect.x + col] = colour;
        }
    }
}

void
draw_char(u32 *pixels, u32 *font, u8 c, i32 font_w, Rect rect) {
    ZPL_ASSERT_NOT_NULL(pixels);
    ZPL_ASSERT_NOT_NULL(font);
    
    c -= 32; // NOTE(ZaKlaus): ASCII indexing for visible glyphs starts at 32
    
    i32 cell_x = font_w / rect.w;
    
    for (i32 row = 0; row < rect.h; ++row) {
        for (i32 col = 0; col < rect.w; ++col) {
            
            i32 fx = rect.w*(c % cell_x) + col;
            i32 fy = rect.h*(round(c / cell_x)) + row;
            
            u32 colour = font[fy*font_w + fx];
            
            if (colour > 0) {
                pixels[(WIN_HEIGHT - row-rect.y-1)*WIN_WIDTH + rect.x + col] = colour;
            }
        }
    }
}

void
draw_text(u32 *pixels, u32 *font, zplString str, i32 font_w, Rect rect) {
    i32 str_len = zpl_string_length(str);
    i32 n = 0;
    
    do {
        draw_char(pixels, font, str[n++], font_w, rect);
        rect.x += rect.w;
    }
    while (--str_len > 0);
}

int
main(void)
{
    zplPlatform p_ = {0};
    zplPlatform *p = &p_;
    
    zpl_platform_init_with_software(p, "Test", WIN_WIDTH, WIN_HEIGHT, 0);
    
    b32 IsRunning = true;
    u32 *pixels = p->sw_framebuffer.memory;
    
    Rect window_rect = {
        .x = 0, .y = 0,
        .w = p->window_width, .h = p->window_height
    };
    
    i32 player_size = 64;
    i32 player_x = window_rect.w/2 - player_size/2; 
    i32 player_y = window_rect.h/2 - player_size/2; 
    i32 player_speed = 2; 
    
    Rect player_rect = {
        .x = player_x, .y = player_y,
        .w = player_size, .h = player_size
    };
    
    i32 font_x, font_y, font_n;
    u8 *font = stbi_load("font.bmp", &font_x, &font_y, &font_n, 4);
    
    i32 image_x, image_y, image_n;
    u8 *image_data = stbi_load("test0.jpg", &image_x, &image_y, &image_n, 4);
    u32 *filt_mem = zpl_malloc(image_x*image_y*4);
    
    f64 filter[5][5] = {
        -1, -1, -1, -1,  0,
        -1, -1, -1,  0,  1,
        -1, -1,  0,  1,  1,
        -1,  0,  1,  1,  1,
        0,  1,  1,  1,  1
    };
    
    zpl_image_rgb_filter(cast(u32 *)image_data, image_x, image_y,
                         filt_mem,
                         &filter[0][0], 5, 5,
                         1.0, 128.0);
    
    zpl_memcopy(image_data, filt_mem, image_x*image_y*4);
    
    Rect image_rect = {
        .x = 10, .y = 10,
        .w = image_x, .h = image_y
    };
    
    i32 gif_x, gif_y, gif_frames, gif_cursor = 0, gif_cd = 0;
    zplGifResult *gif = zpl_image_gif_load("test1.gif", &gif_x, &gif_y, &gif_frames);
    zplGifResult *gif_ptr = gif;
    //u32 *blur_mem = zpl_malloc(gif_x*gif_y*64);
    
    
    zplGifResult *node = gif;
    for(; node; node = node->next) {
        u8 *old_mem = node->data;
        u8 *new_mem = zpl_malloc(gif_x*gif_y*64);
        
        zpl_image_rgb_resize(cast(u32 *)old_mem, gif_x, gif_y,
                             cast(u32 *)new_mem, gif_x*4, gif_y*4, 
                             512, new_mem);
        
        node->data = new_mem;
        
        STBI_FREE(old_mem);
    }
    
    //zpl_mfree(blur_mem);
    zpl_mfree(filt_mem);
    
    gif_x *= 4;
    gif_y *= 4;
    
    Rect gif_rect = {
        .x = image_x+10, .y = 10,
        .w = gif_x, .h = gif_y
    };
    
    zplString str = 0;
    str = zpl_string_make(zpl_heap_allocator(), "Hello World!");
    
    while (IsRunning) {
        zpl_platform_update(p);
        
        if (p->keys[zplKey_Escape] == true ||
            p->keys[zplKey_Space]  == true) {
            IsRunning = false;
        }
        else if (p->window_is_closed) {
            IsRunning = false;
        }
        
        // NOTE(ZaKlaus): player controls
        if(p->keys[zplKey_A]) {
            player_x -= player_speed;
        }
        if (p->keys[zplKey_D]) {
            player_x += player_speed;
        }
        if (p->keys[zplKey_W]) {
            player_y -= player_speed;
        }
        if (p->keys[zplKey_S]) {
            player_y += player_speed;
        }
        
        slider = zpl_clamp(slider, 0, 255);
        
        /**/ if (p->keys[zplKey_K]) {
            slider--;
        }
        else if (p->keys[zplKey_L]) {
            slider++;
        }
        
        player_x = zpl_clamp(player_x, 0, p->window_width -player_size);
        player_y = zpl_clamp(player_y, 0, p->window_height-player_size);
        
        player_rect.x = player_x;
        player_rect.y = player_y;
        
        // NOTE(ZaKlaus): gif playback
        
        //gif_delay = &gif_data[(slider+1) * 4 * gif_x * gif_y + pad];
        //u8 delay[2] = { *(gif_delay), *(gif_delay+1) };
        
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
        
        // NOTE(ZaKlaus): clear the screen
        fill_rect(pixels, 0x000000, window_rect);
        
        // NOTE(ZaKlaus): draw the image
        paint_rect(pixels, cast(u32 *)image_data, image_x, image_rect);
        
        // NOTE(ZaKlaus): draw the gif
        paint_rect(pixels, cast(u32 *)gif_ptr->data, gif_x, gif_rect);
        
        // NOTE(ZaKlaus): render the player
        fill_rect(pixels, 0xFF0000, player_rect);
        
        Rect font_rect = {
            .x = 10, .y = WIN_HEIGHT - 100,
            .w = 32, .h = 32
        };
        
        draw_text(pixels, cast(u32 *)font, str, font_x, font_rect);
        
        zpl_platform_display(p);
        
        zpl_sleep_ms(16);
    }
    
    zpl_image_gif_free(gif, true);
    
    return 0;
}