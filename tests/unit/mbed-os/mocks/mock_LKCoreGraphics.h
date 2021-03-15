// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_GRAPHICS_MOCK_H_
#define _LEKA_OS_GRAPHICS_MOCK_H_

#include "LKCoreGraphicsBase.h"
#include "gmock/gmock.h"

namespace leka {

class LKCoreGraphicsMock : public LKCoreGraphicsBase
{
  public:
	MOCK_METHOD(void, clearScreen, (Color color), (override));
	MOCK_METHOD(void, drawPixel, (Pixel pixel, Color color), (override));
	MOCK_METHOD(void, drawRectangle, (FilledRectangle rectangle, Color color), (override));

	MOCK_METHOD(uintptr_t, getDestinationAddress, (), (override));
	MOCK_METHOD(uint32_t, getDestinationColor, (), (override));
};

}	// namespace leka

#endif	 // _LEKA_OS_GRAPHICS_MOCK_H_
