// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "rtos/ThisThread.h"

#include "./utils.h"
#include "CoreAccelerometer.h"
#include "CoreGyroscope.h"
#include "CoreI2C.h"
#include "CoreLSM6DSOX.h"
#include "tests/config.h"

using namespace leka;
using namespace std::chrono;
using namespace boost::ut;
using namespace boost::ut::bdd;

suite suite_lsm6dsox = [] {
	auto i2c	  = CoreI2C(PinName::SENSOR_IMU_TH_I2C_SDA, PinName::SENSOR_IMU_TH_I2C_SCL);
	auto lsm6dsox = CoreLSM6DSOX {i2c};
	auto accel	  = CoreAccelerometer {lsm6dsox};
	auto gyro	  = CoreGyroscope {lsm6dsox};

	"initialization"_test = [&] {
		expect(neq(&lsm6dsox, nullptr));
		lsm6dsox.init();
	};

	scenario("lsm6dsox - power mode") = [&] {
		given("powermode is set to off") = [&] {
			lsm6dsox.setPowerMode(CoreLSM6DSOX::PowerMode::Off);

			then("I expect accel data to not change over time") = [&] {
				expect(not values_did_change_over_time(accel));
			};

			then("I expect gyro data to not change over time") = [&] { expect(not values_did_change_over_time(gyro)); };
		};

		given("powermode is set to normal again") = [&] {
			lsm6dsox.setPowerMode(CoreLSM6DSOX::PowerMode::Normal);

			then("I expect accel data to change over time") = [&] { expect(values_did_change_over_time(accel)); };

			then("I expect gyro data to change over time") = [&] { expect(values_did_change_over_time(gyro)); };
		};
	};
};
