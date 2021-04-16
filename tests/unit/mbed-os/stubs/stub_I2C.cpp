// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "stub_I2C.h"

namespace mbed {

I2C::I2C(PinName, PinName)
{
	return;
}

int I2C::read(int address, char *data, int length, bool repeated)
{
	assert(length < 64);

	for (int i = 0; i < length; ++i) {
		data[i] = leka::spy_I2C_read_value[i];
	}

	return 0;
}

int I2C::write(int address, const char *data, int length, bool repeated)
{
	for (int i = 0; i < length; i++) {
		leka::spy_I2C_read_value.push_back(data[i]);
	}
	return 0;
}

void I2C::lock(void)
{
	// nothing to do
}

void I2C::unlock(void)
{
	// nothing to do
}

};	 // namespace mbed

namespace leka {

std::vector<char> spy_I2C_read_value;

std::vector<char> spy_temperatureSensor_setValue(std::vector<char> value)
{
	for (int i = 0; i < value.size(); i++) {
		spy_I2C_read_value.push_back(value[i]);
	}
	return spy_I2C_read_value;
}

std::vector<char> spy_temperatureSensor_getValue()
{
	std::vector<char> mem_vector;
	mem_vector.assign(spy_I2C_read_value.begin(), spy_I2C_read_value.end());
	spy_I2C_read_value.clear();
	return mem_vector;
}
}	// namespace leka
