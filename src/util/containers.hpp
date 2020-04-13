#ifndef UTIL_CONTAINERS_HPP
#define UTIL_CONTAINERS_HPP

#include <algorithm>

namespace util {

template <typename Container, typename Value>
auto find(Container const& c, Value const& v)
{
    return std::find(begin(c), end(c), v);
}

template <typename Container, typename Value>
auto contains(Container const& c, Value const& v)
{
    return find(c, v) != end(c);
}

}

#endif
