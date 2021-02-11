// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "dma2d.h"

namespace leka {

DMA2D_HandleTypeDef LKCoreDMA2D::hdma2d;

LKCoreDMA2D::LKCoreDMA2D() {}

void LKCoreDMA2D::initialize()
{
	/* Init DMA2D */
	/*##-1- Configure the DMA2D Mode, Color Mode and output offset #############*/
	hdma2d.Init.Mode		  = DMA2D_M2M_PFC;
	hdma2d.Init.ColorMode	  = DMA2D_OUTPUT_ARGB8888;
	hdma2d.Init.OutputOffset  = 0;
	hdma2d.Init.AlphaInverted = DMA2D_REGULAR_ALPHA; /* No Output Alpha Inversion*/
	hdma2d.Init.RedBlueSwap	  = DMA2D_RB_REGULAR;	 /* No Output Red & Blue swap */

	/*##-2- DMA2D Callbacks Configuration ######################################*/
	// hdma2d.XferCpltCallback	 = DMA2D_TransferCompleteCallback;
	// hdma2d.XferErrorCallback = DMA2D_TransferErrorCallback;

	/*##-3- Background Configuration ###########################################*/
	hdma2d.LayerCfg[0].AlphaMode	  = DMA2D_REPLACE_ALPHA;
	hdma2d.LayerCfg[0].InputAlpha	  = 0xFF;
	hdma2d.LayerCfg[0].InputColorMode = DMA2D_INPUT_ARGB8888;

	hdma2d.LayerCfg[0].InputOffset	 = 0;
	hdma2d.LayerCfg[0].RedBlueSwap	 = DMA2D_RB_REGULAR;	/* No Background Red/Blue swap */
	hdma2d.LayerCfg[0].AlphaInverted = DMA2D_REGULAR_ALPHA; /* No Background Alpha inversion */

	/*##-3- Foreground Configuration ###########################################*/
	hdma2d.LayerCfg[1].AlphaMode	  = DMA2D_REPLACE_ALPHA;
	hdma2d.LayerCfg[1].InputAlpha	  = 0xFF;
	hdma2d.LayerCfg[1].InputColorMode = DMA2D_INPUT_ARGB8888;

	hdma2d.LayerCfg[1].InputOffset	 = 0;
	hdma2d.LayerCfg[1].RedBlueSwap	 = DMA2D_RB_REGULAR;	/* No ForeGround Red/Blue swap */
	hdma2d.LayerCfg[1].AlphaInverted = DMA2D_REGULAR_ALPHA; /* No ForeGround Alpha inversion */

	hdma2d.Instance = DMA2D;

	/*##-4- DMA2D Initialization     ###########################################*/
	HAL_DMA2D_Init(&hdma2d);
	HAL_DMA2D_ConfigLayer(&hdma2d, 0);
	HAL_DMA2D_ConfigLayer(&hdma2d, 1);
}

void LKCoreDMA2D::load(Mode mode, uint32_t source_address_color, uint32_t destination_address, uint16_t xsize,
					   uint16_t ysize, uint16_t output_offset, uint32_t width_offset)
{
	hdma2d.Init.Mode			   = (uint32_t)mode;
	hdma2d.Init.OutputOffset	   = output_offset;
	hdma2d.LayerCfg[1].InputOffset = width_offset;

	if (HAL_DMA2D_Init(&hdma2d) == HAL_OK) {
		if (HAL_DMA2D_ConfigLayer(&hdma2d, 1) == HAL_OK) {
			if (HAL_DMA2D_Start(&hdma2d, source_address_color, destination_address, xsize, ysize) == HAL_OK) {
				HAL_DMA2D_PollForTransfer(&hdma2d, 100);
			}
		}
	}
}

}	// namespace leka
