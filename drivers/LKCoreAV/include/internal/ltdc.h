// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_LTDC_H_
#define _LEKA_OS_LIB_LTDC_H_

#include "lcd_properties.h"
#include "stm32f7xx_hal.h"

namespace leka {

class LKLTDC
{
  public:
	LKLTDC();

	void initialize(DSI_VidCfgTypeDef hdsivideo);
	void initializeLayer();

  private:
	LTDC_HandleTypeDef hltdc;
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_LTDC_H_
