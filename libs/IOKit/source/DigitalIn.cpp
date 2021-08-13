// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "DigitalIn.h"

using namespace leka::io::expanded;

DigitalIn::DigitalIn(interface::IOExpander &parent, uint16_t pin) : IO(parent, pin)
{
	internalInput();
}

auto DigitalIn::read() -> int
{
	return IO::internalRead();
}

void DigitalIn::mode(PinMode mode)
{
	IO::internalMode(mode);
}

auto DigitalIn::is_connected() -> int
{
	return 1;
}
