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
		.battery		  = 0x2A,
		.is_charging	  = 0x2B,
		.version_major	  = 0x2C,
		.version_minor	  = 0x2D,
		.version_revision = 0x2E2F,
	};

	auto expected_data_array = std::to_array({
		0x2A,
		0x2B,
		0x2C,
		0x2D,
		0x2E,
		0x2F,
	});

	auto actual_data_array = std::span {advertising_data.data(), advertising_data.size()};

	EXPECT_EQ(std::size(actual_data_array), std::size(expected_data_array));
	EXPECT_TRUE(1 == std::equal(expected_data_array.begin(), expected_data_array.end(), actual_data_array.begin()));
}
