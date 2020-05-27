// file: librg_hedley.h

/* Hedley - https://nemequ.github.io/hedley
 * Created by Evan Nemerson <evan@nemerson.com>
 *
 * To the extent possible under law, the author(s) have dedicated all
 * copyright and related and neighboring rights to this software to
 * the public domain worldwide. This software is distributed without
 * any warranty.
 *
 * For details, see <http://creativecommons.org/publicdomain/zero/1.0/>.
 * SPDX-License-Identifier: CC0-1.0
 */

#if !defined(LIBRG_HEDLEY_VERSION) || (LIBRG_HEDLEY_VERSION < 13)
#if defined(LIBRG_HEDLEY_VERSION)
#  undef LIBRG_HEDLEY_VERSION
#endif
#define LIBRG_HEDLEY_VERSION 13

#if defined(LIBRG_STRINGIFY_EX)
#  undef LIBRG_STRINGIFY_EX
#endif
#define LIBRG_STRINGIFY_EX(x) #x

#if defined(LIBRG_STRINGIFY)
#  undef LIBRG_STRINGIFY
#endif
#define LIBRG_STRINGIFY(x) LIBRG_STRINGIFY_EX(x)

#if defined(LIBRG_CONCAT_EX)
#  undef LIBRG_CONCAT_EX
#endif
#define LIBRG_CONCAT_EX(a,b) a##b

#if defined(LIBRG_CONCAT)
#  undef LIBRG_CONCAT
#endif
#define LIBRG_CONCAT(a,b) LIBRG_CONCAT_EX(a,b)

#if defined(LIBRG_CONCAT3_EX)
#  undef LIBRG_CONCAT3_EX
#endif
#define LIBRG_CONCAT3_EX(a,b,c) a##b##c

#if defined(LIBRG_CONCAT3)
#  undef LIBRG_CONCAT3
#endif
#define LIBRG_CONCAT3(a,b,c) LIBRG_CONCAT3_EX(a,b,c)

#if defined(LIBRG_VERSION_ENCODE)
#  undef LIBRG_VERSION_ENCODE
#endif
#define LIBRG_VERSION_ENCODE(major,minor,revision) (((major) * 1000000) + ((minor) * 1000) + (revision))

#if defined(LIBRG_VERSION_DECODE_MAJOR)
#  undef LIBRG_VERSION_DECODE_MAJOR
#endif
#define LIBRG_VERSION_DECODE_MAJOR(version) ((version) / 1000000)

#if defined(LIBRG_VERSION_DECODE_MINOR)
#  undef LIBRG_VERSION_DECODE_MINOR
#endif
#define LIBRG_VERSION_DECODE_MINOR(version) (((version) % 1000000) / 1000)

#if defined(LIBRG_VERSION_DECODE_REVISION)
#  undef LIBRG_VERSION_DECODE_REVISION
#endif
#define LIBRG_VERSION_DECODE_REVISION(version) ((version) % 1000)

#if defined(LIBRG_GNUC_VERSION)
#  undef LIBRG_GNUC_VERSION
#endif
#if defined(__GNUC__) && defined(__GNUC_PATCHLEVEL__)
#  define LIBRG_GNUC_VERSION LIBRG_VERSION_ENCODE(__GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__)
#elif defined(__GNUC__)
#  define LIBRG_GNUC_VERSION LIBRG_VERSION_ENCODE(__GNUC__, __GNUC_MINOR__, 0)
#endif

#if defined(LIBRG_GNUC_VERSION_CHECK)
#  undef LIBRG_GNUC_VERSION_CHECK
#endif
#if defined(LIBRG_GNUC_VERSION)
#  define LIBRG_GNUC_VERSION_CHECK(major,minor,patch) (LIBRG_GNUC_VERSION >= LIBRG_VERSION_ENCODE(major, minor, patch))
#else
#  define LIBRG_GNUC_VERSION_CHECK(major,minor,patch) (0)
#endif

#if defined(LIBRG_MSVC_VERSION)
#  undef LIBRG_MSVC_VERSION
#endif
#if defined(_MSC_FULL_VER) && (_MSC_FULL_VER >= 140000000)
#  define LIBRG_MSVC_VERSION LIBRG_VERSION_ENCODE(_MSC_FULL_VER / 10000000, (_MSC_FULL_VER % 10000000) / 100000, (_MSC_FULL_VER % 100000) / 100)
#elif defined(_MSC_FULL_VER)
#  define LIBRG_MSVC_VERSION LIBRG_VERSION_ENCODE(_MSC_FULL_VER / 1000000, (_MSC_FULL_VER % 1000000) / 10000, (_MSC_FULL_VER % 10000) / 10)
#elif defined(_MSC_VER)
#  define LIBRG_MSVC_VERSION LIBRG_VERSION_ENCODE(_MSC_VER / 100, _MSC_VER % 100, 0)
#endif

#if defined(LIBRG_MSVC_VERSION_CHECK)
#  undef LIBRG_MSVC_VERSION_CHECK
#endif
#if !defined(_MSC_VER)
#  define LIBRG_MSVC_VERSION_CHECK(major,minor,patch) (0)
#elif defined(_MSC_VER) && (_MSC_VER >= 1400)
#  define LIBRG_MSVC_VERSION_CHECK(major,minor,patch) (_MSC_FULL_VER >= ((major * 10000000) + (minor * 100000) + (patch)))
#elif defined(_MSC_VER) && (_MSC_VER >= 1200)
#  define LIBRG_MSVC_VERSION_CHECK(major,minor,patch) (_MSC_FULL_VER >= ((major * 1000000) + (minor * 10000) + (patch)))
#else
#  define LIBRG_MSVC_VERSION_CHECK(major,minor,patch) (_MSC_VER >= ((major * 100) + (minor)))
#endif

#if defined(LIBRG_INTEL_VERSION)
#  undef LIBRG_INTEL_VERSION
#endif
#if defined(__INTEL_COMPILER) && defined(__INTEL_COMPILER_UPDATE)
#  define LIBRG_INTEL_VERSION LIBRG_VERSION_ENCODE(__INTEL_COMPILER / 100, __INTEL_COMPILER % 100, __INTEL_COMPILER_UPDATE)
#elif defined(__INTEL_COMPILER)
#  define LIBRG_INTEL_VERSION LIBRG_VERSION_ENCODE(__INTEL_COMPILER / 100, __INTEL_COMPILER % 100, 0)
#endif

#if defined(LIBRG_INTEL_VERSION_CHECK)
#  undef LIBRG_INTEL_VERSION_CHECK
#endif
#if defined(LIBRG_INTEL_VERSION)
#  define LIBRG_INTEL_VERSION_CHECK(major,minor,patch) (LIBRG_INTEL_VERSION >= LIBRG_VERSION_ENCODE(major, minor, patch))
#else
#  define LIBRG_INTEL_VERSION_CHECK(major,minor,patch) (0)
#endif

#if defined(LIBRG_PGI_VERSION)
#  undef LIBRG_PGI_VERSION
#endif
#if defined(__PGI) && defined(__PGIC__) && defined(__PGIC_MINOR__) && defined(__PGIC_PATCHLEVEL__)
#  define LIBRG_PGI_VERSION LIBRG_VERSION_ENCODE(__PGIC__, __PGIC_MINOR__, __PGIC_PATCHLEVEL__)
#endif

#if defined(LIBRG_PGI_VERSION_CHECK)
#  undef LIBRG_PGI_VERSION_CHECK
#endif
#if defined(LIBRG_PGI_VERSION)
#  define LIBRG_PGI_VERSION_CHECK(major,minor,patch) (LIBRG_PGI_VERSION >= LIBRG_VERSION_ENCODE(major, minor, patch))
#else
#  define LIBRG_PGI_VERSION_CHECK(major,minor,patch) (0)
#endif

#if defined(LIBRG_SUNPRO_VERSION)
#  undef LIBRG_SUNPRO_VERSION
#endif
#if defined(__SUNPRO_C) && (__SUNPRO_C > 0x1000)
#  define LIBRG_SUNPRO_VERSION LIBRG_VERSION_ENCODE((((__SUNPRO_C >> 16) & 0xf) * 10) + ((__SUNPRO_C >> 12) & 0xf), (((__SUNPRO_C >> 8) & 0xf) * 10) + ((__SUNPRO_C >> 4) & 0xf), (__SUNPRO_C & 0xf) * 10)
#elif defined(__SUNPRO_C)
#  define LIBRG_SUNPRO_VERSION LIBRG_VERSION_ENCODE((__SUNPRO_C >> 8) & 0xf, (__SUNPRO_C >> 4) & 0xf, (__SUNPRO_C) & 0xf)
#elif defined(__SUNPRO_CC) && (__SUNPRO_CC > 0x1000)
#  define LIBRG_SUNPRO_VERSION LIBRG_VERSION_ENCODE((((__SUNPRO_CC >> 16) & 0xf) * 10) + ((__SUNPRO_CC >> 12) & 0xf), (((__SUNPRO_CC >> 8) & 0xf) * 10) + ((__SUNPRO_CC >> 4) & 0xf), (__SUNPRO_CC & 0xf) * 10)
#elif defined(__SUNPRO_CC)
#  define LIBRG_SUNPRO_VERSION LIBRG_VERSION_ENCODE((__SUNPRO_CC >> 8) & 0xf, (__SUNPRO_CC >> 4) & 0xf, (__SUNPRO_CC) & 0xf)
#endif

#if defined(LIBRG_SUNPRO_VERSION_CHECK)
#  undef LIBRG_SUNPRO_VERSION_CHECK
#endif
#if defined(LIBRG_SUNPRO_VERSION)
#  define LIBRG_SUNPRO_VERSION_CHECK(major,minor,patch) (LIBRG_SUNPRO_VERSION >= LIBRG_VERSION_ENCODE(major, minor, patch))
#else
#  define LIBRG_SUNPRO_VERSION_CHECK(major,minor,patch) (0)
#endif

#if defined(LIBRG_EMSCRIPTEN_VERSION)
#  undef LIBRG_EMSCRIPTEN_VERSION
#endif
#if defined(__EMSCRIPTEN__)
#  define LIBRG_EMSCRIPTEN_VERSION LIBRG_VERSION_ENCODE(__EMSCRIPTEN_major__, __EMSCRIPTEN_minor__, __EMSCRIPTEN_tiny__)
#endif

#if defined(LIBRG_EMSCRIPTEN_VERSION_CHECK)
#  undef LIBRG_EMSCRIPTEN_VERSION_CHECK
#endif
#if defined(LIBRG_EMSCRIPTEN_VERSION)
#  define LIBRG_EMSCRIPTEN_VERSION_CHECK(major,minor,patch) (LIBRG_EMSCRIPTEN_VERSION >= LIBRG_VERSION_ENCODE(major, minor, patch))
#else
#  define LIBRG_EMSCRIPTEN_VERSION_CHECK(major,minor,patch) (0)
#endif

#if defined(LIBRG_ARM_VERSION)
#  undef LIBRG_ARM_VERSION
#endif
#if defined(__CC_ARM) && defined(__ARMCOMPILER_VERSION)
#  define LIBRG_ARM_VERSION LIBRG_VERSION_ENCODE(__ARMCOMPILER_VERSION / 1000000, (__ARMCOMPILER_VERSION % 1000000) / 10000, (__ARMCOMPILER_VERSION % 10000) / 100)
#elif defined(__CC_ARM) && defined(__ARMCC_VERSION)
#  define LIBRG_ARM_VERSION LIBRG_VERSION_ENCODE(__ARMCC_VERSION / 1000000, (__ARMCC_VERSION % 1000000) / 10000, (__ARMCC_VERSION % 10000) / 100)
#endif

#if defined(LIBRG_ARM_VERSION_CHECK)
#  undef LIBRG_ARM_VERSION_CHECK
#endif
#if defined(LIBRG_ARM_VERSION)
#  define LIBRG_ARM_VERSION_CHECK(major,minor,patch) (LIBRG_ARM_VERSION >= LIBRG_VERSION_ENCODE(major, minor, patch))
#else
#  define LIBRG_ARM_VERSION_CHECK(major,minor,patch) (0)
#endif

#if defined(LIBRG_IBM_VERSION)
#  undef LIBRG_IBM_VERSION
#endif
#if defined(__ibmxl__)
#  define LIBRG_IBM_VERSION LIBRG_VERSION_ENCODE(__ibmxl_version__, __ibmxl_release__, __ibmxl_modification__)
#elif defined(__xlC__) && defined(__xlC_ver__)
#  define LIBRG_IBM_VERSION LIBRG_VERSION_ENCODE(__xlC__ >> 8, __xlC__ & 0xff, (__xlC_ver__ >> 8) & 0xff)
#elif defined(__xlC__)
#  define LIBRG_IBM_VERSION LIBRG_VERSION_ENCODE(__xlC__ >> 8, __xlC__ & 0xff, 0)
#endif

#if defined(LIBRG_IBM_VERSION_CHECK)
#  undef LIBRG_IBM_VERSION_CHECK
#endif
#if defined(LIBRG_IBM_VERSION)
#  define LIBRG_IBM_VERSION_CHECK(major,minor,patch) (LIBRG_IBM_VERSION >= LIBRG_VERSION_ENCODE(major, minor, patch))
#else
#  define LIBRG_IBM_VERSION_CHECK(major,minor,patch) (0)
#endif

#if defined(LIBRG_TI_VERSION)
#  undef LIBRG_TI_VERSION
#endif
#if \
    defined(__TI_COMPILER_VERSION__) && \
    ( \
      defined(__TMS470__) || defined(__TI_ARM__) || \
      defined(__MSP430__) || \
      defined(__TMS320C2000__) \
    )
#  if (__TI_COMPILER_VERSION__ >= 16000000)
#    define LIBRG_TI_VERSION LIBRG_VERSION_ENCODE(__TI_COMPILER_VERSION__ / 1000000, (__TI_COMPILER_VERSION__ % 1000000) / 1000, (__TI_COMPILER_VERSION__ % 1000))
#  endif
#endif

