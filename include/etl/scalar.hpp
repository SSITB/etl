//=======================================================================
// Copyright (c) 2014-2015 Baptiste Wicht
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

#ifndef ETL_SCALAR_HPP
#define ETL_SCALAR_HPP

#include "cpp_utils/tmp.hpp"

namespace etl {

template<typename T>
struct scalar {
    using vec_type = intrinsic_type<T>;

    const T value;

    explicit constexpr scalar(T v) : value(v) {}

    constexpr const T operator[](std::size_t) const noexcept {
        return value;
    }

    constexpr const vec_type load(std::size_t) const noexcept {
        return vec::set(value);
    }

    template<typename... S>
    T operator()(S... /*args*/) const {
        static_assert(cpp::all_convertible_to<std::size_t, S...>::value, "Invalid size types");

        return value;
    }
};

template<typename T>
std::ostream& operator<<(std::ostream& os, const scalar<T>& s){
    return os << s.value;
}

} //end of namespace etl

#endif
