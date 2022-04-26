// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "drivers/PwmOut.h"

#include "interface/LCDDriver.hpp"
#include "interface/drivers/LCD.hpp"

namespace leka {

class CoreLCD : public interface::LCD
{
  public:
	explicit CoreLCD(interface::LCDDriver &driver);

	void initialize() final;

	void turnOn() final;
	void turnOff() final;

	void setBrightness(float value) final;

  private:
	interface::LCDDriver &_driver;
};

}	// namespace leka
