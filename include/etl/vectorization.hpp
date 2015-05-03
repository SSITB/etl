//=======================================================================
// Copyright (c) 2014-2015 Baptiste Wicht
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

#ifndef ETL_VECTORIZATION_HPP
#define ETL_VECTORIZATION_HPP

#include <immintrin.h>

namespace etl {

template<typename T>
struct intrinsic_traits {
    static constexpr const bool vectorizable = false;

    using intrinsic_type = T;
};

template<typename T>
using intrinsic_type = typename intrinsic_traits<T>::intrinsic_type;

} //end of namespace etl

#ifdef ETL_VECTORIZE

#ifdef __AVX__

#include "avx_vectorization.hpp"

#elif defined(__SSE__)

#include "sse_vectorization.hpp"

#endif

#endif //ETL_VECTORIZ

#endif //ETL_VECTORIZATION_HPP
