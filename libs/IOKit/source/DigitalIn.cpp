// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "DigitalIn.h"

using namespace leka::io::expanded;

DigitalIn::DigitalIn(interface::IOExpander &parent, uint16_t pin) : internal::IO(parent, pin)
{
	// TODO (@ladislas) make sure doing work in the constructor is okay
	internal::IO::setAsInput();
}

auto DigitalIn::read() -> int
{
	return internal::IO::read();
}

void DigitalIn::mode(PinMode mode)
{
	internal::IO::setMode(mode);
}

auto DigitalIn::is_connected() -> int
{
	return 1;
}
