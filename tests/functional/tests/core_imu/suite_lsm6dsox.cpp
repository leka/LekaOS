// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "rtos/ThisThread.h"

#include "CoreAccelerometer.h"
#include "CoreGyroscope.h"
#include "CoreI2C.h"
#include "CoreLSM6DSOX.h"
#include "tests/config.h"

using namespace leka;
using namespace std::chrono;
using namespace boost::ut;

suite suite_lsm6dsox = [] {
	auto i2c	  = CoreI2C(PinName::SENSOR_IMU_TH_I2C_SDA, PinName::SENSOR_IMU_TH_I2C_SCL);
	auto lsm6dsox = CoreLSM6DSOX {i2c};
	auto accel	  = CoreAccelerometer {lsm6dsox};
	auto gyro	  = CoreGyroscope {lsm6dsox};

	lsm6dsox.init();
	auto default_min_bound_az = 800._f;
	auto default_max_bound_az = 1200._f;

	"Initialization"_test = [&] { expect(neq(&lsm6dsox, nullptr)); };

	"Get imu data"_test = [&] {
		auto [ax, ay, az] = accel.getXYZ();
		auto [gx, gy, gz] = gyro.getXYZ();
		rtos::ThisThread::sleep_for(100ms);
		auto [current_ax, current_ay, current_az] = accel.getXYZ();
		auto [current_gx, current_gy, current_gz] = gyro.getXYZ();
		expect(ax != current_ax) << "Lsm6dsox still off " << ax << "==" << current_ax;
		expect(ay != current_ay) << "Lsm6dsox still off " << ay << "==" << current_ay;
		expect(az != current_az) << "Lsm6dsox still off " << az << "==" << current_az;
		expect(current_az >= default_min_bound_az) << "Acceleration on Z axis:" << az << "axis too low";
		expect(current_az <= default_max_bound_az) << "Acceleration on Z axis:" << az << "axis too high";
		expect(gx != current_gx) << "Lsm6dsox still off " << gx << "==" << current_gx;
		expect(gy != current_gy) << "Lsm6dsox still off " << gy << "==" << current_gy;
		expect(gz != current_gz) << "Lsm6dsox still off " << gz << "==" << current_gz;
	};

	"Turn off lsm6dsox"_test = [&] {
		lsm6dsox.setPowerMode(CoreLSM6DSOX::PowerMode::Off);
		auto [ax, ay, az] = accel.getXYZ();
		auto [gx, gy, gz] = gyro.getXYZ();
		rtos::ThisThread::sleep_for(100ms);
		auto [current_ax, current_ay, current_az] = accel.getXYZ();
		auto [current_gx, current_gy, current_gz] = gyro.getXYZ();
		expect(ax == current_ax) << "Lsm6dsox not turned off " << ax << "!=" << current_ax;
		expect(ay == current_ay) << "Lsm6dsox not turned off " << ay << "!=" << current_ay;
		expect(az == current_az) << "Lsm6dsox not turned off " << az << "!=" << current_az;
		expect(gx == current_gx) << "Lsm6dsox not turned off " << gx << "!=" << current_gx;
		expect(gy == current_gy) << "Lsm6dsox not turned off " << gy << "!=" << current_gy;
		expect(gz == current_gz) << "Lsm6dsox not turned off " << gz << "!=" << current_gz;
	};

	"Assert data are different when lsm6dsox turned on"_test = [&] {
		lsm6dsox.setPowerMode(CoreLSM6DSOX::PowerMode::Normal);
		auto [gx, gy, gz] = gyro.getXYZ();
		rtos::ThisThread::sleep_for(100ms);
		auto [current_gx, current_gy, current_gz] = gyro.getXYZ();
		expect(gz != current_gz) << "Datas are all the same " << gz << "!=" << current_gz;
	};

	"Turn on lsm6dsox after turned off"_test = [&] {
		lsm6dsox.setPowerMode(CoreLSM6DSOX::PowerMode::Off);
		lsm6dsox.setPowerMode(CoreLSM6DSOX::PowerMode::Normal);
		auto [ax, ay, az] = accel.getXYZ();
		auto [gx, gy, gz] = gyro.getXYZ();
		rtos::ThisThread::sleep_for(100ms);
		auto [current_ax, current_ay, current_az] = accel.getXYZ();
		auto [current_gx, current_gy, current_gz] = gyro.getXYZ();
		expect(ax != current_ax) << "Lsm6dsox still off " << ax << "==" << current_ax;
		expect(ay != current_ay) << "Lsm6dsox still off " << ay << "==" << current_ay;
		expect(az != current_az) << "Lsm6dsox still off " << az << "==" << current_az;
		expect(gx != current_gx) << "Lsm6dsox still off " << gx << "==" << current_gx;
		expect(gy != current_gy) << "Lsm6dsox still off " << gy << "==" << current_gy;
		expect(gz != current_gz) << "Lsm6dsox still off " << gz << "==" << current_gz;
	};
};
