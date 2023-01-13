// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// LCOV_EXCL_START

#include "CoreBufferedSerial.h"

using namespace leka;

auto CoreBufferedSerial::read(uint8_t *buffer, std::size_t length) -> std::size_t
{
	return _serial.read(buffer, length);
}

auto CoreBufferedSerial::write(const uint8_t *buffer, std::size_t length) -> std::size_t
{
	return _serial.write(buffer, length);
}

auto CoreBufferedSerial::readable() -> bool
{
	return _serial.readable();
}

void CoreBufferedSerial::enable_input()
{
	_serial.enable_input(true);
}

void CoreBufferedSerial::disable_input()
{
	_serial.enable_input(false);
}

void CoreBufferedSerial::sigio(mbed::Callback<void()> func)
{
	_serial.sigio(func);
}

// LCOV_EXCL_STOP
