/*

ZPL - JSON parser module

License:
    This software is dual-licensed to the public domain and under the following
    license: you are granted a perpetual, irrevocable license to copy, modify,
    publish, and distribute this file as you see fit.

Warranty:
    No warranty is implied, use at your own risk.

Usage:
    #define ZPLJ_IMPLEMJSONATION exactly in ONE source file right BEFORE including the library, like:

    #define ZPLJ_IMPLEMJSONATION
    #include"zpl_json.h"

Credits:
    Dominik Madarasz (GitHub: zaklaus)

Version History:
    1.0.0 - Initial version

*/

#ifndef ZPL_INCLUDE_ZPL_JSON_H
#define ZPL_INCLUDE_ZPL_JSON_H

#ifdef __cplusplus
extern "C" {
#endif

    typedef enum zplj_type_e {
        zplj_type_object_ev,
        zplj_type_string_ev,
        zplj_type_array_ev,
        zplj_type_integer_ev,
        zplj_type_real_ev,
        zplj_type_constant_ev
    } zplj_type_e;

    typedef enum zplj_constant_e {
        zplj_constant_null_ev,
        zplj_constant_false_ev,
        zplj_constant_true_ev,
    } zplj_constant_e;

    typedef struct zplj_object_t {
        zpl_allocator_t backing;
        char *name;
        u8    type;
        zpl_array_t(struct zplj_object_t) nodes;

        union {
            char *string;
            zpl_array_t(struct zplj_object_t) elements;
            i64   integer;
            f64   real;
            u8    constant;
        };
    } zplj_object_t;

    ZPL_DEF void zplj_parse(zplj_object_t *root, usize len, char *const source, zpl_allocator_t a, b32 strip_comments);
    ZPL_DEF void zplj_free (zplj_object_t *obj);

    ZPL_DEF char *zplj__parse_object(zplj_object_t *obj, char *base, zpl_allocator_t a);
    ZPL_DEF char *zplj__parse_value (zplj_object_t *obj, char *base, zpl_allocator_t a);
    ZPL_DEF char *zplj__parse_array (zplj_object_t *obj, char *base, zpl_allocator_t a);

    ZPL_DEF char *zplj__trim        (char *str);
    ZPL_DEF char *zplj__skip        (char *str, char c);
    ZPL_DEF b32 zplj__validate_name (char *str, char *err);

#ifdef __cplusplus
}
#endif

#if defined(ZPLJ_IMPLEMENTATION) && !defined(ZPLJ_IMPLEMENTATION_DONE)
#define ZPLJ_IMPLEMENTATION_DONE

