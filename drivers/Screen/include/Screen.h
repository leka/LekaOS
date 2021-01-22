// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_SCREEN_H_
#define _LEKA_OS_DRIVER_SCREEN_H_

#include "drivers/PwmOut.h"

namespace leka {

enum LCD
{
	OTM8009A
};

typedef struct {
	LCD name;

	uint16_t width;
	uint16_t height;

	uint16_t VSA;
	uint16_t VBP;
	uint16_t VFP;
	uint16_t VACT;
	uint16_t HSA;
	uint16_t HBP;
	uint16_t HFP;
	uint16_t HACT;

	void (*rotateUpsideDown)(bool upside_down);
} LCD_Model;

class LKScreen
{
  public:
	LKScreen(LCD_Model &lcd_model);

	uint16_t getWidth();
	uint16_t getHeight();
	void setBrightness(float value);
	void turnOff();
	void turnOn();

	void (*rotateUpsideDown)(bool upside_down);

  private:
	LCD_Model _lcd_model;
	mbed::PwmOut _brightness;
	float _brightness_value = 0;
};

}	// namespace leka

#endif	 // _LEKA_OS_DRIVER_SCREEN_H_
