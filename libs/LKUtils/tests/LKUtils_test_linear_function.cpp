// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LKUtils.h"
#include "gtest/gtest.h"

using namespace leka::utils;

TEST(LKUtilsTest, LinearFunctionyInterceptValue)
{
	float x1 = 0.0;
	float x2 = 1.0;
	float y1 = 5.0;
	float y2 = 6.0;

	LinearFunctionCoefficients coefficients;
	coefficients = computeLinearCoefficients(x1, x2, y1, y2);

	ASSERT_EQ(coefficients.slope, 1.0);
	ASSERT_EQ(coefficients.y_intercept, 5.0);
}

TEST(LKUtilsTest, LinearFunctionDividedByZero)
{
	float x1 = 5.0;
	float x2 = 5.0;
	float y1 = 0.0;
	float y2 = 1.0;

	LinearFunctionCoefficients coefficients;
	coefficients = computeLinearCoefficients(x1, x2, y1, y2);

	ASSERT_EQ(coefficients.slope, 0);
	ASSERT_EQ(coefficients.y_intercept, 0);
}

TEST(LKUtilsTest, LinearFunctionyInterceptValueTuple)
{
	float x1 = 0.0;
	float x2 = 1.0;
	float y1 = 5.0;
	float y2 = 6.0;

	auto coefficients = leka::utils::computeLinearCoefficientsTuple(x1, x2, y1, y2);

	ASSERT_EQ(std::get<0>(coefficients), 1.0);
	ASSERT_EQ(std::get<1>(coefficients), 5.0);
}

TEST(LKUtilsTest, LinearFunctionDividedByZeroTuple)
{
	float x1 = 5.0;
	float x2 = 5.0;
	float y1 = 0.0;
	float y2 = 1.0;

	auto coefficients = leka::utils::computeLinearCoefficientsTuple(x1, x2, y1, y2);

	ASSERT_EQ(std::get<0>(coefficients), 0);
	ASSERT_EQ(std::get<1>(coefficients), 0);
}
