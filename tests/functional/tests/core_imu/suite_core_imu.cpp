// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "rtos/ThisThread.h"

#include "./utils.h"
#include "CoreI2C.h"
#include "CoreIMU.hpp"
#include "tests/config.h"

using namespace leka;
using namespace std::chrono;
using namespace boost::ut;
using namespace boost::ut::bdd;

suite suite_coreimu = [] {
	auto i2c		 = CoreI2C(PinName::SENSOR_IMU_TH_I2C_SDA, PinName::SENSOR_IMU_TH_I2C_SCL);
	auto irq		 = CoreInterruptIn {PinName::SENSOR_IMU_IRQ};
	auto coreimu	 = CoreIMU {i2c, irq};
	auto sensor_data = leka::interface::IMU::SensorData();

	"initialization"_test = [&] {
		expect(neq(&coreimu, nullptr));
		coreimu.init();

		auto sensor_callback = [&](const leka::interface::IMU::SensorData &data) { sensor_data = data; };
		coreimu.registerOnDataReadyCallback(sensor_callback);
	};

	scenario("imu - power mode") = [&] {
		given("powermode is set to off") = [&] {
			coreimu.setPowerMode(CoreIMU::PowerMode::Off);

			then("I expect imu data to not change over time") = [&] {
				expect(not values_did_change_over_time(coreimu));
			};
		};

		given("powermode is set to normal again") = [&] {
			coreimu.setPowerMode(CoreIMU::PowerMode::Normal);

			then("I expect imu data to change over time") = [&] { expect(values_did_change_over_time(coreimu)); };
		};
	};
};
