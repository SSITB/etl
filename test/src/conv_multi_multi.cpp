//=======================================================================
// Copyright (c) 2014-2016 Baptiste Wicht
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

#include "test.hpp"
#include "conv_test.hpp"

CONV2_VALID_MULTI_MULTI_TEST_CASE("conv_2d/valid/multi_multi/1", "[conv][conv2][conv_multi_multi]") {
    etl::fast_matrix<T, 5, 7, 7> I;
    etl::fast_matrix<T, 3, 5, 3> K;

    etl::fast_matrix<T, 3, 5, 3, 5> C;
    etl::fast_matrix<T, 3, 5, 3, 5> C_ref;

    I = 0.5 * etl::sequence_generator(1.0);
    K = 0.123 * etl::sequence_generator(1.0);

    for (size_t k = 0; k < etl::dim<0>(K); ++k) {
        for (size_t i = 0; i < etl::dim<0>(I); ++i) {
            C_ref(k)(i) = conv_2d_valid(I(i), K(k));
        }
    }

    Impl::apply(I, K, C);

    for (std::size_t i = 0; i < etl::size(C_ref); ++i) {
        REQUIRE_EQUALS_APPROX(C[i], C_ref[i]);
    }
}

CONV2_VALID_MULTI_MULTI_TEST_CASE("conv_2d/valid/multi_multi/2", "[conv][conv2][conv_multi_multi]") {
    etl::fast_matrix<T, 6, 9, 7> I;
    etl::fast_matrix<T, 9, 5, 2> K;

    etl::fast_matrix<T, 9, 6, 5, 6> C;
    etl::fast_matrix<T, 9, 6, 5, 6> C_ref;

    I = -0.5 * etl::sequence_generator(10.0);
    K = 0.123 * etl::sequence_generator(2.0);

    for (size_t k = 0; k < etl::dim<0>(K); ++k) {
        for (size_t i = 0; i < etl::dim<0>(I); ++i) {
            C_ref(k)(i) = conv_2d_valid(I(i), K(k));
        }
    }

    Impl::apply(I, K, C);

    for (std::size_t i = 0; i < etl::size(C_ref); ++i) {
        REQUIRE_EQUALS_APPROX(C[i], C_ref[i]);
    }
}

CONV2_VALID_MULTI_MULTI_TEST_CASE("conv_2d/valid/multi_multi/3", "[conv][conv2][conv_multi_multi]") {
    etl::fast_matrix<T, 7, 10, 7> I;
    etl::fast_matrix<T, 8, 5, 2> K;

    etl::fast_matrix<T, 8, 7, 6, 6> C;
    etl::fast_matrix<T, 8, 7, 6, 6> C_ref;

    I = -0.66 * etl::sequence_generator(3.0);
    K = 0.23 * etl::sequence_generator(2.0);

    for (size_t k = 0; k < etl::dim<0>(K); ++k) {
        for (size_t i = 0; i < etl::dim<0>(I); ++i) {
            C_ref(k)(i) = conv_2d_valid(I(i), K(k));
        }
    }

    Impl::apply(I, K, C);

    for (std::size_t i = 0; i < etl::size(C_ref); ++i) {
        REQUIRE_EQUALS_APPROX(C[i], C_ref[i]);
    }
}

CONV2_VALID_MULTI_MULTI_FLIPPED_TEST_CASE("conv_2d/valid/multi_multi_flipped/1", "[conv][conv2][conv_multi_multi]") {
    etl::fast_matrix<T, 5, 7, 7> I;
    etl::fast_matrix<T, 3, 5, 3> K;

    etl::fast_matrix<T, 3, 5, 3, 5> C;
    etl::fast_matrix<T, 3, 5, 3, 5> C_ref;

    I = 0.5 * etl::sequence_generator(1.0);
    K = 0.123 * etl::sequence_generator(1.0);

    for (size_t k = 0; k < etl::dim<0>(K); ++k) {
        for (size_t i = 0; i < etl::dim<0>(I); ++i) {
            C_ref(k)(i) = conv_2d_valid_flipped(I(i), K(k));
        }
    }

    Impl::apply(I, K, C);

    for (std::size_t i = 0; i < etl::size(C_ref); ++i) {
        REQUIRE_EQUALS_APPROX(C[i], C_ref[i]);
    }
}

CONV2_VALID_MULTI_MULTI_FLIPPED_TEST_CASE("conv_2d/valid/multi_multi_flipped/2", "[conv][conv2][conv_multi_multi]") {
    etl::fast_matrix<T, 6, 9, 7> I;
    etl::fast_matrix<T, 9, 5, 2> K;

    etl::fast_matrix<T, 9, 6, 5, 6> C;
    etl::fast_matrix<T, 9, 6, 5, 6> C_ref;

    I = -0.5 * etl::sequence_generator(10.0);
    K = 0.123 * etl::sequence_generator(2.0);

    for (size_t k = 0; k < etl::dim<0>(K); ++k) {
        for (size_t i = 0; i < etl::dim<0>(I); ++i) {
            C_ref(k)(i) = conv_2d_valid_flipped(I(i), K(k));
        }
    }

    Impl::apply(I, K, C);

    for (std::size_t i = 0; i < etl::size(C_ref); ++i) {
        REQUIRE_EQUALS_APPROX(C[i], C_ref[i]);
    }
}

CONV2_VALID_MULTI_MULTI_FLIPPED_TEST_CASE("conv_2d/valid/multi_multi_flipped/3", "[conv][conv2][conv_multi_multi]") {
    etl::fast_matrix<T, 7, 10, 7> I;
    etl::fast_matrix<T, 8, 5, 2> K;

    etl::fast_matrix<T, 8, 7, 6, 6> C;
    etl::fast_matrix<T, 8, 7, 6, 6> C_ref;

    I = -0.66 * etl::sequence_generator(3.0);
    K = 0.23 * etl::sequence_generator(2.0);

    for (size_t k = 0; k < etl::dim<0>(K); ++k) {
        for (size_t i = 0; i < etl::dim<0>(I); ++i) {
            C_ref(k)(i) = conv_2d_valid_flipped(I(i), K(k));
        }
    }

    Impl::apply(I, K, C);

    for (std::size_t i = 0; i < etl::size(C_ref); ++i) {
        REQUIRE_EQUALS_APPROX(C[i], C_ref[i]);
    }
}