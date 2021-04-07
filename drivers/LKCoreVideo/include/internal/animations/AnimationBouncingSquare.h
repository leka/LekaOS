// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_ANIMATION_BOUNCINGSQUARE_H_
#define _LEKA_OS_LIB_ANIMATION_BOUNCINGSQUARE_H_

#include <cstdint>

#include "rtos/ThisThread.h"

#include "CGAnimation.h"
#include "LKCoreGraphicsBase.h"

namespace leka {

class AnimationBouncingSquare : public interface::CGAnimation
{
  public:
	explicit AnimationBouncingSquare(LKCoreGraphicsBase &coregraphics);

	void start(void) final;
	void stop(void) final;

	bool squareIsOutOfBoundVertical() const;
	bool squareIsOutOfBoundHorizontal() const;
	void updateDirection();
	void updatePosition();
	void updateColor();

  private:
	LKCoreGraphicsBase &_coregraphics;
	bool _is_running = false;

	std::chrono::milliseconds _refresh_rate = std::chrono::milliseconds(1);

	LKCoreGraphicsBase::FilledRectangle _square {{0, 0}, 100, 100};
	Color _color {0xFF, 0x00, 0x00};
	int16_t _horizontal_shift = 1;
	int16_t _vertical_shift	  = 1;
};

}	// namespace leka

#endif