#if defined(LIBRG_TI_VERSION_CHECK)
#  undef LIBRG_TI_VERSION_CHECK
#endif
#if defined(LIBRG_TI_VERSION)
#  define LIBRG_TI_VERSION_CHECK(major,minor,patch) (LIBRG_TI_VERSION >= LIBRG_VERSION_ENCODE(major, minor, patch))
#else
#  define LIBRG_TI_VERSION_CHECK(major,minor,patch) (0)
#endif

#if defined(LIBRG_TI_CL2000_VERSION)
#  undef LIBRG_TI_CL2000_VERSION
#endif
#if defined(__TI_COMPILER_VERSION__) && defined(__TMS320C2000__)
#  define LIBRG_TI_CL2000_VERSION LIBRG_VERSION_ENCODE(__TI_COMPILER_VERSION__ / 1000000, (__TI_COMPILER_VERSION__ % 1000000) / 1000, (__TI_COMPILER_VERSION__ % 1000))
#endif

#if defined(LIBRG_TI_CL2000_VERSION_CHECK)
#  undef LIBRG_TI_CL2000_VERSION_CHECK
#endif
#if defined(LIBRG_TI_CL2000_VERSION)
#  define LIBRG_TI_CL2000_VERSION_CHECK(major,minor,patch) (LIBRG_TI_CL2000_VERSION >= LIBRG_VERSION_ENCODE(major, minor, patch))
#else
#  define LIBRG_TI_CL2000_VERSION_CHECK(major,minor,patch) (0)
#endif

#if defined(LIBRG_TI_CL430_VERSION)
#  undef LIBRG_TI_CL430_VERSION
#endif
#if defined(__TI_COMPILER_VERSION__) && defined(__MSP430__)
#  define LIBRG_TI_CL430_VERSION LIBRG_VERSION_ENCODE(__TI_COMPILER_VERSION__ / 1000000, (__TI_COMPILER_VERSION__ % 1000000) / 1000, (__TI_COMPILER_VERSION__ % 1000))
#endif

#if defined(LIBRG_TI_CL430_VERSION_CHECK)
#  undef LIBRG_TI_CL430_VERSION_CHECK
#endif
#if defined(LIBRG_TI_CL430_VERSION)
#  define LIBRG_TI_CL430_VERSION_CHECK(major,minor,patch) (LIBRG_TI_CL430_VERSION >= LIBRG_VERSION_ENCODE(major, minor, patch))
#else
#  define LIBRG_TI_CL430_VERSION_CHECK(major,minor,patch) (0)
#endif

#if defined(LIBRG_TI_ARMCL_VERSION)
#  undef LIBRG_TI_ARMCL_VERSION
#endif
#if defined(__TI_COMPILER_VERSION__) && (defined(__TMS470__) || defined(__TI_ARM__))
#  define LIBRG_TI_ARMCL_VERSION LIBRG_VERSION_ENCODE(__TI_COMPILER_VERSION__ / 1000000, (__TI_COMPILER_VERSION__ % 1000000) / 1000, (__TI_COMPILER_VERSION__ % 1000))
#endif

#if defined(LIBRG_TI_ARMCL_VERSION_CHECK)
#  undef LIBRG_TI_ARMCL_VERSION_CHECK
#endif
#if defined(LIBRG_TI_ARMCL_VERSION)
#  define LIBRG_TI_ARMCL_VERSION_CHECK(major,minor,patch) (LIBRG_TI_ARMCL_VERSION >= LIBRG_VERSION_ENCODE(major, minor, patch))
#else
#  define LIBRG_TI_ARMCL_VERSION_CHECK(major,minor,patch) (0)
#endif

#if defined(LIBRG_TI_CL6X_VERSION)
#  undef LIBRG_TI_CL6X_VERSION
#endif
#if defined(__TI_COMPILER_VERSION__) && defined(__TMS320C6X__)
#  define LIBRG_TI_CL6X_VERSION LIBRG_VERSION_ENCODE(__TI_COMPILER_VERSION__ / 1000000, (__TI_COMPILER_VERSION__ % 1000000) / 1000, (__TI_COMPILER_VERSION__ % 1000))
#endif

#if defined(LIBRG_TI_CL6X_VERSION_CHECK)
#  undef LIBRG_TI_CL6X_VERSION_CHECK
#endif
#if defined(LIBRG_TI_CL6X_VERSION)
#  define LIBRG_TI_CL6X_VERSION_CHECK(major,minor,patch) (LIBRG_TI_CL6X_VERSION >= LIBRG_VERSION_ENCODE(major, minor, patch))
#else
#  define LIBRG_TI_CL6X_VERSION_CHECK(major,minor,patch) (0)
#endif

#if defined(LIBRG_TI_CL7X_VERSION)
#  undef LIBRG_TI_CL7X_VERSION
#endif
#if defined(__TI_COMPILER_VERSION__) && defined(__C7000__)
#  define LIBRG_TI_CL7X_VERSION LIBRG_VERSION_ENCODE(__TI_COMPILER_VERSION__ / 1000000, (__TI_COMPILER_VERSION__ % 1000000) / 1000, (__TI_COMPILER_VERSION__ % 1000))
#endif

#if defined(LIBRG_TI_CL7X_VERSION_CHECK)
#  undef LIBRG_TI_CL7X_VERSION_CHECK
#endif
#if defined(LIBRG_TI_CL7X_VERSION)
#  define LIBRG_TI_CL7X_VERSION_CHECK(major,minor,patch) (LIBRG_TI_CL7X_VERSION >= LIBRG_VERSION_ENCODE(major, minor, patch))
#else
#  define LIBRG_TI_CL7X_VERSION_CHECK(major,minor,patch) (0)
#endif

#if defined(LIBRG_TI_CLPRU_VERSION)
#  undef LIBRG_TI_CLPRU_VERSION
#endif
#if defined(__TI_COMPILER_VERSION__) && defined(__PRU__)
#  define LIBRG_TI_CLPRU_VERSION LIBRG_VERSION_ENCODE(__TI_COMPILER_VERSION__ / 1000000, (__TI_COMPILER_VERSION__ % 1000000) / 1000, (__TI_COMPILER_VERSION__ % 1000))
#endif

#if defined(LIBRG_TI_CLPRU_VERSION_CHECK)
#  undef LIBRG_TI_CLPRU_VERSION_CHECK
#endif
#if defined(LIBRG_TI_CLPRU_VERSION)
#  define LIBRG_TI_CLPRU_VERSION_CHECK(major,minor,patch) (LIBRG_TI_CLPRU_VERSION >= LIBRG_VERSION_ENCODE(major, minor, patch))
#else
#  define LIBRG_TI_CLPRU_VERSION_CHECK(major,minor,patch) (0)
#endif

#if defined(LIBRG_CRAY_VERSION)
#  undef LIBRG_CRAY_VERSION
#endif
#if defined(_CRAYC)
#  if defined(_RELEASE_PATCHLEVEL)
#    define LIBRG_CRAY_VERSION LIBRG_VERSION_ENCODE(_RELEASE_MAJOR, _RELEASE_MINOR, _RELEASE_PATCHLEVEL)
#  else
#    define LIBRG_CRAY_VERSION LIBRG_VERSION_ENCODE(_RELEASE_MAJOR, _RELEASE_MINOR, 0)
#  endif
#endif

#if defined(LIBRG_CRAY_VERSION_CHECK)
#  undef LIBRG_CRAY_VERSION_CHECK
#endif
#if defined(LIBRG_CRAY_VERSION)
#  define LIBRG_CRAY_VERSION_CHECK(major,minor,patch) (LIBRG_CRAY_VERSION >= LIBRG_VERSION_ENCODE(major, minor, patch))
#else
#  define LIBRG_CRAY_VERSION_CHECK(major,minor,patch) (0)
#endif

#if defined(LIBRG_IAR_VERSION)
#  undef LIBRG_IAR_VERSION
#endif
#if defined(__IAR_SYSTEMS_ICC__)
#  if __VER__ > 1000
#    define LIBRG_IAR_VERSION LIBRG_VERSION_ENCODE((__VER__ / 1000000), ((__VER__ / 1000) % 1000), (__VER__ % 1000))
#  else
#    define LIBRG_IAR_VERSION LIBRG_VERSION_ENCODE(VER / 100, __VER__ % 100, 0)
#  endif
#endif

#if defined(LIBRG_IAR_VERSION_CHECK)
#  undef LIBRG_IAR_VERSION_CHECK
#endif
#if defined(LIBRG_IAR_VERSION)
#  define LIBRG_IAR_VERSION_CHECK(major,minor,patch) (LIBRG_IAR_VERSION >= LIBRG_VERSION_ENCODE(major, minor, patch))
#else
#  define LIBRG_IAR_VERSION_CHECK(major,minor,patch) (0)
#endif

#if defined(LIBRG_TINYC_VERSION)
#  undef LIBRG_TINYC_VERSION
#endif
#if defined(__TINYC__)
#  define LIBRG_TINYC_VERSION LIBRG_VERSION_ENCODE(__TINYC__ / 1000, (__TINYC__ / 100) % 10, __TINYC__ % 100)
#endif

#if defined(LIBRG_TINYC_VERSION_CHECK)
#  undef LIBRG_TINYC_VERSION_CHECK
#endif
#if defined(LIBRG_TINYC_VERSION)
#  define LIBRG_TINYC_VERSION_CHECK(major,minor,patch) (LIBRG_TINYC_VERSION >= LIBRG_VERSION_ENCODE(major, minor, patch))
#else
#  define LIBRG_TINYC_VERSION_CHECK(major,minor,patch) (0)
#endif

#if defined(LIBRG_DMC_VERSION)
#  undef LIBRG_DMC_VERSION
#endif
#if defined(__DMC__)
#  define LIBRG_DMC_VERSION LIBRG_VERSION_ENCODE(__DMC__ >> 8, (__DMC__ >> 4) & 0xf, __DMC__ & 0xf)
#endif

#if defined(LIBRG_DMC_VERSION_CHECK)
#  undef LIBRG_DMC_VERSION_CHECK
#endif
#if defined(LIBRG_DMC_VERSION)
#  define LIBRG_DMC_VERSION_CHECK(major,minor,patch) (LIBRG_DMC_VERSION >= LIBRG_VERSION_ENCODE(major, minor, patch))
#else
#  define LIBRG_DMC_VERSION_CHECK(major,minor,patch) (0)
#endif

#if defined(LIBRG_COMPCERT_VERSION)
#  undef LIBRG_COMPCERT_VERSION
#endif
#if defined(__COMPCERT_VERSION__)
#  define LIBRG_COMPCERT_VERSION LIBRG_VERSION_ENCODE(__COMPCERT_VERSION__ / 10000, (__COMPCERT_VERSION__ / 100) % 100, __COMPCERT_VERSION__ % 100)
#endif

#if defined(LIBRG_COMPCERT_VERSION_CHECK)
#  undef LIBRG_COMPCERT_VERSION_CHECK
#endif
#if defined(LIBRG_COMPCERT_VERSION)
#  define LIBRG_COMPCERT_VERSION_CHECK(major,minor,patch) (LIBRG_COMPCERT_VERSION >= LIBRG_VERSION_ENCODE(major, minor, patch))
#else
#  define LIBRG_COMPCERT_VERSION_CHECK(major,minor,patch) (0)
#endif

#if defined(LIBRG_PELLES_VERSION)
#  undef LIBRG_PELLES_VERSION
#endif
#if defined(__POCC__)
#  define LIBRG_PELLES_VERSION LIBRG_VERSION_ENCODE(__POCC__ / 100, __POCC__ % 100, 0)
#endif

#if defined(LIBRG_PELLES_VERSION_CHECK)
#  undef LIBRG_PELLES_VERSION_CHECK
#endif
#if defined(LIBRG_PELLES_VERSION)
#  define LIBRG_PELLES_VERSION_CHECK(major,minor,patch) (LIBRG_PELLES_VERSION >= LIBRG_VERSION_ENCODE(major, minor, patch))
#else
#  define LIBRG_PELLES_VERSION_CHECK(major,minor,patch) (0)
#endif

#if defined(LIBRG_GCC_VERSION)
#  undef LIBRG_GCC_VERSION
#endif
#if \
  defined(LIBRG_GNUC_VERSION) && \
  !defined(__clang__) && \
  !defined(LIBRG_INTEL_VERSION) && \
  !defined(LIBRG_PGI_VERSION) && \
  !defined(LIBRG_ARM_VERSION) && \
  !defined(LIBRG_TI_VERSION) && \
  !defined(LIBRG_TI_ARMCL_VERSION) && \
  !defined(LIBRG_TI_CL430_VERSION) && \
  !defined(LIBRG_TI_CL2000_VERSION) && \
  !defined(LIBRG_TI_CL6X_VERSION) && \
  !defined(LIBRG_TI_CL7X_VERSION) && \
  !defined(LIBRG_TI_CLPRU_VERSION) && \
  !defined(__COMPCERT__)
#  define LIBRG_GCC_VERSION LIBRG_GNUC_VERSION
#endif

#if defined(LIBRG_GCC_VERSION_CHECK)
#  undef LIBRG_GCC_VERSION_CHECK
#endif
#if defined(LIBRG_GCC_VERSION)
#  define LIBRG_GCC_VERSION_CHECK(major,minor,patch) (LIBRG_GCC_VERSION >= LIBRG_VERSION_ENCODE(major, minor, patch))
#else
#  define LIBRG_GCC_VERSION_CHECK(major,minor,patch) (0)
#endif

#if defined(LIBRG_HAS_ATTRIBUTE)
#  undef LIBRG_HAS_ATTRIBUTE
#endif
#if defined(__has_attribute)
#  define LIBRG_HAS_ATTRIBUTE(attribute) __has_attribute(attribute)
#else
#  define LIBRG_HAS_ATTRIBUTE(attribute) (0)
#endif

