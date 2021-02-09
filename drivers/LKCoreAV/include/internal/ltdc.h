// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_LTDC_H_
#define _LEKA_OS_LIB_LTDC_H_

#include "dsi.h"
#include "lcd_properties.h"

namespace leka {

extern LTDC_HandleTypeDef hltdc;
namespace ltdc {
	void init();
	void layerInit();
}	// namespace ltdc
}	// namespace leka

#endif	 // _LEKA_OS_LIB_LTDC_H_
