/*

ZPL - Math module

License:
    This software is dual-licensed to the public domain and under the following
    license: you are granted a perpetual, irrevocable license to copy, modify,
    publish, and distribute this file as you see fit.

Warranty:
    No warranty is implied, use at your own risk.

Usage:
    #define ZPLM_IMPLEMENTATION exactly in ONE source file right BEFORE including the library, like:

    #define ZPLM_IMPLEMENTATION
    #include"zpl_math.h"

Credits:
    Vladislav Gritsenko (GitHub: inlife)
    Dominik Madarasz (GitHub: zaklaus)
    Ginger Bill (GitHub: gingerBill)

Version History:
    1.0.1 - Initial Version

*/

#ifndef ZPL_MATH_INCLUDE_ZPL_MATH_H
#define ZPL_MATH_INCLUDE_ZPL_MATH_H

#include <stddef.h>

#if !defined(ZPLM_NO_MATH_H)
#include <math.h>
#else
#include <intrin.h>
#endif

#ifndef ZPLM_DEF
#ifdef ZPLM_STATIC
#define ZPLM_DEF static
#else
#define ZPLM_DEF extern
#endif
#endif

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4201)
#endif

typedef union zplm_vec2_t {
    struct { float x, y; };
    float e[2];
} zplm_vec2_t;

typedef union zplm_vec3_t {
    struct { float x, y, z; };
    struct { float r, g, b; };

    zplm_vec2_t xy;
    float e[3];
} zplm_vec3_t;

typedef union zplm_vec4_t {
    struct { float x, y, z, w; };
    struct { float r, g, b, a; };
    struct { zplm_vec2_t xy, zw; };
    zplm_vec3_t xyz;
    zplm_vec3_t rzpl;
    float e[4];
} zplm_vec4_t;

typedef union zplm_mat2_t {
    struct { zplm_vec2_t x, y; };
    zplm_vec2_t col[2];
    float e[4];
} zplm_mat2_t;

typedef union zplm_mat3_t {
    struct { zplm_vec3_t x, y, z; };
    zplm_vec3_t col[3];
    float e[9];
} zplm_mat3_t;

typedef union zplm_mat4_t {
    struct { zplm_vec4_t x, y, z, w; };
    zplm_vec4_t col[4];
    float e[16];
} zplm_mat4_t;


typedef union zplm_quat_t {
    struct { float x, y, z, w; };
    zplm_vec4_t xyzw;
    zplm_vec3_t xyz;
    float e[4];
} zplm_quat_t;


#if defined(_MSC_VER)
#pragma warning(pop)
#endif

typedef float zplm_float2_t[2];
typedef float zplm_float3_t[3];
typedef float zplm_float4_t[4];


typedef struct zplm_rect2_t { zplm_vec2_t pos, dim; } zplm_rect2_t;
typedef struct zplm_rect3_t { zplm_vec3_t pos, dim; } zplm_rect3_t;

typedef struct zplm_aabb2_t { zplm_vec2_t centre, half_size; } zplm_aabb2_t;
typedef struct zplm_aabb3_t { zplm_vec3_t centre, half_size; } zplm_aabb3_t;

#if defined(_MSC_VER)
typedef unsigned __int32 zplm_u32;
typedef unsigned __int64 zplm_u64;
#else
#if defined(ZPLM_USE_STDINT)
#include <stdint.h>
typedef uint32_t zplm_u32;
typedef uint64_t zplm_u64;
#else
typedef unsigned int       zplm_u32;
typedef unsigned long long zplm_u64;
#endif
#endif

typedef short zplm_half_t;


#ifndef ZPLM_CONSTANTS
#define ZPLM_CONSTANTS
#define ZPLM_EPSILON      1.19209290e-7f
#define ZPLM_ZERO         0.0f
#define ZPLM_ONE          1.0f
#define ZPLM_TWO_THIRDS   0.666666666666666666666666666666666666667f

#define ZPLM_TAU          6.28318530717958647692528676655900576f
#define ZPLM_PI           3.14159265358979323846264338327950288f
#define ZPLM_ONE_OVER_TAU 0.636619772367581343075535053490057448f
#define ZPLM_ONE_OVER_PI  0.159154943091895335768883763372514362f

#define ZPLM_TAU_OVER_2   3.14159265358979323846264338327950288f
#define ZPLM_TAU_OVER_4   1.570796326794896619231321691639751442f
#define ZPLM_TAU_OVER_8   0.785398163397448309615660845819875721f

#define ZPLM_E            2.71828182845904523536f
#define ZPLM_SQRT_TWO     1.41421356237309504880168872420969808f
#define ZPLM_SQRT_THREE   1.73205080756887729352744634150587236f
#define ZPLM_SQRT_FIVE    2.23606797749978969640917366873127623f

#define ZPLM_LOG_TWO      0.693147180559945309417232121458176568f
#define ZPLM_LOG_TEN      2.30258509299404568401799145468436421f
#endif


