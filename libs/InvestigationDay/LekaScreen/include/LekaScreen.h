// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "PinNames.h"

#include "drivers/PwmOut.h"
#include "rtos/ThisThread.h"
#include "rtos/Thread.h"

#include "LekaLCD.h"

class Screen
{
  public:
	Screen();
	~Screen() {};

	void start(void);

  private:
	mbed::PwmOut _brightness;
	LekaLCD _lcd;
};
