// Leka - LekaOS
// Copyright 2023 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreInterruptIn.h"

using namespace leka;

auto CoreInterruptIn::read() -> int
{
	return _interrupt_in.read();
}

void CoreInterruptIn::rise(std::function<void()> const &callback)
{
	_on_rise_callback = callback;
	_interrupt_in.rise(mbed::Callback<void()> {[this] { _on_rise_callback(); }});
}

void CoreInterruptIn::fall(std::function<void()> const &callback)
{
	_on_fall_callback = callback;
	_interrupt_in.fall(mbed::Callback<void()> {[this] { _on_fall_callback(); }});
}
