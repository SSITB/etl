//=======================================================================
// Copyright (c) 2014-2017 Baptiste Wicht
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

#include "test.hpp"
#include "etl/stop.hpp"

#include "mmul_test.hpp"

// Matrix Matrix multiplication tests

GEMM_TT_TEST_CASE("gemm_tt/cm/1", "[gemm]") {
    etl::fast_matrix_cm<T, 2, 3> aa = {1, 4, 2, 5, 3, 6};
    etl::fast_matrix_cm<T, 3, 2> bb = {7, 9, 11, 8, 10, 12};

    etl::fast_matrix_cm<T, 3, 2> a;
    etl::fast_matrix_cm<T, 2, 3> b;

    a = transpose(aa);
    b = transpose(bb);

    etl::fast_matrix_cm<T, 2, 2> c;

    Impl::apply(a, b, c);

    REQUIRE_EQUALS(c(0, 0), 58);
    REQUIRE_EQUALS(c(0, 1), 64);
    REQUIRE_EQUALS(c(1, 0), 139);
    REQUIRE_EQUALS(c(1, 1), 154);
}

GEMM_TT_TEST_CASE("gemm_tt/cm/2", "[gemm]") {
    etl::fast_matrix_cm<T, 3, 3> a = {1, 4, 7, 2, 5, 8, 3, 6, 9};
    etl::fast_matrix_cm<T, 3, 3> b = {7, 9, 11, 8, 10, 12, 9, 11, 13};
    etl::fast_matrix_cm<T, 3, 3> c;

    a = transpose(a);
    b = transpose(b);

    Impl::apply(a, b, c);

    REQUIRE_EQUALS(c(0, 0), 58);
    REQUIRE_EQUALS(c(0, 1), 64);
    REQUIRE_EQUALS(c(0, 2), 70);
    REQUIRE_EQUALS(c(1, 0), 139);
    REQUIRE_EQUALS(c(1, 1), 154);
    REQUIRE_EQUALS(c(1, 2), 169);
    REQUIRE_EQUALS(c(2, 0), 220);
    REQUIRE_EQUALS(c(2, 1), 244);
    REQUIRE_EQUALS(c(2, 2), 268);
}

GEMM_TT_TEST_CASE("gemm_tt/cm/3", "[gemm]") {
    etl::dyn_matrix_cm<T> a(4, 4, etl::values(1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15, 4, 8, 12, 16));
    etl::dyn_matrix_cm<T> b(4, 4, etl::values(1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15, 4, 8, 12, 16));
    etl::dyn_matrix_cm<T> c(4, 4);

    a = transpose(a);
    b = transpose(b);

    Impl::apply(a, b, c);

    REQUIRE_EQUALS(c(0, 0), 90);
    REQUIRE_EQUALS(c(0, 1), 100);
    REQUIRE_EQUALS(c(1, 0), 202);
    REQUIRE_EQUALS(c(1, 1), 228);
    REQUIRE_EQUALS(c(2, 0), 314);
    REQUIRE_EQUALS(c(2, 1), 356);
    REQUIRE_EQUALS(c(3, 0), 426);
    REQUIRE_EQUALS(c(3, 1), 484);
}

GEMM_TT_TEST_CASE("gemm_tt/cm/4", "[gemm]") {
    etl::dyn_matrix_cm<T> a(2, 2, etl::values(1, 3, 2, 4));
    etl::dyn_matrix_cm<T> b(2, 2, etl::values(1, 3, 2, 4));
    etl::dyn_matrix_cm<T> c(2, 2);

    a = transpose(a);
    b = transpose(b);

    Impl::apply(a, b, c);

    REQUIRE_EQUALS(c(0, 0), 7);
    REQUIRE_EQUALS(c(0, 1), 10);
    REQUIRE_EQUALS(c(1, 0), 15);
    REQUIRE_EQUALS(c(1, 1), 22);
}

