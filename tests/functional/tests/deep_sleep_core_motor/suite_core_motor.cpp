// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreMotor.h"
#include "CorePwm.h"
#include "tests/config.h"
#include "tests/utils.h"
#include "tests/utils_sleep.h"

using namespace leka;
using namespace boost::ut;
using namespace std::chrono;
using namespace boost::ut::bdd;

suite suite_core_motor = [] {
	scenario("base system check") = [] {
		given("motor is not instantiated") = [] {
			then("I expect deep sleep TO BE possible") = [] {
				auto status = utils::sleep::system_deep_sleep_check();

				expect(status.can_deep_sleep);
				expect(status.test_check_ok);
			};
		};
	};

	scenario("motor initialization") = [] {
		given("motor is in default configuration") = [] {
			auto dir_1 = mbed::DigitalOut {MOTOR_LEFT_DIRECTION_1};
			auto dir_2 = mbed::DigitalOut {MOTOR_LEFT_DIRECTION_2};
			auto speed = CorePwm {MOTOR_LEFT_PWM};

			auto motor = CoreMotor {dir_1, dir_2, speed};
			rtos::ThisThread::sleep_for(5ms);

			expect(neq(&motor, nullptr));

			when("I do nothing") = [&] {
				then("I expect deep sleep TO NOT BE possible") = [] {
					auto status = utils::sleep::system_deep_sleep_check();

					expect(not status.can_deep_sleep);
					expect(not status.test_check_ok);
				};
			};
		};
	};

	scenario("enabled/disable deepsleep") = [] {
		given("motor is in default configuration") = [] {
			auto dir_1 = mbed::DigitalOut {MOTOR_LEFT_DIRECTION_1};
			auto dir_2 = mbed::DigitalOut {MOTOR_LEFT_DIRECTION_2};
			auto speed = CorePwm {MOTOR_LEFT_PWM};

			auto motor = CoreMotor {dir_1, dir_2, speed};
			rtos::ThisThread::sleep_for(5ms);

			expect(neq(&motor, nullptr));

			when("I do nothing") = [&] {
				then("I expect deep sleep TO NOT BE possible") = [] {
					auto status = utils::sleep::system_deep_sleep_check();

					expect(not status.can_deep_sleep);
					expect(not status.test_check_ok);
				};
			};

			when("I enable motor deep sleep") = [&] {
				rtos::ThisThread::sleep_for(500ms);
				motor.enableDeepSleep();
				rtos::ThisThread::sleep_for(500ms);

				then("I expect deep sleep TO BE possible") = [] {
					auto status = utils::sleep::system_deep_sleep_check();

					expect(status.can_deep_sleep);
					expect(status.test_check_ok);
				};
			};

			when("I disable motor deep sleep") = [&] {
				rtos::ThisThread::sleep_for(500ms);
				motor.disableDeepSleep();
				rtos::ThisThread::sleep_for(500ms);

				then("I expect deep sleep TO NOT BE possible") = [] {
					auto status = utils::sleep::system_deep_sleep_check();

					expect(not status.can_deep_sleep);
					expect(not status.test_check_ok);
				};
			};

			when("I enable motor deep sleep") = [&] {
				rtos::ThisThread::sleep_for(500ms);
				motor.enableDeepSleep();
				rtos::ThisThread::sleep_for(500ms);

				then("I expect deep sleep TO BE possible") = [] {
					auto status = utils::sleep::system_deep_sleep_check();

					expect(status.can_deep_sleep);
					expect(status.test_check_ok);
				};
			};

			when("I disable motor deep sleep") = [&] {
				rtos::ThisThread::sleep_for(500ms);
				motor.disableDeepSleep();
				rtos::ThisThread::sleep_for(500ms);

				then("I expect deep sleep TO NOT BE possible") = [] {
					auto status = utils::sleep::system_deep_sleep_check();

					expect(not status.can_deep_sleep);
					expect(not status.test_check_ok);
				};
			};
		};
	};
};