#if defined(__cplusplus)
extern "C" {
#endif

#ifndef zplm_clamp
#define zplm_clamp(x, lower, upper) (zplm_min(zplm_max(x, (lower)), (upper)))
#endif
#ifndef zplm_clamp01
#define zplm_clamp01(x) zplm_clamp(x, 0, 1)
#endif

#ifndef zplm_square
#define zplm_square(x) ((x)*(x))
#endif

#ifndef zplm_cube
#define zplm_cube(x) ((x)*(x)*(x))
#endif

#ifndef zplm_abs
#define zplm_abs(x) ((x) > 0 ? (x) : -(x))
#endif

#ifndef zplm_sign
#define zplm_sign(x) ((x) >= 0 ? 1 : -1)
#endif


    ZPLM_DEF float zplm_to_radians(float degrees);
    ZPLM_DEF float zplm_to_degrees(float radians);

    /* NOTE: Because to interpolate angles */
    ZPLM_DEF float zplm_angle_diff(float radians_a, float radians_b);

#ifndef zplm_min
#define zplm_min(a, b) ((a) < (b) ? (a) : (b))
#endif
#ifndef zplm_max
#define zplm_max(a, b) ((a) > (b) ? (a) : (b))
#endif

#ifndef zplm_min3
#define zplm_min3(a, b, c) zplm_min(zplm_min(a, b), c)
#endif

#ifndef zplm_max3
#define zplm_max3(a, b, c) zplm_max(zplm_max(a, b), c)
#endif


    ZPLM_DEF float zplm_copy_sign  (float x, float y);
    ZPLM_DEF float zplm_remainder  (float x, float y);
    ZPLM_DEF float zplm_mod        (float x, float y);
    ZPLM_DEF float zplm_sqrt       (float a);
    ZPLM_DEF float zplm_rsqrt      (float a);
    ZPLM_DEF float zplm_quake_rsqrt(float a); /* NOTE: It's probably better to use 1.0f/zplm_sqrt(a)
* And for simd, there is usually isqrt functions too!
*/
    ZPLM_DEF float zplm_sin    (float radians);
    ZPLM_DEF float zplm_cos    (float radians);
    ZPLM_DEF float zplm_tan    (float radians);
    ZPLM_DEF float zplm_arcsin (float a);
    ZPLM_DEF float zplm_arccos (float a);
    ZPLM_DEF float zplm_arctan (float a);
    ZPLM_DEF float zplm_arctan2(float y, float x);

    ZPLM_DEF float zplm_exp      (float x);
    ZPLM_DEF float zplm_exp2     (float x);
    ZPLM_DEF float zplm_log      (float x);
    ZPLM_DEF float zplm_log2     (float x);
    ZPLM_DEF float zplm_fast_exp (float x);  /* NOTE: Only valid from -1 <= x <= +1 */
    ZPLM_DEF float zplm_fast_exp2(float x); /* NOTE: Only valid from -1 <= x <= +1 */
    ZPLM_DEF float zplm_pow      (float x, float y); /* x^y */

    ZPLM_DEF float zplm_round(float x);
    ZPLM_DEF float zplm_floor(float x);
    ZPLM_DEF float zplm_ceil (float x);

    ZPLM_DEF float  zplm_half_to_float(zplm_half_t value);
    ZPLM_DEF zplm_half_t zplm_float_to_half(float value);


    ZPLM_DEF zplm_vec2_t zplm_vec2_zero(void);
    ZPLM_DEF zplm_vec2_t zplm_vec2     (float x, float y);
    ZPLM_DEF zplm_vec2_t zplm_vec2v    (float x[2]);

    ZPLM_DEF zplm_vec3_t zplm_vec3_zero(void);
    ZPLM_DEF zplm_vec3_t zplm_vec3     (float x, float y, float z);
    ZPLM_DEF zplm_vec3_t zplm_vec3v    (float x[3]);

    ZPLM_DEF zplm_vec4_t zplm_vec4_zero(void);
    ZPLM_DEF zplm_vec4_t zplm_vec4     (float x, float y, float z, float w);
    ZPLM_DEF zplm_vec4_t zplm_vec4v    (float x[4]);


    ZPLM_DEF void zplm_vec2_add(zplm_vec2_t *d, zplm_vec2_t v0, zplm_vec2_t v1);
    ZPLM_DEF void zplm_vec2_sub(zplm_vec2_t *d, zplm_vec2_t v0, zplm_vec2_t v1);
    ZPLM_DEF void zplm_vec2_mul(zplm_vec2_t *d, zplm_vec2_t v,  float s);
    ZPLM_DEF void zplm_vec2_div(zplm_vec2_t *d, zplm_vec2_t v,  float s);

    ZPLM_DEF void zplm_vec3_add(zplm_vec3_t *d, zplm_vec3_t v0, zplm_vec3_t v1);
    ZPLM_DEF void zplm_vec3_sub(zplm_vec3_t *d, zplm_vec3_t v0, zplm_vec3_t v1);
    ZPLM_DEF void zplm_vec3_mul(zplm_vec3_t *d, zplm_vec3_t v,  float s);
    ZPLM_DEF void zplm_vec3_div(zplm_vec3_t *d, zplm_vec3_t v,  float s);

    ZPLM_DEF void zplm_vec4_add(zplm_vec4_t *d, zplm_vec4_t v0, zplm_vec4_t v1);
    ZPLM_DEF void zplm_vec4_sub(zplm_vec4_t *d, zplm_vec4_t v0, zplm_vec4_t v1);
    ZPLM_DEF void zplm_vec4_mul(zplm_vec4_t *d, zplm_vec4_t v,  float s);
    ZPLM_DEF void zplm_vec4_div(zplm_vec4_t *d, zplm_vec4_t v,  float s);

    ZPLM_DEF void zplm_vec2_addeq(zplm_vec2_t *d, zplm_vec2_t v);
    ZPLM_DEF void zplm_vec2_subeq(zplm_vec2_t *d, zplm_vec2_t v);
    ZPLM_DEF void zplm_vec2_muleq(zplm_vec2_t *d, float s);
    ZPLM_DEF void zplm_vec2_diveq(zplm_vec2_t *d, float s);

    ZPLM_DEF void zplm_vec3_addeq(zplm_vec3_t *d, zplm_vec3_t v);
    ZPLM_DEF void zplm_vec3_subeq(zplm_vec3_t *d, zplm_vec3_t v);
    ZPLM_DEF void zplm_vec3_muleq(zplm_vec3_t *d, float s);
    ZPLM_DEF void zplm_vec3_diveq(zplm_vec3_t *d, float s);

    ZPLM_DEF void zplm_vec4_addeq(zplm_vec4_t *d, zplm_vec4_t v);
    ZPLM_DEF void zplm_vec4_subeq(zplm_vec4_t *d, zplm_vec4_t v);
    ZPLM_DEF void zplm_vec4_muleq(zplm_vec4_t *d, float s);
    ZPLM_DEF void zplm_vec4_diveq(zplm_vec4_t *d, float s);

    ZPLM_DEF float zplm_vec2_dot(zplm_vec2_t v0, zplm_vec2_t v1);
    ZPLM_DEF float zplm_vec3_dot(zplm_vec3_t v0, zplm_vec3_t v1);
    ZPLM_DEF float zplm_vec4_dot(zplm_vec4_t v0, zplm_vec4_t v1);

    ZPLM_DEF void zplm_vec2_cross(float *d, zplm_vec2_t v0, zplm_vec2_t v1);
    ZPLM_DEF void zplm_vec3_cross(zplm_vec3_t *d, zplm_vec3_t v0, zplm_vec3_t v1);

    ZPLM_DEF float zplm_vec2_mag2(zplm_vec2_t v);
    ZPLM_DEF float zplm_vec3_mag2(zplm_vec3_t v);
    ZPLM_DEF float zplm_vec4_mag2(zplm_vec4_t v);

    ZPLM_DEF float zplm_vec2_mag(zplm_vec2_t v);
    ZPLM_DEF float zplm_vec3_mag(zplm_vec3_t v);
    ZPLM_DEF float zplm_vec4_mag(zplm_vec4_t v);

    ZPLM_DEF void zplm_vec2_norm(zplm_vec2_t *d, zplm_vec2_t v);
    ZPLM_DEF void zplm_vec3_norm(zplm_vec3_t *d, zplm_vec3_t v);
    ZPLM_DEF void zplm_vec4_norm(zplm_vec4_t *d, zplm_vec4_t v);

    ZPLM_DEF void zplm_vec2_norm0(zplm_vec2_t *d, zplm_vec2_t v);
    ZPLM_DEF void zplm_vec3_norm0(zplm_vec3_t *d, zplm_vec3_t v);
    ZPLM_DEF void zplm_vec4_norm0(zplm_vec4_t *d, zplm_vec4_t v);

    ZPLM_DEF void zplm_vec2_reflect(zplm_vec2_t *d, zplm_vec2_t i, zplm_vec2_t n);
    ZPLM_DEF void zplm_vec3_reflect(zplm_vec3_t *d, zplm_vec3_t i, zplm_vec3_t n);
    ZPLM_DEF void zplm_vec2_refract(zplm_vec2_t *d, zplm_vec2_t i, zplm_vec2_t n, float eta);
    ZPLM_DEF void zplm_vec3_refract(zplm_vec3_t *d, zplm_vec3_t i, zplm_vec3_t n, float eta);

    ZPLM_DEF float zplm_vec2_aspect_ratio(zplm_vec2_t v);


    ZPLM_DEF void zplm_mat2_identity   (zplm_mat2_t *m);
    ZPLM_DEF void zplm_float22_identity(float m[2][2]);

    ZPLM_DEF void  zplm_mat2_transpose  (zplm_mat2_t *m);
    ZPLM_DEF void  zplm_mat2_mul        (zplm_mat2_t *out, zplm_mat2_t *m1, zplm_mat2_t *m2);
    ZPLM_DEF void  zplm_mat2_mul_vec2   (zplm_vec2_t *out, zplm_mat2_t *m, zplm_vec2_t in);
    ZPLM_DEF void  zplm_mat2_inverse    (zplm_mat2_t *out, zplm_mat2_t *in);
    ZPLM_DEF float zplm_mat2_determinate(zplm_mat2_t *m);

    ZPLM_DEF zplm_mat2_t *zplm_mat2_v(zplm_vec2_t m[2]);
    ZPLM_DEF zplm_mat2_t *zplm_mat2_f(float m[2][2]);
    ZPLM_DEF zplm_float2_t *zplm_float22_m(zplm_mat2_t *m);
    ZPLM_DEF zplm_float2_t *zplm_float22_v(zplm_vec2_t m[2]);
    ZPLM_DEF zplm_float2_t *zplm_float22_4(float m[4]);

    ZPLM_DEF void zplm_float22_transpose(float (*vec)[2]);
    ZPLM_DEF void zplm_float22_mul      (float (*out)[2], float (*mat1)[2], float (*mat2)[2]);
    ZPLM_DEF void zplm_float22_mul_vec2 (zplm_vec2_t *out, float m[2][2], zplm_vec2_t in);


    ZPLM_DEF void zplm_mat3_identity   (zplm_mat3_t *m);
    ZPLM_DEF void zplm_float33_identity(float m[3][3]);

    ZPLM_DEF void  zplm_mat3_transpose  (zplm_mat3_t *m);
    ZPLM_DEF void  zplm_mat3_mul        (zplm_mat3_t *out, zplm_mat3_t *m1, zplm_mat3_t *m2);
    ZPLM_DEF void  zplm_mat3_mul_vec3   (zplm_vec3_t *out, zplm_mat3_t *m, zplm_vec3_t in);
    ZPLM_DEF void  zplm_mat3_inverse    (zplm_mat3_t *out, zplm_mat3_t *in);
    ZPLM_DEF float zplm_mat3_determinate(zplm_mat3_t *m);


    ZPLM_DEF zplm_mat3_t *zplm_mat3_v(zplm_vec3_t m[3]);
    ZPLM_DEF zplm_mat3_t *zplm_mat3_f(float m[3][3]);

    ZPLM_DEF zplm_float3_t *zplm_float33_m(zplm_mat3_t *m);
    ZPLM_DEF zplm_float3_t *zplm_float33_v(zplm_vec3_t m[3]);
    ZPLM_DEF zplm_float3_t *zplm_float33_9(float m[9]);

    ZPLM_DEF void zplm_float33_transpose(float (*vec)[3]);
    ZPLM_DEF void zplm_float33_mul      (float (*out)[3], float (*mat1)[3], float (*mat2)[3]);
    ZPLM_DEF void zplm_float33_mul_vec3 (zplm_vec3_t *out, float m[3][3], zplm_vec3_t in);

    ZPLM_DEF void zplm_mat4_identity   (zplm_mat4_t *m);
    ZPLM_DEF void zplm_float44_identity(float m[4][4]);

    ZPLM_DEF void  zplm_mat4_transpose  (zplm_mat4_t *m);
    ZPLM_DEF void  zplm_mat4_mul        (zplm_mat4_t *out, zplm_mat4_t *m1, zplm_mat4_t *m2);
    ZPLM_DEF void  zplm_mat4_mul_vec4   (zplm_vec4_t *out, zplm_mat4_t *m, zplm_vec4_t in);
    ZPLM_DEF void  zplm_mat4_inverse    (zplm_mat4_t *out, zplm_mat4_t *in);

    ZPLM_DEF zplm_mat4_t *zplm_mat4_v(zplm_vec4_t m[4]);
    ZPLM_DEF zplm_mat4_t *zplm_mat4_f(float m[4][4]);

    ZPLM_DEF zplm_float4_t *zplm_float44_m (zplm_mat4_t *m);
    ZPLM_DEF zplm_float4_t *zplm_float44_v (zplm_vec4_t m[4]);
    ZPLM_DEF zplm_float4_t *zplm_float44_16(float m[16]);

    ZPLM_DEF void zplm_float44_transpose(float (*vec)[4]);
    ZPLM_DEF void zplm_float44_mul      (float (*out)[4], float (*mat1)[4], float (*mat2)[4]);
    ZPLM_DEF void zplm_float44_mul_vec4 (zplm_vec4_t *out, float m[4][4], zplm_vec4_t in);


    ZPLM_DEF void zplm_mat4_translate           (zplm_mat4_t *out, zplm_vec3_t v);
    ZPLM_DEF void zplm_mat4_rotate              (zplm_mat4_t *out, zplm_vec3_t v, float angle_radians);
    ZPLM_DEF void zplm_mat4_scale               (zplm_mat4_t *out, zplm_vec3_t v);
    ZPLM_DEF void zplm_mat4_scalef              (zplm_mat4_t *out, float s);
    ZPLM_DEF void zplm_mat4_ortho2d             (zplm_mat4_t *out, float left, float right, float bottom, float top);
    ZPLM_DEF void zplm_mat4_ortho3d             (zplm_mat4_t *out, float left, float right, float bottom, float top, float z_near, float z_far);
    ZPLM_DEF void zplm_mat4_perspective         (zplm_mat4_t *out, float fovy, float aspect, float z_near, float z_far);
    ZPLM_DEF void zplm_mat4_infinite_perspective(zplm_mat4_t *out, float fovy, float aspect, float z_near);

    ZPLM_DEF void zplm_mat4_look_at(zplm_mat4_t *out, zplm_vec3_t eye, zplm_vec3_t centre, zplm_vec3_t up);



    ZPLM_DEF zplm_quat_t zplm_quat             (float x, float y, float z, float w);
    ZPLM_DEF zplm_quat_t zplm_quatv            (float e[4]);
    ZPLM_DEF zplm_quat_t zplm_quat_axis_angle  (zplm_vec3_t axis, float angle_radians);
    ZPLM_DEF zplm_quat_t zplm_quat_euler_angles(float pitch, float yaw, float roll);
    ZPLM_DEF zplm_quat_t zplm_quat_identity    (void);

    ZPLM_DEF void zplm_quat_add(zplm_quat_t *d, zplm_quat_t q0, zplm_quat_t q1);
    ZPLM_DEF void zplm_quat_sub(zplm_quat_t *d, zplm_quat_t q0, zplm_quat_t q1);
    ZPLM_DEF void zplm_quat_mul(zplm_quat_t *d, zplm_quat_t q0, zplm_quat_t q1);
    ZPLM_DEF void zplm_quat_div(zplm_quat_t *d, zplm_quat_t q0, zplm_quat_t q1);

    ZPLM_DEF void zplm_quat_mulf(zplm_quat_t *d, zplm_quat_t q, float s);
    ZPLM_DEF void zplm_quat_divf(zplm_quat_t *d, zplm_quat_t q, float s);


    ZPLM_DEF void zplm_quat_addeq(zplm_quat_t *d, zplm_quat_t q);
    ZPLM_DEF void zplm_quat_subeq(zplm_quat_t *d, zplm_quat_t q);
    ZPLM_DEF void zplm_quat_muleq(zplm_quat_t *d, zplm_quat_t q);
    ZPLM_DEF void zplm_quat_diveq(zplm_quat_t *d, zplm_quat_t q);


    ZPLM_DEF void zplm_quat_muleqf(zplm_quat_t *d, float s);
    ZPLM_DEF void zplm_quat_diveqf(zplm_quat_t *d, float s);




    ZPLM_DEF float zplm_quat_dot(zplm_quat_t q0, zplm_quat_t q1);
    ZPLM_DEF float zplm_quat_mag(zplm_quat_t q);

    ZPLM_DEF void zplm_quat_norm   (zplm_quat_t *d, zplm_quat_t q);
    ZPLM_DEF void zplm_quat_conj   (zplm_quat_t *d, zplm_quat_t q);
    ZPLM_DEF void zplm_quat_inverse(zplm_quat_t *d, zplm_quat_t q);

    ZPLM_DEF void  zplm_quat_axis (zplm_vec3_t *axis, zplm_quat_t q);
    ZPLM_DEF float zplm_quat_angle(zplm_quat_t q);

    ZPLM_DEF float zplm_quat_pitch(zplm_quat_t q);
    ZPLM_DEF float zplm_quat_yaw  (zplm_quat_t q);
    ZPLM_DEF float zplm_quat_roll (zplm_quat_t q);

    /* NOTE: Rotate v by q */
    ZPLM_DEF void zplm_quat_rotate_vec3(zplm_vec3_t *d, zplm_quat_t q, zplm_vec3_t v);
    ZPLM_DEF void zplm_mat4_from_quat  (zplm_mat4_t *out, zplm_quat_t q);
    ZPLM_DEF void zplm_quat_from_mat4  (zplm_quat_t *out, zplm_mat4_t *m);



    /* Interpolations */
    ZPLM_DEF float zplm_lerp         (float a, float b, float t);
    ZPLM_DEF float zplm_unlerp       (float t, float a, float b);
    ZPLM_DEF float zplm_smooth_step  (float a, float b, float t);
    ZPLM_DEF float zplm_smoother_step(float a, float b, float t);

    ZPLM_DEF void zplm_vec2_lerp(zplm_vec2_t *d, zplm_vec2_t a, zplm_vec2_t b, float t);
    ZPLM_DEF void zplm_vec3_lerp(zplm_vec3_t *d, zplm_vec3_t a, zplm_vec3_t b, float t);
    ZPLM_DEF void zplm_vec4_lerp(zplm_vec4_t *d, zplm_vec4_t a, zplm_vec4_t b, float t);

    ZPLM_DEF void zplm_quat_lerp (zplm_quat_t *d, zplm_quat_t a, zplm_quat_t b, float t);
    ZPLM_DEF void zplm_quat_nlerp(zplm_quat_t *d, zplm_quat_t a, zplm_quat_t b, float t);
    ZPLM_DEF void zplm_quat_slerp(zplm_quat_t *d, zplm_quat_t a, zplm_quat_t b, float t);
    ZPLM_DEF void zplm_quat_nquad(zplm_quat_t *d, zplm_quat_t p, zplm_quat_t a, zplm_quat_t b, zplm_quat_t q, float t);
    ZPLM_DEF void zplm_quat_squad(zplm_quat_t *d, zplm_quat_t p, zplm_quat_t a, zplm_quat_t b, zplm_quat_t q, float t);
    ZPLM_DEF void zplm_quat_slerp_approx(zplm_quat_t *d, zplm_quat_t a, zplm_quat_t b, float t);
    ZPLM_DEF void zplm_quat_squad_approx(zplm_quat_t *d, zplm_quat_t p, zplm_quat_t a, zplm_quat_t b, zplm_quat_t q, float t);


    /* Rects */
    ZPLM_DEF zplm_rect2_t zplm_rect2(zplm_vec2_t pos, zplm_vec2_t dim);
    ZPLM_DEF zplm_rect3_t zplm_rect3(zplm_vec3_t pos, zplm_vec3_t dim);

    ZPLM_DEF int zplm_rect2_contains           (zplm_rect2_t a, float x, float y);
    ZPLM_DEF int zplm_rect2_contains_vec2      (zplm_rect2_t a, zplm_vec2_t p);
    ZPLM_DEF int zplm_rect2_intersects         (zplm_rect2_t a, zplm_rect2_t b);
    ZPLM_DEF int zplm_rect2_intersection_result(zplm_rect2_t a, zplm_rect2_t b, zplm_rect2_t *intersection);


#ifndef	ZPLM_MURMUR64_DEFAULT_SEED
#define ZPLM_MURMUR64_DEFAULT_SEED 0x9747b28c
#endif
    /* Hashing */
    ZPLM_DEF zplm_u64 zplm_hash_murmur64(void const *key, size_t num_bytes, zplm_u64 seed);

    /* Random */
    /* TODO: Use a generator for the random numbers */
    ZPLM_DEF float zplm_random_range_float(float min_inc, float max_inc);
    ZPLM_DEF int   zplm_random_range_int  (int min_inc, int max_inc);
    ZPLM_DEF float zplm_random01          (void);



#if defined(__cplusplus)
}
#endif

#if defined(__cplusplus)

