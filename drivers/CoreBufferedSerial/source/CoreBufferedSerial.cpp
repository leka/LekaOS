// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// ? LCOV_EXCL_START - Exclude from coverage report

#include "CoreBufferedSerial.h"

using namespace leka;

auto CoreBufferedSerial::read(uint8_t *buffer, ssize_t length) -> ssize_t
{
	return _serial.read(buffer, length);
}

auto CoreBufferedSerial::write(const uint8_t *buffer, ssize_t length) -> ssize_t
{
	return _serial.write(buffer, length);
}

auto CoreBufferedSerial::readable() -> bool
{
	return _serial.readable();
}

void CoreBufferedSerial::registerIOCallback(mbed::Callback<void()> callback)
{
	return _serial.sigio(callback);
}

// ? LCOV_EXCL_STOP - Exclude from coverage report
