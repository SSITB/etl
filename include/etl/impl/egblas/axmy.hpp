//=======================================================================
// Copyright (c) 2014-2017 Baptiste Wicht
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

/*!
 * \file
 * \brief EGBLAS wrappers for the axmy operation.
 */

#pragma once

#ifdef ETL_EGBLAS_MODE

#include "etl/impl/cublas/cuda.hpp"

#include <egblas.hpp>

#endif

namespace etl {

namespace impl {

namespace egblas {

#ifdef EGBLAS_HAS_SAXMY

/*!
 * \brief Wrappers for single-precision egblas axpy operation
 * \param n The size of the vector
 * \param alpha The scaling factor alpha
 * \param A The memory of the vector a
 * \param lda The leading dimension of a
 * \param B The memory of the vector b
 * \param ldb The leading dimension of b
 */
inline void axmy(size_t n, float* alpha, float* A , size_t lda, float* B , size_t ldb){
    egblas_saxmy(n, *alpha, A, lda, B, ldb);
}

#endif

#ifdef EGBLAS_HAS_DAXMY

/*!
 * \brief Wrappers for double-precision egblas axpy operation
 * \param n The size of the vector
 * \param alpha The scaling factor alpha
 * \param A The memory of the vector a
 * \param lda The leading dimension of a
 * \param B The memory of the vector b
 * \param ldb The leading dimension of b
 */
inline void axmy(size_t n, double* alpha, double* A , size_t lda, double* B , size_t ldb){
    egblas_daxmy(n, *alpha, A, lda, B, ldb);
}

#endif

} //end of namespace egblas
} //end of namespace impl
} //end of namespace etl
