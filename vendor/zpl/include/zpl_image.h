/*

ZPL - Image module

License:
  This software is dual-licensed to the public domain and under the following
  license: you are granted a perpetual, irrevocable license to copy, modify,
  publish, and distribute this file as you see fit.

Warranty:
  No warranty is implied, use at your own risk.

Usage:
  #define ZPLI_IMPLEMENTATION exactly in ONE source file right BEFORE including the library, like:

  #define ZPLI_IMPLEMENTATION
  #include"zpl_image.h"

Dependencies:
  zpl.h
  stb_image.h

  Make sure you properly include them!

Optional switches:
  ZPLE_NO_GIF
  ZPLE_NO_IMAGE_OPS

Credits:
  Dominik Madarasz (GitHub: zaklaus)
  Sean Barrett (GitHub: nothings)
  GitHub: urraka

Version History:
  1.0.2 -- Switch fixes
  1.0.1 -- Got rid of unused switches and fixes
  1.0.0 -- Initial version
*/

#ifndef ZPL_INCLUDE_ZPL_IMAGE_H
#define ZPL_INCLUDE_ZPL_IMAGE_H

#if defined(__cplusplus)
extern "C" {
#endif

  typedef union zpli_rgb_colour_t {
      u32 colour;
      struct { u8 r, g, b;};
  } zpli_rgb_colour_t;

  typedef struct zpli_hsv_colour_t {
      u32 colour;
      struct { u8 h, s, v; };
  } zpli_hsv_colour_t;

  ZPL_DEF zpli_rgb_colour_t zpli_rgb_lerp(zpli_rgb_colour_t a, zpli_rgb_colour_t b, f32 t);

  ////////////////////////////////////////////////////////////////
  //
  // GIF Loader
  //
  // Uses stb_image.h for loading gif frames.
  //

#ifndef ZPLE_NO_GIF
  typedef struct zpli_gif_result_t {
      i32 delay;
      u8 *data;
      struct zpli_gif_result_t *next;
  } zpli_gif_result_t;

  ZPL_DEF zpli_gif_result_t *zpli_gif_load(char const *filename, i32 *x, i32 *y, i32 *frames);
  ZPL_DEF void          zpli_gif_free(zpli_gif_result_t *gif, b32 aligned);
#endif

  ////////////////////////////////////////////////////////////////
  //
  // Image Operations
  //

#ifndef ZPLE_NO_IMAGE_OPS
  // NOTE(ZaKlaus): This is not sRGB aware!
  ZPL_DEF void zpli_rgb_resize(u32 *source, i32 source_w, i32 source_h,
                                    u32 *dest, i32 dest_w, i32 dest_h,
                                    i32 blur_iter, u32 *blur_mem);

  ZPL_DEF void zpli_rgb_filter(u32 *source, i32 source_w, i32 source_h,
                                    u32 *dest,
                                    f64 *filter, i32 filter_w, i32 filter_h,
                                    f64 factor, f64 bias);

  // TODO(ZaKlaus): Implement this
  /*
  ZPL_DEF void         zpli_init_srgb_table(u8 **table);
  ZPL_DEF zpli_rgb_colour_t zpli_lin_to_srgb    (u8 *table, f64 vals[3]);
  */

  ZPL_DEF zpli_hsv_colour_t zpli_rgb_to_hsv     (zpli_rgb_colour_t colour);
  ZPL_DEF zpli_rgb_colour_t zpli_hsv_to_rgb     (zpli_hsv_colour_t colour);
#endif

#if defined(__cplusplus)
}
#endif

#if defined(ZPLI_IMPLEMENTATION) && !defined(ZPLI_IMPLEMENTATION_DONE)
#define ZPLI_IMPLEMENTATION_DONE

