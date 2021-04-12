// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_ANIMATION_BOUNCINGSQUARE_H_
#define _LEKA_OS_LIB_ANIMATION_BOUNCINGSQUARE_H_

// MARK: GCOVR_EXCL_START

#include <cstdint>

#include "CGAnimation.h"
#include "LKCoreGraphicsBase.h"

namespace leka::animation {

class BouncingSquare : public interface::CGAnimation
{
  public:
	explicit BouncingSquare(LKCoreGraphicsBase &coregraphics);

	void start(void) final;
	void run(void) final;
	void stop(void) final;

  private:
	LKCoreGraphicsBase &_coregraphics;

	LKCoreGraphicsBase::FilledRectangle _square {{0, 0}, 100, 100};
	CGColor _color {0xFF, 0x00, 0x00};
	int16_t _horizontal_shift = 1;
	int16_t _vertical_shift	  = 1;

	bool squareWillBeOutOfBoundVertical() const;
	bool squareWillBeOutOfBoundHorizontal() const;
	void updateDirection();
	void updatePosition();
	void updateColor();
};

}	// namespace leka::animation

// MARK: GCOVR_EXCL_STOP

#endif
