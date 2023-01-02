// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "Utils.h"
#include "gtest/gtest.h"

using namespace leka::utils::math;

constexpr auto rounding_error = float {0.0001F};

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

	constexpr Point p1 {1.2566F, -2.4868F};
	constexpr Point p2 {3.4395F, 1.8790F};

	auto [slope, y_intercept] = computeLinearCoefficients(p1, p2);

	ASSERT_NEAR(slope, 2.0, rounding_error);
	ASSERT_NEAR(y_intercept, -5.0, rounding_error);
}

TEST(MathUtilsTest, computeLinearCoefficientsForFunction4)
{
	// function: y = -x/2 + 0.75

	constexpr Point p1 {-2.6860F, 2.0930F};
	constexpr Point p2 {3.2299F, -0.8649F};

	auto [slope, y_intercept] = computeLinearCoefficients(p1, p2);

	ASSERT_NEAR(slope, -0.5, rounding_error);
	ASSERT_NEAR(y_intercept, 0.75, rounding_error);
}

TEST(MathUtilsTest, computeLinearCoefficientsWithPointsHavingSameXValue)
{
	constexpr Point p1 {-2, -2};
	constexpr Point p2 {-2, 3};

	auto [slope, y_intercept] = computeLinearCoefficients(p2, p1);

	ASSERT_NEAR(slope, 0.0, rounding_error);
	ASSERT_NEAR(y_intercept, 0.0, rounding_error);
}
