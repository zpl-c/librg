/**
 * librg - a library for building simple and elegant cross-platform multiplayer client-server solutions.
 *
 * Usage:
 * #define LIBRG_IMPLEMENTATION exactly in ONE source file right BEFORE including the library, like:
 *
 * #define LIBRG_IMPLEMENTATION
 * #include <librg.h>
 *
 * Credits:
 *  - Vladyslav Hrytsenko (GitHub: inlife)
 *  - Dominik Madarasz (GitHub: zaklaus)
 *
 * Dependencies:
 *  - librg.h -> zpl.h (built-in)
 *  - librg_enet.h -> enet.h (built-in)
 *
 * ===================================
 *  Version History:
 * ===================================
 *
 * 6.0.0
 *  - Major library rewrite
 *
 * 5.0.6 - Fix forced_inline on librg__space_insert
 * 5.0.5 - Fixes to selection and deduplication flow
 * 5.0.3 - Minor fixes by @markatk
 * 5.0.2 - Fixed issue related to visiblity destruction
 * 5.0.1 - Fixed entity visiblity states after disconnection
 *
 * 5.0.0
 *  - Changed API for visibility feature:
 *    - Instead of booleans setting whether or not entity would be included in the query or not
 *      a multiple constant-based states were inroduced:
 *        - LIBRG_VISIBILITY_DEFAULT - the original state of visibility, entity is only visible if it is in the stream range
 *        - LIBRG_ALWAYS_VISIBLE - the entity is visible disregarding if it is the stream range or not
 *        - LIBRG_ALWAYS_INVISIBLE - opposite of the above, entity will be always invisible
 *      Entity visibility can be set on per entity-to-entity or global levels. The entity-to-entity relation has a bigger
 *      priority, so f.e. setting entity to be LIBRG_ALWAYS_VISIBLE on relation level, will override global visibility.
 *   - Aditionally, if the virtual world feature is enabled, it gains main priotity over visibility, thus entities located in 2 different
 *     virtual worlds will not be able to see each other in spite of visibility settings.
 *
 * 4.1.5
 *  - Fix to data send
 *
 * 4.1.4
 *  - Fixed issue with async flow inconsitensies of control_generations
 *  - Fixed boolean validation in disconnection flow
 *  - Added proxying of user_data from msg to evt in disconnect event
 *
 * 4.1.1
 * - Added compile-time 'features':
 *     - Ability to enable/disable some librg compile-time features
 *     - Entity igore tables are now optional, and can be disabled
 *     - Implmented simple optional Virtual world feature for entities
 *     - Implemented a feature to enable/disable octree culler (falls back to linear check)
 *     - Multiple features can be combined
 * - Added 'generation' to entity control lists:
 *     Setting, removing and setting control to the same entity again with same owner
 *     will now distinct between old and new controllers, and messages still coming
 *     from old control generation will be rejected in favor of new ones.
 * - Added guard to minimum sized packet in receive for both sides
 * - Added spherical culler handler, and ability to do runtime switch (LIBRG_USE_RADIUS_CULLING)
 * - Added return codes to some functions @markatk
 * - Streamed entities are now going to be always returned in query for controlling peer
 * - Fixed issue with host setting on the server side
 * - Fixed nullptr crash on empty host string for client on connect
 * - Removed experimental multithreading code
 *
 * 4.1.0  - Added new, extended message methods and sending options
 * 4.0.0 - Coding style changes and bug fixes
 *
 * 3.3.1
 * - Updated zpl dependencies
 * - Removed zpl_math dependency (replaced by internal module in zpl)
 *
 * 3.3.0
 * - Added ipv6 support
 * - Added safe bitstream reads for internal methods
 * - Updated enet to latest version (2.0.1, ipv6 support)
 * - Updated zpl to latest version
 *
 * 3.2.0
 * - Fixed minor memory client-side memory leak with empty control list
 * - Fixed issue with client stream update and removed entity on server
 * - Updated zpl to new major version, watch out for possible incompatibilities
 * - Added method for alloc/dealloc the librg_ctx, librg_data, librg_event for the bindings
 * - Added experimental support for update buffering, disabled by default, and not recommended to use
 * - Added built-in timesyncer, working on top of monotonic time, syncing client clock to server one
 * - Added helper methods: librg_time_now, librg_standard_deviation
 * - Changed ctx->tick_delay from u16 to f64 (slightly more precision)
 *
 * 3.1.0
 * - Removed zpl_cull and zpl_event dependencies
 * - added librg_network_kick()
 * - saving current librg_address to ctx->network
 * - refactor to proper disconnection code
 * - exclude local client entity from LIBRG_CONNECTION_DISCONNECT
 * - moved options and some few other things to the implementation part
 * - fixed issue with replacing entity control
 * - fixed issue with add control queuing beign sent before create entity packet
 *
 * 3.0.7 - Fix for entity query dublication for player entities
 * 3.0.5 - Patched librg_callback_cb arg value
 * 3.0.4 - Fixed Android and iOS support
 * 3.0.3 - Small fixes
 * 3.0.2 - Dependency updates
 * 3.0.1 - minor api patch
 * 3.0.0 - contexts, major api changes, fried potato, other stuff
 *
 * 2.2.3 - fixed mem leak on net event
 * 2.2.2 - Fixed client issue with librg_message_send_instream_except
 * 2.2.1 - Fixed cpp issues with librg_data pointers
 * 2.2.0 - Inner message system rafactor
 * 2.1.0 - Inner bitstream refactors, with slight interface changes
 * 2.0.2 - C++ and MSVC related fixes
 * 2.0.0 - Initial C version rewrite
 *
 * ============================
 *  License notice
 * ============================
 * Copyright (c) 2017-2020 Vladyslav Hrytsenko. All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef LIBRG_H
#define LIBRG_H

#define LIBRG_VERSION_MAJOR 6
#define LIBRG_VERSION_MINOR 0
#define LIBRG_VERSION_PATCH 4
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
    #elif defined(LIBRG_EMSCRIPTEN)
        #define LIBRG_API EMSCRIPTEN_KEEPALIVE
    #else
        #define LIBRG_API LIBRG_EXTERN
    #endif
#endif

#include <stdint.h>
#include <stddef.h>

#if defined(__EMSCRIPTEN__)
    #include <emscripten.h>
#endif

#include "header/types.h"
#include "header/general.h"
#include "header/entity.h"
#include "header/query.h"
#include "header/packing.h"

/* Implementation part */
#if defined(LIBRG_IMPLEMENTATION) && !defined(LIBRG_IMPLEMENTATION_DONE)
#define LIBRG_IMPLEMENTATION_DONE

#ifndef LIBRG_CUSTOM_ZPL
    #define ZPL_NANO
    #define ZPL_IMPL

    #include "vendor/zpl.h"
#endif

#include "source/types.c"
#include "source/general.c"
#include "source/entity.c"
#include "source/query.c"
#include "source/packing.c"

#endif // LIBRG_IMPLEMENTATION

#endif // LIBRG_H
