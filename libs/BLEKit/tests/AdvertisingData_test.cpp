// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "AdvertisingData.h"
#include <array>
#include <span>

#include "gtest/gtest.h"

using namespace leka;

TEST(AdvertisingDataTest, initialisation)
{
	auto advertising_data = AdvertisingData {};

	EXPECT_NE(&advertising_data, nullptr);
}

TEST(AdvertisingDataTest, data)
{
	auto advertising_data = AdvertisingData {
		.version_major	  = 0x3A,
		.version_minor	  = 0x3B,
		.battery		  = 0x3C,
		.is_charging	  = 0x3D,
		.is_deep_sleeping = 0x3E,
	};

	auto expected_data_array = std::to_array({
		0x3A,
		0x3B,
		0x3C,
		0x3D,
		0x3E,
	});

	auto actual_data_array = std::span {advertising_data.data(), advertising_data.size()};

	EXPECT_EQ(std::size(actual_data_array), std::size(expected_data_array));
	EXPECT_TRUE(1 == std::equal(expected_data_array.begin(), expected_data_array.end(), actual_data_array.begin()));
}
