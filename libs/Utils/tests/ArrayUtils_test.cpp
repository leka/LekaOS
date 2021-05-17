// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <memory>
#include <string_view>

#include "Utils.h"
#include "gtest/gtest.h"

using namespace leka::utils;

TEST(ArrayUtilsTest, toArrayFromStringLiteral)
{
	auto arr = to_array("foo");

	ASSERT_EQ(arr.size(), 4);
}

TEST(ArrayUtilsTest, toArrayFromInitializerList)
{
	auto arr = to_array({1, 2, 3, 4});

	ASSERT_EQ(arr.size(), 4);
}

TEST(ArrayUtilsTest, toArrayDefaultTypeIsInt)
{
	auto arr = to_array({1, 2, 3, 4});

	auto is_same_type = std::is_same_v<decltype(arr[0]), int &>;
	ASSERT_TRUE(is_same_type);
}

TEST(ArrayUtilsTest, toArrayFromCStyleArray)
{
	const char c_arr[] = {1, 2, 3, 4};

	auto arr = to_array(c_arr);

	ASSERT_EQ(arr.size(), 4);
}

TEST(ArrayUtilsTest, toArrayFromCStyleString)
{
	const char c_arr[] = "foo";

	auto arr = to_array(c_arr);

	ASSERT_EQ(arr.size(), 4);
}

TEST(ArrayUtilsTest, toArrayWithSpecificTypeUint8t)
{
	auto arr = to_array<uint8_t>({0, 1, 2, 3});

	ASSERT_EQ(arr.size(), 4);

	auto is_same_type = std::is_same_v<decltype(arr[0]), uint8_t &>;
	ASSERT_TRUE(is_same_type);
}

TEST(ArrayUtilsTest, toArrayWithSpecificTypeDouble)
{
	auto arr = to_array<double>({0, 1, 2, 3});

	ASSERT_EQ(arr.size(), 4);

	auto is_same_type = std::is_same_v<decltype(arr[0]), double &>;
	ASSERT_TRUE(is_same_type);
}

TEST(ArrayUtilsTest, toArrayWithSpecificTypePair)
{
	auto arr = to_array<std::pair<int, float>>({{3, .0f}, {4, .1f}, {4, .1e23f}});

	ASSERT_EQ(arr.size(), 3);

	auto is_same_type = std::is_same_v<decltype(arr[0]), std::pair<int, float> &>;
	ASSERT_TRUE(is_same_type);
}

TEST(ArrayUtilsTest, toArrayWithSpecificTypeOptional)
{
	auto arr = to_array<std::optional<uint8_t>>({0, {}, 2, std::nullopt, 4});

	ASSERT_EQ(arr.size(), 5);

	auto is_same_type = std::is_same_v<decltype(arr[0]), std::optional<uint8_t> &>;
	ASSERT_TRUE(is_same_type);
}

TEST(ArrayUtilsTest, toArrayWithSpecificTypeSmartPointer)
{
	auto arr = to_array({std::make_unique<int>(3)});

	ASSERT_EQ(arr.size(), 1);

	auto is_same_type = std::is_same_v<decltype(arr[0]), std::unique_ptr<int> &>;
	ASSERT_TRUE(is_same_type);
}
