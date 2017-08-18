//=======================================================================
// Copyright (c) 2014-2017 Baptiste Wicht
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

#pragma once

#include "etl/expr/base_temporary_expr.hpp"

//Get the implementations
#include "etl/impl/conv.hpp"

namespace etl {

/*!
 * \brief A transposition expression.
 * \tparam A The transposed type
 */
template <typename A, typename B, bool Flipped>
struct conv_2d_same_multi_expr : base_temporary_expr_bin<conv_2d_same_multi_expr<A, B, Flipped>, A, B> {
    using value_type  = value_t<A>;                               ///< The type of value of the expression
    using this_type   = conv_2d_same_multi_expr<A, B, Flipped>;         ///< The type of this expression
    using base_type   = base_temporary_expr_bin<this_type, A, B>; ///< The base type
    using left_traits = decay_traits<A>;                          ///< The traits of the sub type

    static constexpr auto storage_order = left_traits::storage_order; ///< The sub storage order

    /*!
     * \brief Construct a new expression
     * \param a The sub expression
     */
    explicit conv_2d_same_multi_expr(A a, B b) : base_type(a, b) {
        //Nothing else to init
    }

    // Assignment functions

    /*!
     * \brief Assert that the convolution is done on correct dimensions
     */
    template <typename I, typename K, typename C, cpp_disable_iff(all_fast<A, B, C>)>
    static void check(const I& input, const K& kernel, const C& conv){
        static_assert(etl::dimensions<I>() == 2, "Invalid number of dimensions for input of conv2_same_multi");
        static_assert(etl::dimensions<K>() == 3, "Invalid number of dimensions for kernel of conv2_same_multi");
        static_assert(etl::dimensions<C>() == 3, "Invalid number of dimensions for conv of conv2_same_multi");

        cpp_assert(etl::dim(conv, 0) == etl::dim(kernel, 0), "Invalid dimensions for conv2_same_multi");
        cpp_assert(etl::dim(conv, 1) == etl::dim(input, 1), "Invalid dimensions for conv2_same_multi");
        cpp_assert(etl::dim(conv, 2) == etl::dim(input, 2), "Invalid dimensions for conv2_same_multi");

        cpp_unused(input);
        cpp_unused(kernel);
        cpp_unused(conv);
    }

    /*!
     * \brief Assert that the convolution is done on correct dimensions
     */
    template <typename I, typename K, typename C, cpp_enable_iff(all_fast<A, B, C>)>
    static void check(const I& input, const K& kernel, const C& conv){
        static_assert(etl::dimensions<I>() == 2, "Invalid number of dimensions for input of conv2_same_multi");
        static_assert(etl::dimensions<K>() == 3, "Invalid number of dimensions for kernel of conv2_same_multi");
        static_assert(etl::dimensions<C>() == 3, "Invalid number of dimensions for conv of conv2_same_multi");

        static_assert(etl::dim<0, C>() == etl::dim<0, K>(), "Invalid dimensions for conv2_same_multi");
        static_assert(etl::dim<1, C>() == etl::dim<0, I>(), "Invalid dimensions for conv2_same_multi");
        static_assert(etl::dim<2, C>() == etl::dim<1, I>(), "Invalid dimensions for conv2_same_multi");

        cpp_unused(input);
        cpp_unused(kernel);
        cpp_unused(conv);
    }

    /*!
     * \brief Select the implementation of the conv multi of I and K in C
     *
     * This does not take the local context into account.
     *
     * \tparam C The conv type
     * \return the implementation to be used
     */
    template <typename C>
    static etl::conv_multi_impl select_default_impl() {
        //Note: since the constexpr values will be known at compile time, the
        //conditions will be a lot simplified

        static constexpr order input_order  = decay_traits<A>::storage_order;
        static constexpr order kernel_order = decay_traits<B>::storage_order;
        static constexpr order output_order = decay_traits<C>::storage_order;

        //Only the standard implementation is able to handle column major
        if (input_order == order::ColumnMajor || kernel_order == order::ColumnMajor || output_order == order::ColumnMajor) {
            return etl::conv_multi_impl::STD;
        }

        if (impl::vec::conv2_possible<vector_mode, A, B, C>) {
            return etl::conv_multi_impl::VEC;
        }

        return etl::conv_multi_impl::STD;
    }

    /*!
     * \brief Select the implementation of the conv of I and K in C
     * \tparam C The conv type
     * \return the implementation to be used
     */
    template <typename C>
    static inline etl::conv_multi_impl select_impl() {
        auto default_impl = select_default_impl<C>();

        if (local_context().conv_multi_selector.forced) {
            auto forced = local_context().conv_multi_selector.impl;

            switch (forced) {
                //VEC cannot always be used
                case conv_multi_impl::VEC:
                    if (!impl::vec::conv2_possible<vector_mode, A, B, C>) {
                        std::cerr << "Forced selection to VEC conv2_same_multi implementation, but not possible for this expression" << std::endl;
                        return default_impl;
                    }

                    return forced;


                    // Although it may be suboptimal the forced selection can
                    // always be achieved
                default:
                    return forced;
            }
        }

        return default_impl;
    }

