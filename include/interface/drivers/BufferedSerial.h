// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>
#include <unistd.h>

namespace leka::interface {

class BufferedSerial
{
  public:
	virtual ~BufferedSerial() = default;

	virtual auto read(uint8_t *buffer, ssize_t length) -> ssize_t		 = 0;
	virtual auto write(const uint8_t *buffer, ssize_t length) -> ssize_t = 0;
	virtual auto readable() -> bool										 = 0;
};

}	// namespace leka::interface
