// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "AudioKit.h"
#include "interface/drivers/Motor.h"
#include "interface/libs/LedKit.h"
#include "interface/libs/VideoKit.h"

namespace leka {

class BehaviorKit
{
  public:
	explicit BehaviorKit(interface::VideoKit &videokit, interface::LedKit &ledkit, interface::Motor &motor_left,
						 interface::Motor &motor_right, AudioKit &audiokit)
		: _videokit(videokit), _ledkit(ledkit), _motor_left(motor_left), _motor_right(motor_right), _audiokit(audiokit)
	{
		// nothing do to
	}

	void launching();
	void sleeping();
	void waiting();

	void blinkOnCharge();

	void lowBattery();
	void mediumLowBattery();

	void chargingEmpty();
	void chargingLow();
	void chargingMedium();
	void chargingHigh();
	void chargingFull();

	void bleConnectionWithoutVideo();
	void bleConnectionWithVideo();
	void working();

	void magicCardDetected();

	void fileExchange();

	void stop();

  private:
	interface::VideoKit &_videokit;
	interface::LedKit &_ledkit;
	interface::Motor &_motor_left;
	interface::Motor &_motor_right;
	AudioKit &_audiokit;
};

}	// namespace leka
