// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreDMA2D.hpp"

#include "internal/corevideo_config.h"

using namespace leka;

CoreDMA2D::CoreDMA2D(interface::STM32Hal &hal) : _hal(hal)
{
	// MARK: Configure DMA2D mode, color mode and output offset
	_hdma2d.Init.Mode		   = DMA2D_M2M_PFC;
	_hdma2d.Init.ColorMode	   = DMA2D_OUTPUT_ARGB8888;
	_hdma2d.Init.OutputOffset  = 0;
	_hdma2d.Init.AlphaInverted = DMA2D_REGULAR_ALPHA;	// No Output Alpha Inversion
	_hdma2d.Init.RedBlueSwap   = DMA2D_RB_REGULAR;		// No Output Red & Blue swap

	// MARK: Configure DMA2D background
	_hdma2d.LayerCfg[0].AlphaMode	   = DMA2D_REPLACE_ALPHA;
	_hdma2d.LayerCfg[0].InputAlpha	   = 0xFF;
	_hdma2d.LayerCfg[0].InputColorMode = DMA2D_INPUT_ARGB8888;

	_hdma2d.LayerCfg[0].InputOffset	  = 0;
	_hdma2d.LayerCfg[0].RedBlueSwap	  = DMA2D_RB_REGULAR;	   // No Background Red/Blue swap
	_hdma2d.LayerCfg[0].AlphaInverted = DMA2D_REGULAR_ALPHA;   // No Background Alpha inversion

	// MARK: Configure DMA2D foreground
	_hdma2d.LayerCfg[1].AlphaMode	   = DMA2D_REPLACE_ALPHA;
	_hdma2d.LayerCfg[1].InputAlpha	   = 0xFF;
	_hdma2d.LayerCfg[1].InputColorMode = DMA2D_INPUT_ARGB8888;

	_hdma2d.LayerCfg[1].InputOffset	  = 0;
	_hdma2d.LayerCfg[1].RedBlueSwap	  = DMA2D_RB_REGULAR;	   // No ForeGround Red/Blue swap
	_hdma2d.LayerCfg[1].AlphaInverted = DMA2D_REGULAR_ALPHA;   // No ForeGround Alpha inversion

	// MARK: Set DMA2D instance
	_hdma2d.Instance = DMA2D;
}

void CoreDMA2D::initialize()
{
	/** @brief Enable the DMA2D clock */
	_hal.HAL_RCC_DMA2D_CLK_ENABLE();

	/** @brief Toggle Sw reset of DMA2D IP */
	_hal.HAL_RCC_DMA2D_FORCE_RESET();
	_hal.HAL_RCC_DMA2D_RELEASE_RESET();

	/** @brief NVIC configuration for DMA2D interrupt that is now enabled */
	_hal.HAL_NVIC_SetPriority(DMA2D_IRQn, 3, 0);
	_hal.HAL_NVIC_EnableIRQ(DMA2D_IRQn);

	// MARK: Initializer DMA2D
	// This part **must not** be moved to the constructor as LCD
	// initialization must be performed in a very specific order
	_hal.HAL_DMA2D_Init(&_hdma2d);
	_hal.HAL_DMA2D_ConfigLayer(&_hdma2d, 0);
	_hal.HAL_DMA2D_ConfigLayer(&_hdma2d, 1);
}

void CoreDMA2D::transferData(uintptr_t input, uintptr_t output, uint32_t width, uint32_t height)
{
	auto isNotReady = [this] { return _hdma2d.State != HAL_DMA2D_STATE_READY; };
	while (isNotReady())
		;

	// TODO(@yann): Check if init and config are needed everytime
	auto is_initialized = [&] { return _hal.HAL_DMA2D_Init(&_hdma2d) == HAL_OK; };
	auto is_configured	= [&] { return _hal.HAL_DMA2D_ConfigLayer(&_hdma2d, 1) == HAL_OK; };

	if (is_initialized() && is_configured()) {
		_hal.HAL_DMA2D_Start_IT(&_hdma2d, input, output, width, height);
	}
}

void CoreDMA2D::transferImage(uint32_t width, uint32_t height, uint32_t width_offset)
{
	_hdma2d.Init.Mode				= DMA2D_M2M_PFC;
	_hdma2d.LayerCfg[1].InputOffset = width_offset;
	_hdma2d.Init.OutputOffset		= lcd::dimension::width - width;

	auto memory_offset_to_center = [](auto image_width, auto image_height) {
		auto unoccupied_width  = lcd::dimension::width - image_width;
		auto unoccupied_height = lcd::dimension::height - image_height;

		return lcd::property::pixel_memory_size *
			   (unoccupied_width / 2 + unoccupied_height / 2 * lcd::dimension::width);
	};

	transferData(jpeg::decoded_buffer_address, lcd::frame_buffer_address + memory_offset_to_center(width, height),
				 width, height);
}

auto CoreDMA2D::getHandle() -> DMA2D_HandleTypeDef &
{
	return _hdma2d;
}

void CoreDMA2D::transferDrawing(uintptr_t first_pixel_address, uint32_t width, uint32_t height, uint32_t color)
{
	_hdma2d.Init.Mode		  = DMA2D_R2M;
	_hdma2d.Init.OutputOffset = lcd::dimension::width - width;

	transferData(color, first_pixel_address, width, height);
}
