// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_ANIMATION_BOUNCINGSQUARE_H_
#define _LEKA_OS_LIB_ANIMATION_BOUNCINGSQUARE_H_

// ? LCOV_EXCL_START - Exclude from coverage report

#include <cstdint>

#include "CGAnimation.h"
#include "interface/LKCoreGraphics.h"

namespace leka::animation {

class BouncingSquare : public interface::CGAnimation
{
  public:
	explicit BouncingSquare(interface::LKCoreGraphics &coregraphics);

	void start(void) final;
	void run(void) final;
	void stop(void) final;

  private:
	bool squareWillBeOutOfBoundVertical() const;
	bool squareWillBeOutOfBoundHorizontal() const;
	void updateSquareDirection();
	void updateSquarePosition();
	void updateSquareColor();
	void drawSquare();

	interface::LKCoreGraphics &_coregraphics;

	interface::LKCoreGraphics::FilledRectangle _square {{0, 0}, 100, 100};
	CGColor _color {.red = 0xFF, .green = 0x00, .blue = 0x00};

	struct Shift {
		int32_t horizontal;
		int32_t vertical;
	};
	Shift _shift {.horizontal = 1, .vertical = 1};
};

}	// namespace leka::animation

// ? LCOV_EXCL_STOP - Exclude from coverage report

#endif
