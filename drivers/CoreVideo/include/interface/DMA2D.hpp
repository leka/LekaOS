// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "LKComponentHandler.h"
#include "stm32f7xx_hal.h"

namespace leka::interface {

class DMA2DBase : public LKComponentHandler<DMA2D_HandleTypeDef>
{
  public:
	virtual ~DMA2DBase() = default;

	virtual void initialize() = 0;

	virtual void setFrameBufferAddress(uintptr_t address) = 0;

	virtual void transferData(uintptr_t src, uintptr_t dst_address, uint32_t width, uint32_t height)			 = 0;
	virtual void transferImage(uint32_t width, uint32_t height, uint32_t width_offset)							 = 0;
	virtual void transferDrawing(uintptr_t first_pixel_address, uint32_t width, uint32_t height, uint32_t color) = 0;

	virtual auto getHandle() -> DMA2D_HandleTypeDef & = 0;

	virtual void fillRect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color) = 0;
};

}	// namespace leka::interface
