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

    UNIT_CASE(general);
    UNIT_CASE(entity);
    UNIT_CASE(query);
    UNIT_CASE(packing);

    return UNIT_RUN();
}
