// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "gmock/gmock.h"
#include "interface/DMA2D.hpp"

namespace leka::mock {

class CoreDMA2D : public interface::DMA2DBase
{
  public:
	CoreDMA2D();
	virtual ~CoreDMA2D();

	MOCK_METHOD(void, initialize, (), (override));
	MOCK_METHOD(void, transferData,
				(uintptr_t input_data_address, uintptr_t output_data_address, uint32_t width, uint32_t height),
				(override));
	MOCK_METHOD(void, transferImage, (uint32_t width, uint32_t height, uint32_t width_offset), (override));
	MOCK_METHOD(void, transferDrawing, (uintptr_t first_pixel_address, uint32_t width, uint32_t height, uint32_t color),
				(override));
	MOCK_METHOD(DMA2D_HandleTypeDef &, getHandle, (), (override));
};

}	// namespace leka::mock