#ifdef __cplusplus
extern "C" {
#endif

    void zplj_parse(zplj_object_t *root, usize len, char *const source, zpl_allocator_t a, b32 strip_comments) {
        ZPL_ASSERT(root && source);

        char *dest = source;

        if (strip_comments) {
            b32 is_lit = false;
            char *p = dest;
            char *b = dest;
            isize l = 0;

            while (*p) {
                if (*p == '"') {
                    is_lit = !is_lit;
                    ++p;
                    continue;
                }

                // TODO(ZaKlaus): @fixme Better literal detection
                if (!is_lit) {
                    // NOTE(ZaKlaus): block comment
                    if (p[0] == '/' && p[1] == '*') {
                        b = p;
                        l=2;
                        p+=2;

                        while (p[0] != '*' && p[1] != '/') {
                            ++p; ++l;
                        }
                        p+=2;
                        l+=2;
                        zpl_memset(b, ' ', l);
                    }

                    // NOTE(ZaKlaus): inline comment
                    if (p[0] == '/' && p[0] == '/') {
                        b = p;
                        l=2;
                        p+=2;

                        while (p[0] != '\n') {
                            ++p; ++l;
                        }
                        ++p;
                        ++l;
                        zpl_memset(b, ' ', l);
                    }
                }

                ++p;
            }
        }

        zplj_object_t root_ = {0};
        zplj__parse_object(&root_, dest, a);

        *root = root_;
    }

    void zplj_free(zplj_object_t *obj) {
        /**/ if (obj->type == zplj_type_array_ev) {
            for (isize i = 0; i < zpl_array_count(obj->elements); ++i) {
                zplj_free(obj->elements+i);
            }

            zpl_array_free(obj->elements);
        }
        else if (obj->type == zplj_type_object_ev) {
            for (isize i = 0; i < zpl_array_count(obj->nodes); ++i) {
                zplj_free(obj->nodes+i);
            }

            zpl_array_free(obj->nodes);
        }
    }

    char *zplj__parse_array(zplj_object_t *obj, char *base, zpl_allocator_t a) {
        ZPL_ASSERT(obj && base);
        char *p = base;

        obj->type = zplj_type_array_ev;
        zpl_array_init(obj->elements, a);
        obj->backing = a;

        while(*p) {
            p = zplj__trim(p);

            zplj_object_t elem = {0};
            p = zplj__parse_value(&elem, p, a);
            zpl_array_append(obj->elements, elem);

            p = zplj__trim(p);

            if (*p == ',') {
                ++p;
                continue;
            }
            else {
                return p;
            }
        }
        return p;
    }

    char *zplj__parse_value(zplj_object_t *obj, char *base, zpl_allocator_t a) {
        ZPL_ASSERT(obj && base);
        char *p = base;
        char *b = base;
        char *e = base;

        /**/ if (*p == '"') {
            obj->type = zplj_type_string_ev;
            b = p+1;
            obj->string = b;

            e = zplj__skip(b, '"');
            *e = '\0';
            p = e+1;
        }
        else if (zpl_char_is_alpha(*p)) {
            obj->type = zplj_type_constant_ev;

            /**/ if (!zpl_strncmp(p, "true", 4)) {
                obj->constant = zplj_constant_true_ev;
                p += 4;
            }
            else if (!zpl_strncmp(p, "false", 5)) {
                obj->constant = zplj_constant_false_ev;
                p += 5;
            }
            else if (!zpl_strncmp(p, "null", 4)) {
                obj->constant = zplj_constant_null_ev;
                p += 4;
            }
            else {
                ZPL_ASSERT_MSG(false, "Failed to parse it!\n", p);
            }
        }
        else if (zpl_char_is_digit(*p) ||
                 *p == '+' || *p == '-') {
            obj->type = zplj_type_integer_ev;

            b = p;
            e = b;

            isize ib = 0;
            char buf[16] = {0};

            /**/ if (*e == '+') ++e;
            else if (*e == '-') {
                buf[ib++] = *e++;
            }

            while(zpl_char_is_hex_digit(*e) || *e == 'x' || *e == 'X') {
                buf[ib++] = *e++;
            }

            if (*e == '.') {
                obj->type = zplj_type_real_ev;

                do {
                    buf[ib++] = *e;
                }
                while(zpl_char_is_digit(*++e));
            }

            i64 exp = 0; f32 eb = 10;
            char expbuf[6] = {0};
            isize expi = 0;

            if (*e == 'e' || *e == 'E') {
                ++e;
                if (*e == '+' || *e == '-' || zpl_char_is_digit(*e)) {
                    if (*e == '-') {
                        eb = 0.1;
                    }

                    if (!zpl_char_is_digit(*e)) {
                        ++e;
                    }

                    while(zpl_char_is_digit(*e)) {
                        expbuf[expi++] = *e++;
                    }

                }

                exp = zpl_str_to_i64(expbuf, NULL, 10);
            }

            ZPL_ASSERT(*e);

            // NOTE(ZaKlaus): @enhance
            if (obj->type == zplj_type_integer_ev) {
                obj->integer = zpl_str_to_i64(buf, 0, 0);

                while(--exp > 0) {
                    obj->integer *= eb;
                }
            }
            else {
                obj->real = zpl_str_to_f64(buf, 0);

                while(--exp > 0) {
                    obj->real *= eb;
                }
            }
            p = e;
        }
        else if (*p == '[') {
            p = zplj__trim(p+1);
            if (*p == ']') return p;
            p = zplj__parse_array(obj, p, a);
            ++p;
        }
        else if (*p == '{') {
            p = zplj__trim(p+1);
            p = zplj__parse_object(obj, p, a);
            ++p;
        }

        return p;
    }

    char *zplj__parse_object(zplj_object_t *obj, char *base, zpl_allocator_t a) {
        ZPL_ASSERT(obj && base);
        char *p = base;
        char *b = base;
        char *e = base;

        zpl_array_init(obj->nodes, a);
        obj->backing = a;

        p = zplj__trim(p);
        if (*p == '{') p++;

        while(*p) {
            zplj_object_t node = {0};
            p = zplj__trim(p);
            if (*p == '}') return p;
            ZPL_ASSERT(*p == '"');
            b = ++p;

            node.name = b;

            e = zplj__skip(b, '"');
            *e = '\0';

            char errc = 0;
            ZPL_ASSERT_MSG(zplj__validate_name(b, &errc), "Failed to validate name '%s'!\n Invalid char: '%c'.\n", b, errc);

            p = ++e;

            p = zplj__trim(p);

            ZPL_ASSERT(*p && *p == ':');

            p = zplj__trim(p+1);
            p = zplj__parse_value(&node, p, a);

            zpl_array_append(obj->nodes, node);

            p = zplj__trim(p);

            /**/ if (*p == ',') {
                ++p;
                continue;
            }
            else if (*p == '\0' || *p == '}') {
                return p;
            }
            else {
                ZPL_ASSERT_MSG(false, "Failed to parse data!\n", p);
            }
        }
        return p;
    }

    zpl_inline char *zplj__trim(char *str) {
        while (*str && zpl_char_is_space(*str)) {
            ++str;
        }

        return str;
    }

    zpl_inline b32 zplj__is_control_char(char c) {
        return (c == '"' || c == '\\' || c == '/' || c == 'b' ||
                c == 'f' || c == 'n'  || c == 'r' || c == 't');
    }

    zpl_inline b32 zplj__is_special_char(char c) {
        return (c == '<' || c == '>' || c == ':' || c == '/');
    }

#define jx(x) !zpl_char_is_hex_digit(str[x])
    zpl_inline b32 zplj__validate_name(char *str, char *err) {
        while(*str) {
            if ((str[0] == '\\' && !zplj__is_control_char(str[1])) &&
                (str[0] == '\\' && jx(1) && jx(2) && jx(3) && jx(4))) {
                *err = *str;
                return false;
            }

            ++str;
        }

        return true;
    }
#undef jx

    zpl_inline char *zplj__skip(char *str, char c) {
        while ((*str && *str != c) || (*(str-1) == '\\' && *str == c && zplj__is_control_char(c))) {
            ++str;
        }

        return str;
    }


#ifdef __cplusplus
}
#endif

#endif // ZPLJ_IMPLEMENTATION

#endif // ZPL_INCLUDE_ZPL_JSON_H
