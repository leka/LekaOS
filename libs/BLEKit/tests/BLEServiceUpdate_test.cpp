// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "BLEServiceUpdate.h"

#include "gtest/gtest.h"

using namespace leka;

class BLEServiceUpdateTest : public testing::Test
{
  protected:
	// void SetUp() override {}
	// void TearDown() override {}

	BLEServiceUpdate service_update {};

	BLEServiceUpdate::data_received_handle_t data_received_handle {};

	bool default_apply_update_value {false};

	void onDataReceivedProcess(const uint8_t *data)
	{
		data_received_handle.data = data;

		service_update.onDataReceived(data_received_handle);
	}
};

TEST_F(BLEServiceUpdateTest, initialisation)
{
	EXPECT_NE(&service_update, nullptr);
}

TEST_F(BLEServiceUpdateTest, getApplyUpdateValueDefault)
{
	auto actual_apply_update_value = service_update.getApplyUpdateValue();
	EXPECT_EQ(actual_apply_update_value, default_apply_update_value);
}

TEST_F(BLEServiceUpdateTest, getApplyUpdateValueTrue)
{
	bool expected_apply_update_value = true;

	auto convert_to_handle_data_type = [](bool value) { return std::make_shared<uint8_t>(value).get(); };
	onDataReceivedProcess(convert_to_handle_data_type(expected_apply_update_value));

	auto actual_apply_update_value = service_update.getApplyUpdateValue();
	EXPECT_EQ(actual_apply_update_value, expected_apply_update_value);
}

TEST_F(BLEServiceUpdateTest, getApplyUpdateValueNotSameHandle)
{
	bool expected_apply_update_value = default_apply_update_value;
	bool sent_value					 = true;

	data_received_handle.handle = 0xFFFF;

	auto convert_to_handle_data_type = [](bool value) { return std::make_shared<uint8_t>(value).get(); };
	onDataReceivedProcess(convert_to_handle_data_type(sent_value));

	auto actual_apply_update_value = service_update.getApplyUpdateValue();
	EXPECT_EQ(actual_apply_update_value, expected_apply_update_value);
	EXPECT_NE(actual_apply_update_value, sent_value);
}
