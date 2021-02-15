// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_LTDC_H_
#define _LEKA_OS_LIB_LTDC_H_

#include "stm32f7xx_hal.h"

namespace leka {

class LKCoreLTDC
{
  public:
	LKCoreLTDC();

	void initialize(DSI_VidCfgTypeDef *dsi_video_config);

	void setupLayerConfig(void);
	void setupTimingConfig(void);
	void setupBackgroundConfig(void);

	void configureLayer(void);

  private:
	LTDC_HandleTypeDef _hltdc;
	LTDC_LayerCfgTypeDef _layerConfig;
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_LTDC_H_
