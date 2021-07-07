// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LKCoreLTDC.h"

#include "corevideo_config.h"

namespace leka {

LKCoreLTDC::LKCoreLTDC(LKCoreSTM32HalBase &hal, LKCoreDSIBase &dsi) : _hal(hal), _dsi(dsi)
{
	_hltdc.Instance = LTDC;

	auto props = dsi.getSyncProps();
	// Timing and synchronization
	_hltdc.Init.HorizontalSync	   = props.hsync;
	_hltdc.Init.AccumulatedHBP	   = props.hsync + props.hbp;
	_hltdc.Init.AccumulatedActiveW = props.hsync + props.hbp + props.activew;
	_hltdc.Init.TotalWidth = props.hsync + props.hbp + props.activew + props.hfp;

	_hltdc.Init.VerticalSync	   = props.vsync;
	_hltdc.Init.AccumulatedVBP	   = props.vsync + props.vbp;
	_hltdc.Init.AccumulatedActiveH = props.vsync + props.vbp + props.activeh;
	_hltdc.Init.TotalHeight = props.vsync + props.vbp + props.activeh + props.vfp;

	// Background values
	_hltdc.Init.Backcolor.Blue	= 0;
	_hltdc.Init.Backcolor.Green = 0;
	_hltdc.Init.Backcolor.Red	= 0;

	// Misc
	_hltdc.Init.HSPolarity = LTDC_HSPOLARITY_AL;
	_hltdc.Init.VSPolarity = LTDC_VSPOLARITY_AL;
	_hltdc.Init.DEPolarity = LTDC_DEPOLARITY_AL;
	_hltdc.Init.PCPolarity = LTDC_PCPOLARITY_IPC;

	// Layer config
	_layerConfig.WindowX0 = 0;
	_layerConfig.WindowX1 = props.activew;
	_layerConfig.WindowY0 = 0;
	_layerConfig.WindowY1 = lcd::dimension.height;

	_layerConfig.PixelFormat = LTDC_PIXEL_FORMAT_ARGB8888;

	_layerConfig.FBStartAdress = lcd::frame_buffer_address;

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

void LKCoreLTDC::initialize()
{
	configurePeriphClock();

	// Get LTDC config from DSI
	DSI_VidCfgTypeDef dsi_video_config;// = _dsi.getConfig();
	_hal.HAL_LTDC_StructInitFromVideoConfig(&_hltdc, &dsi_video_config);

	// Initialize LTDC
	// This part **must not** be moved to the constructor as LCD
	// initialization must be performed in a very specific order
	_hal.HAL_LTDC_Init(&_hltdc);

	// Initialize LTDC layer
	// This part **must not** be moved to the constructor as LCD
	// initialization must be performed in a very specific order
	_hal.HAL_LTDC_ConfigLayer(&_hltdc, &_layerConfig, 1);
}

void LKCoreLTDC::configurePeriphClock()
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

	_hal.HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
}

LTDC_HandleTypeDef LKCoreLTDC::getHandle() const
{
	return _hltdc;
}

LTDC_LayerCfgTypeDef LKCoreLTDC::getLayerConfig() const
{
	return _layerConfig;
}

}	// namespace leka
