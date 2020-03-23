#ifndef ASSERTPP_ASSERT_HPP
#define ASSERTPP_ASSERT_HPP

#include <stdexcept>

#include <stdexcept>
#include <iostream>

// Exception-based assertions, so that they can be caught on main to ensure
// destructors run properly.

namespace assertpp {

// Use our own flag to enable/disable. This way assertions can be used in
// release if needed.
constexpr auto assertions_enabled =
#ifdef ASSERTPP_ENABLE
    true
#else
    false
#endif
;

// Signals a failed assertion.
struct AssertionError: std::runtime_error {
    using std::runtime_error::runtime_error;
};

// Evaluate a predicate if assertions are enabled. Else, becomes an
// empty function that can be easily optimized away.
// Args:
//     predicate: A boolean predicate to evaluate.
//     message: A message to use in the thrown exception.
template <typename Predicate>
void assert_predicate(
    [[maybe_unused]] Predicate predicate,
    [[maybe_unused]] const char* message
)
{
    if constexpr (assertions_enabled) {
        if (not predicate()) {
            throw AssertionError{message};
        }
    }
}

}

#endif
