// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreLCD.hpp"

#include "rtos/ThisThread.h"

namespace leka {

CoreLCD::CoreLCD(interface::LCDDriver &driver) : _driver(driver)
{
	// nothing to do
}

void CoreLCD::initialize()
{
	_driver.initialize();
	_driver.setLandscapeOrientation();
}

void CoreLCD::turnOn()
{
	_driver.turnOn();
}

void CoreLCD::turnOff()
{
	_driver.turnOff();
}

void CoreLCD::setBrightness(float value)
{
	_driver.setBrightness(value);
}

}	// namespace leka
