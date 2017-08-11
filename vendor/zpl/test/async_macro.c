#define ZPL_IMPLEMENTATION
#include <zpl.h>


typedef struct {
    i32 a,b,c;
} numbers_t;

ZPL_ASYNC_CB(do_calc) {
    numbers_t *n = cast(numbers_t *)data;
    n->a += n->c;
    n->b += n->a;
    n->c += n->b;
}

ZPL_ASYNC_CB(print_results) {
    numbers_t *n = cast(numbers_t *)data;

    zpl_printf("a: %d, b: %d, c: %d\n", n->a, n->b, n->c);
}

int main(void) {
    numbers_t t_ = {1, 2, 3};
    numbers_t *t = zpl_malloc(sizeof(numbers_t));
    *t = t_;
    zpl_async(t, do_calc, print_results);
    zpl_mfree(t);

    return 0;
}
