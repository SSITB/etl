//=======================================================================
// Copyright (c) 2014-2017 Baptiste Wicht
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

/*!
 * \file
 * \brief Contains lower triangular matrix reference proxy implementation.
 */

#pragma once

namespace etl {

namespace lower_detail {

/*!
 * \brief A proxy representing a reference to a mutable element of a lower triangular matrix
 * \tparam M The matrix type
 */
template <typename M>
struct lower_reference {
    using matrix_type              = M;                                ///< The matrix type
    using value_type               = typename matrix_type::value_type; ///< The value type
    using raw_pointer_type         = value_type*;                      ///< A raw pointer type
    using raw_reference_type       = value_type&;                      ///< A raw reference type
    using const_raw_reference_type = std::add_const_t<value_type>&;    ///< A raw reference type
    using expr_t                   = M;                                ///< The lower triangular matrix

    matrix_type& matrix;   ///< Reference to the matrix
    std::size_t i;         ///< The first index
    std::size_t j;         ///< The second index
    value_type& value;     ///< Reference to the value

    /*!
     * \brief Constructs a new lower_reference
     * \param matrix The source matrix
     * \param i The index i of the first dimension
     * \param j The index j of the second dimension
     */
    lower_reference(matrix_type& matrix, std::size_t i, std::size_t j)
            : matrix(matrix), i(i), j(j), value(matrix(i, j)) {
        //Nothing else to init
    }

    /*!
     * \brief Sets a new value to the proxy reference
     * \param rhs The new value
     * \return a reference to the proxy reference
     */
    lower_reference& operator=(const value_type& rhs) {
        if(i >= j || rhs == value_type(0)){
            value = rhs;
        } else {
            throw lower_exception();
        }

        return *this;
    }

    /*!
     * \brief Adds a new value to the proxy reference
     * \param rhs The new value
     * \return a reference to the proxy reference
     */
    lower_reference& operator+=(value_type rhs) {
        if(i >= j || rhs == value_type(0)){
            value += rhs;
        } else {
            throw lower_exception();
        }

        return *this;
    }

    /*!
     * \brief Subtract a new value from the proxy reference
     * \param rhs The new value
     * \return a reference to the proxy reference
     */
    lower_reference& operator-=(value_type rhs) {
        if(i >= j || rhs == value_type(0)){
            value -= rhs;
        } else {
            throw lower_exception();
        }

        return *this;
    }

    /*!
     * \brief Multiply by a new value the proxy reference
     * \param rhs The new value
     * \return a reference to the proxy reference
     */
    lower_reference& operator*=(value_type rhs) {
        if(i >= j || rhs == value_type(1)){
            value *= rhs;
        } else {
            throw lower_exception();
        }

        return *this;
    }

    /*!
     * \brief Divide by a new value the proxy reference
     * \param rhs The new value
     * \return a reference to the proxy reference
     */
    lower_reference& operator/=(value_type rhs) {
        if(i >= j || rhs == value_type(1)){
            value /= rhs;
        } else {
            throw lower_exception();
        }

        return *this;
    }

    /*!
     * \brief Modulo by a new value the proxy reference
     * \param rhs The new value
     * \return a reference to the proxy reference
     */
    lower_reference& operator%=(value_type rhs) {
        if(i >= j || (value % rhs) == value_type(0)){
            value %= rhs;
        } else {
            throw lower_exception();
        }

        return *this;
    }

    /*!
     * \brief Casts the proxy reference to the raw reference type
     * \return a raw reference to the element
     */
    operator const_raw_reference_type&() const {
        return value;
    }
};

} //end of namespace lower_detail

} //end of namespace etl
