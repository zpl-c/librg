/**
  ZPL - Unit testing framework

Usage:
  #include "unit.h" in EXACTLY one source file, usually the one containing your testing app's entry point.

  There really is no need to include this file multiple times within a project, unless you wish to run
  multiple tests within a single executable or split test cases to multiple compilation units, in such case
  define UNIT_STATIC to ensure the library won't leak symbols outside compilation units.

  and cover your beautiful code already!

GitHub:
  https://github.com/zpl-c/tester

Version History:
  1.0.1 - Small tweaks
  1.0.0 - Where it all started... (not really)

License:
  This Software is dual licensed under the following licenses:

  Unlicense
  This is free and unencumbered software released into the public domain.

  Anyone is free to copy, modify, publish, use, compile, sell, or
  distribute this software, either in source code form or as a compiled
  binary, for any purpose, commercial or non-commercial, and by any
  means.

  In jurisdictions that recognize copyright laws, the author or authors
  of this software dedicate any and all copyright interest in the
  software to the public domain. We make this dedication for the benefit
  of the public at large and to the detriment of our heirs and
  successors. We intend this dedication to be an overt act of
  relinquishment in perpetuity of all present and future rights to this
  software under copyright law.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
  IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
  OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
  ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
  OTHER DEALINGS IN THE SOFTWARE.

  For more information, please refer to <http://unlicense.org/>

  BSD 3-Clause
  Copyright (c) 2016-2021 Dominik Madarász. All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

  1. Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.
  2. Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.
  3. Neither the name of the copyright holder nor the names of its contributors
  may be used to endorse or promote products derived from this software without
  specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

/* Adjust it to your needs, preferably to the number of modules you wish to test against. */
#ifndef UNIT_MAX_MODULES
#define UNIT_MAX_MODULES 256
#endif

#ifndef UNIT_SKIP_MAGIC
#define UNIT_SKIP_MAGIC 0xFF
#endif

#ifndef UNIT_JOIN2
#define UNIT_JOIN2(a,b) a##b
#endif

/* Isolates test results within compilation units, this allows for running
    multiple test suites per single binary. */
#ifdef UNIT_STATIC
#define UNIT_DEF static
#else
#define UNIT_DEF
#endif

#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>

