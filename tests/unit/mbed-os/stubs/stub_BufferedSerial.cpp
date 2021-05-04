// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "stub_BufferedSerial.h"

namespace mbed {

BufferedSerial::BufferedSerial(PinName tx, PinName rx, int baud) : SerialBase(tx, rx, baud)
{
	return;
}

BufferedSerial::~BufferedSerial()
{
	return;
}

short BufferedSerial::poll(short events) const
{
	return 0;
}

ssize_t BufferedSerial::write(const void *buffer, size_t length)
{
	return 0;
}
ssize_t BufferedSerial::read(void *buffer, size_t length)
{
	return 0;
}

int BufferedSerial::close()
{
	return 0;
}
int BufferedSerial::isatty()
{
	return 0;
}
off_t BufferedSerial::seek(off_t offset, int whence)
{
	return 0;
}
int BufferedSerial::sync()
{
	return 0;
}

int BufferedSerial::enable_input(bool enabled)
{
	return 0;
}
int BufferedSerial::enable_output(bool enabled)
{
	return 0;
}

void BufferedSerial::sigio(Callback<void()> func) {}

}	// namespace mbed
