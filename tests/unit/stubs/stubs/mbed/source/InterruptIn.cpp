// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "../InterruptIn.h"

// SingletonPtr<PlatformMutex> mbed::InterruptIn::_mutex;

namespace mbed {

InterruptIn::InterruptIn(PinName pin)
{
	// nothing to do
}

InterruptIn::~InterruptIn() = default;

auto InterruptIn::read() -> int
{
	return leka::spy_InterruptIn_value;
}

void InterruptIn::rise(Callback<void()> func)
{
	leka::spy_InterruptIn_risecallback = func;
}

void InterruptIn::fall(Callback<void()> func)
{
	leka::spy_InterruptIn_fallcallback = func;
}

}	// namespace mbed

namespace leka {

int spy_InterruptIn_value = -42.0;
mbed::Callback<void()> spy_InterruptIn_risecallback {};
mbed::Callback<void()> spy_InterruptIn_fallcallback {};

void spy_InterruptIn_setValue(int value)
{
	spy_InterruptIn_value = value;
}

auto spy_InterruptIn_getRiseCallback() -> mbed::Callback<void()>
{
	return spy_InterruptIn_risecallback;
}

auto spy_InterruptIn_getFallCallback() -> mbed::Callback<void()>
{
	return spy_InterruptIn_fallcallback;
}

}	// namespace leka
