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

suite suite_mbed_hal = [] {
	// NOLINTBEGIN(bugprone-lambda-function-name)
	log_free("\t\033[31m! Important notice\033[0m\n");
	log_free("\tThose tests are very sensitive to timing and system state and they can easily fail\n");
	log_free("\twhen sleep_manager_can_deep_sleep() is called while something is blocking deep sleep\n");
	log_free("\tfor example: ticker, serial write, etc.\n");
	log_free("\tIt might be necessary to run the test multiple time to assess if failure is constant\n");
	log_free("\tor just a glitch in the \033[32mmatrix\033[0m...\n\n");
	// NOLINTEND(bugprone-lambda-function-name)

	rtos::ThisThread::sleep_for(1000ms);

	"high speed clocks - turned off on deep sleep"_test = [] {
		const ticker_data_t *us_ticker	   = get_us_ticker_data();
		const ticker_data_t *lp_ticker	   = get_lp_ticker_data();
		const unsigned int us_ticker_freq  = us_ticker->interface->get_info()->frequency;
		const unsigned int lp_ticker_freq  = lp_ticker->interface->get_info()->frequency;
		const unsigned int us_ticker_width = us_ticker->interface->get_info()->bits;
		const unsigned int lp_ticker_width = lp_ticker->interface->get_info()->bits;
		const unsigned int us_ticker_mask  = ((1 << us_ticker_width) - 1);

		// ? Give time to test to finish UART transmission before entering deep sleep mode
		utils::sleep::busy_wait(utils::sleep::SERIAL_FLUSH_TIME_MS);

		auto can_deep_sleep = sleep_manager_can_deep_sleep();
		expect(can_deep_sleep) << "deep sleep not possible";

		const timestamp_t wakeup_time = lp_ticker_read() + utils::sleep::us_to_ticks(20000, lp_ticker_freq);
		lp_ticker_set_interrupt(wakeup_time);

		auto can_deep_sleep_test_check = sleep_manager_can_deep_sleep_test_check();
		expect(can_deep_sleep_test_check);

		auto us_ticks_before_sleep = us_ticker_read();	 // NOLINT

		mbed_sleep();

		auto us_ticks_after_sleep = us_ticker_read();	// NOLINT
		auto lp_ticks_after_sleep = lp_ticker_read();

		// ? High freqency ticker should be disabled in deep-sleep mode. We expect that time difference between
		// ? ticker reads before and after the sleep represents only code execution time between calls.
		// ? Since we went to sleep for about 20 ms check if time counted by high frequency timer does not
		// ? exceed 1 ms.

		auto us_ticks_diff = (us_ticks_before_sleep <= us_ticks_after_sleep)
								 ? (us_ticks_after_sleep - us_ticks_before_sleep)
								 : (us_ticker_mask - us_ticks_before_sleep + us_ticks_after_sleep + 1);

		expect(utils::sleep::ticks_to_us(us_ticks_diff, us_ticker_freq) < 1000_u);

		// ? Used for deep-sleep mode, a target should be awake within 10 ms. Define us delta value as follows:
		// ? delta = default 10 ms + worst ticker resolution + extra time for code execution

		auto info = "Delta ticks: " +
					std::to_string(utils::sleep::us_to_ticks(utils::sleep::DEEPSLEEP_MODE_DELTA_US, lp_ticker_freq)) +
					"Ticker width: " + std::to_string(lp_ticker_width) +
					", Expected wake up tick: " + std::to_string(wakeup_time) +
					" Actual wake up tick: " + std::to_string(lp_ticks_after_sleep);

		auto timestamps_are_the_same = utils::sleep::compare_timestamps(
			utils::sleep::us_to_ticks(utils::sleep::DEEPSLEEP_MODE_DELTA_US, lp_ticker_freq), lp_ticker_width,
			wakeup_time, lp_ticks_after_sleep);

		expect(timestamps_are_the_same) << "Delta ticks: "
										<< utils::sleep::us_to_ticks(utils::sleep::DEEPSLEEP_MODE_DELTA_US,
																	 lp_ticker_freq)
										<< ", Ticker width: " << lp_ticker_width
										<< ", Expected wake up tick: " << wakeup_time
										<< ", Actual wake up tick: " << lp_ticks_after_sleep;
	};

	"lpticker - source of wake-up + 10ms timing"_test = [] {
		// ? Test that wake-up time from sleep should be less than 10 ms and
		// ? low power ticker interrupt can wake-up target from sleep

		const ticker_data_t *ticker		= get_lp_ticker_data();
		const unsigned int ticker_freq	= ticker->interface->get_info()->frequency;
		const unsigned int ticker_width = ticker->interface->get_info()->bits;

		const ticker_irq_handler_type lp_ticker_irq_handler_org =
			set_lp_ticker_irq_handler(utils::sleep::lp_ticker_isr);

		// ? Give time to test to finish UART transmission before entering deep sleep mode
		utils::sleep::busy_wait(utils::sleep::SERIAL_FLUSH_TIME_MS);

		auto can_deep_sleep = sleep_manager_can_deep_sleep();
		expect(can_deep_sleep) << "deep sleep not possible";

		// ? Testing wake-up time 10 ms
		for (timestamp_t i = 20'000; i < 200'000; i += 20'000) {
			// ? Give time to test to finish UART transmission before entering deep sleep mode
			utils::sleep::busy_wait(utils::sleep::SERIAL_FLUSH_TIME_MS);

			// ? Note: lp_ticker_read() operates on ticks
			const timestamp_t start_timestamp	   = lp_ticker_read();
			const timestamp_t next_match_timestamp = utils::sleep::overflow_protect(
				start_timestamp + utils::sleep::us_to_ticks(i, ticker_freq), ticker_width);

			lp_ticker_set_interrupt(next_match_timestamp);

			// ? On some targets like STM family boards with LPTIM enabled there is a required delay (~100 us) before
			// ? we are able to reprogram LPTIM_COMPARE register back to back. This is handled by the low level lp
			// ? ticker wrapper which uses LPTIM_CMPOK interrupt. CMPOK fires when LPTIM_COMPARE register can be
			// ? safely reprogrammed again. During this period deep-sleep is locked. This means that on these
			// ? platforms we have additional interrupt (CMPOK) fired always ~100 us after programming lp ticker.
			// ? Since this interrupt wakes-up the board from the sleep we need to go to sleep after CMPOK is handled.

			auto can_deep_sleep_test_check = sleep_manager_can_deep_sleep_test_check();
			expect(can_deep_sleep_test_check);

			mbed_sleep();

#if MBED_CONF_TARGET_LPTICKER_LPTIM

			// ? On some targets like STM family boards with LPTIM enabled an interrupt is triggered on counter
			// ? rollover. We need special handling for cases when next_match_timestamp < start_timestamp (interrupt
			// ? is to be fired after rollover). In such case after first wake-up we need to reset interrupt and go
			// ? back to sleep waiting for the valid one. NOTE: Above comment (CMPOK) applies also here.

			if ((next_match_timestamp < start_timestamp) && lp_ticker_read() < next_match_timestamp) {
				lp_ticker_set_interrupt(next_match_timestamp);
				wait_ns(200000);
				mbed_sleep();
			}

#endif	 // MBED_CONF_TARGET_LPTICKER_LPTIM

			const timestamp_t wakeup_timestamp = lp_ticker_read();

			auto timestamps_are_the_same = utils::sleep::compare_timestamps(
				utils::sleep::us_to_ticks(utils::sleep::DEEPSLEEP_MODE_DELTA_US, ticker_freq), ticker_width,
				next_match_timestamp, wakeup_timestamp);

			expect(timestamps_are_the_same)
				<< "Delta ticks: " << utils::sleep::us_to_ticks(utils::sleep::DEEPSLEEP_MODE_DELTA_US, ticker_freq)
				<< ", Ticker width: " << ticker_width << ", Expected wake up tick: " << next_match_timestamp
				<< ", Actual wake up tick: " << wakeup_timestamp
				<< ", delay ticks:  " << utils::sleep::us_to_ticks(i, ticker_freq)
				<< ", wake up after ticks: " << wakeup_timestamp - start_timestamp;
		}

		set_lp_ticker_irq_handler(lp_ticker_irq_handler_org);
	};
};