/* TODO: How should I apply ZPLM_DEF to these operator overloads? */

inline bool operator==(zplm_vec2_t a, zplm_vec2_t b) { return (a.x == b.x) && (a.y == b.y); }
inline bool operator!=(zplm_vec2_t a, zplm_vec2_t b) { return !operator==(a, b); }

inline zplm_vec2_t operator+(zplm_vec2_t a) { return a; }
inline zplm_vec2_t operator-(zplm_vec2_t a) { zplm_vec2_t r = {-a.x, -a.y}; return r; }

inline zplm_vec2_t operator+(zplm_vec2_t a, zplm_vec2_t b) { zplm_vec2_t r; zplm_vec2_add(&r, a, b); return r; }
inline zplm_vec2_t operator-(zplm_vec2_t a, zplm_vec2_t b) { zplm_vec2_t r; zplm_vec2_sub(&r, a, b); return r; }

inline zplm_vec2_t operator*(zplm_vec2_t a, float scalar) { zplm_vec2_t r; zplm_vec2_mul(&r, a, scalar); return r; }
inline zplm_vec2_t operator*(float scalar, zplm_vec2_t a) { return operator*(a, scalar); }

inline zplm_vec2_t operator/(zplm_vec2_t a, float scalar) { return operator*(a, 1.0f/scalar); }

/* Hadamard Product */
inline zplm_vec2_t operator*(zplm_vec2_t a, zplm_vec2_t b) { zplm_vec2_t r = {a.x*b.x, a.y*b.y}; return r; }
inline zplm_vec2_t operator/(zplm_vec2_t a, zplm_vec2_t b) { zplm_vec2_t r = {a.x/b.x, a.y/b.y}; return r; }

inline zplm_vec2_t &operator+=(zplm_vec2_t &a, zplm_vec2_t b)       { return (a = a + b); }
inline zplm_vec2_t &operator-=(zplm_vec2_t &a, zplm_vec2_t b)       { return (a = a - b); }
inline zplm_vec2_t &operator*=(zplm_vec2_t &a, float scalar) { return (a = a * scalar); }
inline zplm_vec2_t &operator/=(zplm_vec2_t &a, float scalar) { return (a = a / scalar); }


inline bool operator==(zplm_vec3_t a, zplm_vec3_t b) { return (a.x == b.x) && (a.y == b.y) && (a.z == b.z); }
inline bool operator!=(zplm_vec3_t a, zplm_vec3_t b) { return !operator==(a, b); }

inline zplm_vec3_t operator+(zplm_vec3_t a) { return a; }
inline zplm_vec3_t operator-(zplm_vec3_t a) { zplm_vec3_t r = {-a.x, -a.y, -a.z}; return r; }

inline zplm_vec3_t operator+(zplm_vec3_t a, zplm_vec3_t b) { zplm_vec3_t r; zplm_vec3_add(&r, a, b); return r; }
inline zplm_vec3_t operator-(zplm_vec3_t a, zplm_vec3_t b) { zplm_vec3_t r; zplm_vec3_sub(&r, a, b); return r; }

inline zplm_vec3_t operator*(zplm_vec3_t a, float scalar) { zplm_vec3_t r; zplm_vec3_mul(&r, a, scalar); return r; }
inline zplm_vec3_t operator*(float scalar, zplm_vec3_t a) { return operator*(a, scalar); }

inline zplm_vec3_t operator/(zplm_vec3_t a, float scalar) { return operator*(a, 1.0f/scalar); }

/* Hadamard Product */
inline zplm_vec3_t operator*(zplm_vec3_t a, zplm_vec3_t b) { zplm_vec3_t r = {a.x*b.x, a.y*b.y, a.z*b.z}; return r; }
inline zplm_vec3_t operator/(zplm_vec3_t a, zplm_vec3_t b) { zplm_vec3_t r = {a.x/b.x, a.y/b.y, a.z/b.z}; return r; }

inline zplm_vec3_t &operator+=(zplm_vec3_t &a, zplm_vec3_t b)       { return (a = a + b); }
inline zplm_vec3_t &operator-=(zplm_vec3_t &a, zplm_vec3_t b)       { return (a = a - b); }
inline zplm_vec3_t &operator*=(zplm_vec3_t &a, float scalar) { return (a = a * scalar); }
inline zplm_vec3_t &operator/=(zplm_vec3_t &a, float scalar) { return (a = a / scalar); }


inline bool operator==(zplm_vec4_t a, zplm_vec4_t b) { return (a.x == b.x) && (a.y == b.y) && (a.z == b.z) && (a.w == b.w); }
inline bool operator!=(zplm_vec4_t a, zplm_vec4_t b) { return !operator==(a, b); }

inline zplm_vec4_t operator+(zplm_vec4_t a) { return a; }
inline zplm_vec4_t operator-(zplm_vec4_t a) { zplm_vec4_t r = {-a.x, -a.y, -a.z, -a.w}; return r; }

inline zplm_vec4_t operator+(zplm_vec4_t a, zplm_vec4_t b) { zplm_vec4_t r; zplm_vec4_add(&r, a, b); return r; }
inline zplm_vec4_t operator-(zplm_vec4_t a, zplm_vec4_t b) { zplm_vec4_t r; zplm_vec4_sub(&r, a, b); return r; }

inline zplm_vec4_t operator*(zplm_vec4_t a, float scalar) { zplm_vec4_t r; zplm_vec4_mul(&r, a, scalar); return r; }
inline zplm_vec4_t operator*(float scalar, zplm_vec4_t a) { return operator*(a, scalar); }

inline zplm_vec4_t operator/(zplm_vec4_t a, float scalar) { return operator*(a, 1.0f/scalar); }

/* Hadamard Product */
inline zplm_vec4_t operator*(zplm_vec4_t a, zplm_vec4_t b) { zplm_vec4_t r = {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w}; return r; }
inline zplm_vec4_t operator/(zplm_vec4_t a, zplm_vec4_t b) { zplm_vec4_t r = {a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w}; return r; }

inline zplm_vec4_t &operator+=(zplm_vec4_t &a, zplm_vec4_t b)       { return (a = a + b); }
inline zplm_vec4_t &operator-=(zplm_vec4_t &a, zplm_vec4_t b)       { return (a = a - b); }
inline zplm_vec4_t &operator*=(zplm_vec4_t &a, float scalar) { return (a = a * scalar); }
inline zplm_vec4_t &operator/=(zplm_vec4_t &a, float scalar) { return (a = a / scalar); }


inline zplm_mat2_t operator+(zplm_mat2_t const &a, zplm_mat2_t const &b) {
    int i, j;
    zplm_mat2_t r = {0};
    for (j = 0; j < 2; j++) {
        for (i = 0; i < 2; i++)
            r.e[2*j+i] = a.e[2*j+i] + b.e[2*j+i];
    }
    return r;
}

inline zplm_mat2_t operator-(zplm_mat2_t const &a, zplm_mat2_t const &b) {
    int i, j;
    zplm_mat2_t r = {0};
    for (j = 0; j < 2; j++) {
        for (i = 0; i < 2; i++)
            r.e[2*j+i] = a.e[2*j+i] - b.e[2*j+i];
    }
    return r;
}

inline zplm_mat2_t operator*(zplm_mat2_t const &a, zplm_mat2_t const &b) { zplm_mat2_t r; zplm_mat2_mul(&r, (zplm_mat2_t *)&a, (zplm_mat2_t *)&b); return r; }
inline zplm_vec2_t operator*(zplm_mat2_t const &a, zplm_vec2_t v) { zplm_vec2_t r; zplm_mat2_mul_vec2(&r, (zplm_mat2_t *)&a, v); return r; }
inline zplm_mat2_t operator*(zplm_mat2_t const &a, float scalar) {
    zplm_mat2_t r = {0};
    int i;
    for (i = 0; i < 2*2; i++) r.e[i] = a.e[i] * scalar;
    return r;
}
inline zplm_mat2_t operator*(float scalar, zplm_mat2_t const &a) { return operator*(a, scalar); }
inline zplm_mat2_t operator/(zplm_mat2_t const &a, float scalar) { return operator*(a, 1.0f/scalar); }

inline zplm_mat2_t& operator+=(zplm_mat2_t& a, zplm_mat2_t const &b) { return (a = a + b); }
inline zplm_mat2_t& operator-=(zplm_mat2_t& a, zplm_mat2_t const &b) { return (a = a - b); }
inline zplm_mat2_t& operator*=(zplm_mat2_t& a, zplm_mat2_t const &b) { return (a = a * b); }



inline zplm_mat3_t operator+(zplm_mat3_t const &a, zplm_mat3_t const &b) {
    int i, j;
    zplm_mat3_t r = {0};
    for (j = 0; j < 3; j++) {
        for (i = 0; i < 3; i++)
            r.e[3*j+i] = a.e[3*j+i] + b.e[3*j+i];
    }
    return r;
}

inline zplm_mat3_t operator-(zplm_mat3_t const &a, zplm_mat3_t const &b) {
    int i, j;
    zplm_mat3_t r = {0};
    for (j = 0; j < 3; j++) {
        for (i = 0; i < 3; i++)
            r.e[3*j+i] = a.e[3*j+i] - b.e[3*j+i];
    }
    return r;
}

inline zplm_mat3_t operator*(zplm_mat3_t const &a, zplm_mat3_t const &b) { zplm_mat3_t r; zplm_mat3_mul(&r, (zplm_mat3_t *)&a, (zplm_mat3_t *)&b); return r; }
inline zplm_vec3_t operator*(zplm_mat3_t const &a, zplm_vec3_t v) { zplm_vec3_t r; zplm_mat3_mul_vec3(&r, (zplm_mat3_t *)&a, v); return r; } inline zplm_mat3_t operator*(zplm_mat3_t const &a, float scalar) {
    zplm_mat3_t r = {0};
    int i;
    for (i = 0; i < 3*3; i++) r.e[i] = a.e[i] * scalar;
    return r;
}
inline zplm_mat3_t operator*(float scalar, zplm_mat3_t const &a) { return operator*(a, scalar); }
inline zplm_mat3_t operator/(zplm_mat3_t const &a, float scalar) { return operator*(a, 1.0f/scalar); }

inline zplm_mat3_t& operator+=(zplm_mat3_t& a, zplm_mat3_t const &b) { return (a = a + b); }
inline zplm_mat3_t& operator-=(zplm_mat3_t& a, zplm_mat3_t const &b) { return (a = a - b); }
inline zplm_mat3_t& operator*=(zplm_mat3_t& a, zplm_mat3_t const &b) { return (a = a * b); }



inline zplm_mat4_t operator+(zplm_mat4_t const &a, zplm_mat4_t const &b) {
    int i, j;
    zplm_mat4_t r = {0};
    for (j = 0; j < 4; j++) {
        for (i = 0; i < 4; i++)
            r.e[4*j+i] = a.e[4*j+i] + b.e[4*j+i];
    }
    return r;
}

inline zplm_mat4_t operator-(zplm_mat4_t const &a, zplm_mat4_t const &b) {
    int i, j;
    zplm_mat4_t r = {0};
    for (j = 0; j < 4; j++) {
        for (i = 0; i < 4; i++)
            r.e[4*j+i] = a.e[4*j+i] - b.e[4*j+i];
    }
    return r;
}

inline zplm_mat4_t operator*(zplm_mat4_t const &a, zplm_mat4_t const &b) { zplm_mat4_t r; zplm_mat4_mul(&r, (zplm_mat4_t *)&a, (zplm_mat4_t *)&b); return r; }
inline zplm_vec4_t operator*(zplm_mat4_t const &a, zplm_vec4_t v) { zplm_vec4_t r; zplm_mat4_mul_vec4(&r, (zplm_mat4_t *)&a, v); return r; }
inline zplm_mat4_t operator*(zplm_mat4_t const &a, float scalar) {
    zplm_mat4_t r = {0};
    int i;
    for (i = 0; i < 4*4; i++) r.e[i] = a.e[i] * scalar;
    return r;
}
inline zplm_mat4_t operator*(float scalar, zplm_mat4_t const &a) { return operator*(a, scalar); }
inline zplm_mat4_t operator/(zplm_mat4_t const &a, float scalar) { return operator*(a, 1.0f/scalar); }

inline zplm_mat4_t& operator+=(zplm_mat4_t &a, zplm_mat4_t const &b) { return (a = a + b); }
inline zplm_mat4_t& operator-=(zplm_mat4_t &a, zplm_mat4_t const &b) { return (a = a - b); }
inline zplm_mat4_t& operator*=(zplm_mat4_t &a, zplm_mat4_t const &b) { return (a = a * b); }



inline bool operator==(zplm_quat_t a, zplm_quat_t b) { return a.xyzw == b.xyzw; }
inline bool operator!=(zplm_quat_t a, zplm_quat_t b) { return !operator==(a, b); }

inline zplm_quat_t operator+(zplm_quat_t q) { return q; }
inline zplm_quat_t operator-(zplm_quat_t q) { return zplm_quat(-q.x, -q.y, -q.z, -q.w); }

inline zplm_quat_t operator+(zplm_quat_t a, zplm_quat_t b) { zplm_quat_t r; zplm_quat_add(&r, a, b); return r; }
inline zplm_quat_t operator-(zplm_quat_t a, zplm_quat_t b) { zplm_quat_t r; zplm_quat_sub(&r, a, b); return r; }

