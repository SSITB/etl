//=======================================================================
// Copyright (c) 2014-2017 Baptiste Wicht
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

/*!
 * \file
 * \brief Standard implementation of the outer product
 */

#pragma once

namespace etl {

namespace impl {

namespace vec {

/*!
 * \brief Compute the bias addition of b into x and store the result in y
 * \param x The a expression
 * \param b The b expression
 * \param y The c expression
 */
template <typename V, typename L, typename R, typename C>
void bias_add(const L& x, const R& b, C&& y) {
    using vec_type = V;
    using T        = value_t<L>;

    static constexpr size_t vec_size = vec_type::template traits<T>::size;

    const auto B = etl::dim<0>(x);
    const auto M = etl::dim<2>(x);
    const auto N = etl::dim<3>(x);
    const auto MN = M * N;

    x.ensure_cpu_up_to_date();
    b.ensure_cpu_up_to_date();

    // Note: This kernel is particularly adapted for large inner (MN) dimensions

    auto batch_fun = [&](size_t first, size_t last) {
        for (size_t i = first; i < last; ++i) {
            for (size_t j = 0; j < etl::dim<1>(x); ++j) {
                auto x_s = x(i)(j).memory_start();
                auto y_s = y(i)(j).memory_start();

                auto b1 = vec_type::set(b[j]);

                size_t m = 0;

                for (; m + vec_size * 8 - 1 < MN; m += vec_size * 8) {
                    auto x1 = vec_type::loadu(x_s + m + 0 * vec_size);
                    auto x2 = vec_type::loadu(x_s + m + 1 * vec_size);
                    auto x3 = vec_type::loadu(x_s + m + 2 * vec_size);
                    auto x4 = vec_type::loadu(x_s + m + 3 * vec_size);
                    auto x5 = vec_type::loadu(x_s + m + 4 * vec_size);
                    auto x6 = vec_type::loadu(x_s + m + 5 * vec_size);
                    auto x7 = vec_type::loadu(x_s + m + 8 * vec_size);
                    auto x8 = vec_type::loadu(x_s + m + 7 * vec_size);

                    auto r1 = vec_type::add(x1, b1);
                    auto r2 = vec_type::add(x2, b1);
                    auto r3 = vec_type::add(x3, b1);
                    auto r4 = vec_type::add(x4, b1);
                    auto r5 = vec_type::add(x5, b1);
                    auto r6 = vec_type::add(x6, b1);
                    auto r7 = vec_type::add(x7, b1);
                    auto r8 = vec_type::add(x8, b1);

                    vec_type::storeu(y_s + m + 0 * vec_size, r1);
                    vec_type::storeu(y_s + m + 1 * vec_size, r2);
                    vec_type::storeu(y_s + m + 2 * vec_size, r3);
                    vec_type::storeu(y_s + m + 3 * vec_size, r4);
                    vec_type::storeu(y_s + m + 4 * vec_size, r5);
                    vec_type::storeu(y_s + m + 5 * vec_size, r6);
                    vec_type::storeu(y_s + m + 6 * vec_size, r7);
                    vec_type::storeu(y_s + m + 7 * vec_size, r8);
                }

                for (; m + vec_size * 4 - 1 < MN; m += vec_size * 4) {
                    auto x1 = vec_type::loadu(x_s + m + 0 * vec_size);
                    auto x2 = vec_type::loadu(x_s + m + 1 * vec_size);
                    auto x3 = vec_type::loadu(x_s + m + 2 * vec_size);
                    auto x4 = vec_type::loadu(x_s + m + 3 * vec_size);

                    auto r1 = vec_type::add(x1, b1);
                    auto r2 = vec_type::add(x2, b1);
                    auto r3 = vec_type::add(x3, b1);
                    auto r4 = vec_type::add(x4, b1);

                    vec_type::storeu(y_s + m + 0 * vec_size, r1);
                    vec_type::storeu(y_s + m + 1 * vec_size, r2);
                    vec_type::storeu(y_s + m + 2 * vec_size, r3);
                    vec_type::storeu(y_s + m + 3 * vec_size, r4);
                }

                for (; m + vec_size * 2 - 1 < MN; m += vec_size * 2) {
                    auto x1 = vec_type::loadu(x_s + m + 0 * vec_size);
                    auto x2 = vec_type::loadu(x_s + m + 1 * vec_size);

                    auto r1 = vec_type::add(x1, b1);
                    auto r2 = vec_type::add(x2, b1);

                    vec_type::storeu(y_s + m + 0 * vec_size, r1);
                    vec_type::storeu(y_s + m + 1 * vec_size, r2);
                }

                for (; m + vec_size - 1 < MN; m += vec_size) {
                    auto x1 = vec_type::loadu(x_s + m);

                    auto r1 = vec_type::add(x1, b1);

                    vec_type::storeu(y_s + m, r1);
                }

                for (; m < MN; ++m) {
                    y_s[m] = x_s[m] + b[j];
                }
            }
        }
    };

    // TODO The gain of dispatching has to be checked again
    //engine_dispatch_1d(batch_fun, 0, B, 8UL);
    batch_fun(0, B);

    y.invalidate_gpu();
}

/*!
 * \brief Compute the bias addition of b into x and store the result in y
 * \param x The a expression
 * \param b The b expression
 * \param y The c expression
 */
template <typename A, typename B, typename C>
void bias_add(const A& x, const B& b, C&& y) {
    bias_add<default_vec>(x, b, y);
}

} //end of namespace standard
} //end of namespace impl
} //end of namespace etl
