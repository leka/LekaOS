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

// ? tests inspired from https://github.com/ARMmbed/mbed-os
// ? https://github.com/ARMmbed/mbed-os/blob/master/hal/tests/TESTS/mbed_hal/sleep/main.cpp

suite suite_log_kit = [] {
	rtos::ThisThread::sleep_for(2000ms);

	scenario("(default) LogKit w/ input_enable(false)") = [] {
		given("I use the default logger") = [] {
			log_info("using default logger");
			rtos::ThisThread::sleep_for(500ms);

			then("I expect deep sleep to be possible") = [] {
				const ticker_data_t *lp_ticker	  = get_lp_ticker_data();
				const unsigned int lp_ticker_freq = lp_ticker->interface->get_info()->frequency;

				// ? Give time to test to finish UART transmission before entering deep sleep mode
				utils::sleep::busy_wait(utils::sleep::SERIAL_FLUSH_TIME_MS);

				auto can_deep_sleep = sleep_manager_can_deep_sleep();
				expect(can_deep_sleep) << "deep sleep not possible";

				const timestamp_t wakeup_time = lp_ticker_read() + utils::sleep::us_to_ticks(20000, lp_ticker_freq);
				lp_ticker_set_interrupt(wakeup_time);

				auto can_deep_sleep_test_check = sleep_manager_can_deep_sleep_test_check();
				expect(can_deep_sleep_test_check);
			};
		};
	};

	scenario("LogKit w/ input_enable(true)") = [] {
		given("I set input_enable(true) and use the default logger") = [] {
			leka::logger::internal::enable_filehandle_input();
			log_info("using default logger");
			rtos::ThisThread::sleep_for(500ms);

			then("I expect deep sleep to NOT be possible") = [] {
				const ticker_data_t *lp_ticker	  = get_lp_ticker_data();
				const unsigned int lp_ticker_freq = lp_ticker->interface->get_info()->frequency;

				// ? Give time to test to finish UART transmission before entering deep sleep mode
				utils::sleep::busy_wait(utils::sleep::SERIAL_FLUSH_TIME_MS);

				auto can_deep_sleep = sleep_manager_can_deep_sleep();
				expect(not can_deep_sleep) << "deep sleep STILL possible";

				const timestamp_t wakeup_time = lp_ticker_read() + utils::sleep::us_to_ticks(20000, lp_ticker_freq);
				lp_ticker_set_interrupt(wakeup_time);

				auto can_deep_sleep_test_check = sleep_manager_can_deep_sleep_test_check();
				expect(not can_deep_sleep_test_check);
			};
		};
	};

	scenario("LogKit w/ disable_filehandle_input()") = [] {
		given("I set input_enable(true) and use the default logger") = [] {
			leka::logger::internal::disable_filehandle_input();
			log_info("using default logger");
			rtos::ThisThread::sleep_for(500ms);

			then("I expect deep sleep to NOT be possible") = [] {
				const ticker_data_t *lp_ticker	  = get_lp_ticker_data();
				const unsigned int lp_ticker_freq = lp_ticker->interface->get_info()->frequency;

				// ? Give time to test to finish UART transmission before entering deep sleep mode
				utils::sleep::busy_wait(utils::sleep::SERIAL_FLUSH_TIME_MS);

				auto can_deep_sleep = sleep_manager_can_deep_sleep();
				expect(can_deep_sleep) << "deep sleep not possible";

				const timestamp_t wakeup_time = lp_ticker_read() + utils::sleep::us_to_ticks(20000, lp_ticker_freq);
				lp_ticker_set_interrupt(wakeup_time);

				auto can_deep_sleep_test_check = sleep_manager_can_deep_sleep_test_check();
				expect(can_deep_sleep_test_check);
			};
		};
	};
};
