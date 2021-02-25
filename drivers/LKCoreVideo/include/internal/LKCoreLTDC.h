// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_LTDC_H_
#define _LEKA_OS_LIB_LTDC_H_

#include "LKCoreDSIBase.h"
#include "LKCoreSTM32HalBase.h"

namespace leka {

class LKCoreLTDC
{
  public:
	LKCoreLTDC(LKCoreSTM32HalBase &hal, LKCoreDSIBase &dsi);

	void initialize();

	void setupLayerConfig(void);
	void setupTimingConfig(void);
	void setupBackgroundConfig(void);

	void configureLayer(void);

	LTDC_HandleTypeDef getHandle(void);
	LTDC_LayerCfgTypeDef getLayerConfig(void);

  private:
	LKCoreSTM32HalBase &_hal;
	LKCoreDSIBase &_dsi;

	LTDC_HandleTypeDef _hltdc;
	LTDC_LayerCfgTypeDef _layerConfig;
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_LTDC_H_