inline zplm_quat_t operator*(zplm_quat_t a, zplm_quat_t b)  { zplm_quat_t r; zplm_quat_mul(&r, a, b); return r; }
inline zplm_quat_t operator*(zplm_quat_t q, float s) { zplm_quat_t r; zplm_quat_mulf(&r, q, s); return r; }
inline zplm_quat_t operator*(float s, zplm_quat_t q) { return operator*(q, s); }
inline zplm_quat_t operator/(zplm_quat_t q, float s) { zplm_quat_t r; zplm_quat_divf(&r, q, s); return r; }

inline zplm_quat_t &operator+=(zplm_quat_t &a, zplm_quat_t b) { zplm_quat_addeq(&a, b); return a; }
inline zplm_quat_t &operator-=(zplm_quat_t &a, zplm_quat_t b) { zplm_quat_subeq(&a, b); return a; }
inline zplm_quat_t &operator*=(zplm_quat_t &a, zplm_quat_t b) { zplm_quat_muleq(&a, b); return a; }
inline zplm_quat_t &operator/=(zplm_quat_t &a, zplm_quat_t b) { zplm_quat_diveq(&a, b); return a; }

inline zplm_quat_t &operator*=(zplm_quat_t &a, float b) { zplm_quat_muleqf(&a, b); return a; }
inline zplm_quat_t &operator/=(zplm_quat_t &a, float b) { zplm_quat_diveqf(&a, b); return a; }

/* Rotate v by a */
inline zplm_vec3_t operator*(zplm_quat_t q, zplm_vec3_t v) { zplm_vec3_t r; zplm_quat_rotate_vec3(&r, q, v); return r; }

#endif




#endif /* ZPLM_INCLUDE_ZPLM_H */

/****************************************************************
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 * Implementation
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 ****************************************************************/

#if defined(ZPLM_IMPLEMENTATION) && !defined(ZPLM_IMPLEMENTATION_DONE)
#define ZPLM_IMPLEMENTATION_DONE

#if (defined(__GCC__) || defined(__GNUC__)) && !defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"
#pragma GCC diagnostic ignored "-Wmissing-braces"
#elif __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wattributes"
#pragma clang diagnostic ignored "-Wmissing-braces"
#endif


/* NOTE: To remove the need for memcpy */
static void zplm__memcpy_4byte(void *dest, void const *src, size_t size) {
    size_t i;
    unsigned int *d, *s;
    d = (unsigned int *)dest;
    s = (unsigned int *)src;
    for (i = 0; i < size/4; i++) {
        *d++ = *s++;
    }
}


float zplm_to_radians(float degrees) { return degrees * ZPLM_TAU / 360.0f; }
float zplm_to_degrees(float radians) { return radians * 360.0f / ZPLM_TAU; }

float zplm_angle_diff(float radians_a, float radians_b) {
    float delta = zplm_mod(radians_b-radians_a, ZPLM_TAU);
    delta = zplm_mod(delta + 1.5f*ZPLM_TAU, ZPLM_TAU);
    delta -= 0.5f*ZPLM_TAU;
    return delta;
}

float zplm_copy_sign(float x, float y) {
    int ix, iy;
    ix = *(int *)&x;
    iy = *(int *)&y;

    ix &= 0x7fffffff;
    ix |= iy & 0x80000000;
    return *(float *)&ix;
}

float zplm_remainder(float x, float y) {
    return x - (zplm_round(x/y)*y);
}

float zplm_mod(float x, float y) {
    float result;
    y = zplm_abs(y);
    result = zplm_remainder(zplm_abs(x), y);
    if (zplm_sign(result)) result += y;
    return zplm_copy_sign(result, x);
}


float zplm_quake_rsqrt(float a) {
    union {
        int i;
        float f;
    } t;
    float x2;
    float const three_halfs = 1.5f;

    x2 = a * 0.5f;
    t.f = a;
    t.i = 0x5f375a86 - (t.i >> 1);                /* What the fuck? */
    t.f = t.f * (three_halfs - (x2 * t.f * t.f)); /* 1st iteration */
    t.f = t.f * (three_halfs - (x2 * t.f * t.f)); /* 2nd iteration, this can be removed */

    return t.f;
}


#if defined(ZPLM_NO_MATH_H)
#if defined(_MSC_VER)

float zplm_rsqrt(float a) { return _mm_cvtss_f32(_mm_rsqrt_ss(_mm_set_ss(a))); }
float zplm_sqrt(float a)  { return _mm_cvtss_f32(_mm_sqrt_ss(_mm_set_ss(a))); };
float
zplm_sin(float a)
{
    static float const a0 = +1.91059300966915117e-31f;
    static float const a1 = +1.00086760103908896f;
    static float const a2 = -1.21276126894734565e-2f;
    static float const a3 = -1.38078780785773762e-1f;
    static float const a4 = -2.67353392911981221e-2f;
    static float const a5 = +2.08026600266304389e-2f;
    static float const a6 = -3.03996055049204407e-3f;
    static float const a7 = +1.38235642404333740e-4f;
    return a0 + a*(a1 + a*(a2 + a*(a3 + a*(a4 + a*(a5 + a*(a6 + a*a7))))));
}
float
zplm_cos(float a)
{
    static float const a0 = +1.00238601909309722f;
    static float const a1 = -3.81919947353040024e-2f;
    static float const a2 = -3.94382342128062756e-1f;
    static float const a3 = -1.18134036025221444e-1f;
    static float const a4 = +1.07123798512170878e-1f;
    static float const a5 = -1.86637164165180873e-2f;
    static float const a6 = +9.90140908664079833e-4f;
    static float const a7 = -5.23022132118824778e-14f;
    return a0 + a*(a1 + a*(a2 + a*(a3 + a*(a4 + a*(a5 + a*(a6 + a*a7))))));
}

float
zplm_tan(float radians)
{
    float rr = radians*radians;
    float a = 9.5168091e-03f;
    a *= rr;
    a += 2.900525e-03f;
    a *= rr;
    a += 2.45650893e-02f;
    a *= rr;
    a += 5.33740603e-02f;
    a *= rr;
    a += 1.333923995e-01f;
    a *= rr;
    a += 3.333314036e-01f;
    a *= rr;
    a += 1.0f;
    a *= radians;
    return a;
}

float zplm_arcsin(float a) { return zplm_arctan2(a, zplm_sqrt((1.0f + a) * (1.0f - a))); }
float zplm_arccos(float a) { return zplm_arctan2(zplm_sqrt((1.0f + a) * (1.0 - a)), a); }

float
zplm_arctan(float a)
{
    float u  = a*a;
    float u2 = u*u;
    float u3 = u2*u;
    float u4 = u3*u;
    float f  = 1.0f+0.33288950512027f*u-0.08467922817644f*u2+0.03252232640125f*u3-0.00749305860992f*u4;
    return a/f;
}

float
zplm_arctan2(float y, float x)
{
    if (zplm_abs(x) > zplm_abs(y)) {
        float a = zplm_arctan(y/x);
        if (x > 0.0f)
            return a;
        else
            return y > 0.0f ? a+ZPLM_TAU_OVER_2:a-ZPLM_TAU_OVER_2;
    } else {
        float a = zplm_arctan(x/y);
        if (x > 0.0f)
            return y > 0.0f ? ZPLM_TAU_OVER_4-a:-ZPLM_TAU_OVER_4-a;
        else
            return y > 0.0f ? ZPLM_TAU_OVER_4+a:-ZPLM_TAU_OVER_4+a;
    }
}

float
zplm_exp(float a)
{
    union { float f; int i; } u, v;
    u.i = (int)(6051102 * a + 1056478197);
    v.i = (int)(1056478197 - 6051102 * a);
    return u.f / v.f;
}

float
zplm_log(float a)
{
    union { float f; int i; } u = {a};
    return (u.i - 1064866805) * 8.262958405176314e-8f; /* 1 / 12102203.0; */
}

float
zplm_pow(float a, float b)
{
    int flipped = 0, e;
    float f, r = 1.0f;
    if (b < 0) {
        flipped = 1;
        b = -b;
    }

    e = (int)b;
    f = zplm_exp(b - e);

    while (e) {
        if (e & 1) r *= a;
        a *= a;
        e >>= 1;
    }

    r *= f;
    return flipped ? 1.0f/r : r;
}

#else

float zplm_rsqrt(float a)            { return 1.0f/__builtin_sqrt(a); }
float zplm_sqrt(float a)             { return __builtin_sqrt(a); }
float zplm_sin(float radians)        { return __builtin_sinf(radians); }
float zplm_cos(float radians)        { return __builtin_cosf(radians); }
float zplm_tan(float radians)        { return __builtin_tanf(radians); }
float zplm_arcsin(float a)           { return __builtin_asinf(a); }
float zplm_arccos(float a)           { return __builtin_acosf(a); }
float zplm_arctan(float a)           { return __builtin_atanf(a); }
float zplm_arctan2(float y, float x) { return __builtin_atan2f(y, x); }


float zplm_exp(float x)  { return __builtin_expf(x); }
float zplm_log(float x)  { return __builtin_logf(x); }

// TODO: Should this be zplm_exp(y * zplm_log(x)) ???
float zplm_pow(float x, float y) { return __builtin_powf(x, y); }

#endif

#else
float zplm_rsqrt(float a)            { return 1.0f/sqrtf(a); }
float zplm_sqrt(float a)             { return sqrtf(a); };
float zplm_sin(float radians)        { return sinf(radians); };
float zplm_cos(float radians)        { return cosf(radians); };
float zplm_tan(float radians)        { return tanf(radians); };
float zplm_arcsin(float a)           { return asinf(a); };
float zplm_arccos(float a)           { return acosf(a); };
float zplm_arctan(float a)           { return atanf(a); };
float zplm_arctan2(float y, float x) { return atan2f(y, x); };

float zplm_exp(float x)          { return expf(x); }
float zplm_log(float x)          { return logf(x); }
float zplm_pow(float x, float y) { return powf(x, y); }
#endif

float zplm_exp2(float x) { return zplm_exp(ZPLM_LOG_TWO * x); }
float zplm_log2(float x) { return zplm_log(x) / ZPLM_LOG_TWO; }


float zplm_fast_exp(float x) {
    /* NOTE: Only works in the range -1 <= x <= +1 */
    float e = 1.0f + x*(1.0f + x*0.5f*(1.0f + x*0.3333333333f*(1.0f + x*0.25f*(1.0f + x*0.2f))));
    return e;
}

float zplm_fast_exp2(float x) { return zplm_fast_exp(ZPLM_LOG_TWO * x); }



float zplm_round(float x) { return (float)((x >= 0.0f) ? zplm_floor(x + 0.5f) : zplm_ceil(x - 0.5f)); }
float zplm_floor(float x) { return (float)((x >= 0.0f) ? (int)x : (int)(x-0.9999999999999999f)); }
float zplm_ceil(float x)  { return (float)((x < 0) ? (int)x : ((int)x)+1); }





float zplm_half_to_float(zplm_half_t value) {
    union { unsigned int i; float f; } result;
    int s = (value >> 15) & 0x001;
    int e = (value >> 10) & 0x01f;
    int m =  value        & 0x3ff;

    if (e == 0) {
        if (m == 0) {
            /* Plus or minus zero */
            result.i = (unsigned int)(s << 31);
            return result.f;
        } else {
            /* Denormalized number */
            while (!(m & 0x00000400)) {
                m <<= 1;
                e -=  1;
            }

            e += 1;
            m &= ~0x00000400;
        }
    } else if (e == 31) {
        if (m == 0) {
            /* Positive or negative infinity */
            result.i = (unsigned int)((s << 31) | 0x7f800000);
            return result.f;
        } else {
            /* Nan */
            result.i = (unsigned int)((s << 31) | 0x7f800000 | (m << 13));
            return result.f;
        }
    }

    e = e + (127 - 15);
    m = m << 13;

    result.i = (unsigned int)((s << 31) | (e << 23) | m);
    return result.f;
}

zplm_half_t zplm_float_to_half(float value) {
    union { unsigned int i; float f; } v;
    int i, s, e, m;

    v.f = value;
    i = (int)v.i;

    s =  (i >> 16) & 0x00008000;
    e = ((i >> 23) & 0x000000ff) - (127 - 15);
    m =   i        & 0x007fffff;


    if (e <= 0) {
        if (e < -10) return (zplm_half_t)s;
        m = (m | 0x00800000) >> (1 - e);

        if (m & 0x00001000)
            m += 0x00002000;

        return (zplm_half_t)(s | (m >> 13));
    } else if (e == 0xff - (127 - 15)) {
        if (m == 0) {
            return (zplm_half_t)(s | 0x7c00); /* NOTE: infinity */
        } else {
            /* NOTE: NAN */
            m >>= 13;
            return (zplm_half_t)(s | 0x7c00 | m | (m == 0));
        }
    } else {
        if (m & 0x00001000) {
            m += 0x00002000;
            if (m & 0x00800000) {
                m = 0;
                e += 1;
            }
        }

        if (e > 30) {
            float volatile f = 1e12f;
            int j;
            for (j = 0; j < 10; j++)
                f *= f; /* NOTE: Cause overflow */

            return (zplm_half_t)(s | 0x7c00);
        }

        return (zplm_half_t)(s | (e << 10) | (m >> 13));
    }
}







#define ZPLM_VEC2_2OP(a,c,post)  \
a->x =        c.x post;    \
a->y =        c.y post;

#define ZPLM_VEC2_3OP(a,b,op,c,post) \
a->x = b.x op c.x post;        \
a->y = b.y op c.y post;

