// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <array>

#include "Utils.h"
#include "gtest/gtest.h"

using namespace leka::utils::math;

TEST(MathUtilsTest, RandomBetween0And10)
{
	auto random = random8(0, 10);
	EXPECT_GE(random, 0);
	EXPECT_LE(random, 10);
}

TEST(MathUtilsTest, RandomBetween0And255)
{
	auto random = random8(0, 255);
	EXPECT_GE(random, 0);
	EXPECT_LE(random, 255);
}

TEST(MathUtilsTest, RandomBetweenOutsideOfRange)
{
	auto random = random8(300, 500);
	EXPECT_GE(random, 0);
	EXPECT_LE(random, 255);
}
