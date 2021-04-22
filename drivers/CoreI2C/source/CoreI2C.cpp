// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreI2C.h"

using namespace leka;

auto CoreI2C::read(int address, uint8_t *data, int length, bool repeated = false) -> int
{
	return _i2c.read(address, reinterpret_cast<char *>(data), length, repeated);
}

auto CoreI2C::write(int address, const uint8_t *data, int length, bool repeated = false) -> int
{
	return _i2c.write(address, reinterpret_cast<const char *>(data), length, repeated);
}
