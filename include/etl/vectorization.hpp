//=======================================================================
// Copyright (c) 2014-2015 Baptiste Wicht
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

/*!
 * \file vectorization.hpp
 * \brief Contains vectorization utilities for the vectorized assignments (done by the evaluator).
 *
 * This file automatically includes the correct header based on which vectorization utility is supported (AVX -> SSE -> NONE).
 */

#pragma once

#include <immintrin.h>

//Include al the vector implementation
#include "etl/avx512_vectorization.hpp"
#include "etl/avx_vectorization.hpp"
#include "etl/sse_vectorization.hpp"
#include "etl/no_vectorization.hpp"

namespace etl {

#ifdef ETL_VECTORIZE_EXPR

#ifdef __AVX512F__

using default_vec = avx512_vec;

template <typename T>
using intrinsic_traits = avx512_intrinsic_traits<T>;

#elif defined(__AVX__)

using default_vec = avx_vec;

template <typename T>
using intrinsic_traits = avx_intrinsic_traits<T>;

#elif defined(__SSE3__)

using default_vec = sse_vec;

template <typename T>
using intrinsic_traits = sse_intrinsic_traits<T>;

#else

using default_vec = no_vec;

template <typename T>
using intrinsic_traits = no_intrinsic_traits<T>;

#endif //defined(__SSE__)

#else //ETL_VECTORIZE_EXPR

using default_vec = no_vec;

template <typename T>
using intrinsic_traits = no_intrinsic_traits<T>;

#endif //ETL_VECTORIZE_EXPR

/*!
 * \brief Helper to get the intrinsic corresponding type of a vectorizable type.
 */
template <typename T>
using intrinsic_type = typename intrinsic_traits<T>::intrinsic_type;

} //end of namespace etl
