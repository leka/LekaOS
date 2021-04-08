// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LKCoreI2C.h"
using namespace leka::interface;

int LKCoreI2C::read(int address, char *data, int length, bool repeated = false)
{
	return _i2c.read(address, data, length, repeated);
}

int LKCoreI2C::write(int address, const char *data, int length, bool repeated = false)
{
	return _i2c.write(address, data, length, repeated);
}
