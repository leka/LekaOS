// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreDSI.hpp"

#include "rtos/ThisThread.h"

#include "internal/corevideo_config.h"

using namespace leka;
using namespace std::chrono_literals;

CoreDSI::CoreDSI(interface::STM32Hal &hal) : _hal(hal)
{
	// Base address of DSI Host/Wrapper registers to be set before calling De-Init
	_hdsi.Instance = DSI;

	// Set number of Lanes
	_hdsi.Init.NumberOfLanes = DSI_TWO_DATA_LANES;

	_hdsi.Init.TXEscapeCkdiv = dsi::txEscapeClockDiv;

	_config.VirtualChannelID = 0;
	_config.ColorCoding		 = DSI_RGB888;
	_config.VSPolarity		 = DSI_VSYNC_ACTIVE_HIGH;
	_config.HSPolarity		 = DSI_HSYNC_ACTIVE_HIGH;
	_config.DEPolarity		 = DSI_DATA_ENABLE_ACTIVE_HIGH;
	_config.Mode			 = DSI_VID_MODE_BURST;	 // Mode Video burst ie : one LgP per line
	_config.NullPacketSize	 = 0xFFF;
	_config.NumberOfChunks	 = 0;
	_config.PacketSize = lcd::property::HACT;	// Value depending on display orientation choice portrait/landscape
	_config.HorizontalSyncActive = (lcd::property::HSA * dsi::laneByteClock_kHz) / dsi::lcdClock;
	_config.HorizontalBackPorch	 = (lcd::property::HBP * dsi::laneByteClock_kHz) / dsi::lcdClock;
	_config.HorizontalLine = ((lcd::property::HACT + lcd::property::HSA + lcd::property::HBP + lcd::property::HFP) *
							  dsi::laneByteClock_kHz) /
							 dsi::lcdClock;	  // Value depending on display orientation choice portrait/landscape
	_config.VerticalSyncActive = lcd::property::VSA;
	_config.VerticalBackPorch  = lcd::property::VBP;
	_config.VerticalFrontPorch = lcd::property::VFP;
	_config.VerticalActive = lcd::property::VACT;	// Value depending on display orientation choice portrait/landscape

	// Enable or disable sending LP command while streaming is active in video mode
	_config.LPCommandEnable = DSI_LP_COMMAND_ENABLE;   // Enable sending commands in mode LP (Low Power)

	// Largest packet size possible to transmit in LP mode in VSA, VBP, VFP regions
	// Only useful when sending LP packets is allowed while streaming is active in video mode
	_config.LPLargestPacketSize = 16;

	// Largest packet size possible to transmit in LP mode in HFP region during VACT period
	// Only useful when sending LP packets is allowed while streaming is active in video mode
	_config.LPVACTLargestPacketSize = 0;

	// Specify for each region of the video frame, if the transmission of command in LP mode is allowed in this region
	// while streaming is active in video mode
	_config.LPHorizontalFrontPorchEnable = DSI_LP_HFP_ENABLE;	  // Allow sending LP commands during HFP period
	_config.LPHorizontalBackPorchEnable	 = DSI_LP_HBP_ENABLE;	  // Allow sending LP commands during HBP period
	_config.LPVerticalActiveEnable		 = DSI_LP_VACT_ENABLE;	  // Allow sending LP commands during VACT period
	_config.LPVerticalFrontPorchEnable	 = DSI_LP_VFP_ENABLE;	  // Allow sending LP commands during VFP period
	_config.LPVerticalBackPorchEnable	 = DSI_LP_VBP_ENABLE;	  // Allow sending LP commands during VBP period
	_config.LPVerticalSyncActiveEnable	 = DSI_LP_VSYNC_ENABLE;	  // Allow sending LP commands during VSync = VSA period
}

void CoreDSI::initialize()
{
	/** @brief Enable DSI Host and wrapper clocks */
	_hal.HAL_RCC_DSI_CLK_ENABLE();

	/** @brief Soft Reset the DSI Host and wrapper */
	_hal.HAL_RCC_DSI_FORCE_RESET();
	_hal.HAL_RCC_DSI_RELEASE_RESET();

	/** @brief NVIC configuration for DSI interrupt that is now enabled */
	_hal.HAL_NVIC_SetPriority(DSI_IRQn, 3, 0);
	_hal.HAL_NVIC_EnableIRQ(DSI_IRQn);

	reset();

	_hal.HAL_DSI_DeInit(&_hdsi);

	DSI_PLLInitTypeDef dsiPllInit;

	dsiPllInit.PLLNDIV = 100;
	dsiPllInit.PLLIDF  = DSI_PLL_IN_DIV5;
	dsiPllInit.PLLODF  = DSI_PLL_OUT_DIV1;

	// Initialize DSI
	// DO NOT MOVE to the constructor as LCD initialization
	// must be performed in a very specific order
	_hal.HAL_DSI_Init(&_hdsi, &dsiPllInit);

	// Configure DSI Video mode timings
	_hal.HAL_DSI_ConfigVideoMode(&_hdsi, &_config);
}

void CoreDSI::start()
{
	_hal.HAL_DSI_Start(&_hdsi);
}

void CoreDSI::reset()
{
	// Reset DSI configuration
	// DO NOT CHANGE this function

	GPIO_InitTypeDef gpio_init_structure;

	_hal.HAL_RCC_GPIOJ_CLK_ENABLE();

	// Configure the GPIO on PJ15 (MIPI_DSI_RESET)
	gpio_init_structure.Pin	  = GPIO_PIN_15;
	gpio_init_structure.Mode  = GPIO_MODE_OUTPUT_PP;
	gpio_init_structure.Pull  = GPIO_PULLUP;
	gpio_init_structure.Speed = GPIO_SPEED_HIGH;

	_hal.HAL_GPIO_Init(GPIOJ, &gpio_init_structure);

	// Activate MIPI_DSI_RESET active low
	_hal.HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_15, GPIO_PIN_RESET);

	rtos::ThisThread::sleep_for(20ms);

	// Desactivate MIPI_DSI_RESET
	_hal.HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_15, GPIO_PIN_SET);

	// Wait for 10ms after releasing MIPI_DSI_RESET before sending commands
	rtos::ThisThread::sleep_for(10ms);
}

auto CoreDSI::getHandle() const -> DSI_HandleTypeDef
{
	return _hdsi;
}

auto CoreDSI::getConfig() -> DSI_VidCfgTypeDef
{
	return _config;
}

void CoreDSI::write(const uint8_t *data, const uint32_t size)
{
	if (size <= 2) {
		_hal.HAL_DSI_ShortWrite(&_hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, data[0], data[1]);
	} else {
		_hal.HAL_DSI_LongWrite(&_hdsi, 0, DSI_DCS_LONG_PKT_WRITE, size, data[size - 1], const_cast<uint8_t *>(data));
	}
}

void CoreDSI::write(std::span<const uint8_t> data)
{
	if (data.size() <= 2) {
		_hal.HAL_DSI_ShortWrite(&_hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, data[0], data[1]);
	} else {
		_hal.HAL_DSI_LongWrite(&_hdsi, 0, DSI_DCS_LONG_PKT_WRITE, data.size(), data.back(),
							   const_cast<uint8_t *>(data.data()));
	}
}
