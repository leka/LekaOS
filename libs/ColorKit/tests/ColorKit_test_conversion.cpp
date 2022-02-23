// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <array>

#include "ColorKit.h"
#include "gtest/gtest.h"

using namespace leka;

TEST(ColorKitConversionTest, convertDefinedColor)
{
	auto expected_color = RGB::pure_green;
	auto oklab_color	= ColorKit::internal::rgb2oklab(expected_color);
	auto rgb_color		= ColorKit::internal::oklab2rgb(oklab_color);

	EXPECT_EQ(rgb_color, expected_color);
}

TEST(ColorKitConversionTest, convertRandomColor)
{
	auto expected_color = RGB {123, 15, 177};
	auto oklab_color	= ColorKit::internal::rgb2oklab(expected_color);
	auto rgb_color		= ColorKit::internal::oklab2rgb(oklab_color);

	EXPECT_EQ(rgb_color, expected_color);
}

TEST(ColorKitConversionTest, clampAroundZero)
{
	EXPECT_EQ(ColorKit::internal::clamp(0.F), 0);
	EXPECT_EQ(ColorKit::internal::clamp(-0.F), 0);
	EXPECT_EQ(ColorKit::internal::clamp(0.001F), 0);
	EXPECT_EQ(ColorKit::internal::clamp(-0.001F), 0);
}

TEST(ColorKitConversionTest, clampNegative)
{
	EXPECT_EQ(ColorKit::internal::clamp(-0.1F), 0);
	EXPECT_EQ(ColorKit::internal::clamp(-1.F), 0);
	EXPECT_EQ(ColorKit::internal::clamp(-10000.F), 0);
}

TEST(ColorKitConversionTest, clampInBound)
{
	EXPECT_EQ(ColorKit::internal::clamp(50.F / 255.F), 50);
	EXPECT_EQ(ColorKit::internal::clamp(150.F / 255.F), 150);
	EXPECT_EQ(ColorKit::internal::clamp(200.F / 255.F), 200);
}

TEST(ColorKitConversionTest, clampAroundOne)
{
	EXPECT_EQ(ColorKit::internal::clamp(1.F), 255);
	EXPECT_EQ(ColorKit::internal::clamp(0.999F), 255);
	EXPECT_EQ(ColorKit::internal::clamp(1.001F), 255);
	EXPECT_EQ(ColorKit::internal::clamp(6.001F), 255);
}
