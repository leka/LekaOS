// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "Utils.h"
#include "gtest/gtest.h"

using namespace leka::utils::math;

auto rounding_error = float {0.0001};

TEST(MathUtilsTest, computeLinearCoefficientsForFunction1)
{
	// function: y = x

	Point p1 {-2, -2};
	Point p2 {3, 3};

	auto [slope, y_intercept] = computeLinearCoefficients(p1, p2);

	ASSERT_NEAR(slope, 1.0, rounding_error);
	ASSERT_NEAR(y_intercept, 0.0, rounding_error);
}

TEST(MathUtilsTest, computeLinearCoefficientsForFunction2)
{
	// function: y = -x

	Point p1 {-2, 2};
	Point p2 {3, -3};

	auto [slope, y_intercept] = computeLinearCoefficients(p1, p2);

	ASSERT_NEAR(slope, -1.0, rounding_error);
	ASSERT_NEAR(y_intercept, 0.0, rounding_error);
}

TEST(MathUtilsTest, computeLinearCoefficientsForFunction3)
{
	// function: y = 2x - 5

	Point p1 {1.2566, -2.4868};
	Point p2 {3.4395, 1.8790};

	auto [slope, y_intercept] = computeLinearCoefficients(p1, p2);

	ASSERT_NEAR(slope, 2.0, rounding_error);
	ASSERT_NEAR(y_intercept, -5.0, rounding_error);
}

TEST(MathUtilsTest, computeLinearCoefficientsForFunction4)
{
	// function: y = -x/2 + 0.75

	Point p1 {-2.6860, 2.0930};
	Point p2 {3.2299, -0.8649};

	auto [slope, y_intercept] = computeLinearCoefficients(p1, p2);

	ASSERT_NEAR(slope, -0.5, rounding_error);
	ASSERT_NEAR(y_intercept, 0.75, rounding_error);
}

TEST(MathUtilsTest, computeLinearCoefficientsWithPointsHavingSameXValue)
{
	Point p1 {-2, -2};
	Point p2 {-2, 3};

	auto [slope, y_intercept] = computeLinearCoefficients(p2, p1);

	ASSERT_NEAR(slope, 0.0, rounding_error);
	ASSERT_NEAR(y_intercept, 0.0, rounding_error);
}
