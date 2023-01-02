// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "Utils.h"
#include "gtest/gtest.h"

using namespace leka::utils::math;

TEST(MathUtilsTest, invSqrtOfAQuarter)
{
	const auto result = invSqrt(0.25F);
	EXPECT_NEAR(result, 2.000F, 0.05);
}

TEST(MathUtilsTest, invSqrtOfAThird)
{
	const auto result = invSqrt(0.33333F);
	EXPECT_NEAR(result, 1.730F, 0.005);
}

TEST(MathUtilsTest, invSqrtOfAHalf)
{
	const auto result = invSqrt(0.5F);
	EXPECT_NEAR(result, 1.415F, 0.005);
}

TEST(MathUtilsTest, invSqrtOfTwoThird)
{
	const auto result = invSqrt(0.6666F);
	EXPECT_NEAR(result, 1.225F, 0.005);
}

TEST(MathUtilsTest, invSqrtOfThreeQuarter)
{
	const auto result = invSqrt(0.75F);
	EXPECT_NEAR(result, 1.155F, 0.005);
}

TEST(MathUtilsTest, invSqrtOfOne)
{
	const auto result = invSqrt(1);
	EXPECT_NEAR(result, 1.000F, 0.005);
}
