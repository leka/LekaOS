// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LKCoreLTDC.h"

#include "lcd_properties.h"

namespace leka {

LKCoreLTDC::LKCoreLTDC()
{
	_hltdc.Instance = LTDC;

	// MARK: Initialize the LCD pixel width and pixel height
	_hltdc.LayerCfg->ImageWidth	 = lcd::dimension.width;
	_hltdc.LayerCfg->ImageHeight = lcd::dimension.height;

	setupLayerConfig();
	setupTimingConfig();
	setupBackgroundConfig();
}

void LKCoreLTDC::setupTimingConfig()
{
	// MARK: Timing and synchronization
	_hltdc.Init.HorizontalSync	   = (lcd::property.HSA - 1);
	_hltdc.Init.AccumulatedHBP	   = (lcd::property.HSA + lcd::property.HBP - 1);
	_hltdc.Init.AccumulatedActiveW = (lcd::dimension.width + lcd::property.HSA + lcd::property.HBP - 1);
	_hltdc.Init.TotalWidth = (lcd::dimension.width + lcd::property.HSA + lcd::property.HBP + lcd::property.HFP - 1);
}

void LKCoreLTDC::setupBackgroundConfig()
{
	// MARK: Background values
	_hltdc.Init.Backcolor.Blue	= 0;
	_hltdc.Init.Backcolor.Green = 0;
	_hltdc.Init.Backcolor.Red	= 0;
	_hltdc.Init.PCPolarity		= LTDC_PCPOLARITY_IPC;
}

void LKCoreLTDC::setupLayerConfig()
{
	// MARK: Layer
	_layerConfig.WindowX0 = 0;
	_layerConfig.WindowX1 = lcd::dimension.width;
	_layerConfig.WindowY0 = 0;
	_layerConfig.WindowY1 = lcd::dimension.height;

	_layerConfig.PixelFormat = LTDC_PIXEL_FORMAT_ARGB8888;

	_layerConfig.FBStartAdress = lcd::frame_buffer_address;	  // Previously FB_Address given in parameter

	_layerConfig.Alpha	= 255;
	_layerConfig.Alpha0 = 0;

	_layerConfig.Backcolor.Blue	 = 0;
	_layerConfig.Backcolor.Green = 0;
	_layerConfig.Backcolor.Red	 = 0;

	_layerConfig.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
	_layerConfig.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;

	_layerConfig.ImageWidth	 = lcd::dimension.width;
	_layerConfig.ImageHeight = lcd::dimension.height;
}

void LKCoreLTDC::initialize(DSI_VidCfgTypeDef *dsi_video_config)
{
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

	// LCD clock configuration
	// PLLSAI_VCO Input = HSE_VALUE/PLL_M = 1 Mhz
	// PLLSAI_VCO Output = PLLSAI_VCO Input * PLLSAIN = 384 Mhz
	// PLLLCDCLK = PLLSAI_VCO Output/PLLSAIR = 384 MHz / 7 = 54.85 MHz
	// LTDC clock frequency = PLLLCDCLK / LTDC_PLLSAI_DIVR_2 = 54.85 MHz / 2 = 27.429 MHz

	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;

	PeriphClkInitStruct.PLLSAI.PLLSAIN = 384;
	PeriphClkInitStruct.PLLSAI.PLLSAIR = 7;
	PeriphClkInitStruct.PLLSAIDivR	   = RCC_PLLSAIDIVR_2;

	HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

	// MARK: Get LTDC Configuration from DSI Configuration
	HAL_LTDC_StructInitFromVideoConfig(&_hltdc, dsi_video_config);

	// MARK: Initializer LTDC
	// This part **must not** be moved to the constructor as LCD
	// initialization must be performed in a very specific order
	HAL_LTDC_Init(&_hltdc);
}

void LKCoreLTDC::configureLayer()
{
	// MARK: Initializer LTDC layer
	// This part **must not** be moved to the constructor as LCD
	// initialization must be performed in a very specific order
	HAL_LTDC_ConfigLayer(&_hltdc, &_layerConfig, 1);
}

}	// namespace leka
