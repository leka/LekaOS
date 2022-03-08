// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "interface/DMA2D.hpp"
#include "interface/drivers/STM32Hal.h"
#include "internal/corevideo_config.h"

namespace leka {

class CoreDMA2D : public interface::DMA2DBase
{
  public:
	explicit CoreDMA2D(interface::STM32Hal &hal);

	void initialize() final;

	void setFrameBufferAddress(uintptr_t address) final;
	auto getPixelAddress(uint32_t x, uint32_t y) const -> uintptr_t;

	void transferData(uintptr_t src, uintptr_t dst_address, uint32_t width, uint32_t height) final;
	void transferImage(uint32_t width, uint32_t height, uint32_t width_offset) final;
	void transferDrawing(uintptr_t first_pixel_address, uint32_t width, uint32_t height, uint32_t color) final;

	void fillRect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color) final;
	void setPixel(uint32_t x, uint32_t y, uint32_t color) final;

	auto getHandle() -> DMA2D_HandleTypeDef & final;
	void setHandle(const DMA2D_HandleTypeDef &hdma2d) { _hdma2d = hdma2d; }
	auto isReady() const -> bool final;

  private:
	DMA2D_HandleTypeDef _hdma2d {};
	interface::STM32Hal &_hal;

	uintptr_t _frame_buffer_address = lcd::frame_buffer_address;
};

}	// namespace leka
