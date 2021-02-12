// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LKCoreDMA2D.h"

namespace leka {

LKCoreDMA2D::LKCoreDMA2D()
{
	// MARK: Configure DMA2D mode, color mode and output offset
	_hdma2d.Init.Mode		   = DMA2D_M2M_PFC;
	_hdma2d.Init.ColorMode	   = DMA2D_OUTPUT_ARGB8888;
	_hdma2d.Init.OutputOffset  = 0;
	_hdma2d.Init.AlphaInverted = DMA2D_REGULAR_ALPHA; /* No Output Alpha Inversion*/
	_hdma2d.Init.RedBlueSwap   = DMA2D_RB_REGULAR;	  /* No Output Red & Blue swap */

	// MARK: Configure DMA2D background
	_hdma2d.LayerCfg[0].AlphaMode	   = DMA2D_REPLACE_ALPHA;
	_hdma2d.LayerCfg[0].InputAlpha	   = 0xFF;
	_hdma2d.LayerCfg[0].InputColorMode = DMA2D_INPUT_ARGB8888;

	_hdma2d.LayerCfg[0].InputOffset	  = 0;
	_hdma2d.LayerCfg[0].RedBlueSwap	  = DMA2D_RB_REGULAR;	 /* No Background Red/Blue swap */
	_hdma2d.LayerCfg[0].AlphaInverted = DMA2D_REGULAR_ALPHA; /* No Background Alpha inversion */

	// MARK: Configure DMA2D foreground
	_hdma2d.LayerCfg[1].AlphaMode	   = DMA2D_REPLACE_ALPHA;
	_hdma2d.LayerCfg[1].InputAlpha	   = 0xFF;
	_hdma2d.LayerCfg[1].InputColorMode = DMA2D_INPUT_ARGB8888;

	_hdma2d.LayerCfg[1].InputOffset	  = 0;
	_hdma2d.LayerCfg[1].RedBlueSwap	  = DMA2D_RB_REGULAR;	 /* No ForeGround Red/Blue swap */
	_hdma2d.LayerCfg[1].AlphaInverted = DMA2D_REGULAR_ALPHA; /* No ForeGround Alpha inversion */

	// MARK: Set DMA2D instance
	_hdma2d.Instance = DMA2D;
}

void LKCoreDMA2D::initialize()
{
	// MARK: Initializer DMA2D
	HAL_DMA2D_Init(&_hdma2d);
	HAL_DMA2D_ConfigLayer(&_hdma2d, 0);
	HAL_DMA2D_ConfigLayer(&_hdma2d, 1);
}

void LKCoreDMA2D::load(Mode mode, uint32_t pdata_or_color, uint32_t destination_address, uint16_t xsize, uint16_t ysize,
					   uint16_t output_offset, uint32_t width_offset)
{
	switch (mode) {
		case Mode::R2M:
			_hdma2d.Init.Mode = DMA2D_R2M;
			break;
		case Mode::M2M_PFC:
			_hdma2d.Init.Mode = DMA2D_M2M_PFC;
			break;
		default:
			return;
	}
	_hdma2d.Init.OutputOffset		= output_offset;
	_hdma2d.LayerCfg[1].InputOffset = width_offset;

	if (HAL_DMA2D_Init(&_hdma2d) == HAL_OK) {
		if (HAL_DMA2D_ConfigLayer(&_hdma2d, 1) == HAL_OK) {
			if (HAL_DMA2D_Start(&_hdma2d, pdata_or_color, destination_address, xsize, ysize) == HAL_OK) {
				HAL_DMA2D_PollForTransfer(&_hdma2d, 100);
			}
		}
	}
}

}	// namespace leka
