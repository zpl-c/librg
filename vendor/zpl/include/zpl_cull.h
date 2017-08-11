/*

ZPL - Tree culler module

License:
    This software is dual-licensed to the public domain and under the following
    license: you are granted a perpetual, irrevocable license to copy, modify,
    publish, and distribute this file as you see fit.

Warranty:
    No warranty is implied, use at your own risk.

Usage:
    #define ZPLC_IMPLEMENTATION exactly in ONE source file right BEFORE including the library, like:

    #define ZPLC_IMPLEMENTATION
    #include"zpl_cull.h"

Dependencies:
    zpl.h

    Make sure you properly include them!

Credits:
    Dominik Madarasz (GitHub: zaklaus)

Version History:
    1.0.1 - Fixes.
    1.0.0 - Initial version

*/

///////////////////////////////////////////////////////////
//
// NOTE(ZaKlaus): If you use memory arena, it gets reset
// when clear() is called, that means,
// if your tree shares memory with other systems
// in the arena, the data might get corrupted!
//
// Due to the nature of how linear allocators work
// you can't free single resource of the arena and
// the only way to clear it is to completely,
// reset it. Please, consider using either
// a different allocator supporting _free_ operation
// or make sure the (sub)arena's memory is
// exclusive to the tree only.
//

#ifndef ZPL_INCLUDE_ZPL_CULL_H
#define ZPL_INCLUDE_ZPL_CULL_H

#ifndef ZPL_IMPLEMENTATION
#error You must include zpl.h !!!
#endif

#if defined(__cplusplus)
extern "C" {
#endif

    //
    // Spatial bounding box
    //

    typedef struct zplc_bounds_t {
        f32 E[3];
        f32 half[3];
    } zplc_bounds_t;

    //
    // Tree culler structure
    //

    typedef enum zplc_dim_e {
        zplc_dim_2d_ev = 2,
        zplc_dim_3d_ev = 3,
    } zplc_dim_e;

    typedef struct zplc_node_t {
        f32               E[3];
        zplc_bounds_t bounds;

        // NOTE(ZaKlaus): Custom data to be set by user.
        void *ptr;
        u64   tag;
    } zplc_node_t;

    typedef struct zplc_t {
        zpl_allocator_t                allocator;

        u32                            max_nodes;
        isize                          dimensions;
        zplc_bounds_t              boundary;
        zpl_array_t(zplc_node_t)   nodes;
        zpl_array_t(struct zplc_t) trees;
    } zplc_t;

    ZPL_DEF void zplc_init  (zplc_t *c, zpl_allocator_t a, isize dims, zplc_bounds_t bounds, u32 max_nodes);
    ZPL_DEF void zplc_query (zplc_t *c, zplc_bounds_t bounds, zpl_array_t(zplc_node_t) out_nodes);
    ZPL_DEF b32  zplc_insert(zplc_t *c, zplc_node_t node);
    ZPL_DEF b32  zplc_remove(zplc_t *c, zplc_node_t node);
    ZPL_DEF void zplc_split (zplc_t *c);
    ZPL_DEF void zplc_clear (zplc_t *c);
    ZPL_DEF void zplc_free  (zplc_t *c);


#if defined(__cplusplus)
}
#endif

#if defined(ZPLC_IMPLEMENTATION) && !defined(ZPLC_IMPLEMENTATION_DONE)
#define ZPLC_IMPLEMENTATION_DONE