GEMM_TT_TEST_CASE("gemm_tt/cm/5", "[gemm]") {
    etl::dyn_matrix_cm<T> a(3, 3, std::initializer_list<T>({1, 4, 7, 2, 5, 8, 3, 6, 9}));
    etl::dyn_matrix_cm<T> b(3, 3, std::initializer_list<T>({7, 9, 11, 8, 10, 12, 9, 11, 13}));
    etl::dyn_matrix_cm<T> c(3, 3);

    a = transpose(a);
    b = transpose(b);

    Impl::apply(a, b, c);

    REQUIRE_EQUALS(c(0, 0), 58);
    REQUIRE_EQUALS(c(0, 1), 64);
    REQUIRE_EQUALS(c(0, 2), 70);
    REQUIRE_EQUALS(c(1, 0), 139);
    REQUIRE_EQUALS(c(1, 1), 154);
    REQUIRE_EQUALS(c(1, 2), 169);
    REQUIRE_EQUALS(c(2, 0), 220);
    REQUIRE_EQUALS(c(2, 1), 244);
    REQUIRE_EQUALS(c(2, 2), 268);
}

GEMM_TT_TEST_CASE("gemm_tt/cm/6", "[gemm]") {
    etl::fast_matrix_cm<T, 19, 19> a;
    etl::fast_matrix_cm<T, 19, 19> b;
    etl::fast_matrix_cm<T, 19, 19> c;

    a = etl::magic(19);
    b = etl::magic(19);

    a = transpose(a);
    b = transpose(b);

    Impl::apply(a, b, c);

    REQUIRE_EQUALS(c(0, 0), 828343);
    REQUIRE_EQUALS(c(1, 1), 825360);
    REQUIRE_EQUALS(c(2, 2), 826253);
    REQUIRE_EQUALS(c(3, 3), 824524);
    REQUIRE_EQUALS(c(18, 18), 828343);
}

GEMM_TT_TEST_CASE("gemm_tt/cm/7", "[gemm]") {
    etl::dyn_matrix_cm<T> a(128, 128);
    etl::dyn_matrix_cm<T> b(128, 128);
    etl::dyn_matrix_cm<T> c(128, 128);
    etl::dyn_matrix_cm<T> r(128, 128);

    a = 0.01 * etl::sequence_generator(1.0);
    b = -0.032 * etl::sequence_generator(1.0);

    Impl::apply(a, b, c);

    r = 0;

    a.transpose_inplace();
    b.transpose_inplace();

    for (size_t i = 0; i < rows(a); i++) {
        for (size_t k = 0; k < columns(a); k++) {
            for (size_t j = 0; j < columns(b); j++) {
                r(i, j) += a(i, k) * b(k, j);
            }
        }
    }

    REQUIRE_DIRECT(etl::approx_equals(c, r, base_eps_etl_large));
}

GEMM_TT_TEST_CASE("gemm_tt/cm/8", "[gemm]") {
    etl::dyn_matrix_cm<T> a(128, 96);
    etl::dyn_matrix_cm<T> b(96, 128);
    etl::dyn_matrix_cm<T> c(96, 96);
    etl::dyn_matrix_cm<T> r(96, 96);

    a = 0.01 * etl::sequence_generator(1.0);
    b = -0.032 * etl::sequence_generator(1.0);

    Impl::apply(a, b, c);

    r = 0;

    a.transpose_inplace();
    b.transpose_inplace();

    for (size_t i = 0; i < rows(a); i++) {
        for (size_t k = 0; k < columns(a); k++) {
            for (size_t j = 0; j < columns(b); j++) {
                r(i, j) += a(i, k) * b(k, j);
            }
        }
    }

    REQUIRE_DIRECT(etl::approx_equals(c, r, base_eps_etl_large));
}

GEMM_TT_TEST_CASE("gemm_tt/cm/9", "[gemm]") {
    etl::dyn_matrix_cm<T> a(96, 156);
    etl::dyn_matrix_cm<T> b(128, 96);
    etl::dyn_matrix_cm<T> c(156, 128);
    etl::dyn_matrix_cm<T> r(156, 128);

    a = 0.01 * etl::sequence_generator(1.0);
    b = -0.032 * etl::sequence_generator(1.0);

    Impl::apply(a, b, c);

    r = 0;

    a.transpose_inplace();
    b.transpose_inplace();

    for (size_t i = 0; i < rows(a); i++) {
        for (size_t k = 0; k < columns(a); k++) {
            for (size_t j = 0; j < columns(b); j++) {
                r(i, j) += a(i, k) * b(k, j);
            }
        }
    }

    REQUIRE_DIRECT(etl::approx_equals(c, r, base_eps_etl_large));
}
