// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreFlashIS25LP016D.h"

#include "rtos/ThisThread.h"

using namespace std::chrono;
using namespace flash::is25lp016d;

namespace leka {

auto CoreFlashIS25LP016D::getSize() -> std::size_t
{
	return flash::is25lp016d::size;
}

void CoreFlashIS25LP016D::reset()
{
	_flash_manager.reset();
}

auto CoreFlashIS25LP016D::read(uint32_t address, std::span<uint8_t> rx_buffer,
							   std::size_t rx_buffer_size) -> std::size_t
{
	if (address + rx_buffer.size() > flash::is25lp016d::size) {
		return 0;
	}

	_flash_manager.waitForChipAvailable();
	if (_flash_manager.chipIsNotAvailable()) {
		return 0;
	}
	auto bytes_read = _qspi.read(command::read, address, rx_buffer, rx_buffer_size);

	return bytes_read;
}

auto CoreFlashIS25LP016D::write(uint32_t address, const std::span<uint8_t> tx_buffer,
								std::size_t tx_buffer_size) -> std::size_t
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
	auto bytes_write = _qspi.write(command::write, address, tx_buffer, tx_buffer_size);

	return bytes_write;
}

void CoreFlashIS25LP016D::erase()
{
	_flash_manager.erase();
}

}	// namespace leka
