// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <cmath>

#include "Utils.h"
#include "gtest/gtest.h"

using namespace leka::utils::math;

static constexpr auto period = 4;

TEST(LedKitTestSinPeriod, SinusZero)
{
	auto expected_result = 0.5F;
	EXPECT_FLOAT_EQ(sinPeriod(period, 0, 0, 0.F, 1.F), expected_result);
}

TEST(LedKitTestSinPeriod, SinusPi)
{
	auto expected_result = 0.5F;
	EXPECT_FLOAT_EQ(sinPeriod(period, 2, 0, 0.F, 1.F), expected_result);
}

TEST(LedKitTestSinPeriod, SinusHalfPi)
{
	auto expected_result = 1.F;
	EXPECT_FLOAT_EQ(sinPeriod(period, 1, 0, 0.F, 1.F), expected_result);
}

TEST(LedKitTestSinPeriod, Sinus0PlusHalfPi)
{
	constexpr auto offsetUint8t2Pi = 255;
	auto expected_result		   = 0.5F;
	EXPECT_FLOAT_EQ(sinPeriod(period, 0, offsetUint8t2Pi, 0.F, 1.F), expected_result);
}

TEST(LedKitTestSinPeriod, SinusHalfPiInt)
{
	auto expected_result = 255;
	EXPECT_FLOAT_EQ(sinPeriod(period, 1, 0, 0, 255), expected_result);
}
