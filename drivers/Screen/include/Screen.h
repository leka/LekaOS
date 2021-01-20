// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_SCREEN_H_
#define _LEKA_OS_DRIVER_SCREEN_H_

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
} LCD_Model;

class LKScreen
{
  public:
	LKScreen(LCD_Model lcd_model);

  private:
	LCD_Model _lcd_model;
};

}	// namespace leka

#endif	 // _LEKA_OS_DRIVER_SCREEN_H_
