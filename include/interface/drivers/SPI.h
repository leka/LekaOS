// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>
#include <span>

namespace leka::interface {

class SPI
{
  public:
	virtual ~SPI() = default;

	virtual auto write(std::span<const uint8_t> data) -> std::size_t = 0;
};

}	// namespace leka::interface