#if defined(LIBRG_GNUC_HAS_ATTRIBUTE)
#  undef LIBRG_GNUC_HAS_ATTRIBUTE
#endif
#if defined(__has_attribute)
#  define LIBRG_GNUC_HAS_ATTRIBUTE(attribute,major,minor,patch) __has_attribute(attribute)
#else
#  define LIBRG_GNUC_HAS_ATTRIBUTE(attribute,major,minor,patch) LIBRG_GNUC_VERSION_CHECK(major,minor,patch)
#endif

#if defined(LIBRG_GCC_HAS_ATTRIBUTE)
#  undef LIBRG_GCC_HAS_ATTRIBUTE
#endif
#if defined(__has_attribute)
#  define LIBRG_GCC_HAS_ATTRIBUTE(attribute,major,minor,patch) __has_attribute(attribute)
#else
#  define LIBRG_GCC_HAS_ATTRIBUTE(attribute,major,minor,patch) LIBRG_GCC_VERSION_CHECK(major,minor,patch)
#endif

#if defined(LIBRG_HAS_CPP_ATTRIBUTE)
#  undef LIBRG_HAS_CPP_ATTRIBUTE
#endif
#if \
  defined(__has_cpp_attribute) && \
  defined(__cplusplus) && \
  (!defined(LIBRG_SUNPRO_VERSION) || LIBRG_SUNPRO_VERSION_CHECK(5,15,0))
#  define LIBRG_HAS_CPP_ATTRIBUTE(attribute) __has_cpp_attribute(attribute)
#else
#  define LIBRG_HAS_CPP_ATTRIBUTE(attribute) (0)
#endif

#if defined(LIBRG_HAS_CPP_ATTRIBUTE_NS)
#  undef LIBRG_HAS_CPP_ATTRIBUTE_NS
#endif
#if !defined(__cplusplus) || !defined(__has_cpp_attribute)
#  define LIBRG_HAS_CPP_ATTRIBUTE_NS(ns,attribute) (0)
#elif \
  !defined(LIBRG_PGI_VERSION) && \
  !defined(LIBRG_IAR_VERSION) && \
  (!defined(LIBRG_SUNPRO_VERSION) || LIBRG_SUNPRO_VERSION_CHECK(5,15,0)) && \
  (!defined(LIBRG_MSVC_VERSION) || LIBRG_MSVC_VERSION_CHECK(19,20,0))
#  define LIBRG_HAS_CPP_ATTRIBUTE_NS(ns,attribute) LIBRG_HAS_CPP_ATTRIBUTE(ns::attribute)
#else
#  define LIBRG_HAS_CPP_ATTRIBUTE_NS(ns,attribute) (0)
#endif

#if defined(LIBRG_GNUC_HAS_CPP_ATTRIBUTE)
#  undef LIBRG_GNUC_HAS_CPP_ATTRIBUTE
#endif
#if defined(__has_cpp_attribute) && defined(__cplusplus)
#  define LIBRG_GNUC_HAS_CPP_ATTRIBUTE(attribute,major,minor,patch) __has_cpp_attribute(attribute)
#else
#  define LIBRG_GNUC_HAS_CPP_ATTRIBUTE(attribute,major,minor,patch) LIBRG_GNUC_VERSION_CHECK(major,minor,patch)
#endif

#if defined(LIBRG_GCC_HAS_CPP_ATTRIBUTE)
#  undef LIBRG_GCC_HAS_CPP_ATTRIBUTE
#endif
#if defined(__has_cpp_attribute) && defined(__cplusplus)
#  define LIBRG_GCC_HAS_CPP_ATTRIBUTE(attribute,major,minor,patch) __has_cpp_attribute(attribute)
#else
#  define LIBRG_GCC_HAS_CPP_ATTRIBUTE(attribute,major,minor,patch) LIBRG_GCC_VERSION_CHECK(major,minor,patch)
#endif

#if defined(LIBRG_HAS_BUILTIN)
#  undef LIBRG_HAS_BUILTIN
#endif
#if defined(__has_builtin)
#  define LIBRG_HAS_BUILTIN(builtin) __has_builtin(builtin)
#else
#  define LIBRG_HAS_BUILTIN(builtin) (0)
#endif

#if defined(LIBRG_GNUC_HAS_BUILTIN)
#  undef LIBRG_GNUC_HAS_BUILTIN
#endif
#if defined(__has_builtin)
#  define LIBRG_GNUC_HAS_BUILTIN(builtin,major,minor,patch) __has_builtin(builtin)
#else
#  define LIBRG_GNUC_HAS_BUILTIN(builtin,major,minor,patch) LIBRG_GNUC_VERSION_CHECK(major,minor,patch)
#endif

#if defined(LIBRG_GCC_HAS_BUILTIN)
#  undef LIBRG_GCC_HAS_BUILTIN
#endif
#if defined(__has_builtin)
#  define LIBRG_GCC_HAS_BUILTIN(builtin,major,minor,patch) __has_builtin(builtin)
#else
#  define LIBRG_GCC_HAS_BUILTIN(builtin,major,minor,patch) LIBRG_GCC_VERSION_CHECK(major,minor,patch)
#endif

#if defined(LIBRG_HAS_FEATURE)
#  undef LIBRG_HAS_FEATURE
#endif
#if defined(__has_feature)
#  define LIBRG_HAS_FEATURE(feature) __has_feature(feature)
#else
#  define LIBRG_HAS_FEATURE(feature) (0)
#endif

#if defined(LIBRG_GNUC_HAS_FEATURE)
#  undef LIBRG_GNUC_HAS_FEATURE
#endif
#if defined(__has_feature)
#  define LIBRG_GNUC_HAS_FEATURE(feature,major,minor,patch) __has_feature(feature)
#else
#  define LIBRG_GNUC_HAS_FEATURE(feature,major,minor,patch) LIBRG_GNUC_VERSION_CHECK(major,minor,patch)
#endif

#if defined(LIBRG_GCC_HAS_FEATURE)
#  undef LIBRG_GCC_HAS_FEATURE
#endif
#if defined(__has_feature)
#  define LIBRG_GCC_HAS_FEATURE(feature,major,minor,patch) __has_feature(feature)
#else
#  define LIBRG_GCC_HAS_FEATURE(feature,major,minor,patch) LIBRG_GCC_VERSION_CHECK(major,minor,patch)
#endif

#if defined(LIBRG_HAS_EXTENSION)
#  undef LIBRG_HAS_EXTENSION
#endif
#if defined(__has_extension)
#  define LIBRG_HAS_EXTENSION(extension) __has_extension(extension)
#else
#  define LIBRG_HAS_EXTENSION(extension) (0)
#endif

#if defined(LIBRG_GNUC_HAS_EXTENSION)
#  undef LIBRG_GNUC_HAS_EXTENSION
#endif
#if defined(__has_extension)
#  define LIBRG_GNUC_HAS_EXTENSION(extension,major,minor,patch) __has_extension(extension)
#else
#  define LIBRG_GNUC_HAS_EXTENSION(extension,major,minor,patch) LIBRG_GNUC_VERSION_CHECK(major,minor,patch)
#endif

#if defined(LIBRG_GCC_HAS_EXTENSION)
#  undef LIBRG_GCC_HAS_EXTENSION
#endif
#if defined(__has_extension)
#  define LIBRG_GCC_HAS_EXTENSION(extension,major,minor,patch) __has_extension(extension)
#else
#  define LIBRG_GCC_HAS_EXTENSION(extension,major,minor,patch) LIBRG_GCC_VERSION_CHECK(major,minor,patch)
#endif

#if defined(LIBRG_HAS_DECLSPEC_ATTRIBUTE)
#  undef LIBRG_HAS_DECLSPEC_ATTRIBUTE
#endif
#if defined(__has_declspec_attribute)
#  define LIBRG_HAS_DECLSPEC_ATTRIBUTE(attribute) __has_declspec_attribute(attribute)
#else
#  define LIBRG_HAS_DECLSPEC_ATTRIBUTE(attribute) (0)
#endif

#if defined(LIBRG_GNUC_HAS_DECLSPEC_ATTRIBUTE)
#  undef LIBRG_GNUC_HAS_DECLSPEC_ATTRIBUTE
#endif
#if defined(__has_declspec_attribute)
#  define LIBRG_GNUC_HAS_DECLSPEC_ATTRIBUTE(attribute,major,minor,patch) __has_declspec_attribute(attribute)
#else
#  define LIBRG_GNUC_HAS_DECLSPEC_ATTRIBUTE(attribute,major,minor,patch) LIBRG_GNUC_VERSION_CHECK(major,minor,patch)
#endif

#if defined(LIBRG_GCC_HAS_DECLSPEC_ATTRIBUTE)
#  undef LIBRG_GCC_HAS_DECLSPEC_ATTRIBUTE
#endif
#if defined(__has_declspec_attribute)
#  define LIBRG_GCC_HAS_DECLSPEC_ATTRIBUTE(attribute,major,minor,patch) __has_declspec_attribute(attribute)
#else
#  define LIBRG_GCC_HAS_DECLSPEC_ATTRIBUTE(attribute,major,minor,patch) LIBRG_GCC_VERSION_CHECK(major,minor,patch)
#endif

#if defined(LIBRG_HAS_WARNING)
#  undef LIBRG_HAS_WARNING
#endif
#if defined(__has_warning)
#  define LIBRG_HAS_WARNING(warning) __has_warning(warning)
#else
#  define LIBRG_HAS_WARNING(warning) (0)
#endif

#if defined(LIBRG_GNUC_HAS_WARNING)
#  undef LIBRG_GNUC_HAS_WARNING
#endif
#if defined(__has_warning)
#  define LIBRG_GNUC_HAS_WARNING(warning,major,minor,patch) __has_warning(warning)
#else
#  define LIBRG_GNUC_HAS_WARNING(warning,major,minor,patch) LIBRG_GNUC_VERSION_CHECK(major,minor,patch)
#endif

#if defined(LIBRG_GCC_HAS_WARNING)
#  undef LIBRG_GCC_HAS_WARNING
#endif
#if defined(__has_warning)
#  define LIBRG_GCC_HAS_WARNING(warning,major,minor,patch) __has_warning(warning)
#else
#  define LIBRG_GCC_HAS_WARNING(warning,major,minor,patch) LIBRG_GCC_VERSION_CHECK(major,minor,patch)
#endif

/* LIBRG_DIAGNOSTIC_DISABLE_CPP98_COMPAT_WRAP_ is for
   LIBRG INTERNAL USE ONLY.  API subject to change without notice. */
#if defined(LIBRG_DIAGNOSTIC_DISABLE_CPP98_COMPAT_WRAP_)
#  undef LIBRG_DIAGNOSTIC_DISABLE_CPP98_COMPAT_WRAP_
#endif
#if defined(__cplusplus)
#  if LIBRG_HAS_WARNING("-Wc++98-compat")
#    if LIBRG_HAS_WARNING("-Wc++17-extensions")
#      define LIBRG_DIAGNOSTIC_DISABLE_CPP98_COMPAT_WRAP_(xpr) \
         LIBRG_DIAGNOSTIC_PUSH \
         _Pragma("clang diagnostic ignored \"-Wc++98-compat\"") \
         _Pragma("clang diagnostic ignored \"-Wc++17-extensions\"") \
         xpr \
         LIBRG_DIAGNOSTIC_POP
#    else
#      define LIBRG_DIAGNOSTIC_DISABLE_CPP98_COMPAT_WRAP_(xpr) \
         LIBRG_DIAGNOSTIC_PUSH \
         _Pragma("clang diagnostic ignored \"-Wc++98-compat\"") \
         xpr \
         LIBRG_DIAGNOSTIC_POP
#    endif
#  endif
#endif
#if !defined(LIBRG_DIAGNOSTIC_DISABLE_CPP98_COMPAT_WRAP_)
#  define LIBRG_DIAGNOSTIC_DISABLE_CPP98_COMPAT_WRAP_(x) x
#endif

#if defined(LIBRG_CONST_CAST)
#  undef LIBRG_CONST_CAST
#endif
#if defined(__cplusplus)
#  define LIBRG_CONST_CAST(T, expr) (const_cast<T>(expr))
#elif \
  LIBRG_HAS_WARNING("-Wcast-qual") || \
  LIBRG_GCC_VERSION_CHECK(4,6,0) || \
  LIBRG_INTEL_VERSION_CHECK(13,0,0)
#  define LIBRG_CONST_CAST(T, expr) (__extension__ ({ \
      LIBRG_DIAGNOSTIC_PUSH \
      LIBRG_DIAGNOSTIC_DISABLE_CAST_QUAL \
      ((T) (expr)); \
      LIBRG_DIAGNOSTIC_POP \
    }))
#else
#  define LIBRG_CONST_CAST(T, expr) ((T) (expr))
#endif

#if defined(LIBRG_REINTERPRET_CAST)
#  undef LIBRG_REINTERPRET_CAST
#endif
#if defined(__cplusplus)
#  define LIBRG_REINTERPRET_CAST(T, expr) (reinterpret_cast<T>(expr))
#else
#  define LIBRG_REINTERPRET_CAST(T, expr) ((T) (expr))
#endif

#if defined(LIBRG_STATIC_CAST)
#  undef LIBRG_STATIC_CAST
#endif
#if defined(__cplusplus)
#  define LIBRG_STATIC_CAST(T, expr) (static_cast<T>(expr))
#else
#  define LIBRG_STATIC_CAST(T, expr) ((T) (expr))
#endif

#if defined(LIBRG_CPP_CAST)
#  undef LIBRG_CPP_CAST
#endif
#if defined(__cplusplus)
#  if LIBRG_HAS_WARNING("-Wold-style-cast")
#    define LIBRG_CPP_CAST(T, expr) \
       LIBRG_DIAGNOSTIC_PUSH \
       _Pragma("clang diagnostic ignored \"-Wold-style-cast\"") \
       ((T) (expr)) \
       LIBRG_DIAGNOSTIC_POP
