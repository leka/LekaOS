// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_DMA2D_H_
#define _LEKA_OS_LIB_DMA2D_H_

#include "LKCoreDMA2DBase.h"

namespace leka {

class LKCoreDMA2D : public LKCoreDMA2DBase
{
  public:
	explicit LKCoreDMA2D(LKCoreSTM32HalBase &hal);

	void initialize() final;

	void transferData(uintptr_t input_data_address, uintptr_t output_data_address, uint32_t width,
					  uint32_t height) final;
	void transferImage(uint32_t width, uint32_t height, uint32_t width_offset) final;
	void transferDrawing(uintptr_t first_pixel_address, uint32_t width, uint32_t height, uint32_t color) final;

	DMA2D_HandleTypeDef getHandle(void) final;

  private:
	DMA2D_HandleTypeDef _hdma2d;
	LKCoreSTM32HalBase &_hal;
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_DMA2D_H_
