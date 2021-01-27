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

	uint32_t VSA;  /*!< Vertical start active time in units of lines */
	uint32_t VBP;  /*!< Vertical Back Porch time in units of lines */
	uint32_t VFP;  /*!< Vertical Front Porch time in units of lines */
	uint32_t VACT; /*!< Vertical Active time in units of lines = imageSize Y in pixels to display */
	uint32_t HSA;  /*!< Horizontal start active time in units of lcdClk */
	uint32_t HBP;  /*!< Horizontal Back Porch time in units of lcdClk */
	uint32_t HFP;  /*!< Horizontal Front Porch time in units of lcdClk */
	uint32_t HACT; /*!< Horizontal Active time in units of lcdClk = imageSize X in pixels to display */

	void (*driverInit)();
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

	void (*driverInit)();
	void (*rotateUpsideDown)(bool upside_down);

	LCD_Model getModel();

  private:
	LCD_Model _lcd_model;
	mbed::PwmOut _brightness;
	float _brightness_value = 0;
};

}	// namespace leka

#endif	 // _LEKA_OS_DRIVER_SCREEN_H_
