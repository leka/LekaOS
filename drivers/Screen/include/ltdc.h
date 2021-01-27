// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_LTDC_H_
#define _LEKA_OS_LIB_LTDC_H_

#include "Screen.h"
#include "dsi.h"
// #include "stm32f7xx_hal.h"

namespace leka {

extern LTDC_HandleTypeDef hltdc;

void LTDCInit(LCD_Model lcd_model);

}	// namespace leka

#endif	 // _LEKA_OS_LIB_LTDC_H_
