// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LKCoreDMA2D.h"

#include "corevideo_config.h"

namespace leka {

LKCoreDMA2D::LKCoreDMA2D(LKCoreSTM32HalBase &hal) : _hal(hal)
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

void LKCoreDMA2D::initialize()
{
	// MARK: Initializer DMA2D
	// This part **must not** be moved to the constructor as LCD
	// initialization must be performed in a very specific order
	_hal.HAL_DMA2D_Init(&_hdma2d);
	_hal.HAL_DMA2D_ConfigLayer(&_hdma2d, 0);
	_hal.HAL_DMA2D_ConfigLayer(&_hdma2d, 1);
}

void LKCoreDMA2D::transferData(uintptr_t input, uintptr_t output, uint32_t width, uint32_t height)
{
	// TODO: Check if init and config are needed everytime
	auto init	= [&] { return _hal.HAL_DMA2D_Init(&_hdma2d) == HAL_OK; };
	auto config = [&] { return _hal.HAL_DMA2D_ConfigLayer(&_hdma2d, 1) == HAL_OK; };
	auto start	= [&] { return _hal.HAL_DMA2D_Start(&_hdma2d, input, output, width, height) == HAL_OK; };

	if (init() && config() && start()) {
		_hal.HAL_DMA2D_PollForTransfer(&_hdma2d, 100);
	}
}

void LKCoreDMA2D::transferImage(uint32_t width, uint32_t height, uint32_t width_offset)
{
	_hdma2d.Init.Mode				= DMA2D_M2M_PFC;
	_hdma2d.LayerCfg[1].InputOffset = width_offset;
	_hdma2d.Init.OutputOffset		= lcd::dimension.width-width;   // TODO: Check if needed

	transferData(jpeg::decoded_buffer_address, lcd::frame_buffer_address, width, height);
}

DMA2D_HandleTypeDef LKCoreDMA2D::getHandle(void)
{
	return _hdma2d;
}

void LKCoreDMA2D::transferDrawing(uintptr_t first_pixel_address, uint32_t width, uint32_t height, uint32_t color)
{
	_hdma2d.Init.Mode		  = DMA2D_R2M;
	_hdma2d.Init.OutputOffset = lcd::dimension.width - width;

	transferData(color, first_pixel_address, width, height);
}

}	// namespace leka
