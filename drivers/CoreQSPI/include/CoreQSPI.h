// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_CORE_QSPI_H_
#define _LEKA_OS_DRIVER_CORE_QSPI_H_

#include "drivers/QSPI.h"

#include "interface/drivers/QSPI.h"

namespace leka {

class CoreQSPI : public interface::QSPI
{
  public:
	explicit CoreQSPI(PinName io0 = QSPI_FLASH_IO0, PinName io1 = QSPI_FLASH_IO1, PinName io2 = QSPI_FLASH_IO2,
					  PinName io3 = QSPI_FLASH_IO3, PinName sclk = QSPI_FLASH_CLK, PinName ssel = QSPI_FLASH_nCS)
		: _qspi(io0, io1, io2, io3, sclk, ssel) {};

	void setDataTransmissionFormat(DataTransmissionFormat &data_transmission_format) final;
	void setFrequency(int hz = ONE_MHZ) final;

	auto read(uint8_t command, int alternate_phase, uint32_t address, lstd::span<uint8_t> rx_buffer) -> size_t final;
	auto write(uint8_t command, int alternate_phase, uint32_t address, const lstd::span<uint8_t> tx_buffer)
		-> size_t final;

	auto sendCommand(uint8_t command, uint32_t address, const lstd::span<uint8_t> tx_buffer,
					 lstd::span<uint8_t> rx_buffer) -> std::tuple<size_t, size_t> final;

  private:
	static auto getBusWidthEnum(int width) -> qspi_bus_width_t;
	static auto getAddressSizeEnum(size_t size) -> qspi_address_size_t;
	static auto getAlternatePhaseSizeEnum(size_t size) -> qspi_alt_size_t;

	mbed::QSPI _qspi;
};

}	// namespace leka

#endif	 // _LEKA_OS_DRIVER_CORE_QSPI_H_
