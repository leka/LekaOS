// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "interface/DMA2D.hpp"

namespace leka {

class CoreDMA2D : public interface::DMA2DBase
{
  public:
	explicit CoreDMA2D(interface::STM32Hal &hal);

	void initialize() final;

	void transferData(uintptr_t input_data_address, uintptr_t output_data_address, uint32_t width,
					  uint32_t height) final;
	void transferImage(uint32_t width, uint32_t height, uint32_t width_offset) final;
	void transferDrawing(uintptr_t first_pixel_address, uint32_t width, uint32_t height, uint32_t color) final;

	auto getHandle() -> DMA2D_HandleTypeDef & final;
	void setHandle(const DMA2D_HandleTypeDef &hdma2d) { _hdma2d = hdma2d; }

  private:
	DMA2D_HandleTypeDef _hdma2d {};
	interface::STM32Hal &_hal;
};

}	// namespace leka
