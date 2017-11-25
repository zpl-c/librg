/**
 * WANRNING: its just a prototype
 * DON'T USE IT IN YOUR PROJECTS
 */

#ifndef LIBRG_COMPONENTS_INCLUDE_H
#define LIBRG_COMPONENTS_INCLUDE_H


#ifdef __cplusplus
extern "C" {
#endif


    /**
     * Entity filter
     * Used for ruinning complex iterations on entity pool
     *
     * Can be used to retrieve only entites containing all
     * of the provided components (logical AND operation).
     * And entities that exclude provided components (logical NOT).
     *
     * Supports up to 8 components for "contains" operation.
     * And up to 4 components for "excludes" operation.
     *
     * First undefined component index in the list will skip all other
     * components for that operation:
     *     { .contains1 = librg_index_c1(), .contains3 = librg_index_c3() }
     *     In this case librg_index_c3() WILL NOT be added to condition.
     *
     * If you want to enable this behavior: make sure
     * you define LIBRG_ENTITY_UNOPTIMIZED_CYCLES before including the librg.h
     *
     *
     * EXAMPLES:
     *     librg_filter_t filter = { librg_index_c1(), librg_index_c2() };
     * OR
     *     librg_filter_t filter = { librg_index_c1(), .excludes1 = librg_index_c2() };
     */
    typedef union librg_filter_t {
        struct {
            u32 contains1; u32 contains2; u32 contains3; u32 contains4;
            u32 contains5; u32 contains6; u32 contains7; u32 contains8;
            u32 excludes1; u32 excludes2; u32 excludes3; u32 excludes4;
        };

        struct {
            u32 contains[8];
            u32 excludes[4];
        };
    } librg_filter_t;

    /**
     * Storage containers
     * for inner librg stuff
     */
    typedef struct {
        usize offset;
        usize size;
        zpl_buffer_t(b32) used;
    } librg_component_meta;




    /**
     * Default components
     */

    typedef struct { u32 type; librg_table_t ignored; } librg_meta_t;
    typedef struct { zplm_vec3_t position; } librg_transform_t;
    typedef struct { u32 range; zpl_array_t(librg_entity_t) last_query; zplc_t *branch; } librg_stream_t;
    typedef struct { librg_peer_t *peer; } librg_control_t;
    typedef struct { librg_peer_t *peer; librg_table_t last_snapshot; } librg_client_t;

    enum {
        librg_dummmy,
        librg_meta,
        librg_transform,
        librg_stream,
        librg_control,
        librg_client,
        librg_component_last,
    };


        struct {
            librg_void *data;
            usize size;
            usize count;
            zpl_buffer_t(librg_component_meta) headers;
        } components;



    /**
     * Try to register provided component
     */
    LIBRG_API void librg_component_register(librg_ctx_t *ctx, u32 index, usize component_size);

    /**
     * Try to attach provided component to/from a particular entity
     */
    LIBRG_API librg_void *librg_component_attach(librg_ctx_t *ctx, u32 index, librg_entity_t entity, librg_void *data);

    /**
     * Try to fetch provided component to/from a particular entity
     */
    LIBRG_API librg_void *librg_component_fetch(librg_ctx_t *ctx, u32 index, librg_entity_t entity);

    /**
     * Try to detach provided component to/from a particular entity
     */
    LIBRG_API void librg_component_detach(librg_ctx_t *ctx, u32 index, librg_entity_t entity);

    /**
     * Try to interate on each entity with particular component
     */
    LIBRG_API void librg_component_each(librg_ctx_t *ctx, u32 index, librg_entity_cb callback);

    /**
     * Try to interate on each entity with provided component filter
     */
    LIBRG_API void librg_entity_each(librg_ctx_t *ctx, librg_filter_t filter, librg_entity_cb callback);

    /**
     * Try to interate on each entity with particular component
     * (macro version, can be used in C/C++ based projects)
     */
    #define librg_component_eachx(ctx, index, name, code) do {  \
        librg_assert(ctx); librg_assert(index < ctx->components.count); \
        librg_component_meta *header = &ctx->components.headers[index]; librg_assert(header);   \
        for (isize i = 0; i < zpl_buffer_count(header->used); i++)  \
            if (header->used[i]) { librg_entity_t name = i; code; } \
    } while(0);

    /**
     * Try to interate on each entity with provided component filter
     * (macro version, can be used in C/C++ based projects)
     */
    #define librg_entity_eachx(ctx, filter, name, code) do {                                                        \
        librg_assert(ctx); if (filter.contains1 == 0) break;                                                        \
        for (usize _ent = 0, valid = 0; valid < (ctx->entity.native.count + ctx->entity.shared.count)               \
            && _ent < (librg_is_server(ctx) ? ctx->max_entities : ctx->max_entities * 2); _ent++) {                 \
            /* check if entity valid */                                                                             \
            if (!ctx->components.headers[librg_meta].used[_ent]) continue; else valid++;                            \
            b32 _used = true;                                                                                       \
            /* check for included components */                                                                     \
            for (isize k = 0; k < 8 && _used; k++) {                                                                \
                if (filter.contains[k] == 0) break;                                                                 \
                librg_component_meta *header = &ctx->components.headers[filter.contains[k]]; librg_assert(header);  \
                if (!header->used[_ent]) { _used = false; }                                                         \
            }                                                                                                       \
            /* check for excluded components */                                                                     \
            for (isize k = 0; k < 4 && _used; k++) {                                                                \
                if (filter.excludes[k] == 0) break;                                                                 \
                librg_component_meta *header = &ctx->components.headers[filter.excludes[k]]; librg_assert(header);  \
                if (header->used[_ent]) { _used = false; }                                                          \
            }                                                                                                       \
            /* execute code */                                                                                      \
            if (_used) { librg_entity_t name = _ent; code; }                                                        \
        }                                                                                                           \
    } while(0)



    static librg_inline librg_client_t *librg_fetch_client(librg_ctx_t *ctx, librg_entity_t entity) {
        return (librg_client_t *)librg_component_fetch(ctx, librg_client, entity);
    }

    static librg_inline librg_transform_t *librg_fetch_transform(librg_ctx_t *ctx, librg_entity_t entity) {
        return (librg_transform_t *)librg_component_fetch(ctx, librg_transform, entity);
    }

    static librg_inline librg_meta_t *librg_fetch_meta(librg_ctx_t *ctx, librg_entity_t entity) {
        return (librg_meta_t *)librg_component_fetch(ctx, librg_meta, entity);
    }

    static librg_inline librg_stream_t *librg_fetch_stream(librg_ctx_t *ctx, librg_entity_t entity) {
        return (librg_stream_t *)librg_component_fetch(ctx, librg_stream, entity);
    }

    #define librg_component(NAME, INDEX, COMP) \
        static librg_inline COMP *ZPL_JOIN2(librg_attach_,NAME) (librg_ctx_t *ctx, librg_entity_t entity, COMP *component) { return (COMP *)librg_component_attach(ctx, INDEX, entity, (char *)component); } \
        static librg_inline COMP *ZPL_JOIN2(librg_fetch_ ,NAME) (librg_ctx_t *ctx, librg_entity_t entity) { return (COMP *)librg_component_fetch(ctx, INDEX, entity); } \
        static librg_inline void  ZPL_JOIN2(librg_detach_,NAME) (librg_ctx_t *ctx, librg_entity_t entity) { librg_component_detach(ctx, INDEX, entity); }


