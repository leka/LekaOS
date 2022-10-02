// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreLTDC.hpp"

#include "internal/corevideo_config.h"

using namespace leka;

CoreLTDC::CoreLTDC(interface::STM32Hal &hal) : _hal(hal)
{
	_hltdc.Instance = LTDC;

	// LCD pixel width/height
	_hltdc.LayerCfg->ImageWidth	 = lcd::dimension::width;
	_hltdc.LayerCfg->ImageHeight = lcd::dimension::height;

	// Timing and synchronization
	_hltdc.Init.HorizontalSync	   = (lcd::property::HSA - 1);
	_hltdc.Init.AccumulatedHBP	   = (lcd::property::HSA + lcd::property::HBP - 1);
	_hltdc.Init.AccumulatedActiveW = (lcd::dimension::width + lcd::property::HSA + lcd::property::HBP - 1);
	_hltdc.Init.TotalWidth = (lcd::dimension::width + lcd::property::HSA + lcd::property::HBP + lcd::property::HFP - 1);

	_hltdc.Init.VerticalSync	   = (lcd::property::VSA - 1);
	_hltdc.Init.AccumulatedVBP	   = (lcd::property::VSA + lcd::property::VBP - 1);
	_hltdc.Init.AccumulatedActiveH = (lcd::dimension::height + lcd::property::VSA + lcd::property::VBP - 1);
	_hltdc.Init.TotalHeigh =
		(lcd::dimension::height + lcd::property::VSA + lcd::property::VBP + lcd::property::VFP - 1);

	// Background values
	_hltdc.Init.Backcolor.Blue	= 0;
	_hltdc.Init.Backcolor.Green = 0;
	_hltdc.Init.Backcolor.Red	= 0;

	// Polarity
	_hltdc.Init.HSPolarity = LTDC_HSPOLARITY_AH;
	_hltdc.Init.VSPolarity = LTDC_VSPOLARITY_AH;
	_hltdc.Init.DEPolarity = LTDC_DEPOLARITY_AH;
	_hltdc.Init.PCPolarity = LTDC_PCPOLARITY_IPC;

	// Layer config
	_layerConfig.WindowX0 = 0;
	_layerConfig.WindowX1 = lcd::dimension::width;
	_layerConfig.WindowY0 = 0;
	_layerConfig.WindowY1 = lcd::dimension::height;

	_layerConfig.PixelFormat = LTDC_PIXEL_FORMAT_ARGB8888;

	_layerConfig.FBStartAdress = lcd::frame_buffer_address;

	_layerConfig.Alpha	= 255;
	_layerConfig.Alpha0 = 0;

	_layerConfig.Backcolor.Blue	 = 0;
	_layerConfig.Backcolor.Green = 0;
	_layerConfig.Backcolor.Red	 = 0;

	_layerConfig.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
	_layerConfig.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;

	_layerConfig.ImageWidth	 = lcd::dimension::width;
	_layerConfig.ImageHeight = lcd::dimension::height;
}

void CoreLTDC::initialize()
{
	/** @brief NVIC configuration for LTDC interrupt that is now enabled */
	_hal.HAL_NVIC_SetPriority(LTDC_IRQn, 3, 0);
	_hal.HAL_NVIC_EnableIRQ(LTDC_IRQn);

	configurePeriphClock();

	// Initialize LTDC
	// This part **must not** be moved to the constructor as LCD
	// initialization must be performed in a very specific order
	_hal.HAL_LTDC_Init(&_hltdc);

	// Initialize LTDC layer
	// This part **must not** be moved to the constructor as LCD
	// initialization must be performed in a very specific order
	_hal.HAL_LTDC_ConfigLayer(&_hltdc, &_layerConfig, 1);
}

void CoreLTDC::configurePeriphClock()
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

auto CoreLTDC::getHandle() -> LTDC_HandleTypeDef &
{
	return _hltdc;
}

auto CoreLTDC::getLayerConfig() const -> LTDC_LayerCfgTypeDef
{
	return _layerConfig;
}
