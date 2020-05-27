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
typedef int64_t librg_chunk;

#define LIBRG_OFFSET_BEG                ((int16_t)0x8000)
#define LIBRG_OFFSET_MID                ((int16_t)0x0000)
#define LIBRG_OFFSET_END                ((int16_t)0x7fff)

#define LIBRG_OBSERVE_ALL               (-0x01)
#define LIBRG_OBSERVE_NONE              (+0x00)

#define LIBRG_IN
#define LIBRG_OUT

// =======================================================================//
// !
// ! Errors, statuses, warnings and information message codes
// !
// =======================================================================//

#define LIBRG_OK                        (+0x0000)
#define LIBRG_FAIL                      (-0x0001)

#define LIBRG_TRUE                      (+0x0001)
#define LIBRG_FALSE                     (+0x0000)
#define LIBRG_VALID                     (+0x0001)
#define LIBRG_INVALID                   (-0x0001)

#define LIBRG_WORLD_INVALID             (-0x0001)
#define LIBRG_OWNER_INVALID             (-0x0001)
#define LIBRG_CHUNK_INVALID             (-0x0002)
#define LIBRG_HANDLER_REPLACED          (-0x0002)
#define LIBRG_HANDLER_EMPTY             (-0x0002)
#define LIBRG_ENTITY_UNTRACKED          (-0x0002)
#define LIBRG_ENTITY_ALREADY_TRACKED    (-0x0002)

LIBRG_END_C_DECLS
