// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DMA2D_MOCK_H_
#define _LEKA_OS_DMA2D_MOCK_H_

#include "gmock/gmock.h"
#include "interface/CoreDMA2D.hpp"

namespace leka {

class CoreDMA2DMock : public interface::CoreDMA2D
{
  public:
	MOCK_METHOD(void, initialize, (), (override));
	MOCK_METHOD(void, transferData,
				(uintptr_t input_data_address, uintptr_t output_data_address, uint32_t width, uint32_t height),
				(override));
	MOCK_METHOD(void, transferImage, (uint32_t width, uint32_t height, uint32_t width_offset), (override));
	MOCK_METHOD(void, transferDrawing, (uintptr_t first_pixel_address, uint32_t width, uint32_t height, uint32_t color),
				(override));
	MOCK_METHOD(DMA2D_HandleTypeDef, getHandle, (), (override));
};

}	// namespace leka

#endif	 // _LEKA_OS_DMA2D_MOCK_H_
