// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_CORE_FLASH_MEMORY_MANAGER_IS25LP016D_H_
#define _LEKA_OS_DRIVER_CORE_FLASH_MEMORY_MANAGER_IS25LP016D_H_

#include <chrono>

#include "drivers/Timer.h"

#include "interface/drivers/FlashManager.h"
#include "interface/drivers/QSPI.h"

namespace leka {

class CoreFlashManagerIS25LP016D : public interface::FlashManager
{
  public:
	explicit CoreFlashManagerIS25LP016D(interface::QSPI &qspi) : _qspi(qspi) {};

	auto getStatusRegister() -> uint8_t final;

	void waitForChipAvailable() final;
	auto chipIsNotAvailable() -> bool final;

	void enableWrite() final;
	auto writeIsNotEnabled() -> bool final;

	void reset() final;

	void erase() final;

  private:
	auto sendSimpleCommand(uint8_t command) -> bool;

	interface::QSPI &_qspi;

	mbed::Timer _timer;
	std::array<uint8_t, 8> tx_buffer {};
	std::array<uint8_t, 8> rx_buffer {};
};

}	// namespace leka

namespace flash::is25lp016d {

constexpr auto max_waiting_operation_duration = std::chrono::seconds(12);

namespace command {
	constexpr uint8_t reset = 0x99;

	constexpr uint8_t read_status = 0x05;

	constexpr uint8_t reset_enable = 0x66;
	constexpr uint8_t write_enable = 0x06;

	constexpr uint8_t erase_chip = 0x60;
}	// namespace command

namespace status {
	constexpr size_t register_size = 0x01;

	constexpr uint8_t work_in_progress	 = 0x01;
	constexpr uint8_t write_enable_latch = 0x02;

	namespace mask {
		constexpr uint8_t work_in_progress	 = 0x01;
		constexpr uint8_t write_enable_latch = 0x02;
	}	// namespace mask
}	// namespace status

}	// namespace flash::is25lp016d

#endif	 //_LEKA_OS_DRIVER_CORE_FLASH_MEMORY_MANAGER_IS25LP016D_H_
