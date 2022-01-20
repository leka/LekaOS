// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "drivers/InterruptIn.h"

namespace leka {

extern int spy_InterruptIn_value;
extern mbed::Callback<void()> spy_InterruptIn_risecallback;
extern mbed::Callback<void()> spy_InterruptIn_fallcallback;

void spy_InterruptIn_setValue(int value);
auto spy_InterruptIn_getRiseCallback() -> mbed::Callback<void()>;
auto spy_InterruptIn_getFallCallback() -> mbed::Callback<void()>;

}	// namespace leka
