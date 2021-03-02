// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LKCoreLCD.h"

#include "rtos/ThisThread.h"

namespace leka {

LKCoreLCD::LKCoreLCD(LKCoreLCDDriverBase &driver) : _driver(driver)
{
	// nothing to do
}

void LKCoreLCD::initialize()
{
	_driver.initialize();
	_driver.setLandscapeOrientation();
}

void LKCoreLCD::turnOn()
{
	_driver.turnOn();
}

void LKCoreLCD::turnOff()
{
	_driver.turnOff();
}

void LKCoreLCD::setBrightness(float brightness)
{
	_driver.setBrightness(brightness);
}

}	// namespace leka
