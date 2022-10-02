// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreQSPI.h"

using namespace leka;

void CoreQSPI::setDataTransmissionFormat()
{
	auto _command_width					  = QSPI_CFG_BUS_SINGLE;
	auto _address_width					  = QSPI_CFG_BUS_SINGLE;
	auto _address_size					  = QSPI_CFG_ADDR_SIZE_24;
	auto _alternate_phase_width			  = QSPI_CFG_BUS_SINGLE;
	qspi_alt_size_t _alternate_phase_size = QSPI_CFG_ALT_SIZE_8;
	auto _data_width					  = QSPI_CFG_BUS_SINGLE;
	auto _dummy_cycles					  = 0;

	_qspi.configure_format(_command_width, _address_width, _address_size, _alternate_phase_width, _alternate_phase_size,
						   _data_width, _dummy_cycles);
}

void CoreQSPI::setFrequency(int hz)
{
	_qspi.set_frequency(hz);
}

auto CoreQSPI::read(uint8_t command, uint32_t address, std::span<uint8_t> rx_buffer, size_t rx_buffer_size) -> size_t
{
	// ? Use local variable as Mbed's QSPI driver returns the number of bytes read as an in/out parameter
	auto size = rx_buffer_size;

	_qspi.read(command, -1, address, reinterpret_cast<char *>(rx_buffer.data()), &size);

	return size;
}

auto CoreQSPI::write(uint8_t command, uint32_t address, const std::span<uint8_t> tx_buffer, size_t tx_buffer_size)
	-> size_t
{
	// ? Use local variable as Mbed's QSPI driver returns the number of bytes written as an in/out parameter
	auto size = tx_buffer_size;

	_qspi.write(command, -1, address, reinterpret_cast<char *>(tx_buffer.data()), &size);

	return size;
}

auto CoreQSPI::sendCommand(uint8_t command, uint32_t address, const std::span<uint8_t> tx_buffer, size_t tx_buffer_size,
						   std::span<uint8_t> rx_buffer, size_t rx_buffer_size) -> std::tuple<size_t, size_t>
{
	// ? Use local variable as Mbed's QSPI drivers returns the number of bytes read/written as an in/out parameter
	auto tx_size = tx_buffer_size;
	auto rx_size = rx_buffer_size;

	_qspi.command_transfer(command, address, reinterpret_cast<char *>(tx_buffer.data()), tx_size,
						   reinterpret_cast<char *>(rx_buffer.data()), rx_size);

	return std::make_tuple(tx_size, rx_size);
}
