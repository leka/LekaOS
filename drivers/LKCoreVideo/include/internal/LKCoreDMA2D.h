// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_DMA2D_H_
#define _LEKA_OS_LIB_DMA2D_H_

#include "LKCoreSTM32HalBase.h"

namespace leka {

class LKCoreDMA2D
{
  public:
	LKCoreDMA2D(LKCoreSTM32HalBase &hal);

	void initialize();

	void transferData(uintptr_t input_data_address, uintptr_t output_data_address, uint16_t width, uint16_t height);
	void transferImage(uint16_t width, uint16_t height, uint16_t width_offset);
	void transferDrawing(uintptr_t first_pixel_address, uint16_t width, uint16_t height, uint32_t color);

	DMA2D_HandleTypeDef getHandle(void);

  private:
	DMA2D_HandleTypeDef _hdma2d;
	LKCoreSTM32HalBase &_hal;
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_DMA2D_H_
