#ifndef UTIL_UNREACHABLE_HPP
#define UTIL_UNREACHABLE_HPP

// This is directly lifted from LLVM's code, which is under the Apache license.
// Documentation/comments were added later.

// We want to mark unreachable code as such so we can avoid warnings by the
// compiler such as "control reaches end of non-void function" when a switch
// is exhaustive for an enum. For that we assemble a few pieces.

// We expect the compiler to have `__has_builtin`. If it doesn't, we will have
// to assume it has no builtins.
#ifndef __has_builtin
#define __has_builtin(x) 0
#endif

// Macros for checking GCC version.
#if defined(__GNUC__) && defined(__GNUC_MINOR__) && defined(__GNUC_PATCHLEVEL__)
#define UTIL_GNUC_PREREQ(maj, min, patch)                                      \
    ((__GNUC__ << 20) + (__GNUC_MINOR__ << 10) + __GNUC_PATCHLEVEL__ >=        \
     ((maj) << 20) + ((min) << 10) + (patch))
#elif defined(__GNUC__) && defined(__GNUC_MINOR__)
#define UTIL_GNUC_PREREQ(maj, min, patch)                                      \
    ((__GNUC__ << 20) + (__GNUC_MINOR__ << 10) >= ((maj) << 20) + ((min) << 10))
#else
#define UTIL_GNUC_PREREQ(maj, min, patch) 0
#endif

// We use `__has_bultin` to check for `__builtin_unreachable`, which is present
// ever since GCC 4.5.0, which apparently didn't have `__has_builtin` yet.
#if __has_builtin(__builtin_unreachable) || UTIL_GNUC_PREREQ(4, 5, 0)
#define UTIL_MARK_UNREACHABLE __builtin_unreachable()
// MSVC has `__assume(false)` which marks a code path as unreachable.
#elif defined(_MSC_VER)
#define UTIL_MARK_UNREACHABLE __assume(false)
#endif

#endif
