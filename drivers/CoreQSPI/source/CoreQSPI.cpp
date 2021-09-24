// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreQSPI.h"

using namespace leka;

void CoreQSPI::setDataTransmissionFormat(DataTransmissionFormat &data_transmission_format)
{
	auto _command_width			= getBusWidthEnum(data_transmission_format.command_width);
	auto _address_width			= getBusWidthEnum(data_transmission_format.address_width);
	auto _address_size			= getAddressSizeEnum(data_transmission_format.address_size);
	auto _alternate_phase_width = getBusWidthEnum(data_transmission_format.alternate_phase_width);
	auto _alternate_phase_size	= getAlternatePhaseSizeEnum(data_transmission_format.alternate_phase_size);
	auto _data_width			= getBusWidthEnum(data_transmission_format.data_width);

	_qspi.configure_format(_command_width, _address_width, _address_size, _alternate_phase_width, _alternate_phase_size,
						   _data_width, data_transmission_format.dummy_cycles);
}

auto CoreQSPI::getBusWidthEnum(int width) -> qspi_bus_width_t
{
	switch (width) {
		case 1:
			return QSPI_CFG_BUS_SINGLE;
		case 2:
			return QSPI_CFG_BUS_DUAL;
		case 4:
			return QSPI_CFG_BUS_QUAD;
		default:
			return QSPI_CFG_BUS_SINGLE;
	}
}

auto CoreQSPI::getAddressSizeEnum(size_t size) -> qspi_address_size_t
{
	switch (size) {
		case 8:
			return QSPI_CFG_ADDR_SIZE_8;
		case 16:
			return QSPI_CFG_ADDR_SIZE_16;
		case 24:
			return QSPI_CFG_ADDR_SIZE_24;
		case 32:
			return QSPI_CFG_ADDR_SIZE_32;
		default:
			return QSPI_CFG_ADDR_SIZE_8;
	}
}

auto CoreQSPI::getAlternatePhaseSizeEnum(size_t size) -> qspi_alt_size_t
{
	switch (size) {
		case 8:
			return QSPI_CFG_ALT_SIZE_8;
		case 16:
			return QSPI_CFG_ALT_SIZE_16;
		case 24:
			return QSPI_CFG_ALT_SIZE_24;
		case 32:
			return QSPI_CFG_ALT_SIZE_32;
		default:
			return QSPI_CFG_ALT_SIZE_8;
	}
}

void CoreQSPI::setFrequency(int hz)
{
	_qspi.set_frequency(hz);
}

auto CoreQSPI::read(uint8_t command, int alternate_phase, int address, lstd::span<char> rx_buffer) -> size_t
{
	// ? Use local variable as Mbed's QSPI driver returns the number of bytes read as an in/out parameter
	auto size = rx_buffer.size();

	_qspi.read(command, alternate_phase, address, rx_buffer.data(), &size);

	return size;
}

auto CoreQSPI::write(uint8_t command, int alternate_phase, int address, const lstd::span<char> tx_buffer) -> size_t
{
	// ? Use local variable as Mbed's QSPI driver returns the number of bytes written as an in/out parameter
	auto size = tx_buffer.size();

	_qspi.write(command, alternate_phase, address, tx_buffer.data(), &size);

	return size;
}

auto CoreQSPI::sendCommand(uint8_t command, int address, const lstd::span<char> tx_buffer, lstd::span<char> rx_buffer)
	-> std::tuple<size_t, size_t>
{
	// ? Use local variable as Mbed's QSPI drivers returns the number of bytes read/written as an in/out parameter
	auto tx_size = tx_buffer.size();
	auto rx_size = rx_buffer.size();

	_qspi.command_transfer(command, address, tx_buffer.data(), tx_size, rx_buffer.data(), rx_size);

	return std::make_tuple(tx_size, rx_size);
}
