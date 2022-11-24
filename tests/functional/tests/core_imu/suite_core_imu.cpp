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

suite suite_core_imu = [] {
	constexpr auto kAccelZAxisDefaultMinBound = 800._f;
	constexpr auto kAccelZAxisDefaultMaxBound = 1200._f;

	auto i2c	  = CoreI2C(PinName::SENSOR_IMU_TH_I2C_SDA, PinName::SENSOR_IMU_TH_I2C_SCL);
	auto lsm6dsox = CoreLSM6DSOX {i2c};
	auto accel	  = CoreAccelerometer {lsm6dsox};
	auto gyro	  = CoreGyroscope {lsm6dsox};

	"initialization"_test = [&] {
		expect(neq(&lsm6dsox, nullptr));
		lsm6dsox.init();
	};

	"accelerometer"_test = [&] {
		"z accel != 0"_test = [&] {
			auto [ax, ay, az] = accel.getXYZ();
			expect(az != 0._f);
		};

		"z accel between min/max bounds"_test = [&] {
			auto [ax, ay, az] = accel.getXYZ();
			expect(az >= kAccelZAxisDefaultMinBound);
			expect(az <= kAccelZAxisDefaultMaxBound);
		};

		"x,y,z accel values change over time"_test = [&] { expect(values_did_change_over_time(accel)); };
	};

	"gyroscope"_test = [&] {
		"x,y,z gyro values change over time"_test = [&] { expect(values_did_change_over_time(gyro)); };
	};
};
