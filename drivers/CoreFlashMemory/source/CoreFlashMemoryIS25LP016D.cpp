// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreFlashMemoryIS25LP016D.h"

#include "rtos/ThisThread.h"

using namespace std::chrono;
using namespace flash_memory::is25lp016d;

namespace leka {

void CoreFlashMemoryIS25LP016D::setSPIMode(spi_mode_t mode)
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
	_qspi.setFrequency(flash_memory::is25lp016d::max_clock_frequency_in_hz);
}

void CoreFlashMemoryIS25LP016D::setReadMode(read_mode_t mode)
{
	_read_mode = mode;
}

auto CoreFlashMemoryIS25LP016D::getSize() -> size_t
{
	return flash_memory::is25lp016d::size;
}

auto CoreFlashMemoryIS25LP016D::getStatusRegister() -> uint8_t
{
	_qspi.sendCommand(command::read_status, -1, tx_buffer, 0, rx_buffer, status_register_size);

	auto status_register = rx_buffer[0];

	return status_register;
}

auto CoreFlashMemoryIS25LP016D::chipIsNotAvailable() -> bool
{
	auto work_in_progress = [&] {
		auto status = getStatusRegister();
		return (status & status::mask::work_in_progress) == status::work_in_progress;
	};

	auto chip_is_not_available = work_in_progress();

	return chip_is_not_available;
}

void CoreFlashMemoryIS25LP016D::waitForChipAvailable()
{
	_timer.start();

	rtos::ThisThread::sleep_for(5ms);
	while (chipIsNotAvailable() && _timer.elapsed_time() < flash_memory::is25lp016d::max_waiting_operation_duration) {
		rtos::ThisThread::sleep_for(200ms);
	}

	_timer.stop();
}

void CoreFlashMemoryIS25LP016D::reset()
{
	waitForChipAvailable();
	if (chipIsNotAvailable()) {
		return;
	}
	_qspi.sendCommand(command::reset_enable, -1, tx_buffer, 0, rx_buffer, 0);

	waitForChipAvailable();
	if (chipIsNotAvailable()) {
		return;
	}
	_qspi.sendCommand(command::reset, -1, tx_buffer, 0, rx_buffer, 0);
}

void CoreFlashMemoryIS25LP016D::enableWrite()
{
	waitForChipAvailable();
	if (chipIsNotAvailable()) {
		return;
	}
	_qspi.sendCommand(command::write_enable, -1, tx_buffer, 0, rx_buffer, 0);
}

auto CoreFlashMemoryIS25LP016D::writeIsNotEnabled() -> bool
{
	auto write_enable_latch = [&] {
		auto status = getStatusRegister();
		return (status & status::mask::write_enable_latch) == status::write_enable_latch;
	};

	waitForChipAvailable();
	if (chipIsNotAvailable()) {
		return false;
	}
	auto write_not_enabled = !write_enable_latch();

	return write_not_enabled;
}

auto CoreFlashMemoryIS25LP016D::read(uint32_t address, lstd::span<uint8_t> rx_buffer, size_t rx_buffer_size) -> size_t
{
	if (address + rx_buffer.size() > flash_memory::is25lp016d::size) {
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

	waitForChipAvailable();
	if (chipIsNotAvailable()) {
		return 0;
	}
	auto bytes_read = _qspi.read(command_read, -1, address, rx_buffer, rx_buffer_size);

	return bytes_read;
}

auto CoreFlashMemoryIS25LP016D::write(uint32_t address, lstd::span<uint8_t> tx_buffer, size_t tx_buffer_size) -> size_t
{
	if (address + tx_buffer.size() > getSize()) {
		return 0;
	}

	enableWrite();
	if (writeIsNotEnabled()) {
		return 0;
	}

	waitForChipAvailable();
	if (chipIsNotAvailable()) {
		return 0;
	}
	auto bytes_write = _qspi.write(command::write, -1, address, tx_buffer, tx_buffer_size);

	return bytes_write;
}

void CoreFlashMemoryIS25LP016D::erase()
{
	enableWrite();
	if (writeIsNotEnabled()) {
		return;
	}

	waitForChipAvailable();
	if (chipIsNotAvailable()) {
		return;
	}
	_qspi.sendCommand(command::erase_chip, -1, tx_buffer, 0, rx_buffer, 0);
}

}	// namespace leka
