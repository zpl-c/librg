#define LIBRG_IMPL
#include "librg.h"

int dostuff(librg_world *w) {
    if (!librg_world_valid(w))
        return 1;

    int *mydata = librg_world_userdata_get(w);
    if (!mydata)
        return 2;

    *mydata += 15;
    return 0;
}

int main() {
    librg_world *world = librg_world_create();

    if (!world) {
        printf("failed to create a world\n");
        return 1;
    }

    int mydata = 235;
    librg_world_userdata_set(world, &mydata);

    if (dostuff(world) != 0) {
        printf("failed to execute my function\n");
        librg_world_destroy(world);
        return 1;
    }

    if (mydata == 250) {
        printf("we succeeded\n");
    }

    librg_world_destroy(world);
    return 0;
}