#define ZPLM_VEC3_2OP(a,c,post) \
a->x =        c.x post;   \
a->y =        c.y post;   \
a->z =        c.z post;

#define ZPLM_VEC3_3OP(a,b,op,c,post) \
a->x = b.x op c.x post;        \
a->y = b.y op c.y post;        \
a->z = b.z op c.z post;

#define ZPLM_VEC4_2OP(a,c,post) \
a->x =        c.x post;   \
a->y =        c.y post;   \
a->z =        c.z post;   \
a->w =        c.w post;

#define ZPLM_VEC4_3OP(a,b,op,c,post) \
a->x = b.x op c.x post;        \
a->y = b.y op c.y post;        \
a->z = b.z op c.z post;        \
a->w = b.w op c.w post;


zplm_vec2_t zplm_vec2_zero(void)        { zplm_vec2_t v = {0, 0};                return v; }
zplm_vec2_t zplm_vec2(float x, float y) { zplm_vec2_t v; v.x = x;    v.y = y;    return v; }
zplm_vec2_t zplm_vec2v(float x[2])      { zplm_vec2_t v; v.x = x[0]; v.y = x[1]; return v; }

zplm_vec3_t zplm_vec3_zero(void)                 { zplm_vec3_t v = {0, 0, 0};                         return v; }
zplm_vec3_t zplm_vec3(float x, float y, float z) { zplm_vec3_t v; v.x = x; v.y = y; v.z = z;          return v; }
zplm_vec3_t zplm_vec3v(float x[3])               { zplm_vec3_t v; v.x = x[0]; v.y = x[1]; v.z = x[2]; return v; }

zplm_vec4_t zplm_vec4_zero(void)                          { zplm_vec4_t v = {0, 0, 0, 0};                                  return v; }
zplm_vec4_t zplm_vec4(float x, float y, float z, float w) { zplm_vec4_t v; v.x = x; v.y = y; v.z = z; v.w = w;             return v; }
zplm_vec4_t zplm_vec4v(float x[4])                        { zplm_vec4_t v; v.x = x[0]; v.y = x[1]; v.z = x[2]; v.w = x[3]; return v; }


void zplm_vec2_add(zplm_vec2_t *d, zplm_vec2_t v0, zplm_vec2_t v1) { ZPLM_VEC2_3OP(d,v0,+,v1,+0); }
void zplm_vec2_sub(zplm_vec2_t *d, zplm_vec2_t v0, zplm_vec2_t v1) { ZPLM_VEC2_3OP(d,v0,-,v1,+0); }
void zplm_vec2_mul(zplm_vec2_t *d, zplm_vec2_t v,  float s)   { ZPLM_VEC2_2OP(d,v,* s);      }
void zplm_vec2_div(zplm_vec2_t *d, zplm_vec2_t v,  float s)   { ZPLM_VEC2_2OP(d,v,/ s);      }

void zplm_vec3_add(zplm_vec3_t *d, zplm_vec3_t v0, zplm_vec3_t v1) { ZPLM_VEC3_3OP(d,v0,+,v1,+0); }
void zplm_vec3_sub(zplm_vec3_t *d, zplm_vec3_t v0, zplm_vec3_t v1) { ZPLM_VEC3_3OP(d,v0,-,v1,+0); }
void zplm_vec3_mul(zplm_vec3_t *d, zplm_vec3_t v,  float s)   { ZPLM_VEC3_2OP(d,v,* s);      }
void zplm_vec3_div(zplm_vec3_t *d, zplm_vec3_t v,  float s)   { ZPLM_VEC3_2OP(d,v,/ s);      }

void zplm_vec4_add(zplm_vec4_t *d, zplm_vec4_t v0, zplm_vec4_t v1) { ZPLM_VEC4_3OP(d,v0,+,v1,+0); }
void zplm_vec4_sub(zplm_vec4_t *d, zplm_vec4_t v0, zplm_vec4_t v1) { ZPLM_VEC4_3OP(d,v0,-,v1,+0); }
void zplm_vec4_mul(zplm_vec4_t *d, zplm_vec4_t v,  float s)   { ZPLM_VEC4_2OP(d,v,* s);      }
void zplm_vec4_div(zplm_vec4_t *d, zplm_vec4_t v,  float s)   { ZPLM_VEC4_2OP(d,v,/ s);      }


void zplm_vec2_addeq(zplm_vec2_t *d, zplm_vec2_t v) { ZPLM_VEC2_3OP(d,(*d),+,v,+0); }
void zplm_vec2_subeq(zplm_vec2_t *d, zplm_vec2_t v) { ZPLM_VEC2_3OP(d,(*d),-,v,+0); }
void zplm_vec2_muleq(zplm_vec2_t *d, float s)  { ZPLM_VEC2_2OP(d,(*d),* s);    }
void zplm_vec2_diveq(zplm_vec2_t *d, float s)  { ZPLM_VEC2_2OP(d,(*d),/ s);    }

void zplm_vec3_addeq(zplm_vec3_t *d, zplm_vec3_t v) { ZPLM_VEC3_3OP(d,(*d),+,v,+0); }
void zplm_vec3_subeq(zplm_vec3_t *d, zplm_vec3_t v) { ZPLM_VEC3_3OP(d,(*d),-,v,+0); }
void zplm_vec3_muleq(zplm_vec3_t *d, float s)  { ZPLM_VEC3_2OP(d,(*d),* s);    }
void zplm_vec3_diveq(zplm_vec3_t *d, float s)  { ZPLM_VEC3_2OP(d,(*d),/ s);    }

void zplm_vec4_addeq(zplm_vec4_t *d, zplm_vec4_t v) { ZPLM_VEC4_3OP(d,(*d),+,v,+0); }
void zplm_vec4_subeq(zplm_vec4_t *d, zplm_vec4_t v) { ZPLM_VEC4_3OP(d,(*d),-,v,+0); }
void zplm_vec4_muleq(zplm_vec4_t *d, float s)  { ZPLM_VEC4_2OP(d,(*d),* s);    }
void zplm_vec4_diveq(zplm_vec4_t *d, float s)  { ZPLM_VEC4_2OP(d,(*d),/ s);    }


#undef ZPLM_VEC2_2OP
#undef ZPLM_VEC2_3OP
#undef ZPLM_VEC3_3OP
#undef ZPLM_VEC3_2OP
#undef ZPLM_VEC4_2OP
#undef ZPLM_VEC4_3OP




float zplm_vec2_dot(zplm_vec2_t v0, zplm_vec2_t v1) { return v0.x*v1.x + v0.y*v1.y; }
float zplm_vec3_dot(zplm_vec3_t v0, zplm_vec3_t v1) { return v0.x*v1.x + v0.y*v1.y + v0.z*v1.z; }
float zplm_vec4_dot(zplm_vec4_t v0, zplm_vec4_t v1) { return v0.x*v1.x + v0.y*v1.y + v0.z*v1.z + v0.w*v1.w; }

void zplm_vec2_cross(float *d, zplm_vec2_t v0, zplm_vec2_t v1) { *d = v0.x*v1.y - v1.x*v0.y; }
void zplm_vec3_cross(zplm_vec3_t *d, zplm_vec3_t v0, zplm_vec3_t v1)  { d->x = v0.y*v1.z - v0.z*v1.y;
    d->y = v0.z*v1.x - v0.x*v1.z;
    d->z = v0.x*v1.y - v0.y*v1.x; }

float zplm_vec2_mag2(zplm_vec2_t v) { return zplm_vec2_dot(v, v); }
float zplm_vec3_mag2(zplm_vec3_t v) { return zplm_vec3_dot(v, v); }
float zplm_vec4_mag2(zplm_vec4_t v) { return zplm_vec4_dot(v, v); }

/* TODO: Create custom sqrt function */
float zplm_vec2_mag(zplm_vec2_t v) { return zplm_sqrt(zplm_vec2_dot(v, v)); }
float zplm_vec3_mag(zplm_vec3_t v) { return zplm_sqrt(zplm_vec3_dot(v, v)); }
float zplm_vec4_mag(zplm_vec4_t v) { return zplm_sqrt(zplm_vec4_dot(v, v)); }

void zplm_vec2_norm(zplm_vec2_t *d, zplm_vec2_t v) {
    float inv_mag = zplm_rsqrt(zplm_vec2_dot(v, v));
    zplm_vec2_mul(d, v, inv_mag);
}
void zplm_vec3_norm(zplm_vec3_t *d, zplm_vec3_t v) {
    float mag = zplm_vec3_mag(v);
    zplm_vec3_div(d, v, mag);
}
void zplm_vec4_norm(zplm_vec4_t *d, zplm_vec4_t v) {
    float mag = zplm_vec4_mag(v);
    zplm_vec4_div(d, v, mag);
}

void zplm_vec2_norm0(zplm_vec2_t *d, zplm_vec2_t v) {
    float mag = zplm_vec2_mag(v);
    if (mag > 0)
        zplm_vec2_div(d, v, mag);
    else
        *d = zplm_vec2_zero();
}
void zplm_vec3_norm0(zplm_vec3_t *d, zplm_vec3_t v) {
    float mag = zplm_vec3_mag(v);
    if (mag > 0)
        zplm_vec3_div(d, v, mag);
    else
        *d = zplm_vec3_zero();
}
void zplm_vec4_norm0(zplm_vec4_t *d, zplm_vec4_t v) {
    float mag = zplm_vec4_mag(v);
    if (mag > 0)
        zplm_vec4_div(d, v, mag);
    else
        *d = zplm_vec4_zero();
}


void zplm_vec2_reflect(zplm_vec2_t *d, zplm_vec2_t i, zplm_vec2_t n) {
    zplm_vec2_t b = n;
    zplm_vec2_muleq(&b, 2.0f*zplm_vec2_dot(n, i));
    zplm_vec2_sub(d, i, b);
}

void zplm_vec3_reflect(zplm_vec3_t *d, zplm_vec3_t i, zplm_vec3_t n) {
    zplm_vec3_t b = n;
    zplm_vec3_muleq(&b, 2.0f*zplm_vec3_dot(n, i));
    zplm_vec3_sub(d, i, b);
}

void zplm_vec2_refract(zplm_vec2_t *d, zplm_vec2_t i, zplm_vec2_t n, float eta) {
    zplm_vec2_t a, b;
    float dv, k;

    dv = zplm_vec2_dot(n, i);
    k = 1.0f - eta*eta * (1.0f - dv*dv);
    zplm_vec2_mul(&a, i, eta);
    zplm_vec2_mul(&b, n, eta*dv*zplm_sqrt(k));
    zplm_vec2_sub(d, a, b);
    zplm_vec2_muleq(d, (float)(k >= 0.0f));
}

void zplm_vec3_refract(zplm_vec3_t *d, zplm_vec3_t i, zplm_vec3_t n, float eta) {
    zplm_vec3_t a, b;
    float dv, k;

    dv = zplm_vec3_dot(n, i);
    k = 1.0f - eta*eta * (1.0f - dv*dv);
    zplm_vec3_mul(&a, i, eta);
    zplm_vec3_mul(&b, n, eta*dv*zplm_sqrt(k));
    zplm_vec3_sub(d, a, b);
    zplm_vec3_muleq(d, (float)(k >= 0.0f));
}





float zplm_vec2_aspect_ratio(zplm_vec2_t v) { return (v.y < 0.0001f) ? 0.0f : v.x/v.y; }





void zplm_mat2_transpose(zplm_mat2_t *m) { zplm_float22_transpose(zplm_float22_m(m)); }
void zplm_mat2_identity(zplm_mat2_t *m)  { zplm_float22_identity(zplm_float22_m(m));  }
void zplm_mat2_mul(zplm_mat2_t *out, zplm_mat2_t *m1, zplm_mat2_t *m2) { zplm_float22_mul(zplm_float22_m(out), zplm_float22_m(m1), zplm_float22_m(m2)); }

void zplm_float22_identity(float m[2][2]) {
    m[0][0] = 1; m[0][1] = 0;
    m[1][0] = 0; m[1][1] = 1;
}

void zplm_mat2_mul_vec2(zplm_vec2_t *out, zplm_mat2_t *m, zplm_vec2_t in) { zplm_float22_mul_vec2(out, zplm_float22_m(m), in); }

zplm_mat2_t *zplm_mat2_v(zplm_vec2_t m[2])   { return (zplm_mat2_t *)m; }
zplm_mat2_t *zplm_mat2_f(float m[2][2]) { return (zplm_mat2_t *)m; }

zplm_float2_t *zplm_float22_m(zplm_mat2_t *m)   { return (zplm_float2_t *)m; }
zplm_float2_t *zplm_float22_v(zplm_vec2_t m[2]) { return (zplm_float2_t *)m; }
zplm_float2_t *zplm_float22_4(float m[4])  { return (zplm_float2_t *)m; }

void zplm_float22_transpose(float (*vec)[2]) {
    int i, j;
    for (j = 0; j < 2; j++) {
        for (i = j + 1; i < 2; i++) {
            float t = vec[i][j];
            vec[i][j] = vec[j][i];
            vec[j][i] = t;
        }
    }
}



void zplm_float22_mul(float (*out)[2], float (*mat1)[2], float (*mat2)[2]) {
    int i, j;
    float temp1[2][2], temp2[2][2];
    if (mat1 == out) { zplm__memcpy_4byte(temp1, mat1, sizeof(temp1)); mat1 = temp1; }
    if (mat2 == out) { zplm__memcpy_4byte(temp2, mat2, sizeof(temp2)); mat2 = temp2; }
    for (j = 0; j < 2; j++) {
        for (i = 0; i < 2; i++) {
            out[j][i] = mat1[0][i]*mat2[j][0]
                + mat1[1][i]*mat2[j][1];
        }
    }
}

