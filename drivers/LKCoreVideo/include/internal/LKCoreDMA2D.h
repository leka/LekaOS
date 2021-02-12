// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_DMA2D_H_
#define _LEKA_OS_LIB_DMA2D_H_

#include "stm32f7xx_hal.h"

namespace leka {

class LKCoreDMA2D
{
  public:
	enum class Mode
	{
		M2M,		 // Memory-to-memory for 2D memory copy operations
		M2M_PFC,	 // Memory-to-memory with pixel format conversion for bitmap drawing with format conversion
		M2M_BLEND,	 // Memory-to-memory with pixel format conversion and blending for text drawing with transparency
		R2M			 // Register-to-memory for rectangle filling operations
	};

	LKCoreDMA2D();

	void initialize();

	static void load(Mode mode, uint32_t pdata_or_color, uint32_t destination_address, uint16_t xsize, uint16_t ysize,
					 uint16_t output_offset, uint32_t width_offset = 0);

  private:
	static DMA2D_HandleTypeDef _hdma2d;
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_DMA2D_H_
