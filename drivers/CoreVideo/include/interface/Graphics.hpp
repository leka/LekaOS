// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>

namespace leka {

struct CGColor;
struct CGPoint;
struct CGRectangle;

}	// namespace leka

namespace leka::interface {

class Graphics
{
  public:
	virtual ~Graphics() = default;

	virtual void clearScreen(CGColor color) = 0;

	virtual void drawRectangle(CGRectangle rectangle, CGColor color) = 0;
};

}	// namespace leka::interface
