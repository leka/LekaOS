// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "internal/IO.h"

using namespace leka::io::expanded::internal;

auto IO::read() -> int
{
	auto val = expander.readInputs() & _pin;
	return val;
}

void IO::setMode(const PinMode mode)
{
	if (mode == PullDown) {	  // ? MCP23017 may not support PullDown mode
		return;
	}

	_mutex.lock();
	uint16_t pullups = expander.getPullups();

	if (mode == PullNone) {
		pullups &= ~_pin;
	} else if (mode == PullUp) {
		pullups |= _pin;
	}

	expander.setPullups(pullups);
	_mutex.unlock();
}

void IO::setAsInput()
{
	expander.setInputPins(_pin);
}
