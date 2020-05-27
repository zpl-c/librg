#ifndef LIBRG_H
#define LIBRG_H

#define LIBRG_VERSION_MAJOR 6
#define LIBRG_VERSION_MINOR 0
#define LIBRG_VERSION_PATCH 0
#define LIBRG_VERSION_PRE ""

#include "librg_hedley.h"

#define LIBRG_VERSION LIBRG_VERSION_ENCODE(LIBRG_VERSION_MAJOR, LIBRG_VERSION_MINOR, LIBRG_VERSION_PATCH)

#ifdef LIBRG_IMPL
    #ifndef LIBRG_IMPLEMENTATION
    #define LIBRG_IMPLEMENTATION
    #endif
#endif

#if defined(__cplusplus) && !defined(LIBRG_EXTERN)
    #define LIBRG_EXTERN extern "C"
#else
    #define LIBRG_EXTERN extern
#endif

#ifndef LIBRG_API
    #if defined(LIBRG_SHARED_LIB)
        #ifdef LIBRG_IMPLEMENTATION
            #define LIBRG_API LIBRG_PUBLIC
        #else
            #define LIBRG_API LIBRG_IMPORT
        #endif
    #elif defined(LIBRG_STATIC_LIB)
        #ifdef LIBRG_IMPLEMENTATION
            #define LIBRG_API
        #else
            #define LIBRG_API LIBRG_EXTERN
        #endif
    #elif defined(LIBRG_STATIC)
        #define LIBRG_API static
    #else
        #define LIBRG_API LIBRG_EXTERN
    #endif
#endif

#include <stdint.h>
#include <stddef.h>

#include "header/types.h"
#include "header/context.h"
#include "header/world.h"
#include "header/entity.h"

/* Implementation part */
#if defined(LIBRG_IMPLEMENTATION) && !defined(LIBRG_IMPLEMENTATION_DONE)
#define LIBRG_IMPLEMENTATION_DONE

#ifndef LIBRG_CUSTOM_ZPL
    #define ZPL_NANO
    #define ZPL_IMPL

    #include "vendor/zpl.h"
#endif

// =======================================================================//
// !
// ! Implementation compile-time configuration
// !
// =======================================================================//

#define LIBRG_ASSERT(x)

/* allows to define a custom allocator */
#ifndef LIBRG_MEM_ALLOC
#define LIBRG_MEM_ALLOC(size) zpl_malloc(size)
#endif

/* allows to define a custom de-allocator */
#ifndef LIBRG_MEM_FREE
#define LIBRG_MEM_FREE(ptr) zpl_mfree(ptr)
#endif

/* defines how many max chunks an entity */
/* can be located in simultaneously */
#ifndef LIBRG_ENTITY_MAXCHUNKS
#define LIBRG_ENTITY_MAXCHUNKS 8
#endif

/* defines how many max chunks an owner */
/* can see in a single query */
#ifndef LIBRG_QUERY_MAXCHUNKS
#define LIBRG_QUERY_MAXCHUNKS 256
#endif

#include "source/context.c"
#include "source/world.c"
#include "source/entity.c"

#endif // LIBRG_IMPLEMENTATION


/* net module () */

// LIBRG_HANDSHAKE_INIT,
// LIBRG_HANDSHAKE_REQUEST,
// LIBRG_HANDSHAKE_ACCEPT,
// LIBRG_HANDSHAKE_REFUSE,

// LIBRG_CONNECTION_CONNECT,
// LIBRG_CONNECTION_DISCONNECT,

// LIBRG_API int           librg_config_tickdelay_set(librg_ctx *, float time_ms);
// LIBRG_API float         librg_config_tickdelay_get(librg_ctx *);

// LIBRG_API int           librg_server_start(librg_ctx *, const char *hostname, uint16_t port);
// LIBRG_API int           librg_server_stop(librg_ctx *);
// LIBRG_API int           librg_server_tick(librg_ctx *);
// LIBRG_API int           librg_server_status(librg_ctx *);
// LIBRG_API int           librg_server_disconnect(librg_ctx *, librg_client);
// LIBRG_API int           librg_server_userdata_set(librg_ctx *, librg_client, void *);
// LIBRG_API void *        librg_server_userdata_get(librg_ctx *, librg_client);
// LIBRG_API int           librg_server_send_single(librg_ctx *, librg_client, void *data, size_t data_size);
// LIBRG_API int           librg_server_send_multiple(librg_ctx *, librg_client *, size_t client_amount, void *data, size_t data_size);
// LIBRG_API int           librg_server_send_all(librg_ctx *, void *data, size_t data_size);
// LIBRG_API int           librg_server_send_allexcept(librg_ctx *, librg_client *, size_t client_amount, void *data, size_t data_size);
// LIBRG_API int           librg_server_send_chunks(librg_ctx *, librg_chunk *, size_t chunk_amount, void *data, size_t data_size);
// LIBRG_API int           librg_server_send_chunksexcept(librg_ctx *, librg_chunk *, size_t chunk_amount, librg_client *, size_t client_amount, void *data, size_t data_size);

// LIBRG_API int           librg_client_connect(librg_ctx *, const char *hostname, uint16_t port);
// LIBRG_API int           librg_client_disconnect(librg_ctx *);
// LIBRG_API int           librg_client_tick(librg_ctx *);
// LIBRG_API int           librg_client_status(librg_ctx *);
// LIBRG_API int           librg_client_send(librg_ctx *, void *data, size_t data_size);

#endif // LIBRG_H
