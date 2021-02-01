// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_OTM8009A_CONF_H_
#define _LEKA_OS_OTM8009A_CONF_H_

#include "LKScreen.h"
#include "dsi.h"
#include "otm8009a.h"
#include "stm32f7xx_hal.h"

void otm8009RotateUpsideDown(bool upside_down);

extern leka::LCD_Model otm8009a_model;

#endif	 // _LEKA_OS_DRIVER_SCREEN_H_
