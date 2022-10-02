// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "AdvertisingData.h"

#include "gtest/gtest.h"

using namespace leka;

TEST(AdvertisingDataTest, initialisation)
{
	auto advertising_data = AdvertisingData {};

	EXPECT_NE(&advertising_data, nullptr);
}

TEST(AdvertisingDataTest, data)
{
	auto advertising_data = AdvertisingData {.battery = 0x2A, .is_charging = 0x2B};

	auto expected_data_array = std::to_array({advertising_data.battery, advertising_data.is_charging});

	auto actual_data_array = std::span {advertising_data.data(), advertising_data.size()};

	EXPECT_EQ(std::size(actual_data_array), std::size(expected_data_array));
	EXPECT_TRUE(0 == memcmp(expected_data_array.data(), actual_data_array.data(), std::size(expected_data_array)));
}
