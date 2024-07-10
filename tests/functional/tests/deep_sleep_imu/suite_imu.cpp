// Leka - LekaOS
// Copyright 2024 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreI2C.h"
#include "CoreLSM6DSOX.hpp"
#include "tests/config.h"
#include "tests/utils_sleep.h"

using namespace leka;
using namespace boost::ut;
using namespace std::chrono_literals;
using namespace boost::ut::bdd;

auto i2c	  = CoreI2C {PinName::SENSOR_IMU_TH_I2C_SDA, PinName::SENSOR_IMU_TH_I2C_SCL};
auto drdy_irq = CoreInterruptIn {PinName::SENSOR_IMU_IRQ};
auto coreimu  = CoreLSM6DSOX {i2c, drdy_irq};

suite suite_imu = [] {
	scenario("imu initialization") = [] {
		given("imu is in default configuration") = [] {
			coreimu.init();
			rtos::ThisThread::sleep_for(5ms);

			expect(neq(&coreimu, nullptr));

			when("I do nothing") = [&] {
				then("I expect deep sleep TO BE LESS than 99%") = [] {
					auto deep_sleep_ratio = utils::sleep::get_deep_sleep_over_idle_time_ratio_for_duration(1s);
					expect(lt(deep_sleep_ratio, 0.99));
				};
			};
		};
	};

	scenario("enabled/disable deepsleep") = [] {
		given("imu is in default configuration") = [] {
			when("I enable imu deep sleep") = [&] {
				coreimu.enableDeepSleep();

				then("I expect deep sleep TO BE MORE than 99%") = [] {
					auto deep_sleep_ratio = utils::sleep::get_deep_sleep_over_idle_time_ratio_for_duration(1s);
					expect(gt(deep_sleep_ratio, 0.99));
				};
			};

			when("I disable imu deep sleep") = [&] {
				coreimu.disableDeepSleep();

				then("I expect deep sleep TO BE LESS than 99%") = [] {
					auto deep_sleep_ratio = utils::sleep::get_deep_sleep_over_idle_time_ratio_for_duration(1s);
					expect(lt(deep_sleep_ratio, 0.99));
				};
			};

			when("I enable imu deep sleep") = [&] {
				coreimu.enableDeepSleep();

				then("I expect deep sleep TO BE MORE than 99%") = [] {
					auto deep_sleep_ratio = utils::sleep::get_deep_sleep_over_idle_time_ratio_for_duration(1s);
					expect(gt(deep_sleep_ratio, 0.99));
				};
			};

			when("I disable imu deep sleep") = [&] {
				coreimu.disableDeepSleep();

				then("I expect deep sleep TO BE LESS than 99%") = [] {
					auto deep_sleep_ratio = utils::sleep::get_deep_sleep_over_idle_time_ratio_for_duration(1s);
					expect(lt(deep_sleep_ratio, 0.99));
				};
			};
		};
	};
};
