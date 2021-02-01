// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LKScreen.h"

namespace leka {

using namespace mbed;

namespace screen {
	uint16_t width	= 800;
	uint16_t height = 480;
}	// namespace screen

LKScreen::LKScreen(LCD_Model &lcd_model) : _lcd_model(lcd_model), _brightness(SCREEN_BACKLIGHT_PWM)
{
	screen::width  = _lcd_model.width;
	screen::height = _lcd_model.height;

	_brightness.period(0.01f);	 // Set PWM at 1/(0.01 seconds) = 100Hz
	_brightness = 0.10f;
	// setBrightness(0.10f);

	driverInit		 = lcd_model.driverInit;
	rotateUpsideDown = lcd_model.rotateUpsideDown;
}

uint16_t LKScreen::getWidth()
{
	return _lcd_model.width;
}
uint16_t LKScreen::getHeight()
{
	return _lcd_model.height;
}

LCD_Model LKScreen::getModel()
{
	return _lcd_model;
}

__weak void LKScreen::setBrightness(float value)
{
	if (value > 1.0f) {
		_brightness_value = 1.0f;
	} else if (value < 0) {
		_brightness_value = 0;
	} else {
		_brightness_value = value;
	}
	_brightness = _brightness_value;
}

__weak void LKScreen::turnOff()
{
	_brightness = 0;
}

__weak void LKScreen::turnOn()
{
	_brightness = _brightness_value;
}

}	// namespace leka