void zplm_float22_mul_vec2(zplm_vec2_t *out, float m[2][2], zplm_vec2_t v) {
    out->x = m[0][0]*v.x + m[0][1]*v.y;
    out->y = m[1][0]*v.x + m[1][1]*v.y;
}

float zplm_mat2_determinate(zplm_mat2_t *m) {
    zplm_float2_t *e = zplm_float22_m(m);
    return e[0][0]*e[1][1] - e[1][0]*e[0][1];
}

void zplm_mat2_inverse(zplm_mat2_t *out, zplm_mat2_t *in) {
    zplm_float2_t *o = zplm_float22_m(out);
    zplm_float2_t *i = zplm_float22_m(in);

    float ood = 1.0f / zplm_mat2_determinate(in);

    o[0][0] = +i[1][1] * ood;
    o[0][1] = -i[0][1] * ood;
    o[1][0] = -i[1][0] * ood;
    o[1][1] = +i[0][0] * ood;
}






void zplm_mat3_transpose(zplm_mat3_t *m) { zplm_float33_transpose(zplm_float33_m(m)); }
void zplm_mat3_identity(zplm_mat3_t *m)  { zplm_float33_identity(zplm_float33_m(m));  }
void zplm_mat3_mul(zplm_mat3_t *out, zplm_mat3_t *m1, zplm_mat3_t *m2) { zplm_float33_mul(zplm_float33_m(out), zplm_float33_m(m1), zplm_float33_m(m2)); }

void zplm_float33_identity(float m[3][3]) {
    m[0][0] = 1; m[0][1] = 0; m[0][2] = 0;
    m[1][0] = 0; m[1][1] = 1; m[1][2] = 0;
    m[2][0] = 0; m[2][1] = 0; m[2][2] = 1;
}

void zplm_mat3_mul_vec3(zplm_vec3_t *out, zplm_mat3_t *m, zplm_vec3_t in) { zplm_float33_mul_vec3(out, zplm_float33_m(m), in); }

zplm_mat3_t *zplm_mat3_v(zplm_vec3_t m[3])   { return (zplm_mat3_t *)m; }
zplm_mat3_t *zplm_mat3_f(float m[3][3]) { return (zplm_mat3_t *)m; }

zplm_float3_t *zplm_float33_m(zplm_mat3_t *m)   { return (zplm_float3_t *)m; }
zplm_float3_t *zplm_float33_v(zplm_vec3_t m[3]) { return (zplm_float3_t *)m; }
zplm_float3_t *zplm_float33_16(float m[9]) { return (zplm_float3_t *)m; }

void zplm_float33_transpose(float (*vec)[3]) {
    int i, j;
    for (j = 0; j < 3; j++) {
        for (i = j + 1; i < 3; i++) {
            float t = vec[i][j];
            vec[i][j] = vec[j][i];
            vec[j][i] = t;
        }
    }
}

void zplm_float33_mul(float (*out)[3], float (*mat1)[3], float (*mat2)[3]) {
    int i, j;
    float temp1[3][3], temp2[3][3];
    if (mat1 == out) { zplm__memcpy_4byte(temp1, mat1, sizeof(temp1)); mat1 = temp1; }
    if (mat2 == out) { zplm__memcpy_4byte(temp2, mat2, sizeof(temp2)); mat2 = temp2; }
    for (j = 0; j < 3; j++) {
        for (i = 0; i < 3; i++) {
            out[j][i] = mat1[0][i]*mat2[j][0]
                + mat1[1][i]*mat2[j][1]
                + mat1[2][i]*mat2[j][2];
        }
    }
}

void zplm_float33_mul_vec3(zplm_vec3_t *out, float m[3][3], zplm_vec3_t v) {
    out->x = m[0][0]*v.x + m[0][1]*v.y + m[0][2]*v.z;
    out->y = m[1][0]*v.x + m[1][1]*v.y + m[1][2]*v.z;
    out->z = m[2][0]*v.x + m[2][1]*v.y + m[2][2]*v.z;
}



float zplm_mat3_determinate(zplm_mat3_t *m) {
    zplm_float3_t *e = zplm_float33_m(m);
    float d = +e[0][0] * (e[1][1] * e[2][2] - e[1][2] * e[2][1])
        -e[0][1] * (e[1][0] * e[2][2] - e[1][2] * e[2][0])
        +e[0][2] * (e[1][0] * e[2][1] - e[1][1] * e[2][0]);
    return d;
}

void zplm_mat3_inverse(zplm_mat3_t *out, zplm_mat3_t *in) {
    zplm_float3_t *o = zplm_float33_m(out);
    zplm_float3_t *i = zplm_float33_m(in);

    float ood = 1.0f / zplm_mat3_determinate(in);

    o[0][0] = +(i[1][1] * i[2][2] - i[2][1] * i[1][2]) * ood;
    o[0][1] = -(i[1][0] * i[2][2] - i[2][0] * i[1][2]) * ood;
    o[0][2] = +(i[1][0] * i[2][1] - i[2][0] * i[1][1]) * ood;
    o[1][0] = -(i[0][1] * i[2][2] - i[2][1] * i[0][2]) * ood;
    o[1][1] = +(i[0][0] * i[2][2] - i[2][0] * i[0][2]) * ood;
    o[1][2] = -(i[0][0] * i[2][1] - i[2][0] * i[0][1]) * ood;
    o[2][0] = +(i[0][1] * i[1][2] - i[1][1] * i[0][2]) * ood;
    o[2][1] = -(i[0][0] * i[1][2] - i[1][0] * i[0][2]) * ood;
    o[2][2] = +(i[0][0] * i[1][1] - i[1][0] * i[0][1]) * ood;
}











void zplm_mat4_transpose(zplm_mat4_t *m) { zplm_float44_transpose(zplm_float44_m(m)); }
void zplm_mat4_identity(zplm_mat4_t *m)  { zplm_float44_identity(zplm_float44_m(m));  }
void zplm_mat4_mul(zplm_mat4_t *out, zplm_mat4_t *m1, zplm_mat4_t *m2) { zplm_float44_mul(zplm_float44_m(out), zplm_float44_m(m1), zplm_float44_m(m2)); }

void zplm_float44_identity(float m[4][4]) {
    m[0][0] = 1; m[0][1] = 0; m[0][2] = 0; m[0][3] = 0;
    m[1][0] = 0; m[1][1] = 1; m[1][2] = 0; m[1][3] = 0;
    m[2][0] = 0; m[2][1] = 0; m[2][2] = 1; m[2][3] = 0;
    m[3][0] = 0; m[3][1] = 0; m[3][2] = 0; m[3][3] = 1;
}

void zplm_mat4_mul_vec4(zplm_vec4_t *out, zplm_mat4_t *m, zplm_vec4_t in) {
    zplm_float44_mul_vec4(out, zplm_float44_m(m), in);
}

zplm_mat4_t *zplm_mat4_v(zplm_vec4_t m[4])   { return (zplm_mat4_t *)m; }
zplm_mat4_t *zplm_mat4_f(float m[4][4]) { return (zplm_mat4_t *)m; }

zplm_float4_t *zplm_float44_m(zplm_mat4_t *m)    { return (zplm_float4_t *)m; }
zplm_float4_t *zplm_float44_v(zplm_vec4_t m[4])  { return (zplm_float4_t *)m; }
zplm_float4_t *zplm_float44_16(float m[16]) { return (zplm_float4_t *)m; }

void zplm_float44_transpose(float (*vec)[4]) {
    float tmp;
    tmp = vec[1][0]; vec[1][0] = vec[0][1]; vec[0][1] = tmp;
    tmp = vec[2][0]; vec[2][0] = vec[0][2]; vec[0][2] = tmp;
    tmp = vec[3][0]; vec[3][0] = vec[0][3]; vec[0][3] = tmp;
    tmp = vec[2][1]; vec[2][1] = vec[1][2]; vec[1][2] = tmp;
    tmp = vec[3][1]; vec[3][1] = vec[1][3]; vec[1][3] = tmp;
    tmp = vec[3][2]; vec[3][2] = vec[2][3]; vec[2][3] = tmp;
}

void zplm_float44_mul(float (*out)[4], float (*mat1)[4], float (*mat2)[4]) {
    int i, j;
    float temp1[4][4], temp2[4][4];
    if (mat1 == out) { zplm__memcpy_4byte(temp1, mat1, sizeof(temp1)); mat1 = temp1; }
    if (mat2 == out) { zplm__memcpy_4byte(temp2, mat2, sizeof(temp2)); mat2 = temp2; }
    for (j = 0; j < 4; j++) {
        for (i = 0; i < 4; i++) {
            out[j][i] = mat1[0][i]*mat2[j][0]
                + mat1[1][i]*mat2[j][1]
                + mat1[2][i]*mat2[j][2]
                + mat1[3][i]*mat2[j][3];
        }
    }
}

void zplm_float44_mul_vec4(zplm_vec4_t *out, float m[4][4], zplm_vec4_t v) {
    out->x = m[0][0]*v.x + m[1][0]*v.y + m[2][0]*v.z + m[3][0]*v.w;
    out->y = m[0][1]*v.x + m[1][1]*v.y + m[2][1]*v.z + m[3][1]*v.w;
    out->z = m[0][2]*v.x + m[1][2]*v.y + m[2][2]*v.z + m[3][2]*v.w;
    out->w = m[0][3]*v.x + m[1][3]*v.y + m[2][3]*v.z + m[3][3]*v.w;
}

void zplm_mat4_inverse(zplm_mat4_t *out, zplm_mat4_t *in) {
    zplm_float4_t *o = zplm_float44_m(out);
    zplm_float4_t *m = zplm_float44_m(in);

    float ood;
    //float tmp;

    float sf00 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
    float sf01 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
    float sf02 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
    float sf03 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
    float sf04 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
    float sf05 = m[2][0] * m[3][1] - m[3][0] * m[2][1];
    float sf06 = m[1][2] * m[3][3] - m[3][2] * m[1][3];
    float sf07 = m[1][1] * m[3][3] - m[3][1] * m[1][3];
    float sf08 = m[1][1] * m[3][2] - m[3][1] * m[1][2];
    float sf09 = m[1][0] * m[3][3] - m[3][0] * m[1][3];
    float sf10 = m[1][0] * m[3][2] - m[3][0] * m[1][2];
    float sf11 = m[1][1] * m[3][3] - m[3][1] * m[1][3];
    float sf12 = m[1][0] * m[3][1] - m[3][0] * m[1][1];
    float sf13 = m[1][2] * m[2][3] - m[2][2] * m[1][3];
    float sf14 = m[1][1] * m[2][3] - m[2][1] * m[1][3];
    float sf15 = m[1][1] * m[2][2] - m[2][1] * m[1][2];
    float sf16 = m[1][0] * m[2][3] - m[2][0] * m[1][3];
    float sf17 = m[1][0] * m[2][2] - m[2][0] * m[1][2];
    float sf18 = m[1][0] * m[2][1] - m[2][0] * m[1][1];

    o[0][0] = +(m[1][1] * sf00 - m[1][2] * sf01 + m[1][3] * sf02);
    o[1][0] = -(m[1][0] * sf00 - m[1][2] * sf03 + m[1][3] * sf04);
    o[2][0] = +(m[1][0] * sf01 - m[1][1] * sf03 + m[1][3] * sf05);
    o[3][0] = -(m[1][0] * sf02 - m[1][1] * sf04 + m[1][2] * sf05);

    o[0][1] = -(m[0][1] * sf00 - m[0][2] * sf01 + m[0][3] * sf02);
    o[1][1] = +(m[0][0] * sf00 - m[0][2] * sf03 + m[0][3] * sf04);
    o[2][1] = -(m[0][0] * sf01 - m[0][1] * sf03 + m[0][3] * sf05);
    o[3][1] = +(m[0][0] * sf02 - m[0][1] * sf04 + m[0][2] * sf05);

    o[0][2] = +(m[0][1] * sf06 - m[0][2] * sf07 + m[0][3] * sf08);
    o[1][2] = -(m[0][0] * sf06 - m[0][2] * sf09 + m[0][3] * sf10);
    o[2][2] = +(m[0][0] * sf11 - m[0][1] * sf09 + m[0][3] * sf12);
    o[3][2] = -(m[0][0] * sf08 - m[0][1] * sf10 + m[0][2] * sf12);

    o[0][3] = -(m[0][1] * sf13 - m[0][2] * sf14 + m[0][3] * sf15);
    o[1][3] = +(m[0][0] * sf13 - m[0][2] * sf16 + m[0][3] * sf17);
    o[2][3] = -(m[0][0] * sf14 - m[0][1] * sf16 + m[0][3] * sf18);
    o[3][3] = +(m[0][0] * sf15 - m[0][1] * sf17 + m[0][2] * sf18);

    ood = 1.0f / (m[0][0] * o[0][0] +
                  m[0][1] * o[1][0] +
                  m[0][2] * o[2][0] +
                  m[0][3] * o[3][0]);

    o[0][0] *= ood;
    o[0][1] *= ood;
    o[0][2] *= ood;
    o[0][3] *= ood;
    o[1][0] *= ood;
    o[1][1] *= ood;
    o[1][2] *= ood;
    o[1][3] *= ood;
    o[2][0] *= ood;
    o[2][1] *= ood;
    o[2][2] *= ood;
    o[2][3] *= ood;
    o[3][0] *= ood;
    o[3][1] *= ood;
    o[3][2] *= ood;
    o[3][3] *= ood;
}







