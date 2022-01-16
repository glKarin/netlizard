#ifndef _KARIN_MATH_DEF_H
#define _KARIN_MATH_DEF_H

#if defined(__WIN32) || defined(__WIN32__) || defined(WIN32) || defined(__MINGW32__) || defined(_WIN32)
#    define NLDECL_EXPORT __declspec(dllexport)
#    define NLDECL_IMPORT __declspec(dllimport)
#    define NLDECL_HIDDEN
#else
#  define NLDECL_EXPORT     __attribute__((visibility("default")))
#  define NLDECL_EXPORT     __attribute__((visibility("default")))
#  define NLDECL_HIDDEN     __attribute__((visibility("hidden")))
#endif

#if defined(_NL_LIBRARY)
#  define NLMATHLIB_EXPORT NLDECL_EXPORT
#else
#  define NLMATHLIB_EXPORT NLDECL_IMPORT
#endif

#endif // _KARIN_MATH_DEF_H
