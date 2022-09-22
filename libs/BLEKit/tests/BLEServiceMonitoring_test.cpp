// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "BLEServiceMonitoring.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace leka;

using ::testing::MockFunction;

class BLEServiceMonitoringTest : public testing::Test
{
  protected:
	// void SetUp() override {}
	// void TearDown() override {}

	BLEServiceMonitoring service_monitoring {};

	BLEServiceMonitoring::data_received_handle_t data_received_handle {};
	BLEServiceMonitoring::data_requested_handle_t data_requested_handle {};

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
	bool expect_screensaver_enabled		 = false;
	auto expect_screensaver_enabled_data = static_cast<uint8_t>(expect_screensaver_enabled);

	onDataReceivedProcess(&expect_screensaver_enabled_data);

	auto actual_is_screensaver_enable = service_monitoring.isScreensaverEnable();
	EXPECT_EQ(actual_is_screensaver_enable, expect_screensaver_enabled);
}

TEST_F(BLEServiceMonitoringTest, isScreensaverEnableNotSameHandle)
{
	bool expect_screensaver_enabled = default_is_screensaver_enable;
	bool sent_value					= false;
	auto sent_value_data			= static_cast<uint8_t>(sent_value);

	data_received_handle.handle = 0xFFFF;

	onDataReceivedProcess(&sent_value_data);

	auto actual_is_screensaver_enable = service_monitoring.isScreensaverEnable();

	EXPECT_EQ(actual_is_screensaver_enable, expect_screensaver_enabled);
	EXPECT_NE(actual_is_screensaver_enable, sent_value);
}

TEST_F(BLEServiceMonitoringTest, onSoftRebootUnset)
{
	bool sent_value		 = true;
	auto sent_value_data = static_cast<uint8_t>(sent_value);

	onDataReceivedProcess(&sent_value_data);
}

TEST_F(BLEServiceMonitoringTest, onSoftRebootReceivedFalse)
{
	bool sent_value		 = false;
	auto sent_value_data = static_cast<uint8_t>(sent_value);

	testing::MockFunction<void()> mock_callback {};
	service_monitoring.onSoftReboot(mock_callback.AsStdFunction());

	EXPECT_CALL(mock_callback, Call).Times(0);

	onDataReceivedProcess(&sent_value_data);
}

TEST_F(BLEServiceMonitoringTest, onSoftRebootReceivedTrue)
{
	bool sent_value		 = true;
	auto sent_value_data = static_cast<uint8_t>(sent_value);

	testing::MockFunction<void()> mock_callback {};
	service_monitoring.onSoftReboot(mock_callback.AsStdFunction());

	EXPECT_CALL(mock_callback, Call).Times(1);

	onDataReceivedProcess(&sent_value_data);
}

TEST_F(BLEServiceMonitoringTest, onSoftRebootNotSameHandle)
{
	bool sent_value		 = true;
	auto sent_value_data = static_cast<uint8_t>(sent_value);

	data_received_handle.handle = 0xFFFF;

	testing::MockFunction<void()> mock_callback {};
	service_monitoring.onSoftReboot(mock_callback.AsStdFunction());

	EXPECT_CALL(mock_callback, Call).Times(0);

	onDataReceivedProcess(&sent_value_data);
}

TEST_F(BLEServiceMonitoringTest, onDataRequested)
{
	service_monitoring.onDataRequested(data_requested_handle);

	// nothing expected
}