#if defined(__cplusplus)
extern "C" {
#endif

  zpli_rgb_colour_t zpli_rgb_lerp(zpli_rgb_colour_t a, zpli_rgb_colour_t b, f32 t) {
#define LERP(c1, c2, c3) c1*(1.0-c3) + c2*c3
      zpli_rgb_colour_t result = {0};

      result.r = LERP(a.r, b.r, t);
      result.g = LERP(a.g, b.g, t);
      result.b = LERP(a.b, b.b, t);

      return result;
#undef LERP
  }

  // NOTE(ZaKlaus): Gif

#ifndef ZPL_NO_GIF
  zpli_gif_result_t *zpli_gif_load(char const *filename, i32 *x, i32 *y, i32 *frames) {
      FILE *file;
      stbi__context s;
      zpli_gif_result_t *result;

      if (!(file = stbi__fopen(filename, "rb"))) {
          stbi__errpuc("can't open file", "Unable to open file"); return 0;
      }

      stbi__start_file(&s, file);

      if (stbi__gif_test(&s)) {
          i32 c;
          stbi__gif g;
          zpli_gif_result_t *head = stbi__malloc(zpl_size_of(zpli_gif_result_t));
          zpli_gif_result_t *prev = 0, *gr = head;

          zpl_zero_item(&g);
          zpl_zero_item(head);

          *frames = 0;

          while(gr->data = stbi__gif_load_next(&s, &g, &c, 4)) {
              if (gr->data == cast(u8 *)&s) {
                  gr->data = 0;
                  break;
              }

              if (prev) prev->next = gr;
              gr->delay = g.delay;
              prev = gr;
              gr = cast(zpli_gif_result_t *)stbi__malloc(zpl_size_of(zpli_gif_result_t));
              zpl_zero_item(gr);
              ++(*frames);
          }

          STBI_FREE(g.out);

          if (gr != head) {
              //STBI_FREE(gr);
          }

          if (*frames > 0) {
              *x = g.w;
              *y = g.h;
          }

          result = head;
      }
      else {
          // TODO(ZaKlaus): Add support.
          result = 0; //stbi__load_main(&s, x, y, frames, &bpp, 4, &ri, 8);
          *frames = !!result;
      }

      fclose(file);
      return result;
  }

  void zpli_gif_free(zpli_gif_result_t *gif, b32 aligned) {
      zpli_gif_result_t *elem, *prev = 0;
      for (elem = gif; elem; elem = elem->next) {
          if (aligned) {
              zpl_mfree(elem->data);
          }
          else {
              STBI_FREE(elem->data);
          }
          STBI_FREE(prev);
          prev = elem;
      }

      STBI_FREE(prev);
  }
#endif

  zpli_hsv_colour_t zpli_rgb_to_hsv(zpli_rgb_colour_t colour) {
      zpli_hsv_colour_t result = {0};
      u8 rgb_min, rgb_max;

      rgb_min = colour.r < colour.g ? (colour.r < colour.b ? colour.r : colour.b) : (colour.g < colour.b ? colour.g : colour.b);
      rgb_max = colour.r > colour.g ? (colour.r > colour.b ? colour.r : colour.b) : (colour.g > colour.b ? colour.g : colour.b);

      result.v = rgb_max;
      if (result.v == 0) {
          result.h = result.s = 0;
          return result;
      }

      result.s = 255 * cast(i64)(rgb_max - rgb_min) / result.v;
      if (result.s == 0) {
          result.h = 0;
          return result;
      }

      /**/ if (rgb_max == colour.r) {
          result.h = 0   + 43 * (colour.g - colour.b) / (rgb_max - rgb_min);
      }
      else if (rgb_max == colour.g) {
          result.h = 85  + 43 * (colour.b - colour.r) / (rgb_max - rgb_min);
      }
      else {
          result.h = 171 + 43 * (colour.r - colour.g) / (rgb_max - rgb_min);
      }

      return result;
  }

  zpli_rgb_colour_t zpli_hsv_to_rgb(zpli_hsv_colour_t colour) {
      zpli_rgb_colour_t result = {0};
      u8 region, rem, p, q, t;

      if (colour.s == 0) {
          result.r = result.g = result.b = colour.v;
          return result;
      }

      region =  colour.h / 43;
      rem    = (colour.h - (region * 43)) * 6;

      p = (colour.v * (255 - colour.s)) >> 8;
      q = (colour.v * (255 - ((colour.s * rem) >> 8))) >> 8;
      t = (colour.v * (255 - ((colour.s * (255 - rem)) >> 8))) >> 8;

      switch (region)
      {
          case 0: result.r = colour.v; result.g = t; result.b = p; break;
          case 1: result.r = q; result.g = colour.v; result.b = p; break;
          case 2: result.r = p; result.g = colour.v; result.b = t; break;
          case 3: result.r = p; result.g = q; result.b = colour.v; break;
          case 4: result.r = t; result.g = p; result.b = colour.v; break;
          default:result.r = colour.v; result.g = p; result.b = q; break;
      }

      return result;
  }

#ifndef ZPL_NO_IMAGE_OPS
  void zpli_rgb_resize(u32 *source, i32 source_w, i32 source_h,
                                       u32 *dest, i32 dest_w, i32 dest_h,
                                       i32 blur_iter, u32 *blur_mem) {

      zpli_rgb_colour_t *src = cast(zpli_rgb_colour_t *)&(source);
      zpli_rgb_colour_t *dst = cast(zpli_rgb_colour_t *)&(dest);

      b32 x_down = dest_w < source_w;
      b32 y_down = dest_h < source_h;

      i32 step_x;
      i32 step_y;

      if(x_down) {
          step_x = cast(i32)(source_w / cast(f32)dest_w);
      }
      else {
          step_x = cast(i32)(dest_w / cast(f32)source_w);
      }

      if(y_down) {
          step_y = cast(i32)(source_h / cast(f32)dest_h);
      }
      else {
          step_y = cast(i32)(dest_h / cast(f32)source_h);
      }



      for (i32 y = 0; y < dest_h; ++y) {
          for(i32 x = 0; x < dest_w; ++x) {
              zpli_rgb_colour_t colour = {0};

              i32 o_x = x/step_x;
              if (x_down) o_x = x*step_x;
              i32 o_y = y/step_y;
              if (y_down) o_y = y*step_y;

              colour = src[o_y*source_w + o_x];

              dst[y*dest_w + x] = colour;
          }
      }

      if (blur_iter > 0) {
          ZPL_ASSERT_NOT_NULL(blur_mem);
          zpl_local_persist f64 filter[5][5] = {
              0, 0, 1, 0, 0,
              0, 1, 1, 1, 0,
              1, 1, 1, 1, 1,
              0, 1, 1, 1, 0,
              0, 0, 1, 0, 0,
          };

          f64 factor = 1.0 / 13.0;
          f64 bias = 0.0;

          zpl_memcopy(blur_mem, dest, dest_w*dest_h*4);

          zpli_rgb_filter(blur_mem, dest_w, dest_h,
                               cast(u32 *)dest,
                               &filter[0][0], 5, 5,
                               factor, bias);


      }
  }

  void zpli_rgb_filter(u32 *source, i32 source_w, i32 source_h,
                                       u32 *dest,
                                       f64 *filter, i32 filter_w, i32 filter_h,
                                       f64 factor, f64 bias) {

      zpli_rgb_colour_t *src = cast(zpli_rgb_colour_t *)(source);
      zpli_rgb_colour_t *dst = cast(zpli_rgb_colour_t *)(dest);

      for (i32 y = 0; y < source_h; ++y) {
          for(i32 x = 0; x < source_w; ++x) {
              i32 r = 0, g = 0, b = 0;

              for (i32 fy = 0; fy < filter_h; ++fy) {
                  for (i32 fx = 0; fx < filter_w; ++fx) {
                      i32 img_x = (x - filter_w / 2 + fx + source_w) % source_w;
                      i32 img_y = (y - filter_h / 2 + fy + source_h) % source_h;

                      r += src[img_y * source_w + img_x].r * filter[fy*filter_w + fx];
                      g += src[img_y * source_w + img_x].g * filter[fy*filter_w + fx];
                      b += src[img_y * source_w + img_x].b * filter[fy*filter_w + fx];
                  }
              }

              dst[y * source_w + x].r = zpl_min(zpl_max(cast(i32)(factor * r + bias), 0), 255);
              dst[y * source_w + x].g = zpl_min(zpl_max(cast(i32)(factor * g + bias), 0), 255);
              dst[y * source_w + x].b = zpl_min(zpl_max(cast(i32)(factor * b + bias), 0), 255);
          }
      }
  }

#endif

#if defined(__cplusplus)
}
#endif

#endif

#endif // ZPL_INCLUDE_ZPL_IMAGE_H
