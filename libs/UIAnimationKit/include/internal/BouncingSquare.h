// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

// LCOV_EXCL_START

#include <cstdint>

#include "CGAnimation.h"
#include "interface/Graphics.hpp"

namespace leka::animation {

class BouncingSquare : public interface::CGAnimation
{
  public:
	explicit BouncingSquare(interface::Graphics &coregraphics);

	void start() final;
	void run() final;
	void stop() final;

  private:
	[[nodiscard]] auto squareWillBeOutOfBoundVertical() const -> bool;
	[[nodiscard]] auto squareWillBeOutOfBoundHorizontal() const -> bool;
	void updateSquareDirection();
	void updateSquarePosition();
	void updateSquareColor();
	void drawSquare();

	interface::Graphics &_coregraphics;

	interface::Graphics::FilledRectangle _square = {{0, 0}, 100, 100};
	CGColor _color {.red = 0xFF, .green = 0x00, .blue = 0x00};

	struct Shift {
		int32_t horizontal;
		int32_t vertical;
	};
	Shift _shift {.horizontal = 1, .vertical = 1};
};

}	// namespace leka::animation

// LCOV_EXCL_STOP
