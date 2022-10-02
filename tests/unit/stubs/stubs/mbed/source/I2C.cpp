// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "../I2C.h"

namespace mbed {

I2C::I2C(PinName, PinName)
{
	return;
}

int I2C::read(int address, char *data, int length, bool repeated)
{
	for (int i = 0; i < length; ++i) {
		data[i] = leka::spy_I2C_read_values.at(i);
	}

	return 0;
}

int I2C::write(int address, const char *data, int length, bool repeated)
{
	leka::spy_I2C_write_values.assign(data, data + length);
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

std::vector<uint8_t> spy_I2C_read_values;
std::vector<uint8_t> spy_I2C_write_values;

void spy_temperatureSensor_setValuesBeforeRead(std::vector<uint8_t> values)
{
	spy_I2C_read_values = values;
}

std::vector<uint8_t> spy_temperatureSensor_getValuesAfterWrite()
{
	return spy_I2C_write_values;
}
}	// namespace leka
