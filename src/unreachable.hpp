#ifndef UTIL_UNREACHABLE_HPP
#define UTIL_UNREACHABLE_HPP

#ifndef __has_builtin
#    define __has_builtin(x) 0
#endif

#if defined(__GNUC__) && defined(__GNUC_MINOR__) && defined(__GNUC_PATCHLEVEL__)
#    define UTIL_GNUC_PREREQ(maj, min, patch) \
         ((__GNUC__ << 20) + (__GNUC_MINOR__ << 10) + __GNUC_PATCHLEVEL__ >= \
         ((maj) << 20) + ((min) << 10) + (patch))
#elif defined(__GNUC__) && defined(__GNUC_MINOR__)
#    define UTIL_GNUC_PREREQ(maj, min, patch) \
         ((__GNUC__ << 20) + (__GNUC_MINOR__ << 10) >= ((maj) << 20) + ((min) << 10))
#else
#    define UTIL_GNUC_PREREQ(maj, min, patch) 0
#endif

#if __has_builtin(__builtin_unreachable) || UTIL_GNUC_PREREQ(4, 5, 0)
#    define UTIL_MARK_UNREACHABLE __builtin_unreachable()
#elif defined(_MSC_VER)
#    define UTIL_MARK_UNREACHABLE __assume(false)
#endif

#endif
