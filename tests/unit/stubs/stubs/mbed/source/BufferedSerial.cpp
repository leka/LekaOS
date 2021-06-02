// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "../BufferedSerial.h"

namespace mbed {

BufferedSerial::BufferedSerial(PinName tx, PinName rx, int baud) : SerialBase(tx, rx, baud)
{
	return;
}

BufferedSerial::~BufferedSerial()
{
	return;
}

auto BufferedSerial::poll(short events) const -> short
{
	return 0;
}

auto BufferedSerial::write(const void *buffer, size_t length) -> ssize_t
{
	return length;
}

auto BufferedSerial::read(void *buffer, size_t length) -> ssize_t
{
	return length;
}

auto BufferedSerial::close() -> int
{
	return 0;
}

auto BufferedSerial::isatty() -> int
{
	return 0;
}

auto BufferedSerial::seek(off_t offset, int whence) -> off_t
{
	return 0;
}

auto BufferedSerial::sync() -> int
{
	return 0;
}

auto BufferedSerial::enable_input(bool enabled) -> int
{
	return 0;
}

auto BufferedSerial::enable_output(bool enabled) -> int
{
	return 0;
}

void BufferedSerial::sigio(Callback<void()> func) {}

}	// namespace mbed
