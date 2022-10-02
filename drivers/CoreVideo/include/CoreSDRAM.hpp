// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0
// Original file:
// https://github.com/STMicroelectronics/STM32CubeF7/blob/master/Drivers/BSP/STM32F769I-Discovery/stm32f769i_discovery_sdram.h

#pragma once

#include "interface/SDRAM.hpp"
#include "interface/drivers/STM32Hal.h"

namespace leka {

class CoreSDRAM : public interface::SDRAM
{
  public:
	explicit CoreSDRAM(interface::STM32Hal &hal);

	void setupSDRAMConfig() final;
	auto setupTimingConfig() -> FMC_SDRAM_TimingTypeDef final;
	auto setupDMA() -> DMA_HandleTypeDef final;

	auto initialize() -> uint8_t final;
	void initializeController() final;
	void initializationSequence() final;

	[[nodiscard]] auto getHandle() const -> SDRAM_HandleTypeDef;

  private:
	interface::STM32Hal &_hal;

	SDRAM_HandleTypeDef _hsdram {};
};

}	// namespace leka

namespace sdram {

// #define SDRAM_MEMORY_WIDTH FMC_SDRAM_MEM_BUS_WIDTH_32
constexpr uint32_t memory_width = FMC_SDRAM_MEM_BUS_WIDTH_32;

// #define SDCLOCK_PERIOD FMC_SDRAM_CLOCK_PERIOD_2
constexpr uint32_t sd_clock_period = FMC_SDRAM_CLOCK_PERIOD_2;

// #define REFRESH_COUNT ((uint32_t)0x0603)   // SDRAM refresh counter (100Mhz SD clock)
constexpr uint32_t refresh_count = 0x0603;

// #define SDRAM_TIMEOUT ((uint32_t)0xFFFF)
constexpr uint32_t timeout = 0xFFFF;

namespace status {
	// #define SDRAM_OK	((uint8_t)0x00)
	// #define SDRAM_ERROR ((uint8_t)0x01)
	constexpr uint8_t ok	= 0x00;
	constexpr uint8_t error = 0x01;

}	// namespace status

namespace mode {

	namespace burst {

		// #define SDRAM_MODEREG_BURST_LENGTH_1 ((uint16_t)0x0000)
		// #define SDRAM_MODEREG_BURST_LENGTH_2	((uint16_t)0x0001)
		// #define SDRAM_MODEREG_BURST_LENGTH_4	((uint16_t)0x0002)
		// #define SDRAM_MODEREG_BURST_LENGTH_8	((uint16_t)0x0004)
		constexpr uint16_t length1 = 0x0000;
		constexpr uint16_t length2 = 0x0001;
		constexpr uint16_t length4 = 0x0002;
		constexpr uint16_t length8 = 0x0004;

		namespace type {

			// #define SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL ((uint16_t)0x0000)
			// #define SDRAM_MODEREG_BURST_TYPE_INTERLEAVED	((uint16_t)0x0008)
			constexpr uint16_t sequential  = 0x0000;
			constexpr uint16_t interleaved = 0x0008;

		}	// namespace type

	}	// namespace burst

	namespace cas {

		// #define SDRAM_MODEREG_CAS_LATENCY_2 ((uint16_t)0x0020)
		// #define SDRAM_MODEREG_CAS_LATENCY_3 ((uint16_t)0x0030)
		constexpr uint16_t latency2 = 0x0020;
		constexpr uint16_t latency3 = 0x0030;

	}	// namespace cas

	namespace operating_mode {

		// #define SDRAM_MODEREG_OPERATING_MODE_STANDARD ((uint16_t)0x0000)
		constexpr uint16_t standard = 0x0000;

	}	// namespace operating_mode

	namespace writeburst_mode {

		// #define SDRAM_MODEREG_WRITEBURST_MODE_PROGRAMMED ((uint16_t)0x0000)
		// #define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE ((uint16_t)0x0200)
		constexpr uint16_t programmed = 0x0000;
		constexpr uint16_t single	  = 0x0200;

	}	// namespace writeburst_mode

}	// namespace mode

}	// namespace sdram

// ? SDRAM #define not used at the moment

// #define SDRAM_DEVICE_ADDR ((uint32_t)0xC0000000)
// #define SDRAM_DEVICE_SIZE ((uint32_t)0x1000000) // SDRAM device size in MBytes

// #define SDRAM_MEMORY_WIDTH FMC_SDRAM_MEM_BUS_WIDTH_8
// #define SDRAM_MEMORY_WIDTH FMC_SDRAM_MEM_BUS_WIDTH_16

// #define SDCLOCK_PERIOD FMC_SDRAM_CLOCK_PERIOD_3