    /*!
     * \brief Assign to a matrix of the same storage order
     * \param conv The expression to which assign
     */
    template<typename C>
    void assign_to(C&& conv)  const {
        static_assert(all_etl_expr<A, B, C>, "conv2_same_multi only supported for ETL expressions");

        if(this->is_evaluated()){
            conv = this->result();
            return;
        }

        auto& input = this->a();
        auto& kernel = this->b();

        check(input, kernel, conv);

        standard_evaluator::pre_assign_rhs(input);
        standard_evaluator::pre_assign_rhs(kernel);

        auto impl = select_impl<C>();

        if /* constexpr */ (Flipped){
            if (impl == etl::conv_multi_impl::VEC) {
                impl::vec::conv2_same_multi_flipped(make_temporary(input), make_temporary(kernel), conv);
            } else if (impl == etl::conv_multi_impl::STD) {
                impl::standard::conv2_same_multi_flipped(make_temporary(input), make_temporary(kernel), conv);
            } else {
                cpp_unreachable("Invalid conv implementation selection");
            }
        } else {
            if (impl == etl::conv_multi_impl::VEC){
                impl::vec::conv2_same_multi(make_temporary(input), make_temporary(kernel), conv);
            } else if (impl == etl::conv_multi_impl::STD){
                impl::standard::conv2_same_multi(make_temporary(input), make_temporary(kernel), conv);
            } else {
                cpp_unreachable("Invalid conv implementation selection");
            }
        }
    }

    /*!
     * \brief Add to the given left-hand-side expression
     * \param lhs The expression to which assign
     */
    template<typename L>
    void assign_add_to(L&& lhs)  const {
        std_add_evaluate(*this, lhs);
    }

    /*!
     * \brief Sub from the given left-hand-side expression
     * \param lhs The expression to which assign
     */
    template<typename L>
    void assign_sub_to(L&& lhs)  const {
        std_sub_evaluate(*this, lhs);
    }

    /*!
     * \brief Multiply the given left-hand-side expression
     * \param lhs The expression to which assign
     */
    template<typename L>
    void assign_mul_to(L&& lhs)  const {
        std_mul_evaluate(*this, lhs);
    }

    /*!
     * \brief Divide the given left-hand-side expression
     * \param lhs The expression to which assign
     */
    template<typename L>
    void assign_div_to(L&& lhs)  const {
        std_div_evaluate(*this, lhs);
    }

    /*!
     * \brief Modulo the given left-hand-side expression
     * \param lhs The expression to which assign
     */
    template<typename L>
    void assign_mod_to(L&& lhs)  const {
        std_mod_evaluate(*this, lhs);
    }

    /*!
     * \brief Print a representation of the expression on the given stream
     * \param os The output stream
     * \param expr The expression to print
     * \return the output stream
     */
    friend std::ostream& operator<<(std::ostream& os, const conv_2d_same_multi_expr& expr) {
        return os << "conv2_same_multi(" << expr._a << ", " << expr._b << ")";
    }
};

/*!
 * \brief Traits for a transpose expression
 * \tparam A The transposed sub type
 */
template <typename A, typename B, bool Flipped>
struct etl_traits<etl::conv_2d_same_multi_expr<A, B, Flipped>> {
    using expr_t       = etl::conv_2d_same_multi_expr<A, B, Flipped>; ///< The expression type
    using left_expr_t  = std::decay_t<A>;                       ///< The left sub expression type
    using right_expr_t = std::decay_t<B>;                       ///< The right sub expression type
    using left_traits  = etl_traits<left_expr_t>;               ///< The left sub traits
    using right_traits = etl_traits<right_expr_t>;              ///< The right sub traits
    using value_type   = value_t<A>;                            ///< The value type of the expression

    static constexpr bool is_etl          = true;                       ///< Indicates if the type is an ETL expression
    static constexpr bool is_transformer  = false;                      ///< Indicates if the type is a transformer
    static constexpr bool is_view         = false;                      ///< Indicates if the type is a view
    static constexpr bool is_magic_view   = false;                      ///< Indicates if the type is a magic view
    static constexpr bool is_fast         = all_fast<A, B>;      ///< Indicates if the expression is fast
    static constexpr bool is_linear       = false;                       ///< Indicates if the expression is linear
    static constexpr bool is_thread_safe  = true;                       ///< Indicates if the expression is thread safe
    static constexpr bool is_value        = false;                      ///< Indicates if the expression is of value type
    static constexpr bool is_direct       = true;                       ///< Indicates if the expression has direct memory access
    static constexpr bool is_generator    = false;                      ///< Indicates if the expression is a generator
    static constexpr bool is_padded       = false;                      ///< Indicates if the expression is padded
    static constexpr bool is_aligned      = true;                       ///< Indicates if the expression is padded
    static constexpr bool is_temporary = true;                       ///< Indicates if the expression needs a evaluator visitor
    static constexpr bool gpu_computable = is_gpu_t<value_type> && cuda_enabled;                                         ///< Indicates if the expression can be computed on GPU
    static constexpr order storage_order  = left_traits::storage_order; ///< The expression's storage order