#define MODULE(name, scope) \
    int32_t UNIT_JOIN2(module__,name)() { \
        printf("--------------------------------------\n"); \
        printf(" module: %s\n", #name); \
        printf("--------------------------------------\n"); \
        fflush(stdout); \
        _g_modules++; \
        int32_t _total = 0; \
        int32_t _errors = 0; \
        int32_t _lasterr = 0; \
        char *_errstr = 0; \
        scope; \
        fflush(stdout); \
        printf("\n results: %d total, %s%d failed\x1B[0m, %s%d passed\x1B[0m\n", _total, _errors>0?"\x1B[31m":"", _errors, _errors==0?"\x1B[32m":"", _total - _errors); \
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
    if (_lasterr != UNIT_SKIP_MAGIC) _errors += _lasterr; \
    printf(" * [%s]: It %s %s\n", (_lasterr == UNIT_SKIP_MAGIC) ? "\x1B[33mSKIP\x1B[0m" : (_lasterr) ? "\x1B[31mFAIL\x1B[0m" : "\x1B[32mPASS\x1B[0m", desc, _errstr);

/* TEST CHECKS */
#define FAIL(a, b)                             { _errstr = unit__bprintf("\n\n\tassert: \x1B[31m%s:%lld %s %s:%lld\x1B[0m\n\tat %s:%d\n", #a, a, (a == b)?"==":"!=", #b, b, __FILE__, __LINE__); _lasterr = 1; break; }
#define UFAIL(a, b)                             { _errstr = unit__bprintf("\n\n\tassert: \x1B[31m%s:%llu %s %s:%llu\x1B[0m\n\tat %s:%d\n", #a, a, (a == b)?"==":"!=", #b, b, __FILE__, __LINE__); _lasterr = 1; break; }
#define STRFAIL(a, b)                          { _errstr = unit__bprintf("\n\n\tassert: \x1B[31m%s:%s %s %s:%s\x1B[0m\n\tat %s:%d\n", #a, (char *)a, (!strcmp(a,b))?"==":"!=", #b, b, __FILE__, __LINE__); _lasterr = 1; break; }
#define EQUALS(a, b)        if (a != b)        { FAIL(a, b); }
#define UEQUALS(a, b)        if (a != b)        { UFAIL(a, b); }
#define STREQUALS(a, b)     if (!!strcmp(a,b)) { STRFAIL(a, b); }
#define STRCEQUALS(a, b, c)     if (!!strncmp(a,b, c)) { STRFAIL(a, b); }
#define STRCNEQUALS(a, b, c)     if (!strncmp(a,b, c)) { STRFAIL(a, b); }
#define STRNEQUALS(a, b)    if (!strcmp(a,b))  { STRFAIL(a, b); }
#define NEQUALS(a, b)       if (a == b)        { FAIL(a, b); }
#define LESSER(a, b)        if (a >= b)        { FAIL(a, b); }
#define GREATER(a, b)       if (a <=b)         { FAIL(a, b); }
#define LESSEREQ(a, b)      if (a < b)         { FAIL(a, b); }
#define GREATEREQ(a, b)     if (a > b)         { FAIL(a, b); }
#define SKIP()                                 { _lasterr = UNIT_SKIP_MAGIC; break; }

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
    #pragma GCC diagnostic ignored "-Wpointer-to-int-cast"
#endif

#if defined(_MSC_VER)
    #pragma warning(push)
    #pragma warning(disable : 4201)
    #pragma warning(disable : 4127) // Conditional expression is constant
#endif

typedef int32_t (*unit_case)();

#define UNIT_CREATE(name) \
    const char *unit_name = name; \
    unit_case unit_modules[UNIT_MAX_MODULES] = {0}; \
    int32_t unit_count = 0;

#define UNIT_MODULE(name) \
    unit_modules[unit_count++] = UNIT_JOIN2(module__,name)

#define UNIT_RUN() \
        unit_main(unit_name, unit_modules, unit_count)

/* INTERNALS */

UNIT_DEF int32_t _g_modules = 0;
UNIT_DEF int32_t _g_modules_err = 0;
UNIT_DEF int32_t _g_total = 0;
UNIT_DEF int32_t _g_errors = 0;

UNIT_DEF int32_t unit_main(const char *name, unit_case *cases, int32_t count) {
    int32_t err = 0, cnt = count;
    printf("> testing suite:\n\n");
    printf("   *   suite: %s\n", name);
    printf("   * modules: %d\n", cnt);
    printf("\n");

    for (int32_t i = 0; i < count; ++i) {
        err += cases[i]();
    }

    fflush(stdout);
    printf("--------------------------------------\n"); \
    printf("> total:\n\n");
    printf(" * modules: %d total, %s%d failed\x1B[0m, %s%d passed\x1B[0m\n", _g_modules, _g_modules_err>0?"\x1B[31m":"" ,_g_modules_err, _g_modules_err==0?"\x1B[32m":"", _g_modules - _g_modules_err);
    printf(" * tests:   %d total, %s%d failed\x1B[0m, %s%d passed\x1B[0m\n", _g_total, _g_errors>0?"\x1B[31m":"" ,_g_errors, _g_errors==0?"\x1B[32m":"", _g_total - _g_errors);
    printf("\n");

    return -err;
}

// Locally persisting buffer
static inline char* unit__bprintf(const char* fmt, ...)
{
    static char buf[128];
    va_list args;
    va_start(args, fmt);
    vsprintf(buf, fmt, args);
    va_end(args);
    return buf;
}

#if defined(ZPL_COMPILER_MSVC)
    #pragma warning(pop)
#endif

#if defined(__GCC__) || defined(__GNUC__) || defined(__clang__)
    #pragma GCC diagnostic pop
#endif
