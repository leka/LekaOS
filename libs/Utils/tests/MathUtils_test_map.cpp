// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "Utils.h"
#include "gtest/gtest.h"

// Test list
// - 0 / 0
// - 0 / 255
// - -155 / 0
// - get last result - done

using namespace leka::utils::math;

TEST(MathUtilsTest, mapUint8ToFloatMin)
{
	uint8_t min	  = 0;
	uint8_t max	  = 255;
	uint8_t value = 0;

	auto result = map(value, min, max, 0.0f, 1.0f);

	EXPECT_EQ(result, 0.0f);
}

TEST(MathUtilsTest, mapUint8ToFloatMax)
{
	uint8_t min	  = 0;
	uint8_t max	  = 255;
	uint8_t value = 255;

	auto result = map(value, min, max, 0.0f, 1.0f);

	EXPECT_EQ(result, 1.0f);
}

TEST(MathUtilsTest, mapUint8ToFloatMiddle)
{
	uint8_t min	  = 0;
	uint8_t max	  = 255;
	uint8_t value = 51;

	auto result = map(value, min, max, 0.0f, 1.0f);

	EXPECT_EQ(result, 0.2f);
}

TEST(MathUtilsTest, mapFloatToUint8Min)
{
	uint8_t min = 0;
	uint8_t max = 255;
	float value = 0.0f;

	auto result = map(value, 0.0f, 1.0f, min, max);

	EXPECT_EQ(result, 0);
}

TEST(MathUtilsTest, mapFloatToUint8Max)
{
	uint8_t min = 0;
	uint8_t max = 255;
	float value = 1.0;

	auto result = map(value, 0.0f, 1.0f, min, max);

	EXPECT_EQ(result, 255);
}

TEST(MathUtilsTest, mapFloatToUint8Middle)
{
	uint8_t min = 0;
	uint8_t max = 255;
	float value = 0.2f;

	auto result = map(value, 0.0f, 1.0f, min, max);

	EXPECT_EQ(result, 51);
}

TEST(MathUtilsTest, mapInverseUint8)
{
	uint8_t in_min	= 0;
	uint8_t in_max	= 255;
	uint8_t out_min = 255;
	uint8_t out_max = 0;

	{
		uint8_t value = 0;

		auto result = map(value, in_min, in_max, out_min, out_max);

		EXPECT_EQ(result, 255);
	}

	{
		uint8_t value = 255;

		auto result = map(value, in_min, in_max, out_min, out_max);

		EXPECT_EQ(result, 0);
	}
}

TEST(MathUtilsTest, mapInverseUint8ToFloat)
{
	uint8_t in_min = 0;
	uint8_t in_max = 255;
	float out_min  = 1.F;
	float out_max  = 0.F;

	{
		uint8_t value = 0;

		auto result = map(value, in_min, in_max, out_min, out_max);

		EXPECT_EQ(result, 1.0F);
	}

	{
		uint8_t value = 255;

		auto result = map(value, in_min, in_max, out_min, out_max);

		EXPECT_EQ(result, 0.0F);
	}
}
