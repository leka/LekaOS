// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "Utils.h"
#include "gtest/gtest.h"

template <typename U, typename V>
constexpr bool same = std::is_same_v<U, V>;

TEST(CastUtilsTest, from_c_string_to_uint8_t_arraySameSize)
{
	const char c_string_reference[] = "Lorem Ipsum";
	auto uint8_t_array				= leka::utils::cast::from_c_string_to_uint8_t_array(c_string_reference);

	EXPECT_EQ(sizeof(c_string_reference), std::size(uint8_t_array));
}

TEST(CastUtilsTest, from_c_string_to_uint8_t_arrayCorrectType)
{
	const char c_string_reference[] = "Lorem Ipsum";
	auto uint8_t_array				= leka::utils::cast::from_c_string_to_uint8_t_array(c_string_reference);

	for (auto elem: uint8_t_array) {
		EXPECT_TRUE((same<decltype(elem), uint8_t>));
	}
}

TEST(CastUtilsTest, from_c_string_to_uint8_t_arraySameElement)
{
	const char c_string_reference[] = "Lorem Ipsum";
	auto uint8_t_array				= leka::utils::cast::from_c_string_to_uint8_t_array(c_string_reference);

	for (std::size_t index = 0; index < std::size(uint8_t_array); index++) {
		EXPECT_EQ(c_string_reference[index], uint8_t_array.at(index));
	}
}
