// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>

namespace leka::interface {

class I2C
{
  public:
	virtual ~I2C() = default;

	virtual auto read(int address, uint8_t *data, int length, bool repeated) -> int		   = 0;
	virtual auto write(int address, const uint8_t *data, int length, bool repeated) -> int = 0;
};

}	// namespace leka::interface