void zplm_mat4_translate(zplm_mat4_t *out, zplm_vec3_t v) {
    zplm_mat4_identity(out);
    out->col[3].xyz = v;
    out->col[3].w  = 1;
}

void zplm_mat4_rotate(zplm_mat4_t *out, zplm_vec3_t v, float angle_radians) {
    float c, s;
    zplm_vec3_t axis, t;
    zplm_float4_t *rot;

    c = zplm_cos(angle_radians);
    s = zplm_sin(angle_radians);

    zplm_vec3_norm(&axis, v);
    zplm_vec3_mul(&t, axis, 1.0f-c);

    zplm_mat4_identity(out);
    rot = zplm_float44_m(out);

    rot[0][0] = c + t.x*axis.x;
    rot[0][1] = 0 + t.x*axis.y + s*axis.z;
    rot[0][2] = 0 + t.x*axis.z - s*axis.y;
    rot[0][3] = 0;

    rot[1][0] = 0 + t.y*axis.x - s*axis.z;
    rot[1][1] = c + t.y*axis.y;
    rot[1][2] = 0 + t.y*axis.z + s*axis.x;
    rot[1][3] = 0;

    rot[2][0] = 0 + t.z*axis.x + s*axis.y;
    rot[2][1] = 0 + t.z*axis.y - s*axis.x;
    rot[2][2] = c + t.z*axis.z;
    rot[2][3] = 0;
}

void zplm_mat4_scale(zplm_mat4_t *out, zplm_vec3_t v) {
    zplm_mat4_identity(out);
    out->e[0]  = v.x;
    out->e[5]  = v.y;
    out->e[10] = v.z;
}

void zplm_mat4_scalef(zplm_mat4_t *out, float s) {
    zplm_mat4_identity(out);
    out->e[0]  = s;
    out->e[5]  = s;
    out->e[10] = s;
}


void zplm_mat4_ortho2d(zplm_mat4_t *out, float left, float right, float bottom, float top) {
    zplm_float4_t *m;
    zplm_mat4_identity(out);
    m = zplm_float44_m(out);

    m[0][0] = 2.0f / (right - left);
    m[1][1] = 2.0f / (top - bottom);
    m[2][2] = -1.0f;
    m[3][0] = -(right + left) / (right - left);
    m[3][1] = -(top + bottom) / (top - bottom);
}

void zplm_mat4_ortho3d(zplm_mat4_t *out, float left, float right, float bottom, float top, float z_near, float z_far) {
    zplm_float4_t *m;
    zplm_mat4_identity(out);
    m = zplm_float44_m(out);

    m[0][0] = +2.0f / (right - left);
    m[1][1] = +2.0f / (top - bottom);
    m[2][2] = -2.0f / (z_far - z_near);
    m[3][0] = -(right + left)   / (right - left);
    m[3][1] = -(top   + bottom) / (top   - bottom);
    m[3][2] = -(z_far + z_near) / (z_far - z_near);
}


void zplm_mat4_perspective(zplm_mat4_t *out, float fovy, float aspect, float z_near, float z_far) {
    float tan_half_fovy = zplm_tan(0.5f * fovy);
    zplm_mat4_t zero_mat = {0};
    zplm_float4_t *m = zplm_float44_m(out);
    *out = zero_mat;

    m[0][0] = 1.0f / (aspect*tan_half_fovy);
    m[1][1] = 1.0f / (tan_half_fovy);
    m[2][2] = -(z_far + z_near) / (z_far - z_near);
    m[2][3] = -1.0f;
    m[3][2] = -2.0f*z_far*z_near / (z_far - z_near);
}

void zplm_mat4_infinite_perspective(zplm_mat4_t *out, float fovy, float aspect, float z_near) {
    float range  = zplm_tan(0.5f * fovy) * z_near;
    float left   = -range * aspect;
    float right  =  range * aspect;
    float bottom = -range;
    float top    =  range;
    zplm_mat4_t zero_mat = {0};
    zplm_float4_t *m = zplm_float44_m(out);
    *out = zero_mat;

    m[0][0] = (2.0f*z_near) / (right - left);
    m[1][1] = (2.0f*z_near) / (top   - bottom);
    m[2][2] = -1.0f;
    m[2][3] = -1.0f;
    m[3][2] = -2.0f*z_near;
}

void zplm_mat4_look_at(zplm_mat4_t *out, zplm_vec3_t eye, zplm_vec3_t centre, zplm_vec3_t up) {
    zplm_vec3_t f, s, u;
    zplm_float4_t *m;

    zplm_vec3_sub(&f, centre, eye);
    zplm_vec3_norm(&f, f);

    zplm_vec3_cross(&s, f, up);
    zplm_vec3_norm(&s, s);

    zplm_vec3_cross(&u, s, f);

    zplm_mat4_identity(out);
    m = zplm_float44_m(out);

    m[0][0] = +s.x;
    m[1][0] = +s.y;
    m[2][0] = +s.z;

    m[0][1] = +u.x;
    m[1][1] = +u.y;
    m[2][1] = +u.z;

    m[0][2] = -f.x;
    m[1][2] = -f.y;
    m[2][2] = -f.z;

    m[3][0] = zplm_vec3_dot(s, eye);
    m[3][1] = zplm_vec3_dot(u, eye);
    m[3][2] = zplm_vec3_dot(f, eye);
}












zplm_quat_t zplm_quat(float x, float y, float z, float w) { zplm_quat_t q; q.x = x; q.y = y; q.z = z; q.w = w; return q; }
zplm_quat_t zplm_quatv(float e[4]) { zplm_quat_t q; q.x = e[0]; q.y = e[1]; q.z = e[2]; q.w = e[3]; return q; }

zplm_quat_t zplm_quat_axis_angle(zplm_vec3_t axis, float angle_radians) {
    zplm_quat_t q;
    zplm_vec3_norm(&q.xyz, axis);
    zplm_vec3_muleq(&q.xyz, zplm_sin(0.5f*angle_radians));
    q.w = zplm_cos(0.5f*angle_radians);
    return q;
}

zplm_quat_t zplm_quat_euler_angles(float pitch, float yaw, float roll) {
    /* TODO: Do without multiplication, i.e. make it faster */
    zplm_quat_t q, p, y, r;
    p = zplm_quat_axis_angle(zplm_vec3(1, 0, 0), pitch);
    y = zplm_quat_axis_angle(zplm_vec3(0, 1, 0), yaw);
    r = zplm_quat_axis_angle(zplm_vec3(0, 0, 1), roll);

    zplm_quat_mul(&q, y, p);
    zplm_quat_muleq(&q, r);

    return q;
}

zplm_quat_t zplm_quat_identity(void) { zplm_quat_t q = {0, 0, 0, 1}; return q; }


void zplm_quat_add(zplm_quat_t *d, zplm_quat_t q0, zplm_quat_t q1) { zplm_vec4_add(&d->xyzw, q0.xyzw, q1.xyzw); }
void zplm_quat_sub(zplm_quat_t *d, zplm_quat_t q0, zplm_quat_t q1) { zplm_vec4_sub(&d->xyzw, q0.xyzw, q1.xyzw); }

void zplm_quat_mul(zplm_quat_t *d, zplm_quat_t q0, zplm_quat_t q1) {
    d->x = q0.w * q1.x + q0.x * q1.w + q0.y * q1.z - q0.z * q1.y;
    d->y = q0.w * q1.y - q0.x * q1.z + q0.y * q1.w + q0.z * q1.x;
    d->z = q0.w * q1.z + q0.x * q1.y - q0.y * q1.x + q0.z * q1.w;
    d->w = q0.w * q1.w - q0.x * q1.x - q0.y * q1.y - q0.z * q1.z;
}

void zplm_quat_div(zplm_quat_t *d, zplm_quat_t q0, zplm_quat_t q1){ zplm_quat_t iq1; zplm_quat_inverse(&iq1, q1); zplm_quat_mul(d, q0, iq1); }

void zplm_quat_mulf(zplm_quat_t *d, zplm_quat_t q0, float s) { zplm_vec4_mul(&d->xyzw, q0.xyzw, s); }
void zplm_quat_divf(zplm_quat_t *d, zplm_quat_t q0, float s) { zplm_vec4_div(&d->xyzw, q0.xyzw, s); }


void zplm_quat_addeq(zplm_quat_t *d, zplm_quat_t q) { zplm_vec4_addeq(&d->xyzw, q.xyzw); }
void zplm_quat_subeq(zplm_quat_t *d, zplm_quat_t q) { zplm_vec4_subeq(&d->xyzw, q.xyzw); }
void zplm_quat_muleq(zplm_quat_t *d, zplm_quat_t q) { zplm_quat_mul(d, *d, q); }
void zplm_quat_diveq(zplm_quat_t *d, zplm_quat_t q) { zplm_quat_div(d, *d, q); }


void zplm_quat_muleqf(zplm_quat_t *d, float s) { zplm_vec4_muleq(&d->xyzw, s); }
void zplm_quat_diveqf(zplm_quat_t *d, float s) { zplm_vec4_diveq(&d->xyzw, s); }

float zplm_quat_dot(zplm_quat_t q0, zplm_quat_t q1) { float r = zplm_vec3_dot(q0.xyz, q1.xyz) + q0.w*q1.w; return r; }
float zplm_quat_mag(zplm_quat_t q)           { float r = zplm_sqrt(zplm_quat_dot(q, q)); return r; }

void zplm_quat_norm(zplm_quat_t *d, zplm_quat_t q)    { zplm_quat_divf(d, q, zplm_quat_mag(q)); }
void zplm_quat_conj(zplm_quat_t *d, zplm_quat_t q)    { d->xyz = zplm_vec3(-q.x, -q.y, -q.z); d->w = q.w; }
void zplm_quat_inverse(zplm_quat_t *d, zplm_quat_t q) { zplm_quat_conj(d, q); zplm_quat_diveqf(d, zplm_quat_dot(q, q)); }


void zplm_quat_axis(zplm_vec3_t *axis, zplm_quat_t q) {
    zplm_quat_t n; zplm_quat_norm(&n, q);
    zplm_vec3_div(axis, n.xyz, zplm_sin(zplm_arccos(q.w)));
}

float zplm_quat_angle(zplm_quat_t q) {
    float mag = zplm_quat_mag(q);
    float c = q.w * (1.0f/mag);
    float angle = 2.0f*zplm_arccos(c);
    return angle;
}


float zplm_quat_roll(zplm_quat_t q)  { return zplm_arctan2(2.0f*q.x*q.y + q.z*q.w, q.x*q.x + q.w*q.w - q.y*q.y - q.z*q.z); }
float zplm_quat_pitch(zplm_quat_t q) { return zplm_arctan2(2.0f*q.y*q.z + q.w*q.x, q.w*q.w - q.x*q.x - q.y*q.y + q.z*q.z); }
float zplm_quat_yaw(zplm_quat_t q)   { return zplm_arcsin(-2.0f*(q.x*q.z - q.w*q.y)); }

void zplm_quat_rotate_vec3(zplm_vec3_t *d, zplm_quat_t q, zplm_vec3_t v) {
    /* zplm_vec3_t t = 2.0f * cross(q.xyz, v);
     * *d = q.w*t + v + cross(q.xyz, t);
     */
    zplm_vec3_t t, p;
    zplm_vec3_cross(&t, q.xyz, v);
    zplm_vec3_muleq(&t, 2.0f);

    zplm_vec3_cross(&p, q.xyz, t);

    zplm_vec3_mul(d, t, q.w);
    zplm_vec3_addeq(d, v);
    zplm_vec3_addeq(d, p);
}


void zplm_mat4_from_quat(zplm_mat4_t *out, zplm_quat_t q) {
    zplm_float4_t *m;
    zplm_quat_t a;
    float xx, yy, zz,
    xy, xz, yz,
    wx, wy, wz;

    zplm_quat_norm(&a, q);
    xx = a.x*a.x; yy = a.y*a.y; zz = a.z*a.z;
    xy = a.x*a.y; xz = a.x*a.z; yz = a.y*a.z;
    wx = a.w*a.x; wy = a.w*a.y; wz = a.w*a.z;

    zplm_mat4_identity(out);
    m = zplm_float44_m(out);

    m[0][0] = 1.0f - 2.0f*(yy + zz);
    m[0][1] =        2.0f*(xy + wz);
    m[0][2] =        2.0f*(xz - wy);

    m[1][0] =        2.0f*(xy - wz);
    m[1][1] = 1.0f - 2.0f*(xx + zz);
    m[1][2] =        2.0f*(yz + wx);

    m[2][0] =        2.0f*(xz + wy);
    m[2][1] =        2.0f*(yz - wx);
    m[2][2] = 1.0f - 2.0f*(xx + yy);
}

