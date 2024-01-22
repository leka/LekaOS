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

TEST_F(BLEServiceMonitoringTest, setTemperature)
{
	std::array<uint8_t, 4> actual_temperature {};
	std::array<uint8_t, 4> expected_temperature {};

	auto spy_callback = [&actual_temperature](const BLEServiceMonitoring::data_to_send_handle_t &handle) {
		for (auto i = 0; i < std::size(actual_temperature); i++) {
			actual_temperature.at(i) = std::get<1>(handle)[i];
		}
	};

	service_monitoring.onDataReadyToSend(spy_callback);

	service_monitoring.setTemperature(31.4159);
	expected_temperature = {0xC3, 0x53, 0xFB, 0x41};   // 31.4159, little-endian as in Swift
	EXPECT_EQ(actual_temperature, expected_temperature);
}

TEST_F(BLEServiceMonitoringTest, setHumidity)
{
	std::array<uint8_t, 4> actual_humidity {};
	std::array<uint8_t, 4> expected_humidity {};

	auto spy_callback = [&actual_humidity](const BLEServiceMonitoring::data_to_send_handle_t &handle) {
		for (auto i = 0; i < std::size(actual_humidity); i++) {
			actual_humidity.at(i) = std::get<1>(handle)[i];
		}
	};

	service_monitoring.onDataReadyToSend(spy_callback);

	service_monitoring.setHumidity(51.24);
	expected_humidity = {0xC3, 0xF5, 0x4C, 0x42};	// 51.24 little-endian as in Swift
	EXPECT_EQ(actual_humidity, expected_humidity);
}

TEST_F(BLEServiceMonitoringTest, onTemperatureRequested)
{
	testing::MockFunction<void()> mock_callback {};
	service_monitoring.onTemperatureRequested(mock_callback.AsStdFunction());

	EXPECT_CALL(mock_callback, Call).Times(1);

	service_monitoring.onDataRequested(data_requested_handle);
}

TEST_F(BLEServiceMonitoringTest, onTemperatureRequestedNotSameHandle)
{
	testing::MockFunction<void()> mock_callback {};
	service_monitoring.onTemperatureRequested(mock_callback.AsStdFunction());

	data_requested_handle.handle = 0xFFFF;

	EXPECT_CALL(mock_callback, Call).Times(0);

	service_monitoring.onDataRequested(data_requested_handle);
}

TEST_F(BLEServiceMonitoringTest, onTemperatureRequestedtUnset)
{
	service_monitoring.onTemperatureRequested(nullptr);

	service_monitoring.onDataRequested(data_requested_handle);
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

TEST_F(BLEServiceMonitoringTest, onDataRequestedNotSameHandle)
{
	data_requested_handle.handle = 0xFFFF;

	service_monitoring.onDataRequested(data_requested_handle);

	// nothing expected
}

TEST_F(BLEServiceMonitoringTest, setNegotiatedMtu)
{
	uint16_t actual_negotiated_mtu {};

	auto spy_callback = [&actual_negotiated_mtu](const BLEServiceMonitoring::data_to_send_handle_t &handle) {
		actual_negotiated_mtu =
			utils::memory::combineBytes({.high = std::get<1>(handle)[0], .low = std::get<1>(handle)[1]});
	};

	service_monitoring.onDataReadyToSend(spy_callback);

	uint16_t expect_negotiated_mtu = 23;
	service_monitoring.setNegotiatedMtu(expect_negotiated_mtu);

	EXPECT_EQ(actual_negotiated_mtu, expect_negotiated_mtu);
}
