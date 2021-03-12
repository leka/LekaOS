// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_LTDC_H_
#define _LEKA_OS_LIB_LTDC_H_

#include "LKCoreDSIBase.h"
#include "LKCoreLTDCBase.h"
#include "LKCoreSTM32HalBase.h"

namespace leka {

class LKCoreLTDC : public LKCoreLTDCBase
{
  public:
	LKCoreLTDC(LKCoreSTM32HalBase &hal, LKCoreDSIBase &dsi);

	void initialize() final;

	void setupLayerConfig(void) final;
	void setupTimingConfig(void) final;
	void setupBackgroundConfig(void) final;

	void configureLayer(void) final;

	LTDC_HandleTypeDef getHandle(void) final;
	LTDC_LayerCfgTypeDef getLayerConfig(void) final;

  private:
	LKCoreSTM32HalBase &_hal;
	LKCoreDSIBase &_dsi;

	LTDC_HandleTypeDef _hltdc;
	LTDC_LayerCfgTypeDef _layerConfig;
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_LTDC_H_
