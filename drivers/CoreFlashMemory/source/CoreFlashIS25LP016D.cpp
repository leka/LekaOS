// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreFlashIS25LP016D.h"

#include "rtos/ThisThread.h"

using namespace std::chrono;
using namespace flash::is25lp016d;

namespace leka {

void CoreFlashIS25LP016D::setSPIMode(spi_mode_t mode)
{
	_spi_mode = mode;

	auto data_transmission_format = interface::QSPI::DataTransmissionFormat {};

	switch (mode) {
		case SPIMode::Standard:
			data_transmission_format = interface::QSPI::DataTransmissionFormat {1, 1, 24, 1, 8, 1, 0};
			break;
		case SPIMode::Dual:
			data_transmission_format = interface::QSPI::DataTransmissionFormat {};
			break;
		case SPIMode::Quad:
			data_transmission_format = interface::QSPI::DataTransmissionFormat {};
			break;
		default:
			data_transmission_format = interface::QSPI::DataTransmissionFormat {1, 1, 24, 1, 8, 1, 0};
	}

	_qspi.setDataTransmissionFormat(data_transmission_format);
	_qspi.setFrequency(flash::is25lp016d::max_clock_frequency_in_hz);
}

void CoreFlashIS25LP016D::setReadMode(read_mode_t mode)
{
	_read_mode = mode;
}

auto CoreFlashIS25LP016D::getSize() -> size_t
{
	return flash::is25lp016d::size;
}

void CoreFlashIS25LP016D::reset()
{
	_flash_manager.reset();
}

auto CoreFlashIS25LP016D::read(uint32_t address, lstd::span<uint8_t> rx_buffer, size_t rx_buffer_size) -> size_t
{
	if (address + rx_buffer.size() > flash::is25lp016d::size) {
		return 0;
	}

	auto command_read = command::read;
	switch (_read_mode) {
		case ReadMode::Normal:
			command_read = command::read;
			break;
		case ReadMode::Fast:
			break;
		default:
			command_read = command::read;
	}

	_flash_manager.waitForChipAvailable();
	if (_flash_manager.chipIsNotAvailable()) {
		return 0;
	}
	auto bytes_read = _qspi.read(command_read, -1, address, rx_buffer, rx_buffer_size);

	return bytes_read;
}

auto CoreFlashIS25LP016D::write(uint32_t address, const lstd::span<uint8_t> tx_buffer, size_t tx_buffer_size) -> size_t
{
	if (address + tx_buffer.size() > getSize()) {
		return 0;
	}

	_flash_manager.enableWrite();
	if (_flash_manager.writeIsNotEnabled()) {
		return 0;
	}

	_flash_manager.waitForChipAvailable();
	if (_flash_manager.chipIsNotAvailable()) {
		return 0;
	}
	auto bytes_write = _qspi.write(command::write, -1, address, tx_buffer, tx_buffer_size);

	return bytes_write;
}

void CoreFlashIS25LP016D::erase()
{
	_flash_manager.erase();
}

}	// namespace leka
