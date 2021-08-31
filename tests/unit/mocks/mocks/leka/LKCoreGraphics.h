// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_GRAPHICS_MOCK_H_
#define _LEKA_OS_GRAPHICS_MOCK_H_

#include "gmock/gmock.h"
#include "interface/CoreGraphics.h"

namespace leka {

class LKCoreGraphicsMock : public interface::CoreGraphics
{
  public:
	MOCK_METHOD(void, clearScreen, (CGColor color), (override));
	MOCK_METHOD(void, drawRectangle, (FilledRectangle rectangle, CGColor color), (override));
};

}	// namespace leka

#endif	 // _LEKA_OS_GRAPHICS_MOCK_H_
