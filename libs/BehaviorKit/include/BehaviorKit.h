// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "CoreMotor.h"
#include "LedKit.h"
#include "VideoKit.h"

namespace leka {

class BehaviorKit
{
  public:
	explicit BehaviorKit(VideoKit &videokit, LedKit &ledkit, CoreMotor &motor_left, CoreMotor &motor_right)
		: _videokit(videokit), _ledkit(ledkit), _motor_left(motor_left), _motor_right(motor_right) {};

	void spinLeft(float speed);
	void spinRight(float speed);

	void launching();
	void sleeping();
	void waiting();

	void chargingZero();
	void chargingRed();
	void chargingOrange();
	void chargingYellow();
	void chargingGreen();

	void blinkGreen();
	void spinBlink();
	void fire();
	void sprinkles();
	void rainbow();

	void stop();

  private:
	VideoKit &_videokit;
	LedKit &_ledkit;
	CoreMotor &_motor_left;
	CoreMotor &_motor_right;
};

}	// namespace leka
