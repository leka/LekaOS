// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#define __STDC_LIMIT_MACROS

#include "../mbed_critical.h"

auto core_util_are_interrupts_enabled() -> bool
{
	return false;
}

auto core_util_is_isr_active() -> bool
{
	return false;
}

auto core_util_in_critical_section() -> bool
{
	return false;
}

void core_util_critical_section_enter()
{
	leka::spy_mbed_critical_enter_calls++;
}

void core_util_critical_section_exit()
{
	leka::spy_mbed_critical_exit_calls++;
}

namespace leka {

int spy_mbed_critical_enter_calls = 0;
int spy_mbed_critical_exit_calls  = 0;

auto spy_mbed_critical_enter_was_called() -> bool
{
	return spy_mbed_critical_enter_calls ? true : false;
}

auto spy_mbed_critical_exit_was_called() -> bool
{
	return spy_mbed_critical_exit_calls ? true : false;
}

}	// namespace leka
