// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_CORE_FLASH_MEMORY_IS25LP016D_H_
#define _LEKA_OS_DRIVER_CORE_FLASH_MEMORY_IS25LP016D_H_

#include <chrono>

#include "drivers/Timer.h"

#include "interface/drivers/FlashMemory.h"
#include "interface/drivers/QSPI.h"

namespace leka {

enum class SPIMode
{
	Standard,
	Dual,
	Quad
};
using spi_mode_t = SPIMode;

enum class ReadMode
{
	Normal,
	Fast
};
using read_mode_t = ReadMode;

class CoreFlashMemoryIS25LP016D : public interface::FlashMemory
{
  public:
	explicit CoreFlashMemoryIS25LP016D(interface::QSPI &qspi) : _qspi(qspi) {};

	void setSPIMode(spi_mode_t mode);
	void setReadMode(read_mode_t mode);
	auto getSize() -> size_t final;

	void reset();

	auto read(uint32_t address, lstd::span<uint8_t> rx_buffer, size_t rx_buffer_size) -> size_t final;
	auto write(uint32_t address, lstd::span<uint8_t> tx_buffer, size_t tx_buffer_size) -> size_t final;

	void erase() final;

  private:
	auto getStatusRegister() -> uint8_t;

	void waitForChipAvailable();
	auto chipIsNotAvailable() -> bool;

	void enableWrite();
	auto writeIsNotEnabled() -> bool;

	interface::QSPI &_qspi;

	spi_mode_t _spi_mode   = SPIMode::Standard;
	read_mode_t _read_mode = ReadMode::Normal;

	mbed::Timer _timer;
	std::array<uint8_t, 8> tx_buffer {};
	std::array<uint8_t, 8> rx_buffer {};
};

}	// namespace leka

namespace flash_memory::is25lp016d {

constexpr size_t size				  = 0x00200000;
constexpr size_t status_register_size = 0x01;

constexpr auto max_clock_frequency_in_hz	  = 133'000'000;
constexpr auto max_waiting_operation_duration = std::chrono::seconds(12);

namespace command {
	constexpr uint8_t reset = 0x99;

	constexpr uint8_t write = 0x02;
	constexpr uint8_t read	= 0x03;

	constexpr uint8_t read_status = 0x05;

	constexpr uint8_t reset_enable = 0x66;
	constexpr uint8_t write_enable = 0x06;

	constexpr uint8_t erase_chip = 0x60;
}	// namespace command

namespace status {
	constexpr uint8_t work_in_progress	 = 0x01;
	constexpr uint8_t write_enable_latch = 0x02;

	namespace mask {
		constexpr uint8_t work_in_progress	 = 0x01;
		constexpr uint8_t write_enable_latch = 0x02;
	}	// namespace mask
}	// namespace status

}	// namespace flash_memory::is25lp016d

// ? IS25LP016D commands not used at the moment

#endif	 //_LEKA_OS_DRIVER_CORE_FLASH_MEMORY_IS25LP016D_H_
