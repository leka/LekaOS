// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "LedKit.h"
#include "interface/drivers/Motor.h"
#include "interface/libs/VideoKit.h"

namespace leka {

class BehaviorKit
{
  public:
	explicit BehaviorKit(interface::VideoKit &videokit, LedKit &ledkit, interface::Motor &motor_left,
						 interface::Motor &motor_right)
		: _videokit(videokit), _ledkit(ledkit), _motor_left(motor_left), _motor_right(motor_right)
	{
		// nothing do to
	}

	void spinLeft(float speed);
	void spinRight(float speed);
	void onScreensaverEnded(std::function<void(void)> const &callback);

	void launching();
	void sleeping();
	void waiting();

	void lowBattery();

	void chargingZero();
	void chargingRed();
	void chargingOrange();
	void chargingYellow();
	void chargingGreen();

	void underwater();
	void bubbles();
	void fly();
	void singing();

	void blinkGreen();
	void spinBlink();
	void fire();
	void sprinkles();
	void rainbow();

	void bleConnection(bool enableVideo);
	void neutral();

	void stop();

  private:
	interface::VideoKit &_videokit;
	LedKit &_ledkit;
	interface::Motor &_motor_left;
	interface::Motor &_motor_right;

	std::function<void(void)> _on_screensaver_ended {};
};

}	// namespace leka
