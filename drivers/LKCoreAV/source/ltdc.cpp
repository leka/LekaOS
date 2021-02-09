// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "ltdc.h"

namespace leka {

LTDC_HandleTypeDef hltdc;

namespace ltdc {
	void init()
	{
		static RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

		/* Timing Configuration */
		hltdc.Init.HorizontalSync	  = (screen_property.HSA - 1);
		hltdc.Init.AccumulatedHBP	  = (screen_property.HSA + screen_property.HBP - 1);
		hltdc.Init.AccumulatedActiveW = (screen_dimension.width + screen_property.HSA + screen_property.HBP - 1);
		hltdc.Init.TotalWidth =
			(screen_dimension.width + screen_property.HSA + screen_property.HBP + screen_property.HFP - 1);

		/* Initialize the LCD pixel width and pixel height */
		hltdc.LayerCfg->ImageWidth	= screen_dimension.width;
		hltdc.LayerCfg->ImageHeight = screen_dimension.height;

		/** LCD clock configuration
		 * Note: The following values should not be changed as the PLLSAI is also used
		 *      to clock the USB FS
		 * PLLSAI_VCO Input = HSE_VALUE/PLL_M = 1 Mhz
		 * PLLSAI_VCO Output = PLLSAI_VCO Input * PLLSAIN = 384 Mhz
		 * PLLLCDCLK = PLLSAI_VCO Output/PLLSAIR = 384 MHz / 7 = 54.85 MHz
		 * LTDC clock frequency = PLLLCDCLK / LTDC_PLLSAI_DIVR_2 = 54.85 MHz / 2 = 27.429 MHz
		 */
		PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
		PeriphClkInitStruct.PLLSAI.PLLSAIN		 = 384;
		PeriphClkInitStruct.PLLSAI.PLLSAIR		 = 7;
		PeriphClkInitStruct.PLLSAIDivR			 = RCC_PLLSAIDIVR_2;
		HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

		/* Background value */
		hltdc.Init.Backcolor.Blue  = 0;
		hltdc.Init.Backcolor.Green = 0;
		hltdc.Init.Backcolor.Red   = 0;
		hltdc.Init.PCPolarity	   = LTDC_PCPOLARITY_IPC;
		hltdc.Instance			   = LTDC;

		/* Get LTDC Configuration from DSI Configuration */
		HAL_LTDC_StructInitFromVideoConfig(&(hltdc), &(leka::hdsivideo));

		/* Initialize the LTDC */
		HAL_LTDC_Init(&hltdc);
	}

	void layerInit()
	{
		LTDC_LayerCfgTypeDef Layercfg;

		/* Layer Init */
		Layercfg.WindowX0		 = 0;
		Layercfg.WindowX1		 = screen_dimension.width;
		Layercfg.WindowY0		 = 0;
		Layercfg.WindowY1		 = screen_dimension.height;
		Layercfg.PixelFormat	 = LTDC_PIXEL_FORMAT_ARGB8888;
		Layercfg.FBStartAdress	 = LCD_FRAME_BUFFER;   // Previously FB_Address given in parameter
		Layercfg.Alpha			 = 255;
		Layercfg.Alpha0			 = 0;
		Layercfg.Backcolor.Blue	 = 0;
		Layercfg.Backcolor.Green = 0;
		Layercfg.Backcolor.Red	 = 0;
		Layercfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
		Layercfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
		Layercfg.ImageWidth		 = screen_dimension.width;
		Layercfg.ImageHeight	 = screen_dimension.height;

		HAL_LTDC_ConfigLayer(&hltdc, &Layercfg, 1);

		// DrawProp[1].BackColor = LCD_COLOR_WHITE;
		// DrawProp[1].pFont	   = &Font24;
		// DrawProp[1].TextColor = LCD_COLOR_BLACK;
	}
}	// namespace ltdc
}	// namespace leka
