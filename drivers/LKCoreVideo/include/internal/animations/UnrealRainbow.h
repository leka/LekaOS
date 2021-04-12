// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_ANIMATION_UNREALRAINBOW_H_
#define _LEKA_OS_LIB_ANIMATION_UNREALRAINBOW_H_

#include <cstdint>

#include "CGAnimation.h"
#include "LKCoreGraphicsBase.h"

namespace leka::animation {

class UnrealRainbow : public interface::CGAnimation
{
  public:
	explicit UnrealRainbow(LKCoreGraphicsBase &coregraphics) : _coregraphics(coregraphics) {};

	void start(void) final {
		// nothing to do
	};
	void run(void) final;
	void stop(void) final;

  private:
	LKCoreGraphicsBase &_coregraphics;

	CGColor _color {0x01, 0x00, 0x01};

	int _dir		  = 1;
	uint8_t *_channel = &_color.red;

	void updateDirections();
};

}	// namespace leka::animation

#endif
