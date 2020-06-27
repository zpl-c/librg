// file: header/types.h

#ifdef LIBRG_EDITOR
#include <librg.h>
#endif

LIBRG_BEGIN_C_DECLS

// =======================================================================//
// !
// ! Main type definitions
// !
// =======================================================================//

typedef void librg_world;
typedef void librg_event;
typedef int64_t librg_chunk;

#define LIBRG_OFFSET_BEG ((int16_t)0x8000)
#define LIBRG_OFFSET_MID ((int16_t)0x0000)
#define LIBRG_OFFSET_END ((int16_t)0x7fff)

#define LIBRG_IN
#define LIBRG_OUT
#define LIBRG_INOUT

typedef enum librg_event_type {
    LIBRG_WRITE_CREATE,
    LIBRG_WRITE_UPDATE,
    LIBRG_WRITE_REMOVE,

    LIBRG_READ_CREATE,
    LIBRG_READ_UPDATE,
    LIBRG_READ_REMOVE,

    LIBRG_ERROR_CREATE,
    LIBRG_ERROR_UPDATE,
    LIBRG_ERROR_REMOVE,
} librg_event_type;

typedef int32_t (*librg_event_fn)(librg_world *world, librg_event *event);

typedef enum librg_visibility {
    LIBRG_VISIBLITY_DEFAULT,
    LIBRG_VISIBLITY_NEVER,
    LIBRG_VISIBLITY_ALWAYS,
} librg_visibility;


// =======================================================================//
// !
// ! Errors, statuses, warnings and information message codes
// !
// =======================================================================//

#define LIBRG_OK                        (+0x0000)
#define LIBRG_FAIL(code)                (code < 0)
#define LIBRG_TRUE                      (+0x0001)
#define LIBRG_FALSE                     (+0x0000)

#define LIBRG_WORLD_INVALID             (-0x0001)
#define LIBRG_OWNER_INVALID             (-0x0002)
#define LIBRG_CHUNK_INVALID             (-0x0003)
#define LIBRG_ENTITY_INVALID            (-0x0004)
#define LIBRG_ENTITY_FOREIGN            (-0x0005)
#define LIBRG_EVENT_INVALID             (-0x0006)
#define LIBRG_HANDLER_REPLACED          (-0x0002)
#define LIBRG_HANDLER_EMPTY             (-0x0002)
#define LIBRG_ENTITY_UNTRACKED          (-0x0002)
#define LIBRG_ENTITY_ALREADY_TRACKED    (-0x0002)
#define LIBRG_ENTITY_VISIBILITY_IGNORED (-0x0003)
#define LIBRG_WRITE_REJECT              (-0x0001)
#define LIBRG_READ_INVALID              (-0x0003)
#define LIBRG_NULL_REFERENCE            (-0x0007)

LIBRG_END_C_DECLS
