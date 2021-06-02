// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "platform/mbed_critical.h"

namespace leka {

extern int spy_mbed_critical_enter_calls;
extern int spy_mbed_critical_exit_calls;

auto spy_mbed_critical_enter_was_called() -> bool;
auto spy_mbed_critical_exit_was_called() -> bool;

}	// namespace leka
