// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LKCoreVideo.h"

namespace leka {

LKCoreVideo::LKCoreVideo(LKCoreSTM32HalBase &hal, LKCoreDMA2DBase &coredma2d, LKCoreDSIBase &coredsi,
						 LKCoreLTDCBase &coreltdc, LKCoreLCDBase &corelcd, LKCoreGraphicsBase &coregraphics,
						 LKCoreFontBase &corefont, LKCoreJPEGBase &corejpeg)
	: _hal(hal),
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
	_coreltdc.configureLayer();

	_coredsi.start();

	BSP_SDRAM_Init();

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

void LKCoreVideo::clearScreen(Color color)
{
	_coregraphics.clearScreen(color);
}

void LKCoreVideo::displayRectangle(LKCoreGraphicsBase::FilledRectangle rectangle, Color color)
{
	_coregraphics.drawRectangle(rectangle, color);
}

void LKCoreVideo::displayImage(FIL *file)
{
	_corejpeg.displayImage(file);
}

void LKCoreVideo::displayText(char *text, uint16_t text_length, uint8_t starting_line, Color foreground,
							  Color background)
{
	_corefont.display(text, text_length, starting_line, foreground, background);
}

}	// namespace leka
