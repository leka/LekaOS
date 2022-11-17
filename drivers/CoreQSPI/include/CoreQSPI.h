// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <span>

#include "drivers/QSPI.h"

#include "interface/drivers/QSPI.h"

namespace leka {

class CoreQSPI : public interface::QSPI
{
  public:
	explicit CoreQSPI(PinName io0 = QSPI_FLASH_IO0, PinName io1 = QSPI_FLASH_IO1, PinName io2 = QSPI_FLASH_IO2,
					  PinName io3 = QSPI_FLASH_IO3, PinName sclk = QSPI_FLASH_CLK, PinName ssel = QSPI_FLASH_nCS)
		: _qspi(io0, io1, io2, io3, sclk, ssel)
	{
		// nothing do to
	}

	void setDataTransmissionFormat() final;
	void setFrequency(int hz = ONE_MHZ) final;

	auto read(uint8_t command, uint32_t address, std::span<uint8_t> rx_buffer, std::size_t rx_buffer_size)
		-> std::size_t final;
	auto write(uint8_t command, uint32_t address, std::span<uint8_t> tx_buffer, std::size_t tx_buffer_size)
		-> std::size_t final;

	auto sendCommand(uint8_t command, uint32_t address, std::span<uint8_t> tx_buffer, std::size_t tx_buffer_size,
					 std::span<uint8_t> rx_buffer, std::size_t rx_buffer_size) -> std::tuple<size_t, std::size_t> final;

  private:
	mbed::QSPI _qspi;
};

}	// namespace leka