#  elif LIBRG_IAR_VERSION_CHECK(8,3,0)
#    define LIBRG_CPP_CAST(T, expr) \
       LIBRG_DIAGNOSTIC_PUSH \
       _Pragma("diag_suppress=Pe137") \
       LIBRG_DIAGNOSTIC_POP \
#  else
#    define LIBRG_CPP_CAST(T, expr) ((T) (expr))
#  endif
#else
#  define LIBRG_CPP_CAST(T, expr) (expr)
#endif

#if \
  (defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)) || \
  defined(__clang__) || \
  LIBRG_GCC_VERSION_CHECK(3,0,0) || \
  LIBRG_INTEL_VERSION_CHECK(13,0,0) || \
  LIBRG_IAR_VERSION_CHECK(8,0,0) || \
  LIBRG_PGI_VERSION_CHECK(18,4,0) || \
  LIBRG_ARM_VERSION_CHECK(4,1,0) || \
  LIBRG_TI_VERSION_CHECK(15,12,0) || \
  LIBRG_TI_ARMCL_VERSION_CHECK(4,7,0) || \
  LIBRG_TI_CL430_VERSION_CHECK(2,0,1) || \
  LIBRG_TI_CL2000_VERSION_CHECK(6,1,0) || \
  LIBRG_TI_CL6X_VERSION_CHECK(7,0,0) || \
  LIBRG_TI_CL7X_VERSION_CHECK(1,2,0) || \
  LIBRG_TI_CLPRU_VERSION_CHECK(2,1,0) || \
  LIBRG_CRAY_VERSION_CHECK(5,0,0) || \
  LIBRG_TINYC_VERSION_CHECK(0,9,17) || \
  LIBRG_SUNPRO_VERSION_CHECK(8,0,0) || \
  (LIBRG_IBM_VERSION_CHECK(10,1,0) && defined(__C99_PRAGMA_OPERATOR))
#  define LIBRG_PRAGMA(value) _Pragma(#value)
#elif LIBRG_MSVC_VERSION_CHECK(15,0,0)
#  define LIBRG_PRAGMA(value) __pragma(value)
#else
#  define LIBRG_PRAGMA(value)
#endif

#if defined(LIBRG_DIAGNOSTIC_PUSH)
#  undef LIBRG_DIAGNOSTIC_PUSH
#endif
#if defined(LIBRG_DIAGNOSTIC_POP)
#  undef LIBRG_DIAGNOSTIC_POP
#endif
#if defined(__clang__)
#  define LIBRG_DIAGNOSTIC_PUSH _Pragma("clang diagnostic push")
#  define LIBRG_DIAGNOSTIC_POP _Pragma("clang diagnostic pop")
#elif LIBRG_INTEL_VERSION_CHECK(13,0,0)
#  define LIBRG_DIAGNOSTIC_PUSH _Pragma("warning(push)")
#  define LIBRG_DIAGNOSTIC_POP _Pragma("warning(pop)")
#elif LIBRG_GCC_VERSION_CHECK(4,6,0)
#  define LIBRG_DIAGNOSTIC_PUSH _Pragma("GCC diagnostic push")
#  define LIBRG_DIAGNOSTIC_POP _Pragma("GCC diagnostic pop")
#elif LIBRG_MSVC_VERSION_CHECK(15,0,0)
#  define LIBRG_DIAGNOSTIC_PUSH __pragma(warning(push))
#  define LIBRG_DIAGNOSTIC_POP __pragma(warning(pop))
#elif LIBRG_ARM_VERSION_CHECK(5,6,0)
#  define LIBRG_DIAGNOSTIC_PUSH _Pragma("push")
#  define LIBRG_DIAGNOSTIC_POP _Pragma("pop")
#elif \
    LIBRG_TI_VERSION_CHECK(15,12,0) || \
    LIBRG_TI_ARMCL_VERSION_CHECK(5,2,0) || \
    LIBRG_TI_CL430_VERSION_CHECK(4,4,0) || \
    LIBRG_TI_CL6X_VERSION_CHECK(8,1,0) || \
    LIBRG_TI_CL7X_VERSION_CHECK(1,2,0) || \
    LIBRG_TI_CLPRU_VERSION_CHECK(2,1,0)
#  define LIBRG_DIAGNOSTIC_PUSH _Pragma("diag_push")
#  define LIBRG_DIAGNOSTIC_POP _Pragma("diag_pop")
#elif LIBRG_PELLES_VERSION_CHECK(2,90,0)
#  define LIBRG_DIAGNOSTIC_PUSH _Pragma("warning(push)")
#  define LIBRG_DIAGNOSTIC_POP _Pragma("warning(pop)")
#else
#  define LIBRG_DIAGNOSTIC_PUSH
#  define LIBRG_DIAGNOSTIC_POP
#endif

