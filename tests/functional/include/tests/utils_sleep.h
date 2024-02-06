// Leka - LekaOS
// Copyright 2017 ARM Limited
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <chrono>

#include "hal/lp_ticker_api.h"
#include "hal/ticker_api.h"
#include "hal/us_ticker_api.h"
#include "platform/mbed_power_mgmt.h"
#include "platform/mbed_wait_api.h"
#include "rtos/ThisThread.h"

// ? utilities taken from https://github.com/ARMmbed/mbed-os
// ? https://github.com/ARMmbed/mbed-os/blob/master/hal/tests/TESTS/mbed_hal/sleep/sleep_test_utils.h

using namespace std::chrono;

namespace tests::utils::sleep {

constexpr inline auto SERIAL_FLUSH_TIME_MS	  = 150ms;
constexpr inline auto US_PER_S				  = 1'000'000;
constexpr inline auto DEEPSLEEP_MODE_DELTA_US = uint32_t {10000 + 125 + 5};

inline auto ticks_to_us(unsigned int ticks, unsigned int freq) -> unsigned int
{
	return static_cast<unsigned int>(static_cast<unsigned long long>(ticks) * US_PER_S / freq);
}

inline auto us_to_ticks(unsigned int us, unsigned int freq) -> unsigned int
{
	return static_cast<unsigned int>(static_cast<unsigned long long>(us) * freq / US_PER_S);
}

inline auto overflow_protect(unsigned int timestamp, unsigned int ticker_width) -> unsigned int
{
	unsigned int counter_mask = ((1 << ticker_width) - 1);

	return (timestamp & counter_mask);
}

inline auto compare_timestamps(unsigned int delta_ticks, unsigned int ticker_width, unsigned int expected,
							   unsigned int actual) -> bool
{
	const unsigned int counter_mask = ((1 << ticker_width) - 1);

	const unsigned int lower_bound = ((expected - delta_ticks) & counter_mask);
	const unsigned int upper_bound = ((expected + delta_ticks) & counter_mask);

	// NOLINTBEGIN
	if (lower_bound < upper_bound) {
		if (actual >= lower_bound && actual <= upper_bound) {
			return true;
		} else {
			return false;
		}
	} else {
		if ((actual >= lower_bound && actual <= counter_mask) || (actual >= 0 && actual <= upper_bound)) {
			return true;
		} else {
			return false;
		}
	}
	// NOLINTEND
}

inline void busy_wait(std::chrono::milliseconds ms)
{
	const auto *info = us_ticker_get_info();
	auto mask		 = static_cast<uint32_t>((1 << info->bits) - 1);

	auto delay = ms.count() * info->frequency / 1000;
	auto prev  = us_ticker_read();	 // NOLINT

	while (delay > 0) {
		auto next = us_ticker_read();	// NOLINT
		delay -= (next - prev) & mask;
		prev = next;
	}
}

inline void us_ticker_isr(const ticker_data_t *const ticker_data)
{
	us_ticker_clear_interrupt();
}

#if DEVICE_LPTICKER
inline void lp_ticker_isr(const ticker_data_t *const ticker_data)
{
	lp_ticker_clear_interrupt();
}
#endif

struct DeepSleepStatus {
	bool can_deep_sleep = {false};
	bool test_check_ok	= {false};
};

using deep_sleep_status_t = DeepSleepStatus;

inline auto system_deep_sleep_check() -> deep_sleep_status_t
{
	const ticker_data_t *lp_ticker	  = get_lp_ticker_data();
	const unsigned int lp_ticker_freq = lp_ticker->interface->get_info()->frequency;

	// ? Give time to test to finish UART transmission before entering deep sleep mode
	utils::sleep::busy_wait(utils::sleep::SERIAL_FLUSH_TIME_MS);

	auto can_deep_sleep = sleep_manager_can_deep_sleep();

	const timestamp_t wakeup_time = lp_ticker_read() + utils::sleep::us_to_ticks(20000, lp_ticker_freq);
	lp_ticker_set_interrupt(wakeup_time);

	auto can_deep_sleep_test_check = sleep_manager_can_deep_sleep_test_check();

	return {can_deep_sleep, can_deep_sleep_test_check};
}

}	// namespace tests::utils::sleep
