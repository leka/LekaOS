// Leka - LekaOS
// Copyright 2023 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreInterruptIn.h"

using namespace leka;

auto CoreInterruptIn::read() -> int
{
	return _irq.read();
}

void CoreInterruptIn::onRise(std::function<void()> const &callback)
{
	_on_rise_callback = callback;
	_irq.rise(mbed::Callback<void()> {[this] { _on_rise_callback(); }});
}

void CoreInterruptIn::onFall(std::function<void()> const &callback)
{
	_on_fall_callback = callback;
	_irq.fall(mbed::Callback<void()> {[this] { _on_fall_callback(); }});
}
