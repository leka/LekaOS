// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "internal/TouchPinNames.h"

#include "CoreIOExpander.h"
#include "IOKit/DigitalIn.h"
#include "LogKit.h"
namespace leka {

class TouchSensorKit
{
  public:
	explicit TouchSensorKit(leka::io::expanded::DigitalIn<> &pin) : _pin(pin) { _pin.mode(PinMode::PullUp); };

	void updateState();
	void printState();

  private:
	leka::io::expanded::DigitalIn<> _pin;
	bool shadow_ear_left_touched;
	bool shadow_ear_right_touched;
	bool shadow_belt_left_back_touched;
	bool shadow_belt_left_front_touched;
	bool shadow_belt_right_back_touched;
	bool shadow_belt_right_front_touched;
};
}	// namespace leka
