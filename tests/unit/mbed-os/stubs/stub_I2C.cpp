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

char spy_I2C_read_value[64] {0};

char *spy_temperatureSensor_setValue(char *value, const int size)
{
	printf("Value of spy : \n");
	for (int i = 0; i < size; i++) {
		spy_I2C_read_value[i] = value[i];
		printf("%c ", value[i]);
	}
	printf("\n");
	return spy_I2C_read_value;
}
}	// namespace leka
