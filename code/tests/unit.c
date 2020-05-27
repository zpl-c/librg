#define LIBRG_IMPL
#include <librg.h>

static int _g_modules = 0;
static int _g_modules_err = 0;
static int _g_total = 0;
static int _g_errors = 0;

#define MODULE(name, scope) \
    int ZPL_JOIN2(module__,name)() { \
        zpl_printf("--------------------------------------\n"); \
        zpl_printf("  MODULE: %s\n", #name); \
        fflush(stdout); \
        _g_modules++; \
        int _total = 0; \
        int _errors = 0; \
        int _lasterr = 0; \
        char *_errstr = 0; \
        scope; \
        fflush(stdout); \
        zpl_printf("  Results: %d total, %d failed, %d passed\n", _total, _errors, _total - _errors); \
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
    printf("    CASE: It %-80s: %s%s\n", desc, (_lasterr) ? "\x1B[31mFAIL\x1B[0m" : "\x1B[32mPASS\x1B[0m", _errstr);

#define FAIL(a, b)                             { _errstr = zpl_bprintf("\n      Reason: %s:%d %s %s:%d\n", #a, (int)a, (a == b)?"==":"!=", #b, b); _lasterr = 1; break; }
#define STRFAIL(a, b)                          { _errstr = zpl_bprintf("\n      Reason: %s:%s %s %s:%s\n", #a, (char *)a, (!strcmp(a,b))?"==":"!=", #b, b); _lasterr = 1; break; }
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
    zpl_printf("librg unit tests\n");

    #define X(name) cnt++;
        MODULE_LIST
    #undef X

    zpl_printf("Modules to be tested: %d\n", cnt);

    #define X(name) err += ZPL_JOIN2(module__,name)();
        MODULE_LIST
    #undef X

    fflush(stdout);
    zpl_printf("--------------------------------------\n");
    zpl_printf("MODULES: %d total, %d failed, %d passed\n", _g_modules, _g_modules_err, _g_modules - _g_modules_err);
    zpl_printf("TESTS:   %d total, %d failed, %d passed\n", _g_total, _g_errors, _g_total - _g_errors);

    return -err;
}

#if defined(ZPL_COMPILER_MSVC)
    #pragma warning(pop)
#endif

#if defined(__GCC__) || defined(__GNUC__) || defined(__clang__)
    #pragma GCC diagnostic pop
#endif
