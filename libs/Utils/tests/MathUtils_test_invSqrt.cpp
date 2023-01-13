// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "Utils.h"
#include "gtest/gtest.h"

using namespace leka::utils::math;

TEST(MathUtilsTest, invSqrtOfAQuarter)
{
	auto result = invSqrt(0.25);
	EXPECT_NEAR(result, 2.000F, 0.05);
}

TEST(MathUtilsTest, invSqrtOfAThird)
{
	auto result = invSqrt(0.33333);
	EXPECT_NEAR(result, 1.730F, 0.005);
}

TEST(MathUtilsTest, invSqrtOfAHalf)
{
	auto result = invSqrt(0.5);
	EXPECT_NEAR(result, 1.415F, 0.005);
}

TEST(MathUtilsTest, invSqrtOfTwoThird)
{
	auto result = invSqrt(0.6666);
	EXPECT_NEAR(result, 1.225F, 0.005);
}

TEST(MathUtilsTest, invSqrtOfThreeQuarter)
{
	auto result = invSqrt(0.75);
	EXPECT_NEAR(result, 1.155F, 0.005);
}

TEST(MathUtilsTest, invSqrtOfOne)
{
	auto result = invSqrt(1);
	EXPECT_NEAR(result, 1.000F, 0.005);
}
