// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "rtos/ThisThread.h"

#include "./utils.h"
#include "CoreI2C.h"
#include "CoreLSM6DSOX.h"
#include "tests/config.h"

using namespace leka;
using namespace std::chrono;
using namespace boost::ut;
using namespace boost::ut::bdd;

suite suite_lsm6dsox = [] {
	auto i2c		 = CoreI2C(PinName::SENSOR_IMU_TH_I2C_SDA, PinName::SENSOR_IMU_TH_I2C_SCL);
	auto drdy_irq	 = CoreInterruptIn {PinName::SENSOR_IMU_IRQ};
	auto lsm6dsox	 = CoreLSM6DSOX {i2c, drdy_irq};
	auto sensor_data = leka::interface::LSM6DSOX::SensorData();

	"initialization"_test = [&] {
		expect(neq(&lsm6dsox, nullptr));
		lsm6dsox.init();

		auto sensor_callback = [&](const leka::interface::LSM6DSOX::SensorData &data) { sensor_data = data; };
		lsm6dsox.registerOnGyDataReadyCallback(sensor_callback);
	};

	scenario("lsm6dsox - power mode") = [&] {
		given("powermode is set to off") = [&] {
			lsm6dsox.setPowerMode(CoreLSM6DSOX::PowerMode::Off);

			then("I expect imu data to not change over time") = [&] {
				expect(not values_did_change_over_time(lsm6dsox));
			};
		};

		given("powermode is set to normal again") = [&] {
			lsm6dsox.setPowerMode(CoreLSM6DSOX::PowerMode::Normal);

			then("I expect imu data to change over time") = [&] { expect(values_did_change_over_time(lsm6dsox)); };
		};
	};
};
