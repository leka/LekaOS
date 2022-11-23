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
using namespace boost::ut::bdd;

suite suite_lsm6dsox = [] {
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

	"get data - accelerometer"_test = [&] {
		"z accel != 0"_test = [&] {
			auto [ax, ay, az] = accel.getXYZ();
			expect(az != 0._f);
		};

		"z accel between min/max bounds"_test = [&] {
			auto [ax, ay, az] = accel.getXYZ();
			expect(az >= kAccelZAxisDefaultMinBound);
			expect(az <= kAccelZAxisDefaultMaxBound);
		};

		"x,y,z accel values change over time"_test = [&] {
			auto i_batch = std::vector<float> {};
			auto f_batch = std::vector<float> {};

			for (auto i = 0; i < 10; ++i) {
				auto [ax, ay, az] = accel.getXYZ();

				i_batch.push_back(ax);
				i_batch.push_back(ay);
				i_batch.push_back(az);

				rtos::ThisThread::sleep_for(25ms);
			}

			rtos::ThisThread::sleep_for(100ms);

			for (auto i = 0; i < 10; ++i) {
				auto [ax, ay, az] = accel.getXYZ();

				f_batch.push_back(ax);
				f_batch.push_back(ay);
				f_batch.push_back(az);

				rtos::ThisThread::sleep_for(25ms);
			}

			expect(neq(i_batch, f_batch));
		};
	};

	"get data - gyroscope"_test = [&] {
		"x,y,z gyro values change over time"_test = [&] {
			auto i_batch = std::vector<float> {};
			auto f_batch = std::vector<float> {};

			for (auto i = 0; i < 10; ++i) {
				auto [gx, gy, gz] = gyro.getXYZ();

				i_batch.push_back(gx);
				i_batch.push_back(gy);
				i_batch.push_back(gz);

				rtos::ThisThread::sleep_for(25ms);
			}

			rtos::ThisThread::sleep_for(100ms);

			for (auto i = 0; i < 10; ++i) {
				auto [gx, gy, gz] = gyro.getXYZ();

				f_batch.push_back(gx);
				f_batch.push_back(gy);
				f_batch.push_back(gz);

				rtos::ThisThread::sleep_for(25ms);
			}

			expect(neq(i_batch, f_batch));
		};
	};

	// TODO (@HPezz): move this to core_lsm6dsox specific tests
	scenario("lsm6dsox - power mode") = [&] {
		given("powermode is set to off") = [&] {
			lsm6dsox.setPowerMode(CoreLSM6DSOX::PowerMode::Off);

			then("I expect accel data to not change over time") = [&] {
				auto i_batch = std::vector<float> {};
				auto f_batch = std::vector<float> {};

				for (auto i = 0; i < 10; ++i) {
					auto [ax, ay, az] = accel.getXYZ();

					i_batch.push_back(ax);
					i_batch.push_back(ay);
					i_batch.push_back(az);

					rtos::ThisThread::sleep_for(25ms);
				}

				rtos::ThisThread::sleep_for(100ms);

				for (auto i = 0; i < 10; ++i) {
					auto [ax, ay, az] = accel.getXYZ();

					f_batch.push_back(ax);
					f_batch.push_back(ay);
					f_batch.push_back(az);

					rtos::ThisThread::sleep_for(25ms);
				}

				expect(eq(i_batch, f_batch));
			};

			then("I expect gyro data to not change over time") = [&] {
				auto i_batch = std::vector<float> {};
				auto f_batch = std::vector<float> {};

				for (auto i = 0; i < 10; ++i) {
					auto [gx, gy, gz] = gyro.getXYZ();

					i_batch.push_back(gx);
					i_batch.push_back(gy);
					i_batch.push_back(gz);

					rtos::ThisThread::sleep_for(25ms);
				}

				rtos::ThisThread::sleep_for(100ms);

				for (auto i = 0; i < 10; ++i) {
					auto [gx, gy, gz] = gyro.getXYZ();

					f_batch.push_back(gx);
					f_batch.push_back(gy);
					f_batch.push_back(gz);

					rtos::ThisThread::sleep_for(25ms);
				}

				expect(eq(i_batch, f_batch));
			};
		};

		given("powermode is set to normal again") = [&] {
			lsm6dsox.setPowerMode(CoreLSM6DSOX::PowerMode::Normal);

			then("I expect accel data to change over time") = [&] {
				auto i_batch = std::vector<float> {};
				auto f_batch = std::vector<float> {};

				for (auto i = 0; i < 10; ++i) {
					auto [ax, ay, az] = accel.getXYZ();

					i_batch.push_back(ax);
					i_batch.push_back(ay);
					i_batch.push_back(az);

					rtos::ThisThread::sleep_for(25ms);
				}

				rtos::ThisThread::sleep_for(100ms);

				for (auto i = 0; i < 10; ++i) {
					auto [ax, ay, az] = accel.getXYZ();

					f_batch.push_back(ax);
					f_batch.push_back(ay);
					f_batch.push_back(az);

					rtos::ThisThread::sleep_for(25ms);
				}

				expect(neq(i_batch, f_batch));
			};

			then("I expect gyro data to change over time") = [&] {
				auto i_batch = std::vector<float> {};
				auto f_batch = std::vector<float> {};

				for (auto i = 0; i < 10; ++i) {
					auto [gx, gy, gz] = gyro.getXYZ();

					i_batch.push_back(gx);
					i_batch.push_back(gy);
					i_batch.push_back(gz);

					rtos::ThisThread::sleep_for(25ms);
				}

				rtos::ThisThread::sleep_for(100ms);

				for (auto i = 0; i < 10; ++i) {
					auto [gx, gy, gz] = gyro.getXYZ();

					f_batch.push_back(gx);
					f_batch.push_back(gy);
					f_batch.push_back(gz);

					rtos::ThisThread::sleep_for(25ms);
				}

				expect(neq(i_batch, f_batch));
			};
		};
	};
};
