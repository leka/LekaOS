// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_LKCORANIMATION_H_
#define _LEKA_OS_DRIVER_LKCORANIMATION_H_

#include "LKCoreGraphics.h"

namespace leka::animation {

class LKCoreAnimation
{
  public:
	enum SelectedAnimation
	{
		bouncing_square,
		slow_rainbow,
		falling_snowflakes,
		sparkle_stars
	};

  public:
	explicit LKCoreAnimation(LKCoreGraphics &coregraphics);

	void run(SelectedAnimation selected_animation);
	void run();
	void stop(void) const;

  private:
	void runFallingSnowflakes(void);
	void runSparkleStars(void);

  private:
	LKCoreGraphics &_coregraphics;
};

}	// namespace leka::animation

#endif	 // _LEKA_OS_DRIVER_LKCORANIMATION_H_