void zplm_quat_from_mat4(zplm_quat_t *out, zplm_mat4_t *mat) {
    zplm_float4_t *m;
    float four_x_squared_minus_1, four_y_squared_minus_1,
    four_z_squared_minus_1, four_w_squared_minus_1,
    four_biggest_squared_minus_1;
    int biggest_index = 0;
    float biggest_value, mult;

    m = zplm_float44_m(mat);

    four_x_squared_minus_1 = m[0][0] - m[1][1] - m[2][2];
    four_y_squared_minus_1 = m[1][1] - m[0][0] - m[2][2];
    four_z_squared_minus_1 = m[2][2] - m[0][0] - m[1][1];
    four_w_squared_minus_1 = m[0][0] + m[1][1] + m[2][2];

    four_biggest_squared_minus_1 = four_w_squared_minus_1;
    if (four_x_squared_minus_1 > four_biggest_squared_minus_1) {
        four_biggest_squared_minus_1 = four_x_squared_minus_1;
        biggest_index = 1;
    }
    if (four_y_squared_minus_1 > four_biggest_squared_minus_1) {
        four_biggest_squared_minus_1 = four_y_squared_minus_1;
        biggest_index = 2;
    }
    if (four_z_squared_minus_1 > four_biggest_squared_minus_1) {
        four_biggest_squared_minus_1 = four_z_squared_minus_1;
        biggest_index = 3;
    }

    biggest_value = zplm_sqrt(four_biggest_squared_minus_1 + 1.0f) * 0.5f;
    mult = 0.25f / biggest_value;

    switch (biggest_index) {
        case 0:
        out->w = biggest_value;
        out->x = (m[1][2] - m[2][1]) * mult;
        out->y = (m[2][0] - m[0][2]) * mult;
        out->z = (m[0][1] - m[1][0]) * mult;
        break;
        case 1:
        out->w = (m[1][2] - m[2][1]) * mult;
        out->x = biggest_value;
        out->y = (m[0][1] + m[1][0]) * mult;
        out->z = (m[2][0] + m[0][2]) * mult;
        break;
        case 2:
        out->w = (m[2][0] - m[0][2]) * mult;
        out->x = (m[0][1] + m[1][0]) * mult;
        out->y = biggest_value;
        out->z = (m[1][2] + m[2][1]) * mult;
        break;
        case 3:
        out->w = (m[0][1] - m[1][0]) * mult;
        out->x = (m[2][0] + m[0][2]) * mult;
        out->y = (m[1][2] + m[2][1]) * mult;
        out->z = biggest_value;
        break;
        default:
        /* NOTE: This shouldn't fucking happen!!! */
        break;
    }

}






float zplm_lerp         (float a, float b, float t) { return a*(1.0f-t) + b*t; }
float zplm_unlerp       (float t, float a, float b) { return (t-a)/(b-a); }
float zplm_smooth_step  (float a, float b, float t) { float x = (t - a)/(b - a); return x*x*(3.0f - 2.0f*x); }
float zplm_smoother_step(float a, float b, float t) { float x = (t - a)/(b - a); return x*x*x*(x*(6.0f*x - 15.0f) + 10.0f); }


#define ZPLM_VEC_LERPN(N, d, a, b, t) \
zplm_vec##N##_t db; \
zplm_vec##N##_sub(&db, b, a); \
zplm_vec##N##_muleq(&db, t); \
zplm_vec##N##_add(d, a, db)
void zplm_vec2_lerp(zplm_vec2_t *d, zplm_vec2_t a, zplm_vec2_t b, float t) { ZPLM_VEC_LERPN(2, d, a, b, t); }
void zplm_vec3_lerp(zplm_vec3_t *d, zplm_vec3_t a, zplm_vec3_t b, float t) { ZPLM_VEC_LERPN(3, d, a, b, t); }
void zplm_vec4_lerp(zplm_vec4_t *d, zplm_vec4_t a, zplm_vec4_t b, float t) { ZPLM_VEC_LERPN(4, d, a, b, t); }

#undef ZPLM_VEC_LERPN

void zplm_quat_lerp(zplm_quat_t *d, zplm_quat_t a, zplm_quat_t b, float t)  { zplm_vec4_lerp(&d->xyzw, a.xyzw, b.xyzw, t); }
void zplm_quat_nlerp(zplm_quat_t *d, zplm_quat_t a, zplm_quat_t b, float t) { zplm_quat_lerp(d, a, b, t); zplm_quat_norm(d, *d); }

void zplm_quat_slerp(zplm_quat_t *d, zplm_quat_t a, zplm_quat_t b, float t) {
    zplm_quat_t x, y, z;
    float cos_theta, angle;
    float s1, s0, is;

    z = b;
    cos_theta = zplm_quat_dot(a, b);

    if (cos_theta < 0.0f) {
        z = zplm_quat(-b.x, -b.y, -b.z, -b.w);
        cos_theta = -cos_theta;
    }

    if (cos_theta > 1.0f) {
        /* NOTE: Use lerp not nlerp as it's not a real angle or they are not normalized */
        zplm_quat_lerp(d, a, b, t);
    }

    angle = zplm_arccos(cos_theta);

    s1 = zplm_sin(1.0f - t*angle);
    s0 = zplm_sin(t*angle);
    is = 1.0f/zplm_sin(angle);
    zplm_quat_mulf(&x, z, s1);
    zplm_quat_mulf(&y, z, s0);
    zplm_quat_add(d, x, y);
    zplm_quat_muleqf(d, is);
}

void zplm_quat_slerp_approx(zplm_quat_t *d, zplm_quat_t a, zplm_quat_t b, float t) {
    /* NOTE: Derived by taylor expanding the geometric interpolation equation
     *             Even works okay for nearly anti-parallel versors!!!
     */
    /* NOTE: Extra interations cannot be used as they require angle^4 which is not worth it to approximate */
    float tp = t + (1.0f - zplm_quat_dot(a, b))/3.0f * t*(-2.0f*t*t + 3.0f*t - 1.0f);
    zplm_quat_nlerp(d, a, b, tp);
}

void zplm_quat_nquad(zplm_quat_t *d, zplm_quat_t p, zplm_quat_t a, zplm_quat_t b, zplm_quat_t q, float t) {
    zplm_quat_t x, y;
    zplm_quat_nlerp(&x, p, q, t);
    zplm_quat_nlerp(&y, a, b, t);
    zplm_quat_nlerp(d, x, y, 2.0f*t*(1.0f-t));
}

void zplm_quat_squad(zplm_quat_t *d, zplm_quat_t p, zplm_quat_t a, zplm_quat_t b, zplm_quat_t q, float t) {
    zplm_quat_t x, y;
    zplm_quat_slerp(&x, p, q, t);
    zplm_quat_slerp(&y, a, b, t);
    zplm_quat_slerp(d, x, y, 2.0f*t*(1.0f-t));
}

void zplm_quat_squad_approx(zplm_quat_t *d, zplm_quat_t p, zplm_quat_t a, zplm_quat_t b, zplm_quat_t q, float t) {
    zplm_quat_t x, y;
    zplm_quat_slerp_approx(&x, p, q, t);
    zplm_quat_slerp_approx(&y, a, b, t);
    zplm_quat_slerp_approx(d, x, y, 2.0f*t*(1.0f-t));
}






zplm_rect2_t zplm_rect2(zplm_vec2_t pos, zplm_vec2_t dim) {
    zplm_rect2_t r;
    r.pos = pos;
    r.dim = dim;
    return r;
}

zplm_rect3_t zplm_rect3(zplm_vec3_t pos, zplm_vec3_t dim) {
    zplm_rect3_t r;
    r.pos = pos;
    r.dim = dim;
    return r;
}

int zplm_rect2_contains(zplm_rect2_t a, float x, float y) {
    float min_x = zplm_min(a.pos.x, a.pos.x+a.dim.x);
    float max_x = zplm_max(a.pos.x, a.pos.x+a.dim.x);
    float min_y = zplm_min(a.pos.y, a.pos.y+a.dim.y);
    float max_y = zplm_max(a.pos.y, a.pos.y+a.dim.y);
    int result = (x >= min_x) & (x < max_x) & (y >= min_y) & (y < max_y);
    return result;
}

int zplm_rect2_contains_vec2(zplm_rect2_t a, zplm_vec2_t p) { return zplm_rect2_contains(a, p.x, p.y); }

int zplm_rect2_intersects(zplm_rect2_t a, zplm_rect2_t b) {
    zplm_rect2_t r = {0};
    return zplm_rect2_intersection_result(a, b, &r);
}

int zplm_rect2_intersection_result(zplm_rect2_t a, zplm_rect2_t b, zplm_rect2_t *intersection) {
    float a_min_x = zplm_min(a.pos.x, a.pos.x+a.dim.x);
    float a_max_x = zplm_max(a.pos.x, a.pos.x+a.dim.x);
    float a_min_y = zplm_min(a.pos.y, a.pos.y+a.dim.y);
    float a_max_y = zplm_max(a.pos.y, a.pos.y+a.dim.y);

    float b_min_x = zplm_min(b.pos.x, b.pos.x+b.dim.x);
    float b_max_x = zplm_max(b.pos.x, b.pos.x+b.dim.x);
    float b_min_y = zplm_min(b.pos.y, b.pos.y+b.dim.y);
    float b_max_y = zplm_max(b.pos.y, b.pos.y+b.dim.y);

    float x0 = zplm_max(a_min_x, b_min_x);
    float y0 = zplm_max(a_min_y, b_min_y);
    float x1 = zplm_min(a_max_x, b_max_x);
    float y1 = zplm_min(a_max_y, b_max_y);

    if ((x0 < x1) && (y0 < y1)) {
        zplm_rect2_t r = zplm_rect2(zplm_vec2(x0, y0), zplm_vec2(x1-x0, y1-y0));
        *intersection = r;
        return 1;
    } else {
        zplm_rect2_t r = {0};
        *intersection = r;
        return 0;
    }
}


#if defined(_WIN64) || defined(__x86_64__) || defined(__ppc64__)
zplm_u64 zplm_hash_murmur64(void const *key, size_t num_bytes, zplm_u64 seed) {
    zplm_u64 const m = 0xc6a4a7935bd1e995ULL;
    zplm_u64 const r = 47;

    zplm_u64 h = seed ^ (num_bytes * m);

    zplm_u64 *data = (zplm_u64 *)(key);
    zplm_u64 *end = data + (num_bytes / 8);
    unsigned char *data2;

    while (data != end) {
        zplm_u64 k = *data++;
        k *= m;
        k ^= k >> r;
        k *= m;
        h ^= k;
        h *= m;
    }

    data2 = (unsigned char *)data;

    switch (num_bytes & 7) {
        case 7: h ^= (zplm_u64)data2[6] << 48;
        case 6: h ^= (zplm_u64)data2[5] << 40;
        case 5: h ^= (zplm_u64)data2[4] << 32;
        case 4: h ^= (zplm_u64)data2[3] << 24;
        case 3: h ^= (zplm_u64)data2[2] << 16;
        case 2: h ^= (zplm_u64)data2[1] << 8;
        case 1: h ^= (zplm_u64)data2[0];
        h *= m;
    };

    h ^= h >> r;
    h *= m;
    h ^= h >> r;

    return h;
}
#else
zplm_u64 zplm_hash_murmur64(void const *key, size_t num_bytes, zplm_u64 seed) {
    zplm_u32 const m = 0x5bd1e995;
    zplm_u32 const r = 24;

    zplm_u64 h  = 0;
    zplm_u32 h1 = (zplm_u32)seed ^ (zplm_u32)num_bytes;
    zplm_u32 h2 = (zplm_u32)((zplm_u64)seed >> 32);

    zplm_u32 *data = (zplm_u32 *)key;


    while (num_bytes >= 8) {
        zplm_u32 k1, k2;
        k1 = *data++;
        k1 *= m;
        k1 ^= k1 >> r;
        k1 *= m;
        h1 *= m;
        h1 ^= k1;
        num_bytes -= 4;

        k2 = *data++;
        k2 *= m;
        k2 ^= k2 >> r;
        k2 *= m;
        h2 *= m;
        h2 ^= k2;
        num_bytes -= 4;
    }

    if (num_bytes >= 4) {
        zplm_u32 k1 = *data++;
        k1 *= m;
        k1 ^= k1 >> r;
        k1 *= m;
        h1 *= m;
        h1 ^= k1;
        num_bytes -= 4;
    }

    switch (num_bytes) {
        zplm_u32 a, b, c;
        case 3: c = data[2]; h2 ^= c << 16;
        case 2: b = data[1]; h2 ^= b <<  8;
        case 1: a = data[0]; h2 ^= a <<  0;
        h2 *= m;
    };

    h1 ^= h2 >> 18;
    h1 *= m;
    h2 ^= h1 >> 22;
    h2 *= m;
    h1 ^= h2 >> 17;
    h1 *= m;
    h2 ^= h1 >> 19;
    h2 *= m;

    h = (zplm_u64)(h << 32) | (zplm_u64)h2;

    return h;
}
#endif


/* TODO: Make better random number generators */
float zplm_random_range_float(float min_inc, float max_inc) {
    int int_result = zplm_random_range_int(0, 2147483646); /* Prevent integer overflow */
    float result = int_result/(float)2147483646;
    result *= max_inc - min_inc;
    result += min_inc;
    return result;
}

int zplm_random_range_int(int min_inc, int max_inc) {
    static unsigned int random_value = 0xdeadbeef; /* Random Value */
    unsigned int diff, result;
    random_value = random_value * 2147001325 + 715136305; /* BCPL generator */
    diff = max_inc - min_inc + 1;
    result = random_value % diff;
    result += min_inc;

    return result;
}

float zplm_random01(void) {
    return zplm_random_range_float(0.0f, 1.0f);
}

#if defined(__GCC__) || defined(__GNUC__)
#pragma GCC diagnostic pop
#elif defined(__clang__)
#pragma clang diagnostic pop
#endif



#endif /* ZPLM_IMPLEMENTATION */
