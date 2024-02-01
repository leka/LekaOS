// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

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

void CoreBufferedSerial::disableDeepSleep()
{
	_serial.enable_input(true);
}

void CoreBufferedSerial::enableDeepSleep()
{
	_serial.enable_input(false);
}

void CoreBufferedSerial::sigio(std::function<void()> const &callback)
{
	_sigio_callback = callback;
	_serial.sigio(mbed::Callback<void()> {[this] { _sigio_callback(); }});
}
