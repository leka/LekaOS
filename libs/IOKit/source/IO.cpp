// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "internal/IO.h"

using namespace leka::io::expanded::internal;

auto IO::read() -> int
{
	auto val = _parent.readInputs() & _pin;
	return val;
}

void IO::setMode(PinMode mode)
{
	if (mode == PullDown) {	  // ? MCP23017 may not support PullDown mode
		return;
	}

	_mutex.lock();
	uint16_t pullups = _parent.getPullups();

	if (mode == PullNone) {
		pullups &= ~_pin;
	} else if (mode == PullUp) {
		pullups |= _pin;
	}

	_parent.setPullups(pullups);
	_mutex.unlock();
}

void IO::setAsInput()
{
	_parent.setInputPins(_pin);
}
