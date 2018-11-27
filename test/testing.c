#define LIBRG_DEBUG
#define LIBRG_IMPLEMENTATION
#include "librg.h"


int main() {
    zpl_aabb3 test;

    test.centre = zpl_vec3f(100,100,100);
    test.half_size = zpl_vec3f(5,5,5);

    zpl_vec3 point = zpl_vec3f(105,95,95);
    librg_log("contains: %d\n", librg__space_contains(3, test, (f32 *)&point));

    return 0;
}
