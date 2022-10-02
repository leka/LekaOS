// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "drivers/I2C.h"

#include "interface/drivers/I2C.h"

namespace leka {

class CoreI2C : public interface::I2C
{
  public:
	explicit CoreI2C(PinName sda, PinName scl) : _i2c(sda, scl) {}

	auto read(int address, uint8_t *data, int length, bool repeated) -> int final;
	auto write(int address, const uint8_t *data, int length, bool repeated) -> int final;

  private:
	mbed::I2C _i2c;
};

}	// namespace leka
