// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_INTERFACE_QSPI_H_
#define _LEKA_OS_DRIVER_INTERFACE_QSPI_H_

#include <tuple>

#include "../../cxxsupport/lstd_span"

namespace leka::interface {

class QSPI
{
  public:
	struct DataTransmissionFormat {
		int command_width			= 1;
		int address_width			= 1;
		size_t address_size			= 8;
		int alternate_phase_width	= 1;
		size_t alternate_phase_size = 8;
		int data_width				= 1;
		int dummy_cycles			= 0;
	};

	virtual ~QSPI() = default;

	virtual void setDataTransmissionFormat(DataTransmissionFormat &data_transmission_format) = 0;
	virtual void setFrequency(int hz = 1'000'000)											 = 0;

	virtual auto read(uint8_t command, int alternate_phase, uint32_t address, lstd::span<uint8_t> rx_buffer)
		-> size_t = 0;
	virtual auto write(uint8_t command, int alternate_phase, uint32_t address, const lstd::span<uint8_t> tx_buffer)
		-> size_t = 0;

	virtual auto sendCommand(uint8_t command, uint32_t address, const lstd::span<uint8_t> tx_buffer,
							 lstd::span<uint8_t> rx_buffer) -> std::tuple<size_t, size_t> = 0;
};

}	// namespace leka::interface

#endif	 // _LEKA_OS_DRIVER_INTERFACE_QSPI_H_
