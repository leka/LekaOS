// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "BLEServiceMonitoring.h"

#include "gtest/gtest.h"

using namespace leka;

class BLEServiceMonitoringTest : public testing::Test
{
  protected:
	// void SetUp() override {}
	// void TearDown() override {}

	BLEServiceMonitoring service_monitoring {};

	BLEServiceMonitoring::data_received_handle_t data_received_handle {};

	bool default_is_screensaver_enable {true};

	void onDataReceivedProcess(const uint8_t *data)
	{
		data_received_handle.data = data;

		service_monitoring.onDataReceived(data_received_handle);
	}
};

TEST_F(BLEServiceMonitoringTest, initialisation)
{
	EXPECT_NE(&service_monitoring, nullptr);
}

TEST_F(BLEServiceMonitoringTest, setChargingStatus)
{
	uint8_t actual_charging_status {};

	auto spy_callback = [&actual_charging_status](const BLEServiceMonitoring::data_to_send_handle_t &handle) {
		actual_charging_status = std::get<1>(handle)[0];
	};
	service_monitoring.onDataReadyToSend(spy_callback);

	service_monitoring.setChargingStatus(true);
	EXPECT_TRUE(actual_charging_status);

	service_monitoring.setChargingStatus(false);
	EXPECT_FALSE(actual_charging_status);
}

TEST_F(BLEServiceMonitoringTest, isScreensaverEnableDefault)
{
	auto actual_is_screensaver_enable = service_monitoring.isScreensaverEnable();
	EXPECT_EQ(actual_is_screensaver_enable, default_is_screensaver_enable);
}

TEST_F(BLEServiceMonitoringTest, isScreensaverEnableFalse)
{
	bool expected_is_screensaver_enable = false;

	auto convert_to_handle_data_type = [](bool value) { return std::make_shared<uint8_t>(value).get(); };
	onDataReceivedProcess(convert_to_handle_data_type(expected_is_screensaver_enable));

	auto actual_is_screensaver_enable = service_monitoring.isScreensaverEnable();
	EXPECT_EQ(actual_is_screensaver_enable, expected_is_screensaver_enable);
}

TEST_F(BLEServiceMonitoringTest, isScreensaverEnableNotSameHandle)
{
	bool expected_is_screensaver_enable = default_is_screensaver_enable;
	bool sent_value						= false;

	data_received_handle.handle = 0xFFFF;

	auto convert_to_handle_data_type = [](bool value) { return std::make_shared<uint8_t>(value).get(); };
	onDataReceivedProcess(convert_to_handle_data_type(sent_value));

	auto actual_is_screensaver_enable = service_monitoring.isScreensaverEnable();
	EXPECT_EQ(actual_is_screensaver_enable, expected_is_screensaver_enable);
	EXPECT_NE(actual_is_screensaver_enable, sent_value);
}
