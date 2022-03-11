// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "BLEServiceDeviceInformation.h"

#include "gtest/gtest.h"

using namespace leka;

TEST(BLEServiceDeviceInformationTest, initialisation)
{
	auto service_device_information = BLEServiceDeviceInformation {};

	EXPECT_NE(&service_device_information, nullptr);
}

TEST(BLEServiceDeviceInformationTest, onDataReceived)
{
	auto service_device_information = BLEServiceDeviceInformation {};

	auto default_params = BLEServiceDeviceInformation::data_received_handle_t {};

	service_device_information.onDataReceived(default_params);
}

TEST(BLEServiceDeviceInformationTest, setSerialNumber)
{
	auto service_device_information = BLEServiceDeviceInformation {};

	const auto serial_number_size = 33;
	std::array<uint8_t, serial_number_size> actual_serial_number {};

	auto spy_callback = [&actual_serial_number](const BLEServiceDeviceInformation::data_to_send_handle_t &handle) {
		for (int i = 0; i < serial_number_size; i++) {
			actual_serial_number.at(i) = std::get<1>(handle)[i];
		}
	};
	service_device_information.onDataReadyToSend(spy_callback);

	std::array<uint8_t, serial_number_size> serial_number = {"LK-2202-003300294E5350092038384B"};
	service_device_information.setSerialNumber(serial_number);

	bool same_content = true;
	for (int i = 0; i < serial_number_size; i++) {
		same_content &= actual_serial_number.at(i) == serial_number.at(i);
	}
	EXPECT_TRUE(same_content);
}
