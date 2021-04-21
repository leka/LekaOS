// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LKUtils.h"

#include "gtest/gtest.h"

// Test list
// - 0 / 0
// - 0 / 255
// - -155 / 0
// - get last result - done

using namespace leka::utils;

TEST(LKUtilsTest, mapUint8ToFloatMin)
{
	uint8_t min	  = 0;
	uint8_t max	  = 255;
	uint8_t value = 0;

	auto result = map(value, min, max, 0.0f, 1.0f);
	ASSERT_EQ(result, 0.0f);
}

TEST(LKUtilsTest, mapUint8ToFloatMax)
{
	uint8_t min	  = 0;
	uint8_t max	  = 255;
	uint8_t value = 255;

	auto result = map(value, min, max, 0.0f, 1.0f);
	ASSERT_EQ(result, 1.0f);
}

TEST(LKUtilsTest, mapUint8ToFloatMiddle)
{
	uint8_t min	  = 0;
	uint8_t max	  = 255;
	uint8_t value = 51;

	auto result = map(value, min, max, 0.0f, 1.0f);
	ASSERT_EQ(result, 0.2f);
}

TEST(LKUtilsTest, mapFloatToUint8Min)
{
	uint8_t min = 0;
	uint8_t max = 255;
	float value = 0.0f;

	auto result = map(value, 0.0f, 1.0f, min, max);
	ASSERT_EQ(result, 0);
}

TEST(LKUtilsTest, mapFloatToUint8Max)
{
	uint8_t min = 0;
	uint8_t max = 255;
	float value = 1.0;

	auto result = map(value, 0.0f, 1.0f, min, max);
	ASSERT_EQ(result, 255);
}

TEST(LKUtilsTest, mapFloatToUint8Middle)
{
	uint8_t min = 0;
	uint8_t max = 255;
	float value = 0.2f;

	auto result = map(value, 0.0f, 1.0f, min, max);
	ASSERT_EQ(result, 51);
}

TEST(LKUtilsTest, LinearFunctionyInterceptValue)
{
	float x1 = 0.0;
	float x2 = 1.0;
	float y1 = 5.0;
	float y2 = 6.0;

	LinearFunctionValues values;
	values = linearFunction(x1, x2, y1, y2);

	ASSERT_EQ(values.slope, 1.0);
	ASSERT_EQ(values.y_intercept, 5.0);
}

TEST(LKUtilsTest, LinearFunctionDividedByZero)
{
	float x1 = 5.0;
	float x2 = 5.0;
	float y1 = 0.0;
	float y2 = 1.0;

	LinearFunctionValues values;
	values = linearFunction(x1, x2, y1, y2);

	ASSERT_EQ(values.slope, 0);
	ASSERT_EQ(values.y_intercept, 0);
}
