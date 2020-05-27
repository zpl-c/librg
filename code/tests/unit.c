#define LIBRG_IMPL
#define LIBRG_DEBUG
#define LIBRG_ASSERT ZPL_ASSERT
#include <librg.h>

static int _g_modules = 0;
static int _g_modules_err = 0;
static int _g_total = 0;
static int _g_errors = 0;

#define MODULE(name, scope) \
    int ZPL_JOIN2(module__,name)() { \
        zpl_printf("--------------------------------------\n"); \
        zpl_printf(" group: %s\n", #name); \
        zpl_printf("--------------------------------------\n"); \
        fflush(stdout); \
        _g_modules++; \
        int _total = 0; \
        int _errors = 0; \
        int _lasterr = 0; \
        char *_errstr = 0; \
        scope; \
        fflush(stdout); \
        zpl_printf("\n results: %d total, %s%d failed\x1B[0m, %s%d passed\x1B[0m\n", _total, _errors>0?"\x1B[31m":"", _errors, _errors==0?"\x1B[32m":"", _total - _errors); \
        _g_total += _total; \
        _g_errors += _errors; \
        if (_errors) _g_modules_err++; \
        return (_errors); \
    }

#define IT(desc, scope) \
    _lasterr = 0; \
    _errstr = ""; \
    _total += 1; \
    do scope while(0); \
    _errors += _lasterr; \
    printf(" * [%s]: It %s %s\n", (_lasterr) ? "\x1B[31mFAIL\x1B[0m" : "\x1B[32mPASS\x1B[0m", desc, _errstr);

#define FAIL(a, b)                             { _errstr = zpl_bprintf("\n\n\tassert: \x1B[31m%s:%d %s %s:%d\x1B[0m\n\tat %s:%d\n", #a, (int)a, (a == b)?"==":"!=", #b, b, __FILE__, __LINE__); _lasterr = 1; break; }
#define STRFAIL(a, b)                          { _errstr = zpl_bprintf("\n\n\tassert: \x1B[31m%s:%s %s %s:%s\x1B[0m\n\tat %s:%d\n", #a, (char *)a, (!strcmp(a,b))?"==":"!=", #b, b, __FILE__, __LINE__); _lasterr = 1; break; }
#define EQUALS(a, b)        if (a != b)        { FAIL(a, b); }
#define STREQUALS(a, b)     if (!!strcmp(a,b)) { STRFAIL(a, b); }
#define STRNEQUALS(a, b)    if (!strcmp(a,b))  { STRFAIL(a, b); }
#define NEQUALS(a, b)       if (a == b)        { FAIL(a, b); }
#define LESSER(a, b)        if (a >= b)        { FAIL(a, b); }
#define GREATER(a, b)       if (a <=b)         { FAIL(a, b); }
#define LESSEREQ(a, b)      if (a < b)         { FAIL(a, b); }
#define GREATEREQ(a, b)     if (a > b)         { FAIL(a, b); }

#if defined(__GCC__) || defined(__GNUC__) || defined(__clang__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wattributes"
    #pragma GCC diagnostic ignored "-Wunused-value"
    #pragma GCC diagnostic ignored "-Wignored-qualifiers"
    #pragma GCC diagnostic ignored "-Wunused-variable"
    #pragma GCC diagnostic ignored "-Wunused-function"
    #pragma GCC diagnostic ignored "-Wwrite-strings"
    #pragma GCC diagnostic ignored "-Wunused-parameter"
    #pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    #pragma GCC diagnostic ignored "-Wmissing-braces"
    #pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif

#if defined(_MSC_VER)
    #pragma warning(push)
    #pragma warning(disable : 4201)
    #pragma warning(disable : 4127) // Conditional expression is constant
#endif

/* TEST CATEGORIES */
#include "unit/general.h"
#include "unit/entity.h"
#include "unit/query.h"

#define MODULE_LIST \
    X(general) \
    X(entity) \
    X(query)

int main() {
    int err = 0, cnt = 0;
    zpl_printf("> testing suite:\n\n");
    zpl_printf("   * library: librg\n");

    #define X(name) cnt++;
        MODULE_LIST
    #undef X

    zpl_printf("   * groups: %d\n", cnt);
    zpl_printf("\n", cnt);

    #define X(name) err += ZPL_JOIN2(module__,name)();
        MODULE_LIST
    #undef X

    fflush(stdout);
    zpl_printf("--------------------------------------\n"); \
    zpl_printf("> total:\n\n");
    zpl_printf(" * modules: %d total, %s%d failed\x1B[0m, %s%d passed\x1B[0m\n", _g_modules, _g_modules_err>0?"\x1B[31m":"" ,_g_modules_err, _g_modules_err==0?"\x1B[32m":"", _g_modules - _g_modules_err);
    zpl_printf(" * tests:   %d total, %s%d failed\x1B[0m, %s%d passed\x1B[0m\n", _g_total, _g_errors>0?"\x1B[31m":"" ,_g_errors, _g_errors==0?"\x1B[32m":"", _g_total - _g_errors);
    zpl_printf("\n", cnt);

    return -err;
}

#if defined(ZPL_COMPILER_MSVC)
    #pragma warning(pop)
#endif

#if defined(__GCC__) || defined(__GNUC__) || defined(__clang__)
    #pragma GCC diagnostic pop
#endif