    /*!
     * \brief Indicates if the expression is vectorizable using the
     * given vector mode
     * \tparam V The vector mode
     */
    template <vector_mode_t V>
    static constexpr bool vectorizable = true;

    /*!
     * \brief Returns the DDth dimension of the expression
     * \return the DDth dimension of the expression
     */
    template <size_t DD>
    static constexpr size_t dim() {
        return DD == 0 ? etl::dim<0, B>()
             : DD == 1 ? etl::dim<0, A>()
                       : etl::dim<1, A>();
    }

    /*!
     * \brief Returns the dth dimension of the expression
     * \param e The sub expression
     * \param d The dimension to get
     * \return the dth dimension of the expression
     */
    static size_t dim(const expr_t& e, size_t d) {
        if(d == 0){
            return etl::dim(e._b, 0);
        } else if(d == 1){
            return etl::dim(e._a, 0);
        } else if(d == 2){
            return etl::dim(e._a, 1);
        }
    }

    /*!
     * \brief Returns the size of the expression
     * \param e The sub expression
     * \return the size of the expression
     */
    static size_t size(const expr_t& e) {
        return (etl::dim(e._b, 0))
            * (etl::dim(e._a, 0))
            * (etl::dim(e._a, 1));
    }

    /*!
     * \brief Returns the size of the expression
     * \return the size of the expression
     */
    static constexpr size_t size() {
        return (etl::dim<0, B>())
             * (etl::dim<0, A>())
             * (etl::dim<1, A>());
    }

    /*!
     * \brief Returns the number of dimensions of the expression
     * \return the number of dimensions of the expression
     */
    static constexpr size_t dimensions() {
        return 3;
    }
};

/*!
 * \brief Creates an expression representing the 'same' 1D convolution of a and b.
 *
 * The convolution is applied with padding so that the output has
 * the same size as the input.
 *
 * \param a The input expression
 * \param b The kernel expression
 *
 * \return an expression representing the 'same' 1D convolution of a and b
 */
template <typename A, typename B>
conv_2d_same_multi_expr<detail::build_type<A>, detail::build_type<B>, false> conv_2d_same_multi(A&& a, B&& b) {
    static_assert(all_etl_expr<A, B>, "Convolution only supported for ETL expressions");

    return conv_2d_same_multi_expr<detail::build_type<A>, detail::build_type<B>, false>{a, b};
}

/*!
 * \brief Creates an expression representing the 'same' 1D convolution of a and b, the result will be stored in c
 *
 * The convolution is applied with padding so that the output has
 * the same size as the input.
 *
 * \param a The input expression
 * \param b The kernel expression
 * \param c The result
 *
 * \return an expression representing the 'same' 1D convolution of a and b
 */
template <typename A, typename B, typename C>
auto conv_2d_same_multi(A&& a, B&& b, C&& c){
    static_assert(all_etl_expr<A, B, C>, "Convolution only supported for ETL expressions");

    c = conv_2d_same_multi(a, b);

    return c;
}

/*!
 * \brief Creates an expression representing the 'same' 1D convolution of a and flipped b.
 *
 * The convolution is applied with padding so that the output has
 * the same size as the input.
 *
 * \param a The input expression
 * \param b The kernel expression
 *
 * \return an expression representing the 'same' 1D convolution of a and b
 */
template <typename A, typename B>
conv_2d_same_multi_expr<detail::build_type<A>, detail::build_type<B>, true> conv_2d_same_multi_flipped(A&& a, B&& b) {
    static_assert(all_etl_expr<A, B>, "Convolution only supported for ETL expressions");

    return conv_2d_same_multi_expr<detail::build_type<A>, detail::build_type<B>, true>{a, b};
}

/*!
 * \brief Creates an expression representing the 'same' 1D convolution of a and flipped b, the result will be stored in c
 *
 * The convolution is applied with padding so that the output has
 * the same size as the input.
 *
 * \param a The input expression
 * \param b The kernel expression
 * \param c The result
 *
 * \return an expression representing the 'same' 1D convolution of a and b
 */
template <typename A, typename B, typename C>
auto conv_2d_same_multi_flipped(A&& a, B&& b, C&& c){
    static_assert(all_etl_expr<A, B, C>, "Convolution only supported for ETL expressions");

    c = conv_2d_same_multi_flipped(a, b);

    return c;
}

} //end of namespace etl
