// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <cstddef>

#include "CorePwm.h"
#include "tests/config.h"
#include "tests/utils.h"
#include "tests/utils_sleep.h"

using namespace std::chrono;
using namespace leka;
using namespace boost::ut;
using namespace boost::ut::bdd;

suite suite_core_pwm = [] {
	scenario("base system check") = [] {
		given("pwm is not instantiated") = [] {
			then("I expect deep sleep TO BE possible") = [] {
				auto status = utils::sleep::system_deep_sleep_check();

				expect(status.test_check_ok);
			};
		};
	};

	scenario("pwm initialization") = [] {
		given("pwm is in default configuration") = [] {
			auto pwm = CorePwm(MOTOR_LEFT_PWM);

			expect(neq(&pwm, nullptr));

			rtos::ThisThread::sleep_for(500ms);

			when("I do nothing") = [&] {
				then("I expect deep sleep TO NOT BE possible") = [] {
					auto status = utils::sleep::system_deep_sleep_check();

					expect(not status.test_check_ok);
				};
			};
		};
	};

	scenario("enabled/disable deepsleep") = [] {
		given("pwm is in default configuration") = [] {
			auto pwm = CorePwm(MOTOR_LEFT_PWM);

			expect(neq(&pwm, nullptr));

			rtos::ThisThread::sleep_for(500ms);

			when("I do nothing") = [&] {
				then("I expect deep sleep TO NOT BE possible") = [] {
					auto status = utils::sleep::system_deep_sleep_check();

					expect(not status.test_check_ok);
				};
			};

			when("I enable deep sleep") = [&] {
				rtos::ThisThread::sleep_for(500ms);
				pwm.enableDeepSleep();
				rtos::ThisThread::sleep_for(500ms);

				then("I expect deep sleep TO BE possible") = [] {
					auto status = utils::sleep::system_deep_sleep_check();

					expect(status.test_check_ok);
				};
			};

			when("I disable deep sleep") = [&] {
				rtos::ThisThread::sleep_for(500ms);
				pwm.disableDeepSleep();
				rtos::ThisThread::sleep_for(500ms);

				then("I expect deep sleep TO NOT BE possible") = [] {
					auto status = utils::sleep::system_deep_sleep_check();

					expect(not status.test_check_ok);
				};
			};

			when("I enable deep sleep") = [&] {
				rtos::ThisThread::sleep_for(500ms);
				pwm.enableDeepSleep();
				rtos::ThisThread::sleep_for(500ms);

				then("I expect deep sleep TO BE possible") = [] {
					auto status = utils::sleep::system_deep_sleep_check();

					expect(status.test_check_ok);
				};
			};

			when("I disable deep sleep") = [&] {
				rtos::ThisThread::sleep_for(500ms);
				pwm.disableDeepSleep();
				rtos::ThisThread::sleep_for(500ms);

				then("I expect deep sleep TO NOT BE possible") = [] {
					auto status = utils::sleep::system_deep_sleep_check();

					expect(not status.test_check_ok);
				};
			};

			when("I enable deep sleep") = [&] {
				rtos::ThisThread::sleep_for(500ms);
				pwm.enableDeepSleep();
				rtos::ThisThread::sleep_for(500ms);

				then("I expect deep sleep TO BE possible") = [] {
					auto status = utils::sleep::system_deep_sleep_check();

					expect(status.test_check_ok);
				};
			};

			when("I disable deep sleep") = [&] {
				rtos::ThisThread::sleep_for(500ms);
				pwm.disableDeepSleep();
				rtos::ThisThread::sleep_for(500ms);

				then("I expect deep sleep TO NOT BE possible") = [] {
					auto status = utils::sleep::system_deep_sleep_check();

					expect(not status.test_check_ok);
				};
			};
		};
	};
};