#ifdef __cplusplus
}
#endif

#if defined(LIBRG_COMPONENTS_IMPLEMENTATION) && !defined(LIBRG_COMPONENTS_IMPLEMENTATION_DONE)
#define LIBRG_COMPONENTS_IMPLEMENTATION_DONE

#ifdef __cplusplus
extern "C" {
#endif


    void librg_component_register(librg_ctx_t *ctx, u32 index, usize component_size) {
        librg_assert(ctx); librg_assert(component_size);
        librg_assert_msg(ctx->components.count == index, "you should register components in order");

        librg_component_meta *header = &ctx->components.headers[index]; librg_assert(header);
        usize size = component_size * ctx->max_entities;

        header->offset = ctx->components.size;
        header->size   = component_size;

        ctx->components.size += size;
        ctx->components.count++;

        zpl_buffer_init(header->used, ctx->allocator, ctx->max_entities);
    }

    librg_void *librg_component_attach(librg_ctx_t *ctx, u32 index, librg_entity_t entity, librg_void *data) {
        librg_component_meta *header = &ctx->components.headers[index];
        librg_assert_msg(header && header->size, "make sure you registered component you are trying to use");
        header->used[entity] = true;
        librg_void *cdata = ctx->components.data + header->offset;
        if (data == NULL) zpl_memset(&cdata[entity * header->size], 0, (usize)header->size);
        else zpl_memcopy(&cdata[entity * header->size], data, (usize)header->size);
        return &cdata[entity * header->size];
    }

    librg_inline librg_void *librg_component_fetch(librg_ctx_t *ctx, u32 index, librg_entity_t entity) {
        librg_component_meta *header = &ctx->components.headers[index]; librg_assert(header);
        librg_void *cdata = ctx->components.data + header->offset;
        return header->used[entity] ? &cdata[entity * header->size] : NULL;
    }

    librg_inline void librg_component_detach(librg_ctx_t *ctx, u32 index, librg_entity_t entity) {
        librg_component_meta *header = &ctx->components.headers[index]; librg_assert(header);
        header->used[entity] = false;
    }

    librg_inline void librg_component_each(librg_ctx_t *ctx, u32 index, librg_entity_cb callback) {
        librg_component_eachx(ctx, index, entity, { callback(ctx, entity); });
    }

    void librg_entity_each(librg_ctx_t *ctx, librg_filter_t filter, librg_entity_cb callback) {
        librg_entity_eachx(ctx, filter, entity, { callback(ctx, entity); });
    }

#ifdef __cplusplus
}
#endif

#endif // LIBRG_COMPONENTS_IMPLEMENTATION
#endif // LIBRG_COMPONENTS_INCLUDE_H
