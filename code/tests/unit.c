#define LIBRG_IMPL
#define LIBRG_DEBUG
#include <librg.h>

#include "unit.h"

/* TEST CATEGORIES */
#include "cases/general.h"
#include "cases/entity.h"
#include "cases/query.h"
#include "cases/packing.h"

int main() {
    UNIT_CREATE("librg");

    UNIT_MODULE(general);
    UNIT_MODULE(entity);
    UNIT_MODULE(query);
    UNIT_MODULE(packing);

    return UNIT_RUN();
}
