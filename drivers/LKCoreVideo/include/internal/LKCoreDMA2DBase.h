// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_DMA2D_BASE_H_
#define _LEKA_OS_LIB_DMA2D_BASE_H_

#include "LKCoreSTM32HalBase.h"

namespace leka {

class LKCoreDMA2DBase
{
  public:
	virtual ~LKCoreDMA2DBase() = default;

	virtual void initialize() = 0;

	virtual auto getHandle() -> DMA2D_HandleTypeDef & = 0;

	virtual void setFrameBufferAddress(uintptr_t address) = 0;

	virtual void transferData(uintptr_t input_data_address, uintptr_t output_data_address, uint32_t width,
							  uint32_t height)																	 = 0;
	virtual void transferImage(uint32_t width, uint32_t height, uint32_t width_offset)							 = 0;
	virtual void transferDrawing(uintptr_t first_pixel_address, uint32_t width, uint32_t height, uint32_t color) = 0;
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_DMA2D_BASE_H_