#if defined(LIBRG_DIAGNOSTIC_DISABLE_DEPRECATED)
#  undef LIBRG_DIAGNOSTIC_DISABLE_DEPRECATED
#endif
#if LIBRG_HAS_WARNING("-Wdeprecated-declarations")
#  define LIBRG_DIAGNOSTIC_DISABLE_DEPRECATED _Pragma("clang diagnostic ignored \"-Wdeprecated-declarations\"")
#elif LIBRG_INTEL_VERSION_CHECK(13,0,0)
#  define LIBRG_DIAGNOSTIC_DISABLE_DEPRECATED _Pragma("warning(disable:1478 1786)")
#elif LIBRG_PGI_VERSION_CHECK(17,10,0)
#  define LIBRG_DIAGNOSTIC_DISABLE_DEPRECATED _Pragma("diag_suppress 1215,1444")
#elif LIBRG_GCC_VERSION_CHECK(4,3,0)
#  define LIBRG_DIAGNOSTIC_DISABLE_DEPRECATED _Pragma("GCC diagnostic ignored \"-Wdeprecated-declarations\"")
#elif LIBRG_MSVC_VERSION_CHECK(15,0,0)
#  define LIBRG_DIAGNOSTIC_DISABLE_DEPRECATED __pragma(warning(disable:4996))
#elif \
    LIBRG_TI_VERSION_CHECK(15,12,0) || \
    (LIBRG_TI_ARMCL_VERSION_CHECK(4,8,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
    LIBRG_TI_ARMCL_VERSION_CHECK(5,2,0) || \
    (LIBRG_TI_CL2000_VERSION_CHECK(6,0,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
    LIBRG_TI_CL2000_VERSION_CHECK(6,4,0) || \
    (LIBRG_TI_CL430_VERSION_CHECK(4,0,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
    LIBRG_TI_CL430_VERSION_CHECK(4,3,0) || \
    (LIBRG_TI_CL6X_VERSION_CHECK(7,2,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
    LIBRG_TI_CL6X_VERSION_CHECK(7,5,0) || \
    LIBRG_TI_CL7X_VERSION_CHECK(1,2,0) || \
    LIBRG_TI_CLPRU_VERSION_CHECK(2,1,0)
#  define LIBRG_DIAGNOSTIC_DISABLE_DEPRECATED _Pragma("diag_suppress 1291,1718")
#elif LIBRG_SUNPRO_VERSION_CHECK(5,13,0) && !defined(__cplusplus)
#  define LIBRG_DIAGNOSTIC_DISABLE_DEPRECATED _Pragma("error_messages(off,E_DEPRECATED_ATT,E_DEPRECATED_ATT_MESS)")
#elif LIBRG_SUNPRO_VERSION_CHECK(5,13,0) && defined(__cplusplus)
#  define LIBRG_DIAGNOSTIC_DISABLE_DEPRECATED _Pragma("error_messages(off,symdeprecated,symdeprecated2)")
#elif LIBRG_IAR_VERSION_CHECK(8,0,0)
#  define LIBRG_DIAGNOSTIC_DISABLE_DEPRECATED _Pragma("diag_suppress=Pe1444,Pe1215")
#elif LIBRG_PELLES_VERSION_CHECK(2,90,0)
#  define LIBRG_DIAGNOSTIC_DISABLE_DEPRECATED _Pragma("warn(disable:2241)")
#else
#  define LIBRG_DIAGNOSTIC_DISABLE_DEPRECATED
#endif

#if defined(LIBRG_DIAGNOSTIC_DISABLE_UNKNOWN_PRAGMAS)
#  undef LIBRG_DIAGNOSTIC_DISABLE_UNKNOWN_PRAGMAS
#endif
#if LIBRG_HAS_WARNING("-Wunknown-pragmas")
#  define LIBRG_DIAGNOSTIC_DISABLE_UNKNOWN_PRAGMAS _Pragma("clang diagnostic ignored \"-Wunknown-pragmas\"")
#elif LIBRG_INTEL_VERSION_CHECK(13,0,0)
#  define LIBRG_DIAGNOSTIC_DISABLE_UNKNOWN_PRAGMAS _Pragma("warning(disable:161)")
#elif LIBRG_PGI_VERSION_CHECK(17,10,0)
#  define LIBRG_DIAGNOSTIC_DISABLE_UNKNOWN_PRAGMAS _Pragma("diag_suppress 1675")
#elif LIBRG_GCC_VERSION_CHECK(4,3,0)
#  define LIBRG_DIAGNOSTIC_DISABLE_UNKNOWN_PRAGMAS _Pragma("GCC diagnostic ignored \"-Wunknown-pragmas\"")
#elif LIBRG_MSVC_VERSION_CHECK(15,0,0)
#  define LIBRG_DIAGNOSTIC_DISABLE_UNKNOWN_PRAGMAS __pragma(warning(disable:4068))
#elif \
    LIBRG_TI_VERSION_CHECK(16,9,0) || \
    LIBRG_TI_CL6X_VERSION_CHECK(8,0,0) || \
    LIBRG_TI_CL7X_VERSION_CHECK(1,2,0) || \
    LIBRG_TI_CLPRU_VERSION_CHECK(2,3,0)
#  define LIBRG_DIAGNOSTIC_DISABLE_UNKNOWN_PRAGMAS _Pragma("diag_suppress 163")
#elif LIBRG_TI_CL6X_VERSION_CHECK(8,0,0)
#  define LIBRG_DIAGNOSTIC_DISABLE_UNKNOWN_PRAGMAS _Pragma("diag_suppress 163")
#elif LIBRG_IAR_VERSION_CHECK(8,0,0)
#  define LIBRG_DIAGNOSTIC_DISABLE_UNKNOWN_PRAGMAS _Pragma("diag_suppress=Pe161")
#else
#  define LIBRG_DIAGNOSTIC_DISABLE_UNKNOWN_PRAGMAS
#endif

#if defined(LIBRG_DIAGNOSTIC_DISABLE_UNKNOWN_CPP_ATTRIBUTES)
#  undef LIBRG_DIAGNOSTIC_DISABLE_UNKNOWN_CPP_ATTRIBUTES
#endif
#if LIBRG_HAS_WARNING("-Wunknown-attributes")
#  define LIBRG_DIAGNOSTIC_DISABLE_UNKNOWN_CPP_ATTRIBUTES _Pragma("clang diagnostic ignored \"-Wunknown-attributes\"")
#elif LIBRG_GCC_VERSION_CHECK(4,6,0)
#  define LIBRG_DIAGNOSTIC_DISABLE_UNKNOWN_CPP_ATTRIBUTES _Pragma("GCC diagnostic ignored \"-Wdeprecated-declarations\"")
#elif LIBRG_INTEL_VERSION_CHECK(17,0,0)
#  define LIBRG_DIAGNOSTIC_DISABLE_UNKNOWN_CPP_ATTRIBUTES _Pragma("warning(disable:1292)")
#elif LIBRG_MSVC_VERSION_CHECK(19,0,0)
#  define LIBRG_DIAGNOSTIC_DISABLE_UNKNOWN_CPP_ATTRIBUTES __pragma(warning(disable:5030))
#elif LIBRG_PGI_VERSION_CHECK(17,10,0)
#  define LIBRG_DIAGNOSTIC_DISABLE_UNKNOWN_CPP_ATTRIBUTES _Pragma("diag_suppress 1097")
#elif LIBRG_SUNPRO_VERSION_CHECK(5,14,0) && defined(__cplusplus)
#  define LIBRG_DIAGNOSTIC_DISABLE_UNKNOWN_CPP_ATTRIBUTES _Pragma("error_messages(off,attrskipunsup)")
#elif \
    LIBRG_TI_VERSION_CHECK(18,1,0) || \
    LIBRG_TI_CL6X_VERSION_CHECK(8,3,0) || \
    LIBRG_TI_CL7X_VERSION_CHECK(1,2,0)
#  define LIBRG_DIAGNOSTIC_DISABLE_UNKNOWN_CPP_ATTRIBUTES _Pragma("diag_suppress 1173")
#elif LIBRG_IAR_VERSION_CHECK(8,0,0)
#  define LIBRG_DIAGNOSTIC_DISABLE_UNKNOWN_CPP_ATTRIBUTES _Pragma("diag_suppress=Pe1097")
#else
#  define LIBRG_DIAGNOSTIC_DISABLE_UNKNOWN_CPP_ATTRIBUTES
#endif

#if defined(LIBRG_DIAGNOSTIC_DISABLE_CAST_QUAL)
#  undef LIBRG_DIAGNOSTIC_DISABLE_CAST_QUAL
#endif
#if LIBRG_HAS_WARNING("-Wcast-qual")
#  define LIBRG_DIAGNOSTIC_DISABLE_CAST_QUAL _Pragma("clang diagnostic ignored \"-Wcast-qual\"")
#elif LIBRG_INTEL_VERSION_CHECK(13,0,0)
#  define LIBRG_DIAGNOSTIC_DISABLE_CAST_QUAL _Pragma("warning(disable:2203 2331)")
#elif LIBRG_GCC_VERSION_CHECK(3,0,0)
#  define LIBRG_DIAGNOSTIC_DISABLE_CAST_QUAL _Pragma("GCC diagnostic ignored \"-Wcast-qual\"")
#else
#  define LIBRG_DIAGNOSTIC_DISABLE_CAST_QUAL
#endif

#if defined(LIBRG_DEPRECATED)
#  undef LIBRG_DEPRECATED
#endif
#if defined(LIBRG_DEPRECATED_FOR)
#  undef LIBRG_DEPRECATED_FOR
#endif
#if LIBRG_MSVC_VERSION_CHECK(14,0,0)
#  define LIBRG_DEPRECATED(since) __declspec(deprecated("Since " # since))
#  define LIBRG_DEPRECATED_FOR(since, replacement) __declspec(deprecated("Since " #since "; use " #replacement))
#elif defined(__cplusplus) && (__cplusplus >= 201402L)
#  define LIBRG_DEPRECATED(since) LIBRG_DIAGNOSTIC_DISABLE_CPP98_COMPAT_WRAP_([[deprecated("Since " #since)]])
#  define LIBRG_DEPRECATED_FOR(since, replacement) LIBRG_DIAGNOSTIC_DISABLE_CPP98_COMPAT_WRAP_([[deprecated("Since " #since "; use " #replacement)]])
#elif \
  LIBRG_HAS_EXTENSION(attribute_deprecated_with_message) || \
  LIBRG_GCC_VERSION_CHECK(4,5,0) || \
  LIBRG_INTEL_VERSION_CHECK(13,0,0) || \
  LIBRG_ARM_VERSION_CHECK(5,6,0) || \
  LIBRG_SUNPRO_VERSION_CHECK(5,13,0) || \
  LIBRG_PGI_VERSION_CHECK(17,10,0) || \
  LIBRG_TI_VERSION_CHECK(18,1,0) || \
  LIBRG_TI_ARMCL_VERSION_CHECK(18,1,0) || \
  LIBRG_TI_CL6X_VERSION_CHECK(8,3,0) || \
  LIBRG_TI_CL7X_VERSION_CHECK(1,2,0) || \
  LIBRG_TI_CLPRU_VERSION_CHECK(2,3,0)
#  define LIBRG_DEPRECATED(since) __attribute__((__deprecated__("Since " #since)))
#  define LIBRG_DEPRECATED_FOR(since, replacement) __attribute__((__deprecated__("Since " #since "; use " #replacement)))
#elif \
  LIBRG_HAS_ATTRIBUTE(deprecated) || \
  LIBRG_GCC_VERSION_CHECK(3,1,0) || \
  LIBRG_ARM_VERSION_CHECK(4,1,0) || \
  LIBRG_TI_VERSION_CHECK(15,12,0) || \
  (LIBRG_TI_ARMCL_VERSION_CHECK(4,8,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
  LIBRG_TI_ARMCL_VERSION_CHECK(5,2,0) || \
  (LIBRG_TI_CL2000_VERSION_CHECK(6,0,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
  LIBRG_TI_CL2000_VERSION_CHECK(6,4,0) || \
  (LIBRG_TI_CL430_VERSION_CHECK(4,0,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
  LIBRG_TI_CL430_VERSION_CHECK(4,3,0) || \
  (LIBRG_TI_CL6X_VERSION_CHECK(7,2,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
  LIBRG_TI_CL6X_VERSION_CHECK(7,5,0) || \
  LIBRG_TI_CL7X_VERSION_CHECK(1,2,0) || \
  LIBRG_TI_CLPRU_VERSION_CHECK(2,1,0)
#  define LIBRG_DEPRECATED(since) __attribute__((__deprecated__))
#  define LIBRG_DEPRECATED_FOR(since, replacement) __attribute__((__deprecated__))
#elif \
  LIBRG_MSVC_VERSION_CHECK(13,10,0) || \
  LIBRG_PELLES_VERSION_CHECK(6,50,0)
#  define LIBRG_DEPRECATED(since) __declspec(deprecated)
#  define LIBRG_DEPRECATED_FOR(since, replacement) __declspec(deprecated)
#elif LIBRG_IAR_VERSION_CHECK(8,0,0)
#  define LIBRG_DEPRECATED(since) _Pragma("deprecated")
#  define LIBRG_DEPRECATED_FOR(since, replacement) _Pragma("deprecated")
#else
#  define LIBRG_DEPRECATED(since)
#  define LIBRG_DEPRECATED_FOR(since, replacement)
#endif

#if defined(LIBRG_UNAVAILABLE)
#  undef LIBRG_UNAVAILABLE
#endif
#if \
  LIBRG_HAS_ATTRIBUTE(warning) || \
  LIBRG_GCC_VERSION_CHECK(4,3,0) || \
  LIBRG_INTEL_VERSION_CHECK(13,0,0)
#  define LIBRG_UNAVAILABLE(available_since) __attribute__((__warning__("Not available until " #available_since)))
#else
#  define LIBRG_UNAVAILABLE(available_since)
#endif

#if defined(LIBRG_WARN_UNUSED_RESULT)
#  undef LIBRG_WARN_UNUSED_RESULT
#endif
#if defined(LIBRG_WARN_UNUSED_RESULT_MSG)
#  undef LIBRG_WARN_UNUSED_RESULT_MSG
#endif
#if (LIBRG_HAS_CPP_ATTRIBUTE(nodiscard) >= 201907L)
#  define LIBRG_WARN_UNUSED_RESULT LIBRG_DIAGNOSTIC_DISABLE_CPP98_COMPAT_WRAP_([[nodiscard]])
#  define LIBRG_WARN_UNUSED_RESULT_MSG(msg) LIBRG_DIAGNOSTIC_DISABLE_CPP98_COMPAT_WRAP_([[nodiscard(msg)]])
#elif LIBRG_HAS_CPP_ATTRIBUTE(nodiscard)
#  define LIBRG_WARN_UNUSED_RESULT LIBRG_DIAGNOSTIC_DISABLE_CPP98_COMPAT_WRAP_([[nodiscard]])
#  define LIBRG_WARN_UNUSED_RESULT_MSG(msg) LIBRG_DIAGNOSTIC_DISABLE_CPP98_COMPAT_WRAP_([[nodiscard]])
#elif \
  LIBRG_HAS_ATTRIBUTE(warn_unused_result) || \
  LIBRG_GCC_VERSION_CHECK(3,4,0) || \
  LIBRG_INTEL_VERSION_CHECK(13,0,0) || \
  LIBRG_TI_VERSION_CHECK(15,12,0) || \
  (LIBRG_TI_ARMCL_VERSION_CHECK(4,8,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
  LIBRG_TI_ARMCL_VERSION_CHECK(5,2,0) || \
  (LIBRG_TI_CL2000_VERSION_CHECK(6,0,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
  LIBRG_TI_CL2000_VERSION_CHECK(6,4,0) || \
  (LIBRG_TI_CL430_VERSION_CHECK(4,0,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
  LIBRG_TI_CL430_VERSION_CHECK(4,3,0) || \
  (LIBRG_TI_CL6X_VERSION_CHECK(7,2,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
  LIBRG_TI_CL6X_VERSION_CHECK(7,5,0) || \
  LIBRG_TI_CL7X_VERSION_CHECK(1,2,0) || \
  LIBRG_TI_CLPRU_VERSION_CHECK(2,1,0) || \
  (LIBRG_SUNPRO_VERSION_CHECK(5,15,0) && defined(__cplusplus)) || \
  LIBRG_PGI_VERSION_CHECK(17,10,0)
#  define LIBRG_WARN_UNUSED_RESULT __attribute__((__warn_unused_result__))
#  define LIBRG_WARN_UNUSED_RESULT_MSG(msg) __attribute__((__warn_unused_result__))
#elif defined(_Check_return_) /* SAL */
#  define LIBRG_WARN_UNUSED_RESULT _Check_return_
#  define LIBRG_WARN_UNUSED_RESULT_MSG(msg) _Check_return_
#else
#  define LIBRG_WARN_UNUSED_RESULT
#  define LIBRG_WARN_UNUSED_RESULT_MSG(msg)
#endif

#if defined(LIBRG_SENTINEL)
#  undef LIBRG_SENTINEL
#endif
#if \
  LIBRG_HAS_ATTRIBUTE(sentinel) || \
  LIBRG_GCC_VERSION_CHECK(4,0,0) || \
  LIBRG_INTEL_VERSION_CHECK(13,0,0) || \
  LIBRG_ARM_VERSION_CHECK(5,4,0)
#  define LIBRG_SENTINEL(position) __attribute__((__sentinel__(position)))
#else
#  define LIBRG_SENTINEL(position)
#endif

#if defined(LIBRG_NO_RETURN)
#  undef LIBRG_NO_RETURN
#endif
#if LIBRG_IAR_VERSION_CHECK(8,0,0)
#  define LIBRG_NO_RETURN __noreturn
#elif LIBRG_INTEL_VERSION_CHECK(13,0,0)
#  define LIBRG_NO_RETURN __attribute__((__noreturn__))
#elif defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
#  define LIBRG_NO_RETURN _Noreturn
#elif defined(__cplusplus) && (__cplusplus >= 201103L)
#  define LIBRG_NO_RETURN LIBRG_DIAGNOSTIC_DISABLE_CPP98_COMPAT_WRAP_([[noreturn]])
#elif \
  LIBRG_HAS_ATTRIBUTE(noreturn) || \
  LIBRG_GCC_VERSION_CHECK(3,2,0) || \
  LIBRG_SUNPRO_VERSION_CHECK(5,11,0) || \
  LIBRG_ARM_VERSION_CHECK(4,1,0) || \
  LIBRG_IBM_VERSION_CHECK(10,1,0) || \
  LIBRG_TI_VERSION_CHECK(15,12,0) || \
  (LIBRG_TI_ARMCL_VERSION_CHECK(4,8,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
  LIBRG_TI_ARMCL_VERSION_CHECK(5,2,0) || \
  (LIBRG_TI_CL2000_VERSION_CHECK(6,0,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
  LIBRG_TI_CL2000_VERSION_CHECK(6,4,0) || \
  (LIBRG_TI_CL430_VERSION_CHECK(4,0,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
  LIBRG_TI_CL430_VERSION_CHECK(4,3,0) || \
  (LIBRG_TI_CL6X_VERSION_CHECK(7,2,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
  LIBRG_TI_CL6X_VERSION_CHECK(7,5,0) || \
  LIBRG_TI_CL7X_VERSION_CHECK(1,2,0) || \
  LIBRG_TI_CLPRU_VERSION_CHECK(2,1,0)
#  define LIBRG_NO_RETURN __attribute__((__noreturn__))
#elif LIBRG_SUNPRO_VERSION_CHECK(5,10,0)
#  define LIBRG_NO_RETURN _Pragma("does_not_return")
#elif LIBRG_MSVC_VERSION_CHECK(13,10,0)
#  define LIBRG_NO_RETURN __declspec(noreturn)
#elif LIBRG_TI_CL6X_VERSION_CHECK(6,0,0) && defined(__cplusplus)
#  define LIBRG_NO_RETURN _Pragma("FUNC_NEVER_RETURNS;")
#elif LIBRG_COMPCERT_VERSION_CHECK(3,2,0)
#  define LIBRG_NO_RETURN __attribute((noreturn))
#elif LIBRG_PELLES_VERSION_CHECK(9,0,0)
#  define LIBRG_NO_RETURN __declspec(noreturn)
#else
#  define LIBRG_NO_RETURN
#endif

#if defined(LIBRG_NO_ESCAPE)
#  undef LIBRG_NO_ESCAPE
#endif
#if LIBRG_HAS_ATTRIBUTE(noescape)
#  define LIBRG_NO_ESCAPE __attribute__((__noescape__))
#else
#  define LIBRG_NO_ESCAPE
#endif

#if defined(LIBRG_UNREACHABLE)
#  undef LIBRG_UNREACHABLE
#endif
#if defined(LIBRG_UNREACHABLE_RETURN)
#  undef LIBRG_UNREACHABLE_RETURN
#endif
#if defined(LIBRG_ASSUME)
#  undef LIBRG_ASSUME
#endif
#if \
  LIBRG_MSVC_VERSION_CHECK(13,10,0) || \
  LIBRG_INTEL_VERSION_CHECK(13,0,0)
#  define LIBRG_ASSUME(expr) __assume(expr)
#elif LIBRG_HAS_BUILTIN(__builtin_assume)
#  define LIBRG_ASSUME(expr) __builtin_assume(expr)
#elif \
    LIBRG_TI_CL2000_VERSION_CHECK(6,2,0) || \
    LIBRG_TI_CL6X_VERSION_CHECK(4,0,0)
#  if defined(__cplusplus)
#    define LIBRG_ASSUME(expr) std::_nassert(expr)
#  else
#    define LIBRG_ASSUME(expr) _nassert(expr)
#  endif
#endif
#if \
  (LIBRG_HAS_BUILTIN(__builtin_unreachable) && (!defined(LIBRG_ARM_VERSION))) || \
  LIBRG_GCC_VERSION_CHECK(4,5,0) || \
  LIBRG_PGI_VERSION_CHECK(18,10,0) || \
  LIBRG_INTEL_VERSION_CHECK(13,0,0) || \
  LIBRG_IBM_VERSION_CHECK(13,1,5)
#  define LIBRG_UNREACHABLE() __builtin_unreachable()
#elif defined(LIBRG_ASSUME)
#  define LIBRG_UNREACHABLE() LIBRG_ASSUME(0)
#endif
#if !defined(LIBRG_ASSUME)
#  if defined(LIBRG_UNREACHABLE)
#    define LIBRG_ASSUME(expr) LIBRG_STATIC_CAST(void, ((expr) ? 1 : (LIBRG_UNREACHABLE(), 1)))
#  else
#    define LIBRG_ASSUME(expr) LIBRG_STATIC_CAST(void, expr)
#  endif
#endif
#if defined(LIBRG_UNREACHABLE)
#  if  \
      LIBRG_TI_CL2000_VERSION_CHECK(6,2,0) || \
      LIBRG_TI_CL6X_VERSION_CHECK(4,0,0)
#    define LIBRG_UNREACHABLE_RETURN(value) return (LIBRG_STATIC_CAST(void, LIBRG_ASSUME(0)), (value))
#  else
#    define LIBRG_UNREACHABLE_RETURN(value) LIBRG_UNREACHABLE()
#  endif
#else
#  define LIBRG_UNREACHABLE_RETURN(value) return (value)
#endif
#if !defined(LIBRG_UNREACHABLE)
#  define LIBRG_UNREACHABLE() LIBRG_ASSUME(0)
#endif

LIBRG_DIAGNOSTIC_PUSH
#if LIBRG_HAS_WARNING("-Wpedantic")
#  pragma clang diagnostic ignored "-Wpedantic"
#endif
#if LIBRG_HAS_WARNING("-Wc++98-compat-pedantic") && defined(__cplusplus)
#  pragma clang diagnostic ignored "-Wc++98-compat-pedantic"
#endif
#if LIBRG_GCC_HAS_WARNING("-Wvariadic-macros",4,0,0)
#  if defined(__clang__)
#    pragma clang diagnostic ignored "-Wvariadic-macros"
#  elif defined(LIBRG_GCC_VERSION)
#    pragma GCC diagnostic ignored "-Wvariadic-macros"
#  endif
#endif
#if defined(LIBRG_NON_NULL)
#  undef LIBRG_NON_NULL
#endif
#if \
  LIBRG_HAS_ATTRIBUTE(nonnull) || \
  LIBRG_GCC_VERSION_CHECK(3,3,0) || \
  LIBRG_INTEL_VERSION_CHECK(13,0,0) || \
  LIBRG_ARM_VERSION_CHECK(4,1,0)
#  define LIBRG_NON_NULL(...) __attribute__((__nonnull__(__VA_ARGS__)))
#else
#  define LIBRG_NON_NULL(...)
#endif
LIBRG_DIAGNOSTIC_POP

#if defined(LIBRG_PRINTF_FORMAT)
#  undef LIBRG_PRINTF_FORMAT
#endif
#if defined(__MINGW32__) && LIBRG_GCC_HAS_ATTRIBUTE(format,4,4,0) && !defined(__USE_MINGW_ANSI_STDIO)
#  define LIBRG_PRINTF_FORMAT(string_idx,first_to_check) __attribute__((__format__(ms_printf, string_idx, first_to_check)))
#elif defined(__MINGW32__) && LIBRG_GCC_HAS_ATTRIBUTE(format,4,4,0) && defined(__USE_MINGW_ANSI_STDIO)
#  define LIBRG_PRINTF_FORMAT(string_idx,first_to_check) __attribute__((__format__(gnu_printf, string_idx, first_to_check)))
#elif \
  LIBRG_HAS_ATTRIBUTE(format) || \
  LIBRG_GCC_VERSION_CHECK(3,1,0) || \
  LIBRG_INTEL_VERSION_CHECK(13,0,0) || \
  LIBRG_ARM_VERSION_CHECK(5,6,0) || \
  LIBRG_IBM_VERSION_CHECK(10,1,0) || \
  LIBRG_TI_VERSION_CHECK(15,12,0) || \
  (LIBRG_TI_ARMCL_VERSION_CHECK(4,8,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
  LIBRG_TI_ARMCL_VERSION_CHECK(5,2,0) || \
  (LIBRG_TI_CL2000_VERSION_CHECK(6,0,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
  LIBRG_TI_CL2000_VERSION_CHECK(6,4,0) || \
  (LIBRG_TI_CL430_VERSION_CHECK(4,0,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
  LIBRG_TI_CL430_VERSION_CHECK(4,3,0) || \
  (LIBRG_TI_CL6X_VERSION_CHECK(7,2,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
  LIBRG_TI_CL6X_VERSION_CHECK(7,5,0) || \
  LIBRG_TI_CL7X_VERSION_CHECK(1,2,0) || \
  LIBRG_TI_CLPRU_VERSION_CHECK(2,1,0)
#  define LIBRG_PRINTF_FORMAT(string_idx,first_to_check) __attribute__((__format__(__printf__, string_idx, first_to_check)))
#elif LIBRG_PELLES_VERSION_CHECK(6,0,0)
#  define LIBRG_PRINTF_FORMAT(string_idx,first_to_check) __declspec(vaformat(printf,string_idx,first_to_check))
#else
#  define LIBRG_PRINTF_FORMAT(string_idx,first_to_check)
#endif

#if defined(LIBRG_CONSTEXPR)
#  undef LIBRG_CONSTEXPR
#endif
#if defined(__cplusplus)
#  if __cplusplus >= 201103L
#    define LIBRG_CONSTEXPR LIBRG_DIAGNOSTIC_DISABLE_CPP98_COMPAT_WRAP_(constexpr)
#  endif
#endif
#if !defined(LIBRG_CONSTEXPR)
#  define LIBRG_CONSTEXPR
#endif

#if defined(LIBRG_PREDICT)
#  undef LIBRG_PREDICT
#endif
#if defined(LIBRG_LIKELY)
#  undef LIBRG_LIKELY
#endif
#if defined(LIBRG_UNLIKELY)
#  undef LIBRG_UNLIKELY
#endif
#if defined(LIBRG_UNPREDICTABLE)
#  undef LIBRG_UNPREDICTABLE
#endif
#if LIBRG_HAS_BUILTIN(__builtin_unpredictable)
#  define LIBRG_UNPREDICTABLE(expr) __builtin_unpredictable((expr))
#endif
#if \
  LIBRG_HAS_BUILTIN(__builtin_expect_with_probability) || \
  LIBRG_GCC_VERSION_CHECK(9,0,0)
#  define LIBRG_PREDICT(expr, value, probability) __builtin_expect_with_probability(  (expr), (value), (probability))
#  define LIBRG_PREDICT_TRUE(expr, probability)   __builtin_expect_with_probability(!!(expr),    1   , (probability))
#  define LIBRG_PREDICT_FALSE(expr, probability)  __builtin_expect_with_probability(!!(expr),    0   , (probability))
#  define LIBRG_LIKELY(expr)                      __builtin_expect                 (!!(expr),    1                  )
#  define LIBRG_UNLIKELY(expr)                    __builtin_expect                 (!!(expr),    0                  )
#elif \
  LIBRG_HAS_BUILTIN(__builtin_expect) || \
  LIBRG_GCC_VERSION_CHECK(3,0,0) || \
  LIBRG_INTEL_VERSION_CHECK(13,0,0) || \
  (LIBRG_SUNPRO_VERSION_CHECK(5,15,0) && defined(__cplusplus)) || \
  LIBRG_ARM_VERSION_CHECK(4,1,0) || \
  LIBRG_IBM_VERSION_CHECK(10,1,0) || \
  LIBRG_TI_VERSION_CHECK(15,12,0) || \
  LIBRG_TI_ARMCL_VERSION_CHECK(4,7,0) || \
  LIBRG_TI_CL430_VERSION_CHECK(3,1,0) || \
  LIBRG_TI_CL2000_VERSION_CHECK(6,1,0) || \
  LIBRG_TI_CL6X_VERSION_CHECK(6,1,0) || \
  LIBRG_TI_CL7X_VERSION_CHECK(1,2,0) || \
  LIBRG_TI_CLPRU_VERSION_CHECK(2,1,0) || \
  LIBRG_TINYC_VERSION_CHECK(0,9,27) || \
  LIBRG_CRAY_VERSION_CHECK(8,1,0)
#  define LIBRG_PREDICT(expr, expected, probability) \
     (((probability) >= 0.9) ? __builtin_expect((expr), (expected)) : (LIBRG_STATIC_CAST(void, expected), (expr)))
#  define LIBRG_PREDICT_TRUE(expr, probability) \
     (__extension__ ({ \
       double librg_hedley_probability_ = (probability); \
       ((librg_hedley_probability_ >= 0.9) ? __builtin_expect(!!(expr), 1) : ((librg_hedley_probability_ <= 0.1) ? __builtin_expect(!!(expr), 0) : !!(expr))); \
     }))
#  define LIBRG_PREDICT_FALSE(expr, probability) \
     (__extension__ ({ \
       double librg_hedley_probability_ = (probability); \
       ((librg_hedley_probability_ >= 0.9) ? __builtin_expect(!!(expr), 0) : ((librg_hedley_probability_ <= 0.1) ? __builtin_expect(!!(expr), 1) : !!(expr))); \
     }))
#  define LIBRG_LIKELY(expr)   __builtin_expect(!!(expr), 1)
#  define LIBRG_UNLIKELY(expr) __builtin_expect(!!(expr), 0)
#else
#  define LIBRG_PREDICT(expr, expected, probability) (LIBRG_STATIC_CAST(void, expected), (expr))
#  define LIBRG_PREDICT_TRUE(expr, probability) (!!(expr))
#  define LIBRG_PREDICT_FALSE(expr, probability) (!!(expr))
#  define LIBRG_LIKELY(expr) (!!(expr))
#  define LIBRG_UNLIKELY(expr) (!!(expr))
#endif
#if !defined(LIBRG_UNPREDICTABLE)
#  define LIBRG_UNPREDICTABLE(expr) LIBRG_PREDICT(expr, 1, 0.5)
#endif

#if defined(LIBRG_MALLOC)
#  undef LIBRG_MALLOC
#endif
#if \
  LIBRG_HAS_ATTRIBUTE(malloc) || \
  LIBRG_GCC_VERSION_CHECK(3,1,0) || \
  LIBRG_INTEL_VERSION_CHECK(13,0,0) || \
  LIBRG_SUNPRO_VERSION_CHECK(5,11,0) || \
  LIBRG_ARM_VERSION_CHECK(4,1,0) || \
  LIBRG_IBM_VERSION_CHECK(12,1,0) || \
  LIBRG_TI_VERSION_CHECK(15,12,0) || \
  (LIBRG_TI_ARMCL_VERSION_CHECK(4,8,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
  LIBRG_TI_ARMCL_VERSION_CHECK(5,2,0) || \
  (LIBRG_TI_CL2000_VERSION_CHECK(6,0,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
  LIBRG_TI_CL2000_VERSION_CHECK(6,4,0) || \
  (LIBRG_TI_CL430_VERSION_CHECK(4,0,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
  LIBRG_TI_CL430_VERSION_CHECK(4,3,0) || \
  (LIBRG_TI_CL6X_VERSION_CHECK(7,2,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
  LIBRG_TI_CL6X_VERSION_CHECK(7,5,0) || \
  LIBRG_TI_CL7X_VERSION_CHECK(1,2,0) || \
  LIBRG_TI_CLPRU_VERSION_CHECK(2,1,0)
#  define LIBRG_MALLOC __attribute__((__malloc__))
#elif LIBRG_SUNPRO_VERSION_CHECK(5,10,0)
#  define LIBRG_MALLOC _Pragma("returns_new_memory")
#elif LIBRG_MSVC_VERSION_CHECK(14, 0, 0)
#  define LIBRG_MALLOC __declspec(restrict)
#else
#  define LIBRG_MALLOC
#endif

#if defined(LIBRG_PURE)
#  undef LIBRG_PURE
#endif
#if \
  LIBRG_HAS_ATTRIBUTE(pure) || \
  LIBRG_GCC_VERSION_CHECK(2,96,0) || \
  LIBRG_INTEL_VERSION_CHECK(13,0,0) || \
  LIBRG_SUNPRO_VERSION_CHECK(5,11,0) || \
  LIBRG_ARM_VERSION_CHECK(4,1,0) || \
  LIBRG_IBM_VERSION_CHECK(10,1,0) || \
  LIBRG_TI_VERSION_CHECK(15,12,0) || \
  (LIBRG_TI_ARMCL_VERSION_CHECK(4,8,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
  LIBRG_TI_ARMCL_VERSION_CHECK(5,2,0) || \
  (LIBRG_TI_CL2000_VERSION_CHECK(6,0,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
  LIBRG_TI_CL2000_VERSION_CHECK(6,4,0) || \
  (LIBRG_TI_CL430_VERSION_CHECK(4,0,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
  LIBRG_TI_CL430_VERSION_CHECK(4,3,0) || \
  (LIBRG_TI_CL6X_VERSION_CHECK(7,2,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
  LIBRG_TI_CL6X_VERSION_CHECK(7,5,0) || \
  LIBRG_TI_CL7X_VERSION_CHECK(1,2,0) || \
  LIBRG_TI_CLPRU_VERSION_CHECK(2,1,0) || \
  LIBRG_PGI_VERSION_CHECK(17,10,0)
#  define LIBRG_PURE __attribute__((__pure__))
#elif LIBRG_SUNPRO_VERSION_CHECK(5,10,0)
#  define LIBRG_PURE _Pragma("does_not_write_global_data")
#elif defined(__cplusplus) && \
    ( \
      LIBRG_TI_CL430_VERSION_CHECK(2,0,1) || \
      LIBRG_TI_CL6X_VERSION_CHECK(4,0,0) || \
      LIBRG_TI_CL7X_VERSION_CHECK(1,2,0) \
    )
#  define LIBRG_PURE _Pragma("FUNC_IS_PURE;")
#else
#  define LIBRG_PURE
#endif

#if defined(LIBRG_CONST)
#  undef LIBRG_CONST
#endif
#if \
  LIBRG_HAS_ATTRIBUTE(const) || \
  LIBRG_GCC_VERSION_CHECK(2,5,0) || \
  LIBRG_INTEL_VERSION_CHECK(13,0,0) || \
  LIBRG_SUNPRO_VERSION_CHECK(5,11,0) || \
  LIBRG_ARM_VERSION_CHECK(4,1,0) || \
  LIBRG_IBM_VERSION_CHECK(10,1,0) || \
  LIBRG_TI_VERSION_CHECK(15,12,0) || \
  (LIBRG_TI_ARMCL_VERSION_CHECK(4,8,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
  LIBRG_TI_ARMCL_VERSION_CHECK(5,2,0) || \
  (LIBRG_TI_CL2000_VERSION_CHECK(6,0,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
  LIBRG_TI_CL2000_VERSION_CHECK(6,4,0) || \
  (LIBRG_TI_CL430_VERSION_CHECK(4,0,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
  LIBRG_TI_CL430_VERSION_CHECK(4,3,0) || \
  (LIBRG_TI_CL6X_VERSION_CHECK(7,2,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
  LIBRG_TI_CL6X_VERSION_CHECK(7,5,0) || \
  LIBRG_TI_CL7X_VERSION_CHECK(1,2,0) || \
  LIBRG_TI_CLPRU_VERSION_CHECK(2,1,0) || \
  LIBRG_PGI_VERSION_CHECK(17,10,0)
#  define LIBRG_CONST __attribute__((__const__))
#elif \
  LIBRG_SUNPRO_VERSION_CHECK(5,10,0)
#  define LIBRG_CONST _Pragma("no_side_effect")
#else
#  define LIBRG_CONST LIBRG_PURE
#endif

#if defined(LIBRG_RESTRICT)
#  undef LIBRG_RESTRICT
#endif
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L) && !defined(__cplusplus)
#  define LIBRG_RESTRICT restrict
#elif \
  LIBRG_GCC_VERSION_CHECK(3,1,0) || \
  LIBRG_MSVC_VERSION_CHECK(14,0,0) || \
  LIBRG_INTEL_VERSION_CHECK(13,0,0) || \
  LIBRG_ARM_VERSION_CHECK(4,1,0) || \
  LIBRG_IBM_VERSION_CHECK(10,1,0) || \
  LIBRG_PGI_VERSION_CHECK(17,10,0) || \
  LIBRG_TI_CL430_VERSION_CHECK(4,3,0) || \
  LIBRG_TI_CL2000_VERSION_CHECK(6,2,4) || \
  LIBRG_TI_CL6X_VERSION_CHECK(8,1,0) || \
  LIBRG_TI_CL7X_VERSION_CHECK(1,2,0) || \
  (LIBRG_SUNPRO_VERSION_CHECK(5,14,0) && defined(__cplusplus)) || \
  LIBRG_IAR_VERSION_CHECK(8,0,0) || \
  defined(__clang__)
#  define LIBRG_RESTRICT __restrict
#elif LIBRG_SUNPRO_VERSION_CHECK(5,3,0) && !defined(__cplusplus)
#  define LIBRG_RESTRICT _Restrict
#else
#  define LIBRG_RESTRICT
#endif

#if defined(LIBRG_INLINE)
#  undef LIBRG_INLINE
#endif
#if \
  (defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)) || \
  (defined(__cplusplus) && (__cplusplus >= 199711L))
#  define LIBRG_INLINE inline
#elif \
  defined(LIBRG_GCC_VERSION) || \
  LIBRG_ARM_VERSION_CHECK(6,2,0)
#  define LIBRG_INLINE __inline__
#elif \
  LIBRG_MSVC_VERSION_CHECK(12,0,0) || \
  LIBRG_ARM_VERSION_CHECK(4,1,0) || \
  LIBRG_TI_ARMCL_VERSION_CHECK(5,1,0) || \
  LIBRG_TI_CL430_VERSION_CHECK(3,1,0) || \
  LIBRG_TI_CL2000_VERSION_CHECK(6,2,0) || \
  LIBRG_TI_CL6X_VERSION_CHECK(8,0,0) || \
  LIBRG_TI_CL7X_VERSION_CHECK(1,2,0) || \
  LIBRG_TI_CLPRU_VERSION_CHECK(2,1,0)
#  define LIBRG_INLINE __inline
#else
#  define LIBRG_INLINE
#endif

#if defined(LIBRG_ALWAYS_INLINE)
#  undef LIBRG_ALWAYS_INLINE
#endif
#if \
  LIBRG_HAS_ATTRIBUTE(always_inline) || \
  LIBRG_GCC_VERSION_CHECK(4,0,0) || \
  LIBRG_INTEL_VERSION_CHECK(13,0,0) || \
  LIBRG_SUNPRO_VERSION_CHECK(5,11,0) || \
  LIBRG_ARM_VERSION_CHECK(4,1,0) || \
  LIBRG_IBM_VERSION_CHECK(10,1,0) || \
  LIBRG_TI_VERSION_CHECK(15,12,0) || \
  (LIBRG_TI_ARMCL_VERSION_CHECK(4,8,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
  LIBRG_TI_ARMCL_VERSION_CHECK(5,2,0) || \
  (LIBRG_TI_CL2000_VERSION_CHECK(6,0,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
  LIBRG_TI_CL2000_VERSION_CHECK(6,4,0) || \
  (LIBRG_TI_CL430_VERSION_CHECK(4,0,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
  LIBRG_TI_CL430_VERSION_CHECK(4,3,0) || \
  (LIBRG_TI_CL6X_VERSION_CHECK(7,2,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
  LIBRG_TI_CL6X_VERSION_CHECK(7,5,0) || \
  LIBRG_TI_CL7X_VERSION_CHECK(1,2,0) || \
  LIBRG_TI_CLPRU_VERSION_CHECK(2,1,0)
#  define LIBRG_ALWAYS_INLINE __attribute__((__always_inline__)) LIBRG_INLINE
#elif LIBRG_MSVC_VERSION_CHECK(12,0,0)
#  define LIBRG_ALWAYS_INLINE __forceinline
#elif defined(__cplusplus) && \
    ( \
      LIBRG_TI_ARMCL_VERSION_CHECK(5,2,0) || \
      LIBRG_TI_CL430_VERSION_CHECK(4,3,0) || \
      LIBRG_TI_CL2000_VERSION_CHECK(6,4,0) || \
      LIBRG_TI_CL6X_VERSION_CHECK(6,1,0) || \
      LIBRG_TI_CL7X_VERSION_CHECK(1,2,0) || \
      LIBRG_TI_CLPRU_VERSION_CHECK(2,1,0) \
    )
#  define LIBRG_ALWAYS_INLINE _Pragma("FUNC_ALWAYS_INLINE;")
#elif LIBRG_IAR_VERSION_CHECK(8,0,0)
#  define LIBRG_ALWAYS_INLINE _Pragma("inline=forced")
#else
#  define LIBRG_ALWAYS_INLINE LIBRG_INLINE
#endif

#if defined(LIBRG_NEVER_INLINE)
#  undef LIBRG_NEVER_INLINE
#endif
#if \
  LIBRG_HAS_ATTRIBUTE(noinline) || \
  LIBRG_GCC_VERSION_CHECK(4,0,0) || \
  LIBRG_INTEL_VERSION_CHECK(13,0,0) || \
  LIBRG_SUNPRO_VERSION_CHECK(5,11,0) || \
  LIBRG_ARM_VERSION_CHECK(4,1,0) || \
  LIBRG_IBM_VERSION_CHECK(10,1,0) || \
  LIBRG_TI_VERSION_CHECK(15,12,0) || \
  (LIBRG_TI_ARMCL_VERSION_CHECK(4,8,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
  LIBRG_TI_ARMCL_VERSION_CHECK(5,2,0) || \
  (LIBRG_TI_CL2000_VERSION_CHECK(6,0,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
  LIBRG_TI_CL2000_VERSION_CHECK(6,4,0) || \
  (LIBRG_TI_CL430_VERSION_CHECK(4,0,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
  LIBRG_TI_CL430_VERSION_CHECK(4,3,0) || \
  (LIBRG_TI_CL6X_VERSION_CHECK(7,2,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
  LIBRG_TI_CL6X_VERSION_CHECK(7,5,0) || \
  LIBRG_TI_CL7X_VERSION_CHECK(1,2,0) || \
  LIBRG_TI_CLPRU_VERSION_CHECK(2,1,0)
#  define LIBRG_NEVER_INLINE __attribute__((__noinline__))
#elif LIBRG_MSVC_VERSION_CHECK(13,10,0)
#  define LIBRG_NEVER_INLINE __declspec(noinline)
#elif LIBRG_PGI_VERSION_CHECK(10,2,0)
#  define LIBRG_NEVER_INLINE _Pragma("noinline")
#elif LIBRG_TI_CL6X_VERSION_CHECK(6,0,0) && defined(__cplusplus)
#  define LIBRG_NEVER_INLINE _Pragma("FUNC_CANNOT_INLINE;")
#elif LIBRG_IAR_VERSION_CHECK(8,0,0)
#  define LIBRG_NEVER_INLINE _Pragma("inline=never")
#elif LIBRG_COMPCERT_VERSION_CHECK(3,2,0)
#  define LIBRG_NEVER_INLINE __attribute((noinline))
#elif LIBRG_PELLES_VERSION_CHECK(9,0,0)
#  define LIBRG_NEVER_INLINE __declspec(noinline)
#else
#  define LIBRG_NEVER_INLINE
#endif

#if defined(LIBRG_PRIVATE)
#  undef LIBRG_PRIVATE
#endif
#if defined(LIBRG_PUBLIC)
#  undef LIBRG_PUBLIC
#endif
#if defined(LIBRG_IMPORT)
#  undef LIBRG_IMPORT
#endif
#if defined(_WIN32) || defined(__CYGWIN__)
#  define LIBRG_PRIVATE
#  define LIBRG_PUBLIC   __declspec(dllexport)
#  define LIBRG_IMPORT   __declspec(dllimport)
#else
#  if \
    LIBRG_HAS_ATTRIBUTE(visibility) || \
    LIBRG_GCC_VERSION_CHECK(3,3,0) || \
    LIBRG_SUNPRO_VERSION_CHECK(5,11,0) || \
    LIBRG_INTEL_VERSION_CHECK(13,0,0) || \
    LIBRG_ARM_VERSION_CHECK(4,1,0) || \
    LIBRG_IBM_VERSION_CHECK(13,1,0) || \
    ( \
      defined(__TI_EABI__) && \
      ( \
        (LIBRG_TI_CL6X_VERSION_CHECK(7,2,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
        LIBRG_TI_CL6X_VERSION_CHECK(7,5,0) \
      ) \
    )
#    define LIBRG_PRIVATE __attribute__((__visibility__("hidden")))
#    define LIBRG_PUBLIC  __attribute__((__visibility__("default")))
#  else
#    define LIBRG_PRIVATE
#    define LIBRG_PUBLIC
#  endif
#  define LIBRG_IMPORT    extern
#endif

#if defined(LIBRG_NO_THROW)
#  undef LIBRG_NO_THROW
#endif
#if \
  LIBRG_HAS_ATTRIBUTE(nothrow) || \
  LIBRG_GCC_VERSION_CHECK(3,3,0) || \
  LIBRG_INTEL_VERSION_CHECK(13,0,0)
#  define LIBRG_NO_THROW __attribute__((__nothrow__))
#elif \
  LIBRG_MSVC_VERSION_CHECK(13,1,0) || \
  LIBRG_ARM_VERSION_CHECK(4,1,0)
#  define LIBRG_NO_THROW __declspec(nothrow)
#else
#  define LIBRG_NO_THROW
#endif

#if defined(LIBRG_FALL_THROUGH)
# undef LIBRG_FALL_THROUGH
#endif
#if \
  LIBRG_HAS_ATTRIBUTE(fallthrough) || \
  LIBRG_GCC_VERSION_CHECK(7,0,0)
#  define LIBRG_FALL_THROUGH __attribute__((__fallthrough__))
#elif LIBRG_HAS_CPP_ATTRIBUTE_NS(clang,fallthrough)
#  define LIBRG_FALL_THROUGH LIBRG_DIAGNOSTIC_DISABLE_CPP98_COMPAT_WRAP_([[clang::fallthrough]])
#elif LIBRG_HAS_CPP_ATTRIBUTE(fallthrough)
#  define LIBRG_FALL_THROUGH LIBRG_DIAGNOSTIC_DISABLE_CPP98_COMPAT_WRAP_([[fallthrough]])
#elif defined(__fallthrough) /* SAL */
#  define LIBRG_FALL_THROUGH __fallthrough
#else
#  define LIBRG_FALL_THROUGH
#endif

#if defined(LIBRG_RETURNS_NON_NULL)
#  undef LIBRG_RETURNS_NON_NULL
#endif
#if \
  LIBRG_HAS_ATTRIBUTE(returns_nonnull) || \
  LIBRG_GCC_VERSION_CHECK(4,9,0)
#  define LIBRG_RETURNS_NON_NULL __attribute__((__returns_nonnull__))
#elif defined(_Ret_notnull_) /* SAL */
#  define LIBRG_RETURNS_NON_NULL _Ret_notnull_
#else
#  define LIBRG_RETURNS_NON_NULL
#endif

#if defined(LIBRG_ARRAY_PARAM)
#  undef LIBRG_ARRAY_PARAM
#endif
#if \
  defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L) && \
  !defined(__STDC_NO_VLA__) && \
  !defined(__cplusplus) && \
  !defined(LIBRG_PGI_VERSION) && \
  !defined(LIBRG_TINYC_VERSION)
#  define LIBRG_ARRAY_PARAM(name) (name)
#else
#  define LIBRG_ARRAY_PARAM(name)
#endif

#if defined(LIBRG_IS_CONSTANT)
#  undef LIBRG_IS_CONSTANT
#endif
#if defined(LIBRG_REQUIRE_CONSTEXPR)
#  undef LIBRG_REQUIRE_CONSTEXPR
#endif
/* LIBRG_IS_CONSTEXPR_ is for
   LIBRG INTERNAL USE ONLY.  API subject to change without notice. */
#if defined(LIBRG_IS_CONSTEXPR_)
#  undef LIBRG_IS_CONSTEXPR_
#endif
#if \
  LIBRG_HAS_BUILTIN(__builtin_constant_p) || \
  LIBRG_GCC_VERSION_CHECK(3,4,0) || \
  LIBRG_INTEL_VERSION_CHECK(13,0,0) || \
  LIBRG_TINYC_VERSION_CHECK(0,9,19) || \
  LIBRG_ARM_VERSION_CHECK(4,1,0) || \
  LIBRG_IBM_VERSION_CHECK(13,1,0) || \
  LIBRG_TI_CL6X_VERSION_CHECK(6,1,0) || \
  (LIBRG_SUNPRO_VERSION_CHECK(5,10,0) && !defined(__cplusplus)) || \
  LIBRG_CRAY_VERSION_CHECK(8,1,0)
#  define LIBRG_IS_CONSTANT(expr) __builtin_constant_p(expr)
#endif
#if !defined(__cplusplus)
#  if \
       LIBRG_HAS_BUILTIN(__builtin_types_compatible_p) || \
       LIBRG_GCC_VERSION_CHECK(3,4,0) || \
       LIBRG_INTEL_VERSION_CHECK(13,0,0) || \
       LIBRG_IBM_VERSION_CHECK(13,1,0) || \
       LIBRG_CRAY_VERSION_CHECK(8,1,0) || \
       LIBRG_ARM_VERSION_CHECK(5,4,0) || \
       LIBRG_TINYC_VERSION_CHECK(0,9,24)
#    if defined(__INTPTR_TYPE__)
#      define LIBRG_IS_CONSTEXPR_(expr) __builtin_types_compatible_p(__typeof__((1 ? (void*) ((__INTPTR_TYPE__) ((expr) * 0)) : (int*) 0)), int*)
#    else
#      include <stdint.h>
#      define LIBRG_IS_CONSTEXPR_(expr) __builtin_types_compatible_p(__typeof__((1 ? (void*) ((intptr_t) ((expr) * 0)) : (int*) 0)), int*)
#    endif
#  elif \
       ( \
          defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L) && \
          !defined(LIBRG_SUNPRO_VERSION) && \
          !defined(LIBRG_PGI_VERSION) && \
          !defined(LIBRG_IAR_VERSION)) || \
       LIBRG_HAS_EXTENSION(c_generic_selections) || \
       LIBRG_GCC_VERSION_CHECK(4,9,0) || \
       LIBRG_INTEL_VERSION_CHECK(17,0,0) || \
       LIBRG_IBM_VERSION_CHECK(12,1,0) || \
       LIBRG_ARM_VERSION_CHECK(5,3,0)
#    if defined(__INTPTR_TYPE__)
#      define LIBRG_IS_CONSTEXPR_(expr) _Generic((1 ? (void*) ((__INTPTR_TYPE__) ((expr) * 0)) : (int*) 0), int*: 1, void*: 0)
#    else
#      include <stdint.h>
#      define LIBRG_IS_CONSTEXPR_(expr) _Generic((1 ? (void*) ((intptr_t) * 0) : (int*) 0), int*: 1, void*: 0)
#    endif
#  elif \
       defined(LIBRG_GCC_VERSION) || \
       defined(LIBRG_INTEL_VERSION) || \
       defined(LIBRG_TINYC_VERSION) || \
       defined(LIBRG_TI_ARMCL_VERSION) || \
       LIBRG_TI_CL430_VERSION_CHECK(18,12,0) || \
       defined(LIBRG_TI_CL2000_VERSION) || \
       defined(LIBRG_TI_CL6X_VERSION) || \
       defined(LIBRG_TI_CL7X_VERSION) || \
       defined(LIBRG_TI_CLPRU_VERSION) || \
       defined(__clang__)
#    define LIBRG_IS_CONSTEXPR_(expr) ( \
         sizeof(void) != \
         sizeof(*( \
           1 ? \
             ((void*) ((expr) * 0L) ) : \
             ((struct { char v[sizeof(void) * 2]; } *) 1) \
           ) \
         ) \
       )
#  endif
#endif
#if defined(LIBRG_IS_CONSTEXPR_)
#  if !defined(LIBRG_IS_CONSTANT)
#    define LIBRG_IS_CONSTANT(expr) LIBRG_IS_CONSTEXPR_(expr)
#  endif
#  define LIBRG_REQUIRE_CONSTEXPR(expr) (LIBRG_IS_CONSTEXPR_(expr) ? (expr) : (-1))
#else
#  if !defined(LIBRG_IS_CONSTANT)
#    define LIBRG_IS_CONSTANT(expr) (0)
#  endif
#  define LIBRG_REQUIRE_CONSTEXPR(expr) (expr)
#endif

#if defined(LIBRG_BEGIN_C_DECLS)
#  undef LIBRG_BEGIN_C_DECLS
#endif
#if defined(LIBRG_END_C_DECLS)
#  undef LIBRG_END_C_DECLS
#endif
#if defined(LIBRG_C_DECL)
#  undef LIBRG_C_DECL
#endif
#if defined(__cplusplus)
#  define LIBRG_BEGIN_C_DECLS extern "C" {
#  define LIBRG_END_C_DECLS }
#  define LIBRG_C_DECL extern "C"
#else
#  define LIBRG_BEGIN_C_DECLS
#  define LIBRG_END_C_DECLS
#  define LIBRG_C_DECL
#endif

#if defined(LIBRG_STATIC_ASSERT)
#  undef LIBRG_STATIC_ASSERT
#endif
#if \
  !defined(__cplusplus) && ( \
      (defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L)) || \
      LIBRG_HAS_FEATURE(c_static_assert) || \
      LIBRG_GCC_VERSION_CHECK(6,0,0) || \
      LIBRG_INTEL_VERSION_CHECK(13,0,0) || \
      defined(_Static_assert) \
    )
#  define LIBRG_STATIC_ASSERT(expr, message) _Static_assert(expr, message)
#elif \
  (defined(__cplusplus) && (__cplusplus >= 201103L)) || \
  LIBRG_MSVC_VERSION_CHECK(16,0,0)
#  define LIBRG_STATIC_ASSERT(expr, message) LIBRG_DIAGNOSTIC_DISABLE_CPP98_COMPAT_WRAP_(static_assert(expr, message))
#else
#  define LIBRG_STATIC_ASSERT(expr, message)
#endif

#if defined(LIBRG_NULL)
#  undef LIBRG_NULL
#endif
#if defined(__cplusplus)
#  if __cplusplus >= 201103L
#    define LIBRG_NULL LIBRG_DIAGNOSTIC_DISABLE_CPP98_COMPAT_WRAP_(nullptr)
#  elif defined(NULL)
#    define LIBRG_NULL NULL
#  else
#    define LIBRG_NULL LIBRG_STATIC_CAST(void*, 0)
#  endif
#elif defined(NULL)
#  define LIBRG_NULL NULL
#else
#  define LIBRG_NULL ((void*) 0)
#endif

#if defined(LIBRG_MESSAGE)
#  undef LIBRG_MESSAGE
#endif
#if LIBRG_HAS_WARNING("-Wunknown-pragmas")
#  define LIBRG_MESSAGE(msg) \
  LIBRG_DIAGNOSTIC_PUSH \
  LIBRG_DIAGNOSTIC_DISABLE_UNKNOWN_PRAGMAS \
  LIBRG_PRAGMA(message msg) \
  LIBRG_DIAGNOSTIC_POP
#elif \
  LIBRG_GCC_VERSION_CHECK(4,4,0) || \
  LIBRG_INTEL_VERSION_CHECK(13,0,0)
#  define LIBRG_MESSAGE(msg) LIBRG_PRAGMA(message msg)
#elif LIBRG_CRAY_VERSION_CHECK(5,0,0)
#  define LIBRG_MESSAGE(msg) LIBRG_PRAGMA(_CRI message msg)
#elif LIBRG_IAR_VERSION_CHECK(8,0,0)
#  define LIBRG_MESSAGE(msg) LIBRG_PRAGMA(message(msg))
#elif LIBRG_PELLES_VERSION_CHECK(2,0,0)
#  define LIBRG_MESSAGE(msg) LIBRG_PRAGMA(message(msg))
#else
#  define LIBRG_MESSAGE(msg)
#endif

#if defined(LIBRG_WARNING)
#  undef LIBRG_WARNING
#endif
#if LIBRG_HAS_WARNING("-Wunknown-pragmas")
#  define LIBRG_WARNING(msg) \
  LIBRG_DIAGNOSTIC_PUSH \
  LIBRG_DIAGNOSTIC_DISABLE_UNKNOWN_PRAGMAS \
  LIBRG_PRAGMA(clang warning msg) \
  LIBRG_DIAGNOSTIC_POP
#elif \
  LIBRG_GCC_VERSION_CHECK(4,8,0) || \
  LIBRG_PGI_VERSION_CHECK(18,4,0) || \
  LIBRG_INTEL_VERSION_CHECK(13,0,0)
#  define LIBRG_WARNING(msg) LIBRG_PRAGMA(GCC warning msg)
#elif LIBRG_MSVC_VERSION_CHECK(15,0,0)
#  define LIBRG_WARNING(msg) LIBRG_PRAGMA(message(msg))
#else
#  define LIBRG_WARNING(msg) LIBRG_MESSAGE(msg)
#endif

#if defined(LIBRG_REQUIRE)
#  undef LIBRG_REQUIRE
#endif
#if defined(LIBRG_REQUIRE_MSG)
#  undef LIBRG_REQUIRE_MSG
#endif
#if LIBRG_HAS_ATTRIBUTE(diagnose_if)
#  if LIBRG_HAS_WARNING("-Wgcc-compat")
#    define LIBRG_REQUIRE(expr) \
       LIBRG_DIAGNOSTIC_PUSH \
       _Pragma("clang diagnostic ignored \"-Wgcc-compat\"") \
       __attribute__((diagnose_if(!(expr), #expr, "error"))) \
       LIBRG_DIAGNOSTIC_POP
#    define LIBRG_REQUIRE_MSG(expr,msg) \
       LIBRG_DIAGNOSTIC_PUSH \
       _Pragma("clang diagnostic ignored \"-Wgcc-compat\"") \
       __attribute__((diagnose_if(!(expr), msg, "error"))) \
       LIBRG_DIAGNOSTIC_POP
#  else
#    define LIBRG_REQUIRE(expr) __attribute__((diagnose_if(!(expr), #expr, "error")))
#    define LIBRG_REQUIRE_MSG(expr,msg) __attribute__((diagnose_if(!(expr), msg, "error")))
#  endif
#else
#  define LIBRG_REQUIRE(expr)
#  define LIBRG_REQUIRE_MSG(expr,msg)
#endif

#if defined(LIBRG_FLAGS)
#  undef LIBRG_FLAGS
#endif
#if LIBRG_HAS_ATTRIBUTE(flag_enum)
#  define LIBRG_FLAGS __attribute__((__flag_enum__))
#endif

#if defined(LIBRG_FLAGS_CAST)
#  undef LIBRG_FLAGS_CAST
#endif
#if LIBRG_INTEL_VERSION_CHECK(19,0,0)
#  define LIBRG_FLAGS_CAST(T, expr) (__extension__ ({ \
  LIBRG_DIAGNOSTIC_PUSH \
      _Pragma("warning(disable:188)") \
      ((T) (expr)); \
      LIBRG_DIAGNOSTIC_POP \
    }))
#else
#  define LIBRG_FLAGS_CAST(T, expr) LIBRG_STATIC_CAST(T, expr)
#endif

#if defined(LIBRG_EMPTY_BASES)
#  undef LIBRG_EMPTY_BASES
#endif
#if LIBRG_MSVC_VERSION_CHECK(19,0,23918) && !LIBRG_MSVC_VERSION_CHECK(20,0,0)
#  define LIBRG_EMPTY_BASES __declspec(empty_bases)
#else
#  define LIBRG_EMPTY_BASES
#endif

/* Remaining macros are deprecated. */

#if defined(LIBRG_GCC_NOT_CLANG_VERSION_CHECK)
#  undef LIBRG_GCC_NOT_CLANG_VERSION_CHECK
#endif
#if defined(__clang__)
#  define LIBRG_GCC_NOT_CLANG_VERSION_CHECK(major,minor,patch) (0)
#else
#  define LIBRG_GCC_NOT_CLANG_VERSION_CHECK(major,minor,patch) LIBRG_GCC_VERSION_CHECK(major,minor,patch)
#endif

#if defined(LIBRG_CLANG_HAS_ATTRIBUTE)
#  undef LIBRG_CLANG_HAS_ATTRIBUTE
#endif
#define LIBRG_CLANG_HAS_ATTRIBUTE(attribute) LIBRG_HAS_ATTRIBUTE(attribute)

#if defined(LIBRG_CLANG_HAS_CPP_ATTRIBUTE)
#  undef LIBRG_CLANG_HAS_CPP_ATTRIBUTE
#endif
#define LIBRG_CLANG_HAS_CPP_ATTRIBUTE(attribute) LIBRG_HAS_CPP_ATTRIBUTE(attribute)

#if defined(LIBRG_CLANG_HAS_BUILTIN)
#  undef LIBRG_CLANG_HAS_BUILTIN
#endif
#define LIBRG_CLANG_HAS_BUILTIN(builtin) LIBRG_HAS_BUILTIN(builtin)

#if defined(LIBRG_CLANG_HAS_FEATURE)
#  undef LIBRG_CLANG_HAS_FEATURE
#endif
#define LIBRG_CLANG_HAS_FEATURE(feature) LIBRG_HAS_FEATURE(feature)

#if defined(LIBRG_CLANG_HAS_EXTENSION)
#  undef LIBRG_CLANG_HAS_EXTENSION
#endif
#define LIBRG_CLANG_HAS_EXTENSION(extension) LIBRG_HAS_EXTENSION(extension)

#if defined(LIBRG_CLANG_HAS_DECLSPEC_DECLSPEC_ATTRIBUTE)
#  undef LIBRG_CLANG_HAS_DECLSPEC_DECLSPEC_ATTRIBUTE
#endif
#define LIBRG_CLANG_HAS_DECLSPEC_ATTRIBUTE(attribute) LIBRG_HAS_DECLSPEC_ATTRIBUTE(attribute)

#if defined(LIBRG_CLANG_HAS_WARNING)
#  undef LIBRG_CLANG_HAS_WARNING
#endif
#define LIBRG_CLANG_HAS_WARNING(warning) LIBRG_HAS_WARNING(warning)

#endif /* !defined(LIBRG_HEDLEY_VERSION) || (LIBRG_HEDLEY_VERSION < X) */
