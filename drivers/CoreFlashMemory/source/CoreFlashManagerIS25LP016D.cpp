// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreFlashManagerIS25LP016D.h"

#include "rtos/ThisThread.h"

using namespace std::chrono;
using namespace flash::is25lp016d;

namespace leka {

auto CoreFlashManagerIS25LP016D::getStatusRegister() -> uint8_t
{
	_qspi.sendCommand(command::read_status, -1, tx_buffer, 0, rx_buffer, status::register_size);

	auto status_register = rx_buffer[0];

	return status_register;
}

auto CoreFlashManagerIS25LP016D::chipIsNotAvailable() -> bool
{
	auto work_in_progress = [&] {
		auto status = getStatusRegister();
		return (status & status::mask::work_in_progress) == status::work_in_progress;
	};

	auto chip_is_not_available = work_in_progress();

	return chip_is_not_available;
}

void CoreFlashManagerIS25LP016D::waitForChipAvailable()
{
	_timer.start();

	rtos::ThisThread::sleep_for(5ms);
	while (chipIsNotAvailable() && _timer.elapsed_time() < flash::is25lp016d::max_waiting_operation_duration) {
		rtos::ThisThread::sleep_for(200ms);
	}

	_timer.stop();
}

auto CoreFlashManagerIS25LP016D::sendSimpleCommand(uint8_t command) -> bool
{
	waitForChipAvailable();
	if (chipIsNotAvailable()) {
		return false;
	}
	_qspi.sendCommand(command, -1, tx_buffer, 0, rx_buffer, 0);

	return true;
}

void CoreFlashManagerIS25LP016D::reset()
{
	if (sendSimpleCommand(command::reset_enable)) {
		sendSimpleCommand(command::reset);
	}
}

void CoreFlashManagerIS25LP016D::enableWrite()
{
	sendSimpleCommand(command::write_enable);
}

auto CoreFlashManagerIS25LP016D::writeIsNotEnabled() -> bool
{
	auto write_enable_latch = [&] {
		auto status = getStatusRegister();
		return (status & status::mask::write_enable_latch) == status::write_enable_latch;
	};

	waitForChipAvailable();
	if (chipIsNotAvailable()) {
		return true;
	}
	auto write_not_enabled = !write_enable_latch();

	return write_not_enabled;
}

void CoreFlashManagerIS25LP016D::erase()
{
	enableWrite();
	if (writeIsNotEnabled()) {
		return;
	}

	sendSimpleCommand(command::erase_chip);
}

}	// namespace leka