#if defined(__cplusplus)
extern "C" {
#endif

    void zplc_init(zplc_t *c, zpl_allocator_t a, isize dims, zplc_bounds_t bounds, u32 max_nodes) {
        zplc_t c_ = {0};
        *c = c_;
        c->allocator = a;
        c->boundary  = bounds;
        c->max_nodes = max_nodes;
        c->dimensions= dims;
    }

    b32 zplc__contains(isize dims, zplc_bounds_t a, f32 *point) {
        for (i32 i = 0; i < dims; ++i) {
            if (!( a.E[i] - a.half[i] <= point[i]
                && a.E[i] + a.half[i] >= point[i])) {
                return false;
            }
        }

        return true;
    }

    b32 zplc__intersects(isize dims, zplc_bounds_t a, zplc_bounds_t b) {
        for (i32 i = 0; i < dims; ++i) {
            if (zpl_abs(a.E[i] - b.E[i]) > (a.half[i] + b.half[i])) return false;
        }

        return true;
    }

    void zplc_query(zplc_t *c, zplc_bounds_t bounds, zpl_array_t(zplc_node_t) out_nodes) {
        if (c->nodes == NULL) return;
        if (!zplc__intersects(c->dimensions, c->boundary, bounds)) return;

        isize nodes_count = zpl_array_count(c->nodes);
        for (i32 i = 0; i < nodes_count; ++i) {
            b32 inside = zplc__contains(c->dimensions, bounds, c->nodes[i].E);

            if (inside) {
                zpl_array_append(out_nodes, c->nodes[i]);
            }

        }
        if(c->trees == NULL) return;

        isize trees_count = zpl_array_count(c->trees);
        if (trees_count == 0) return;

        for (i32 i = 0; i < trees_count; ++i) {
            zplc_query((c->trees+i), bounds, out_nodes);
        }
    }

    b32 zplc_insert(zplc_t *c, zplc_node_t node) {
        if(!zplc__contains(c->dimensions, c->boundary, node.E)) return false;

        if (c->nodes == NULL) {
            zpl_array_init(c->nodes, c->allocator);
        }

        if(zpl_array_count(c->nodes) < c->max_nodes) {
            zpl_array_append(c->nodes, node);
            return true;
        }

        if (c->trees == NULL) {
            zpl_array_init(c->trees, c->allocator);
        }

        isize trees_count = zpl_array_count(c->trees);
        if (trees_count == 0) {
            zplc_split(c);
        }

        for (i32 i = 0; i < trees_count; ++i) {
            if (zplc_insert((c->trees+i), node)) return true;
        }

        return false;
    }

    zpl_global f32 zplc__tpl[][3] = {
        {-1, -1, -1},
        {+1, -1, -1},
        {-1, +1, -1},
        {+1, +1, -1},
        {-1, -1, +1},
        {+1, -1, +1},
        {-1, +1, +1},
        {+1, +1, +1},
    };

    void zplc_split(zplc_t *c) {
        zplc_bounds_t hd = c->boundary;
        for (i32 i = 0; i < c->dimensions; ++i) {
            hd.half[i] /= 2;
        }

        i32 loops = 0;
        /**/ if (c->dimensions == zplc_dim_2d_ev) {
            loops = 4;
        }
        else if (c->dimensions == zplc_dim_3d_ev) {
            loops = 8;
        }

        f32 p[3] = {0};
        for (i32 i = 0; i < loops; ++i) {
            zplc_t tree = {0};
            zplc_bounds_t bounds = {0};
            p[0] = c->boundary.E[0] + hd.half[0] + (hd.half[0]/2.0)*zplc__tpl[i][0];
            p[1] = c->boundary.E[1] + hd.half[1] + (hd.half[1]/2.0)*zplc__tpl[i][1];
            p[2] = c->boundary.E[2] + hd.half[2] + (hd.half[2]/2.0)*zplc__tpl[i][2];

            zpl_memcopy(bounds.E, p, 3*zpl_size_of(f32));
            zpl_memcopy(bounds.half, hd.half, 3*zpl_size_of(f32));

            tree.boundary   = bounds;
            tree.max_nodes  = c->max_nodes;
            tree.dimensions = c->dimensions;
            tree.allocator  = c->allocator;

            zpl_array_append(c->trees, tree);
        }
    }

    void zplc_clear(zplc_t *c) {
        // TODO(ZaKlaus): Support more allocators?
        if (c->allocator.proc == zpl_arena_allocator_proc) {
            zpl_free_all(c->allocator);
            c->nodes = NULL;
            c->trees = NULL;
            return;
        }
        zpl_array_free(c->nodes);
        c->nodes = NULL;

        isize trees_count = zpl_array_count(c->trees);
        for (i32 i = 0; i < trees_count; ++i) {
            zplc_clear((c->trees+i));
        }

        zpl_array_free(c->trees);
        c->trees = NULL;
    }

#if defined(__cplusplus)
}
#endif

#endif // ZPLC_IMPLEMENTATION

#endif // ZPL_INCLUDE_ZPL_CULL_H
