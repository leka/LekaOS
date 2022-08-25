// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <interface/drivers/Motor.h>
#include <interface/libs/VideoKit.h>

#include "LedKit.h"

enum class Reinforcer
{
	BlinkGreen = 0,
	SpinBlink  = 1,
	Fire	   = 2,
	Sprinkles  = 3,
	Rainbow	   = 4
};

namespace leka {

class ReinforcerKit
{
  public:
	explicit ReinforcerKit(interface::VideoKit &videokit, LedKit &ledkit, interface::Motor &motor_left,
						   interface::Motor &motor_right)
		: _videokit(videokit), _ledkit(ledkit), _motor_left(motor_left), _motor_right(motor_right)
	{
		// nothing do to
	}

	void runDefault();
	void run(const Reinforcer &reinforcer);
	void setDefaultReinforcer(const Reinforcer &reinforcer);

  private:
	interface::VideoKit &_videokit;
	LedKit &_ledkit;
	interface::Motor &_motor_left;
	interface::Motor &_motor_right;
	Reinforcer default_reinforcer = Reinforcer::BlinkGreen;

	void playBlinkGreen();
	void playSpinBlink();
	void playFire();
	void playSprinkles();
	void playRainbow();
};

}	// namespace leka
