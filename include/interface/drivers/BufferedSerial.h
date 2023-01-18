// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>
#include <functional>

#include "Callback.h"

namespace leka::interface {

class BufferedSerial
{
  public:
	virtual ~BufferedSerial() = default;

	virtual auto read(uint8_t *buffer, std::size_t length) -> std::size_t		 = 0;
	virtual auto write(const uint8_t *buffer, std::size_t length) -> std::size_t = 0;

	virtual auto readable() -> bool = 0;

	virtual void disable_input() = 0;
	virtual void enable_input()	 = 0;

	virtual void sigio(std::function<void()> const &callback) = 0;
};

}	// namespace leka::interface
