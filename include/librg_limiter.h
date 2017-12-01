/**
 * An example limiter implementation
 *
 * Can be used to reduce number of udpate sends for every entity
 * Can be easily modified to one's needs
 */

#ifndef LIBRG_LIMITER_INCLUDE_H
#define LIBRG_LIMITER_INCLUDE_H

#ifdef __cplusplus
extern "C" {
#endif


    #define LIBRG_LIMITER_DETEORIATION 0.25f

    typedef struct {
        b32 update_now;

        f32 update_initial_delay;
        f32 update_delay;
        f32 update_max_delay;
        f32 update_time;
        f32 update_deteoriation;
        f32 update_moving_treshold;

        zplm_vec3_t last_position;
    } librg_limiter_t;

    LIBRG_API void librg_limiter_init(librg_limiter_t *entity_limit);
    LIBRG_API void librg_limiter_fire(librg_event_t *event, librg_limiter_t *entity_limit);


#if defined(LIBRG_LIMITER_IMPLEMENTATION) && !defined(LIBRG_LIMITER_IMPLEMENTATION_DONE)
#define LIBRG_LIMITER_IMPLEMENTATION_DONE


    void librg_limiter_init(librg_limiter_t *entity_limit) {
        entity_limit->update_deteoriation    = LIBRG_LIMITER_DETEORIATION;
        entity_limit->update_initial_delay   = entity_limit->update_delay = 0.0f;
        entity_limit->update_moving_treshold = 0.03f;
    }

    void librg_limiter_fire(librg_event_t *event, librg_limiter_t *entity_limit) {
        librg_assert(event && event->ctx);

        librg_ctx_t *ctx = event->ctx;
        librg_entity_t *entity = event->entity;

        if (entity_limit->update_time < entity_limit->update_delay) {
            entity_limit->update_time += ctx->tick_delay;
            {
                zplm_vec3_t dir;
                zplm_vec3_sub(&dir, entity_limit->last_position, entity->position);

                b32 is_moving = (zplm_vec3_dot(dir, dir) > entity_limit->update_moving_treshold);

                if (is_moving || entity_limit->update_now) {
                    entity_limit->update_delay = entity_limit->update_initial_delay;
                    entity_limit->last_position = entity->position;
                    entity_limit->update_now = false;
                }
            }

            librg_event_reject(event);
            return;
        }
        else {
            entity_limit->update_time = 0.0f;

            if (entity_limit->update_delay < entity_limit->update_max_delay) {
                entity_limit->update_delay += entity_limit->update_delay * entity_limit->update_deteoriation;
            }
        }
    }

#endif

#ifdef __cplusplus
}
#endif

#endif // LIBRG_LIMITER_INCLUDE_H
