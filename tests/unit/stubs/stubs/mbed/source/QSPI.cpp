// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "../QSPI.h"

namespace mbed {

QSPI::QSPI(PinName io0, PinName io1, PinName io2, PinName io3, PinName sclk, PinName ssel, int mode)
{
	// nothing do to
}

QSPI::~QSPI() = default;

void QSPI::lock()
{
	// nothing do to
}

void QSPI::unlock()
{
	// nothing do to
}

auto QSPI::configure_format(qspi_bus_width_t inst_width, qspi_bus_width_t address_width,
							qspi_address_size_t address_size, qspi_bus_width_t alt_width, qspi_alt_size_t alt_size,
							qspi_bus_width_t data_width, int dummy_cycles) -> qspi_status_t
{
	leka::spy_QSPI_command_width		 = inst_width;
	leka::spy_QSPI_address_width		 = address_width;
	leka::spy_QSPI_address_size			 = address_size;
	leka::spy_QSPI_alternate_phase_width = alt_width;
	leka::spy_QSPI_alternate_phase_size	 = alt_size;
	leka::spy_QSPI_data_width			 = data_width;
	leka::spy_QSPI_dummy_cycle			 = dummy_cycles;

	return QSPI_STATUS_OK;
}

auto QSPI::set_frequency(int value) -> qspi_status_t
{
	leka::spy_QSPI_frequency = value;

	return QSPI_STATUS_OK;
}

auto QSPI::read(qspi_inst_t instruction, int alt, int address, char *rx_buffer, std::size_t *rx_length) -> qspi_status_t
{
	leka::spy_QSPI_command = instruction;
	leka::spy_QSPI_address = address;

	if (*rx_length > 2) {
		*rx_length = *rx_length - 1;
	}
	leka::spy_QSPI_rx_buffer_length = *rx_length;

	for (int i = 0; i < *rx_length; i++) {
		rx_buffer[i] = leka::spy_QSPI_rx_buffer[i];
	}

	return QSPI_STATUS_OK;
}

auto QSPI::write(qspi_inst_t instruction, int alt, int address, const char *tx_buffer,
				 std::size_t *tx_length) -> qspi_status_t
{
	leka::spy_QSPI_command = instruction;
	leka::spy_QSPI_address = address;

	if (*tx_length > 3) {
		*tx_length = *tx_length - 2;
	}
	leka::spy_QSPI_tx_buffer_length = *tx_length;

	for (int i = 0; i < *tx_length; i++) {
		leka::spy_QSPI_tx_buffer[i] = tx_buffer[i];
	}

	return QSPI_STATUS_OK;
}

auto QSPI::command_transfer(qspi_inst_t instruction, int address, const char *tx_buffer, std::size_t tx_length,
							const char *rx_buffer, std::size_t rx_length) -> qspi_status_t
{
	leka::spy_QSPI_command			= instruction;
	leka::spy_QSPI_address			= address;
	leka::spy_QSPI_tx_buffer_length = tx_length;
	leka::spy_QSPI_rx_buffer_length = rx_length;

	for (int i = 0; i < tx_length; i++) {
		leka::spy_QSPI_tx_buffer[i] = tx_buffer[i];
	}
	for (int i = 0; i < rx_length; i++) {
		// rx_buffer[i] = leka::spy_QSPI_rx_buffer[i];
	}

	return QSPI_STATUS_OK;
}

}	// namespace mbed

namespace leka {

qspi_bus_width_t spy_QSPI_command_width			= QSPI_CFG_BUS_DUAL;
qspi_bus_width_t spy_QSPI_address_width			= QSPI_CFG_BUS_DUAL;
qspi_address_size_t spy_QSPI_address_size		= QSPI_CFG_ADDR_SIZE_32;
qspi_bus_width_t spy_QSPI_alternate_phase_width = QSPI_CFG_BUS_DUAL;
qspi_alt_size_t spy_QSPI_alternate_phase_size	= QSPI_CFG_ALT_SIZE_32;
qspi_bus_width_t spy_QSPI_data_width			= QSPI_CFG_BUS_DUAL;
int spy_QSPI_dummy_cycle						= -1;

int spy_QSPI_frequency = -42;

mbed::qspi_inst_t spy_QSPI_command = 0x00;
int spy_QSPI_alternate_phase;
int spy_QSPI_address;
std::string spy_QSPI_rx_buffer;
size_t spy_QSPI_rx_buffer_length;
std::string spy_QSPI_tx_buffer;
size_t spy_QSPI_tx_buffer_length;

}	// namespace leka
