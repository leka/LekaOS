// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "gmock/gmock.h"
#include "interface/Graphics.hpp"

namespace leka::mock {

class CoreGraphics : public interface::Graphics
{
  public:
	CoreGraphics();
	virtual ~CoreGraphics();

	MOCK_METHOD(void, clearScreen, (CGColor color), (override));
	MOCK_METHOD(void, drawRectangle, (FilledRectangle rectangle, CGColor color), (override));
};

}	// namespace leka::mock
