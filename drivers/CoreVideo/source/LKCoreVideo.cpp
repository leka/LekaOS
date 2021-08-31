// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LKCoreVideo.h"

namespace leka {

LKCoreVideo::LKCoreVideo(LKCoreSTM32HalBase &hal, interface::CoreSDRAM &coresdram, interface::CoreDMA2D &coredma2d,
						 interface::CoreDSI &coredsi, interface::CoreLTDC &coreltdc, interface::CoreLCD &corelcd,
						 interface::CoreGraphics &coregraphics, interface::CoreFont &corefont,
						 interface::CoreJPEG &corejpeg)
	: _hal(hal),
	  _coresdram(coresdram),
	  _coredma2d(coredma2d),
	  _coredsi(coredsi),
	  _coreltdc(coreltdc),
	  _corelcd(corelcd),
	  _coregraphics(coregraphics),
	  _corefont(corefont),
	  _corejpeg(corejpeg)
{
}

void LKCoreVideo::initialize(void)
{
	_coredsi.reset();

	/** @brief Enable the LTDC clock */
	_hal.HAL_RCC_LTDC_CLK_ENABLE();

	/** @brief Toggle Sw reset of LTDC IP */
	_hal.HAL_RCC_LTDC_FORCE_RESET();
	_hal.HAL_RCC_LTDC_RELEASE_RESET();

	/** @brief Enable the DMA2D clock */
	_hal.HAL_RCC_DMA2D_CLK_ENABLE();

	/** @brief Toggle Sw reset of DMA2D IP */
	_hal.HAL_RCC_DMA2D_FORCE_RESET();
	_hal.HAL_RCC_DMA2D_RELEASE_RESET();

	/** @brief Enable DSI Host and wrapper clocks */
	_hal.HAL_RCC_DSI_CLK_ENABLE();

	/** @brief Soft Reset the DSI Host and wrapper */
	_hal.HAL_RCC_DSI_FORCE_RESET();
	_hal.HAL_RCC_DSI_RELEASE_RESET();

	/** @brief NVIC configuration for LTDC interrupt that is now enabled */
	_hal.HAL_NVIC_SetPriority(LTDC_IRQn, 3, 0);
	_hal.HAL_NVIC_EnableIRQ(LTDC_IRQn);

	/** @brief NVIC configuration for DMA2D interrupt that is now enabled */
	_hal.HAL_NVIC_SetPriority(DMA2D_IRQn, 3, 0);
	_hal.HAL_NVIC_EnableIRQ(DMA2D_IRQn);

	/** @brief NVIC configuration for DSI interrupt that is now enabled */
	_hal.HAL_NVIC_SetPriority(DSI_IRQn, 3, 0);
	_hal.HAL_NVIC_EnableIRQ(DSI_IRQn);

	_coredsi.initialize();

	_coreltdc.initialize();

	_coredsi.start();

	_coresdram.initialize();

	_corelcd.initialize();
	_corejpeg.initialize();
	_coredma2d.initialize();

	_corelcd.setBrightness(0.5f);
}

void LKCoreVideo::turnOff()
{
	_corelcd.turnOff();
}

void LKCoreVideo::turnOn()
{
	_corelcd.turnOn();
}

void LKCoreVideo::setBrightness(float value)
{
	_corelcd.setBrightness(value);
}

void LKCoreVideo::clearScreen(CGColor color)
{
	_coregraphics.clearScreen(color);
}

void LKCoreVideo::displayRectangle(interface::CoreGraphics::FilledRectangle rectangle, CGColor color)
{
	_coregraphics.drawRectangle(rectangle, color);
}

void LKCoreVideo::displayImage(FIL *file)
{
	_corejpeg.displayImage(file);
}

void LKCoreVideo::displayText(const char *text, uint32_t size, uint32_t starting_line, CGColor foreground,
							  CGColor background)
{
	_corefont.display(text, size, starting_line, foreground, background);
}

}	// namespace leka
