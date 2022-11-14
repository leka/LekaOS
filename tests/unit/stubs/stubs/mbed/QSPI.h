// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <string>

#include "drivers/QSPI.h"

namespace leka {

extern qspi_bus_width_t spy_QSPI_command_width;
extern qspi_bus_width_t spy_QSPI_address_width;
extern qspi_address_size_t spy_QSPI_address_size;
extern qspi_bus_width_t spy_QSPI_alternate_phase_width;
extern qspi_alt_size_t spy_QSPI_alternate_phase_size;
extern qspi_bus_width_t spy_QSPI_data_width;
extern int spy_QSPI_dummy_cycle;

extern int spy_QSPI_frequency;

extern mbed::qspi_inst_t spy_QSPI_command;
extern int spy_QSPI_address;
extern std::string spy_QSPI_rx_buffer;
extern std::size_t spy_QSPI_rx_buffer_length;
extern std::string spy_QSPI_tx_buffer;
extern std::size_t spy_QSPI_tx_buffer_length;

}	// namespace leka
