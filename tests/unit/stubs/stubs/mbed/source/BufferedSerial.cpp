// Leka - LekaOS
// Copyright 2024 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "../BufferedSerial.h"

namespace {
bool spy_BufferedSerial_enable_input = true;
}	// namespace

namespace mbed {

BufferedSerial::BufferedSerial(PinName, PinName, int) : SerialBase(NC, NC, 0)
{
	spy_BufferedSerial_enable_input = true;
}

BufferedSerial::~BufferedSerial()
{
	spy_BufferedSerial_enable_input = false;
}

ssize_t BufferedSerial::read(void *buffer, size_t length)
{
	char *buf_ptr = static_cast<char *>(buffer);

	for (int i = 0; i < length; ++i) {
		buf_ptr[i] = leka::spy_BufferedSerial_read_values.at(i);
	}

	return 0;
}

ssize_t BufferedSerial::write(const void *buffer, size_t length)
{
	const char *buf_ptr = static_cast<const char *>(buffer);

	leka::spy_BufferedSerial_write_values.assign(buf_ptr, buf_ptr + length);
	return 0;
}

off_t BufferedSerial::seek(off_t offset, int whence)
{
	return -ESPIPE;
}

int BufferedSerial::close()
{
	return 0;
}

void BufferedSerial::dcd_irq() {}

void BufferedSerial::set_baud(int baud) {}

void BufferedSerial::set_data_carrier_detect(PinName dcd_pin, bool active_high) {}

void BufferedSerial::set_format(int bits, Parity parity, int stop_bits) {}

int BufferedSerial::isatty()
{
	return 1;
}

int BufferedSerial::sync()
{
	return 0;
}

void BufferedSerial::sigio(Callback<void()> func)
{
	leka::spy_BufferedSerial_sigio_callback = func;
}

ssize_t BufferedSerial::write_unbuffered(const char *buf_ptr, size_t length)
{
	return 0;
}

bool BufferedSerial::hup() const
{
	return false;
}

void BufferedSerial::wake() {}

short BufferedSerial::poll(short events) const
{
	return 0;
}

void BufferedSerial::api_lock(void) {}

void BufferedSerial::api_unlock(void) {}

void BufferedSerial::rx_irq(void) {}

void BufferedSerial::tx_irq(void) {}

int BufferedSerial::enable_input(bool enabled)
{
	spy_BufferedSerial_enable_input = enabled;
	if (not(enabled)) {
		leka::spy_BufferedSerial_sigio_callback = [] {};   // ? Current behavior on robot
	}

	return 0;
}

int BufferedSerial::enable_output(bool enabled)
{
	return 0;
}

}	// namespace mbed

namespace leka {

std::vector<uint8_t> spy_BufferedSerial_read_values;
std::vector<uint8_t> spy_BufferedSerial_write_values;
mbed::Callback<void()> spy_BufferedSerial_sigio_callback {};

void spy_BufferedSerial_setValuesBeforeRead(std::vector<uint8_t> values)
{
	spy_BufferedSerial_read_values = values;
}

auto spy_BufferedSerial_getValuesAfterWrite() -> std::vector<uint8_t>
{
	return spy_BufferedSerial_write_values;
}

auto spy_BufferedSerial_getSigioCallback() -> mbed::Callback<void()>
{
	return spy_BufferedSerial_sigio_callback;
}

auto spy_BufferedSerial_getEnableInput() -> bool
{
	return spy_BufferedSerial_enable_input;
}

}	// namespace leka
