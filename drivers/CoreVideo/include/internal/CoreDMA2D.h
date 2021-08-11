// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_DMA2D_H_
#define _LEKA_OS_LIB_DMA2D_H_

#include "LKCoreSTM32HalBase.h"
#include "corevideo_config.h"
#include "interface/CoreDMA2D.h"

namespace leka {

class CoreDMA2D : public interface::CoreDMA2D
{
  public:
	CoreDMA2D(LKCoreSTM32HalBase &hal);

	void initialize() final;

	void setFrameBufferAddress(uintptr_t address) final;
	auto getPositionAddress(uint32_t x, uint32_t y) -> uintptr_t;

	void transferData(uintptr_t src, uintptr_t dst_address, uint32_t width, uint32_t height) final;
	void transferImage(uint32_t width, uint32_t height, uint32_t width_offset) final;
	void transferDrawing(uintptr_t first_pixel_address, uint32_t width, uint32_t height, uint32_t color) final;
	void fillRect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color) final;

  private:
	LKCoreSTM32HalBase &_hal;

	uintptr_t _frame_buffer_address = lcd::frame_buffer_address;
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_DMA2D_H_
