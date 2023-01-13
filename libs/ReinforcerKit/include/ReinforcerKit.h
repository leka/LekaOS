// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <interface/drivers/Motor.h>
#include <interface/libs/LedKit.h>
#include <interface/libs/VideoKit.h>

#include "MotionKit.h"

namespace leka {

class ReinforcerKit
{
  public:
	explicit ReinforcerKit(interface::VideoKit &videokit, interface::LedKit &ledkit, MotionKit &motion_kit)
		: _videokit(videokit), _ledkit(ledkit), _motionkit(motion_kit)
	{
		// nothing do to
	}

	enum class Reinforcer
	{
		BlinkGreen = 0,
		SpinBlink  = 1,
		Fire	   = 2,
		Sprinkles  = 3,
		Rainbow	   = 4
	};

	void playDefault();
	void play(const Reinforcer &reinforcer);
	void setDefaultReinforcer(const Reinforcer &reinforcer);
	void stop();

  private:
	interface::VideoKit &_videokit;
	interface::LedKit &_ledkit;
	MotionKit &_motionkit;
	Reinforcer _default_reinforcer = Reinforcer::Rainbow;

	void playBlinkGreen();
	void playSpinBlink();
	void playFire();
	void playSprinkles();
	void playRainbow();
};

}	// namespace leka
