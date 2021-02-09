// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LKCoreAV.h"

namespace leka {

using namespace mbed;

LKCoreAV::LKCoreAV(PinName backlight) : _screen(backlight) {}

uint16_t LKCoreAV::getWidth()
{
	return screen_dimension.width;
}
uint16_t LKCoreAV::getHeight()
{
	return screen_dimension.height;
}

void LKCoreAV::setBrightness(float value)
{
	_screen.setBrightness(value);
}

void LKCoreAV::turnOff()
{
	_screen.turnOff();
}

void LKCoreAV::turnOn()
{
	_screen.turnOn();
}

void LKCoreAV::rotateUpsideDown(bool upside_down)
{
	_screen.rotateUpsideDown(upside_down);
}

void LKCoreAV::Init()
{
	leka::dsi::reset();
	MSPInit();
	DSIInit();
	LTDCInit();
	leka::dsi::start();
	SDRAMInit();
	LCDDriverInit();
	JPEGCodecInit();
	DMA2DInit();

	rotateUpsideDown(false);
}

void LKCoreAV::MSPInit()
{
	// MSP : MCU Support Package. https://stackoverflow.com/a/37520805

	/** @brief Enable the LTDC clock */
	__HAL_RCC_LTDC_CLK_ENABLE();

	/** @brief Toggle Sw reset of LTDC IP */
	__HAL_RCC_LTDC_FORCE_RESET();
	__HAL_RCC_LTDC_RELEASE_RESET();

	/** @brief Enable the DMA2D clock */
	__HAL_RCC_DMA2D_CLK_ENABLE();

	/** @brief Toggle Sw reset of DMA2D IP */
	__HAL_RCC_DMA2D_FORCE_RESET();
	__HAL_RCC_DMA2D_RELEASE_RESET();

	/** @brief Enable DSI Host and wrapper clocks */
	__HAL_RCC_DSI_CLK_ENABLE();

	/** @brief Soft Reset the DSI Host and wrapper */
	__HAL_RCC_DSI_FORCE_RESET();
	__HAL_RCC_DSI_RELEASE_RESET();

	/** @brief NVIC configuration for LTDC interrupt that is now enabled */
	HAL_NVIC_SetPriority(LTDC_IRQn, 3, 0);
	HAL_NVIC_EnableIRQ(LTDC_IRQn);

	/** @brief NVIC configuration for DMA2D interrupt that is now enabled */
	HAL_NVIC_SetPriority(DMA2D_IRQn, 3, 0);
	HAL_NVIC_EnableIRQ(DMA2D_IRQn);

	/** @brief NVIC configuration for DSI interrupt that is now enabled */
	HAL_NVIC_SetPriority(DSI_IRQn, 3, 0);
	HAL_NVIC_EnableIRQ(DSI_IRQn);

	leka::jpeg::mspInit();
}

void LKCoreAV::MSPDeInit()
{
	/** @brief Disable IRQ of LTDC IP */
	HAL_NVIC_DisableIRQ(LTDC_IRQn);

	/** @brief Disable IRQ of DMA2D IP */
	HAL_NVIC_DisableIRQ(DMA2D_IRQn);

	/** @brief Disable IRQ of DSI IP */
	HAL_NVIC_DisableIRQ(DSI_IRQn);

	/** @brief Force and let in reset state LTDC, DMA2D and DSI Host + Wrapper IPs */
	__HAL_RCC_LTDC_FORCE_RESET();
	__HAL_RCC_DMA2D_FORCE_RESET();
	__HAL_RCC_DSI_FORCE_RESET();

	/** @brief Disable the LTDC, DMA2D and DSI Host and Wrapper clocks */
	__HAL_RCC_LTDC_CLK_DISABLE();
	__HAL_RCC_DMA2D_CLK_DISABLE();
	__HAL_RCC_DSI_CLK_DISABLE();
}

void LKCoreAV::DSIInit()
{
	leka::dsi::init();
}

void LKCoreAV::LTDCInit()
{
	leka::ltdc::init();
	leka::ltdc::layerInit();
}

void LKCoreAV::SDRAMInit()
{
	BSP_SDRAM_Init();
}

void LKCoreAV::LCDDriverInit()
{
	_screen.initializeDriver();
}

void LKCoreAV::JPEGCodecInit()
{
	leka::jpeg::init();
}

void LKCoreAV::DMA2DInit()
{
	leka::dma2d::init(screen_dimension.width);
}

}	// namespace leka
