// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>
#include <span>
#include <tuple>

namespace leka::interface {

class QSPI
{
  public:
	virtual ~QSPI() = default;

	virtual void setDataTransmissionFormat()	  = 0;
	virtual void setFrequency(int hz = 1'000'000) = 0;

	virtual auto read(uint8_t command, uint32_t address, std::span<uint8_t> rx_buffer, std::size_t rx_buffer_size)
		-> std::size_t = 0;
	virtual auto write(uint8_t command, uint32_t address, std::span<uint8_t> tx_buffer, std::size_t tx_buffer_size)
		-> std::size_t = 0;

	virtual auto sendCommand(uint8_t command, uint32_t address, std::span<uint8_t> tx_buffer,
							 std::size_t tx_buffer_size, std::span<uint8_t> rx_buffer, std::size_t rx_buffer_size)
		-> std::tuple<size_t, std::size_t> = 0;
};

}	// namespace leka::interface
