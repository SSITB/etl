//=======================================================================
// Copyright (c) 2014-2016 Baptiste Wicht
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

#include "test.hpp"
#include "conv_test.hpp"

CONV2_VALID_TEST_CASE("conv/2/stride/valid/1", "[conv][stride]") {
    etl::fast_matrix<T, 4, 4> a = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0};
    etl::fast_matrix<T, 2, 2> b = {1.0, 0.0, 0.5, 0.5};
    etl::fast_matrix<T, 2, 2> c;

    Impl::template apply<2, 2>(a, b, c);

    REQUIRE_EQUALS_APPROX(c(0, 0), T(7.5));
    REQUIRE_EQUALS_APPROX(c(0, 1), T(11.5));
    REQUIRE_EQUALS_APPROX(c(1, 0), T(23.5));
    REQUIRE_EQUALS_APPROX(c(1, 1), T(27.5));
}

CONV2_VALID_TEST_CASE("conv/2/stride/valid/2", "[conv][stride]") {
    etl::fast_matrix<T, 3, 3> a = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0};
    etl::fast_matrix<T, 2, 2> b = {1.0, 0.0, 0.5, 0.5};
    etl::fast_matrix<T, 4, 4> c;

    c = selected_helper(etl::conv_impl::STD, (etl::conv_2d_valid<1,1,1,1>(a, b)));
    //Impl::template apply<1, 1, 1, 1>(a, b, c);

    REQUIRE_EQUALS_APPROX(c(0, 0), T(1.0));
    REQUIRE_EQUALS_APPROX(c(0, 1), T(2.0));
    REQUIRE_EQUALS_APPROX(c(0, 2), T(3.0));
    REQUIRE_EQUALS_APPROX(c(0, 3), T(0.0));

    REQUIRE_EQUALS_APPROX(c(1, 0), T(4.5));
    REQUIRE_EQUALS_APPROX(c(1, 1), T(6.5));
    REQUIRE_EQUALS_APPROX(c(1, 2), T(8.5));
    REQUIRE_EQUALS_APPROX(c(1, 3), T(1.5));

    REQUIRE_EQUALS_APPROX(c(2, 0), T(9.0));
    REQUIRE_EQUALS_APPROX(c(2, 1), T(12.5));
    REQUIRE_EQUALS_APPROX(c(2, 2), T(14.5));
    REQUIRE_EQUALS_APPROX(c(2, 3), T(3.0));

    REQUIRE_EQUALS_APPROX(c(3, 0), T(3.5));
    REQUIRE_EQUALS_APPROX(c(3, 1), T(7.5));
    REQUIRE_EQUALS_APPROX(c(3, 2), T(8.5));
    REQUIRE_EQUALS_APPROX(c(3, 3), T(4.5));
}

CONV2_VALID_FLIPPED_TEST_CASE("conv/2/stride/valid/flipped/1", "[conv][stride]") {
    etl::fast_matrix<T, 4, 4> a = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0};
    etl::fast_matrix<T, 2, 2> b = {0.5, 0.5, 0.0, 1.0};
    etl::fast_matrix<T, 2, 2> c;

    Impl::template apply<2, 2>(a, b, c);

    REQUIRE_EQUALS_APPROX(c(0, 0), T(7.5));
    REQUIRE_EQUALS_APPROX(c(0, 1), T(11.5));
    REQUIRE_EQUALS_APPROX(c(1, 0), T(23.5));
    REQUIRE_EQUALS_APPROX(c(1, 1), T(27.5));
}
