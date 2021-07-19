// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreLCD.h"

#include "rtos/ThisThread.h"

using namespace leka;

CoreLCD::CoreLCD(interface::CoreLCDDriver &driver) : _driver(driver)
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

void CoreLCD::setBrightness(float brightness)
{
	_driver.setBrightness(brightness);
}
