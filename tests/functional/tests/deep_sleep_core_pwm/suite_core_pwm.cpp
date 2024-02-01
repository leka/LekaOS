// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <cstddef>

#include "CorePwm.h"
#include "tests/config.h"
#include "tests/utils.h"
#include "tests/utils_sleep.h"

using namespace leka;
using namespace boost::ut;
using namespace std::chrono;
using namespace boost::ut::bdd;

suite suite_core_pwm = [] {
	scenario("write") = [] {
		given("pwm is in default configuration") = [] {
			auto pwm = CorePwm(MOTOR_LEFT_PWM);

			expect(neq(&pwm, nullptr));

			when("I do nothing") = [&] {
				then("I expect deep sleep TO NOT BE possible") = [] {
					auto status = utils::sleep::system_deep_sleep_check();

					expect(not status.can_deep_sleep);
					expect(not status.test_check_ok);
				};
			};

			when("pwm is running at 100%") = [&] {
				pwm.write(1.0);

				then("I expect deep sleep TO NOT BE possible") = [] {
					auto status = utils::sleep::system_deep_sleep_check();

					expect(not status.can_deep_sleep);
					expect(not status.test_check_ok);
				};
			};

			when("pwm is running at 50%") = [&] {
				pwm.write(0.5);

				then("I expect deep sleep TO NOT BE possible") = [] {
					auto status = utils::sleep::system_deep_sleep_check();

					expect(not status.can_deep_sleep);
					expect(not status.test_check_ok);
				};
			};

			when("pwm is running at 0%") = [&] {
				pwm.write(0.0);

				then("I expect deep sleep TO NOT BE possible") = [] {
					auto status = utils::sleep::system_deep_sleep_check();

					expect(not status.can_deep_sleep);
					expect(not status.test_check_ok);
				};
			};
		};
	};

	scenario("suspend, resume") = [] {
		given("pwm is in default configuration") = [] {
			auto pwm = CorePwm(MOTOR_LEFT_PWM);

			expect(neq(&pwm, nullptr));

			when("I do nothing") = [&] {
				then("I expect deep sleep TO NOT BE possible") = [] {
					auto status = utils::sleep::system_deep_sleep_check();

					expect(not status.can_deep_sleep);
					expect(not status.test_check_ok);
				};
			};

			when("pwm is suspended") = [&] {
				pwm.enableDeepSleep();
				rtos::ThisThread::sleep_for(5ms);

				then("I expect deep sleep TO BE possible") = [] {
					auto status = utils::sleep::system_deep_sleep_check();

					expect(status.can_deep_sleep);
					expect(status.test_check_ok);
				};
			};

			when("pwm is resumed") = [&] {
				pwm.disableDeepSleep();

				then("I expect deep sleep TO NOT BE possible") = [] {
					auto status = utils::sleep::system_deep_sleep_check();

					expect(not status.can_deep_sleep);
					expect(not status.test_check_ok);
				};
			};
		};
	};

	scenario("write 100%, suspend, resume") = [] {
		given("pwm is in default configuration") = [] {
			auto pwm = CorePwm(MOTOR_LEFT_PWM);

			expect(neq(&pwm, nullptr));

			when("I do nothing") = [&] {
				then("I expect deep sleep TO NOT BE possible") = [] {
					auto status = utils::sleep::system_deep_sleep_check();

					expect(not status.can_deep_sleep);
					expect(not status.test_check_ok);
				};
			};

			when("pwm is running at 100%") = [&] {
				pwm.write(1.0);

				then("I expect deep sleep TO NOT BE possible") = [] {
					auto status = utils::sleep::system_deep_sleep_check();

					expect(not status.can_deep_sleep);
					expect(not status.test_check_ok);
				};
			};

			when("pwm is suspended") = [&] {
				pwm.enableDeepSleep();
				rtos::ThisThread::sleep_for(5ms);

				then("I expect deep sleep TO BE possible") = [] {
					auto status = utils::sleep::system_deep_sleep_check();

					expect(status.can_deep_sleep);
					expect(status.test_check_ok);
				};
			};

			when("pwm is resumed") = [&] {
				pwm.disableDeepSleep();

				then("I expect deep sleep TO NOT BE possible") = [] {
					auto status = utils::sleep::system_deep_sleep_check();

					expect(not status.can_deep_sleep);
					expect(not status.test_check_ok);
				};
			};
		};
	};

	scenario("suspend, write 0%, resume") = [] {
		given("pwm is in default configuration") = [] {
			auto pwm = CorePwm(MOTOR_LEFT_PWM);

			expect(neq(&pwm, nullptr));

			when("I do nothing") = [&] {
				then("I expect deep sleep TO NOT BE possible") = [] {
					auto status = utils::sleep::system_deep_sleep_check();

					expect(not status.can_deep_sleep);
					expect(not status.test_check_ok);
				};
			};

			when("pwm is running at 0%") = [&] {
				pwm.write(0.0);

				then("I expect deep sleep TO NOT BE possible") = [] {
					auto status = utils::sleep::system_deep_sleep_check();

					expect(not status.can_deep_sleep);
					expect(not status.test_check_ok);
				};
			};

			when("pwm is suspended") = [&] {
				pwm.enableDeepSleep();
				rtos::ThisThread::sleep_for(5ms);

				then("I expect deep sleep TO BE possible") = [] {
					auto status = utils::sleep::system_deep_sleep_check();

					expect(status.can_deep_sleep);
					expect(status.test_check_ok);
				};
			};

			when("pwm is resumed") = [&] {
				pwm.disableDeepSleep();

				then("I expect deep sleep TO NOT BE possible") = [] {
					auto status = utils::sleep::system_deep_sleep_check();

					expect(not status.can_deep_sleep);
					expect(not status.test_check_ok);
				};
			};
		};
	};
};
