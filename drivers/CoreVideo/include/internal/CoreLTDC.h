// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_LTDC_H_
#define _LEKA_OS_LIB_LTDC_H_

#include "LKCoreDSIBase.h"
#include "LKCoreLTDCBase.h"
#include "LKCoreSTM32HalBase.h"

namespace leka {

class CoreLTDC : public LKCoreLTDCBase
{
  public:
	CoreLTDC(LKCoreSTM32HalBase &hal, LKCoreDSIBase &dsi);

	void initialize(void) final;

	LTDC_HandleTypeDef getHandle(void) const;
	LTDC_LayerCfgTypeDef getLayerConfig(void) const;

  private:
	LKCoreSTM32HalBase &_hal;
	LKCoreDSIBase &_dsi;

	LTDC_HandleTypeDef _hltdc;
	LTDC_LayerCfgTypeDef _layerConfig;

	void configurePeriphClock(void);
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_LTDC_H_
