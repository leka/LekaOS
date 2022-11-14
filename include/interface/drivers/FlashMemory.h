// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>
#include <span>

namespace leka::interface {

class FlashMemory
{
  public:
	virtual ~FlashMemory() = default;

	virtual auto getSize() -> std::size_t = 0;

	virtual auto read(uint32_t address, std::span<uint8_t> rx_buffer, std::size_t rx_buffer_size) -> std::size_t  = 0;
	virtual auto write(uint32_t address, std::span<uint8_t> tx_buffer, std::size_t tx_buffer_size) -> std::size_t = 0;

	virtual void erase() = 0;
};
}	// namespace leka::interface
