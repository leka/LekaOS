// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <cstddef>

#include "tests/config.h"
#include "tests/utils.h"
#include "tests/utils_sleep.h"

using namespace boost::ut;
using namespace std::chrono;
using namespace boost::ut::bdd;

suite suite_log_kit = [] {
	rtos::ThisThread::sleep_for(2000ms);

	scenario("(default) LogKit w/ input_enable(false)") = [] {
		given("I use the default logger") = [] {
			log_info("using default logger");
			rtos::ThisThread::sleep_for(500ms);

			then("I expect deep sleep to be possible") = [] {
				auto status = utils::sleep::system_deep_sleep_check();

				expect(status.test_check_ok);
			};
		};
	};

	scenario("LogKit w/ input_enable(true)") = [] {
		given("I set input_enable(true) and use the default logger") = [] {
			leka::logger::internal::enable_filehandle_input();
			log_info("using default logger");
			rtos::ThisThread::sleep_for(500ms);

			then("I expect deep sleep to NOT be possible") = [] {
				auto status = utils::sleep::system_deep_sleep_check();

				expect(not status.test_check_ok);
			};
		};
	};

	scenario("LogKit w/ disable_filehandle_input()") = [] {
		given("I set input_enable(true) and use the default logger") = [] {
			leka::logger::internal::disable_filehandle_input();
			log_info("using default logger");
			rtos::ThisThread::sleep_for(500ms);

			then("I expect deep sleep to NOT be possible") = [] {
				auto status = utils::sleep::system_deep_sleep_check();

				expect(status.test_check_ok);
			};
		};
	};
};
