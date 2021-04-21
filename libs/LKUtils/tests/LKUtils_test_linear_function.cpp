// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LKUtils.h"
#include "gtest/gtest.h"

using namespace leka::utils;

TEST(LKUtilsTest, LinearFunctionyInterceptValuePair)
{
	float x1 = 0.0;
	float x2 = 1.0;
	float y1 = 5.0;
	float y2 = 6.0;

	std::pair<float, float> coefficients;
	coefficients = computeLinearCoefficientsPair(x1, x2, y1, y2);

	ASSERT_EQ(coefficients.first, 1.0);
	ASSERT_EQ(coefficients.second, 5.0);
}

TEST(LKUtilsTest, LinearFunctionDividedByZeroPair)
{
	float x1 = 5.0;
	float x2 = 5.0;
	float y1 = 0.0;
	float y2 = 1.0;

	std::pair<float, float> coefficients;
	coefficients = computeLinearCoefficientsPair(x1, x2, y1, y2);

	ASSERT_EQ(coefficients.first, 0);
	ASSERT_EQ(coefficients.second, 0);
}

TEST(LKUtilsTest, LinearFunctionyInterceptValueTuple)
{
	float x1 = 0.0;
	float x2 = 1.0;
	float y1 = 5.0;
	float y2 = 6.0;

	auto coefficients = computeLinearCoefficientsPair(x1, x2, y1, y2);

	ASSERT_EQ(std::get<0>(coefficients), 1.0);
	ASSERT_EQ(std::get<1>(coefficients), 5.0);
}

TEST(LKUtilsTest, LinearFunctionDividedByZeroTuple)
{
	float x1 = 5.0;
	float x2 = 5.0;
	float y1 = 0.0;
	float y2 = 1.0;

	auto coefficients = computeLinearCoefficientsPair(x1, x2, y1, y2);

	ASSERT_EQ(std::get<0>(coefficients), 0);
	ASSERT_EQ(std::get<1>(coefficients), 0);
}
