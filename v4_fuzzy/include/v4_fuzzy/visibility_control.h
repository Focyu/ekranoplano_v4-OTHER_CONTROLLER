#ifndef V4_FUZZY__VISIBILITY_CONTROL_H_
#define V4_FUZZY__VISIBILITY_CONTROL_H_
#if defined _WIN32 || defined __CYGWIN__
  #ifdef __GNUC__
    #define V4_FUZZY_EXPORT __attribute__ ((dllexport))
    #define V4_FUZZY_IMPORT __attribute__ ((dllimport))
  #else
    #define V4_FUZZY_EXPORT __declspec(dllexport)
    #define V4_FUZZY_IMPORT __declspec(dllimport)
  #endif
  #ifdef V4_FUZZY_BUILDING_LIBRARY
    #define V4_FUZZY_PUBLIC V4_FUZZY_EXPORT
  #else
    #define V4_FUZZY_PUBLIC V4_FUZZY_IMPORT
  #endif
  #define V4_FUZZY_PUBLIC_TYPE V4_FUZZY_PUBLIC
  #define V4_FUZZY_LOCAL
#else
  #define V4_FUZZY_EXPORT __attribute__ ((visibility("default")))
  #define V4_FUZZY_IMPORT
  #if __GNUC__ >= 4
    #define V4_FUZZY_PUBLIC __attribute__ ((visibility("default")))
    #define V4_FUZZY_LOCAL  __attribute__ ((visibility("hidden")))
  #else
    #define V4_FUZZY_PUBLIC
    #define V4_FUZZY_LOCAL
  #endif
  #define V4_FUZZY_PUBLIC_TYPE
#endif
#endif  // V4_FUZZY__VISIBILITY_CONTROL_H_
// Generated 24-May-2026 10:28:47
 