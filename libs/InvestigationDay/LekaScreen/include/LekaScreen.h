/**
 * @file LekaScreen.h
 * @author Yann Locatelli
 *
 * @version 0.1
 * @date 2020-09-26
 *
 * @copyright Copyright (c) 2020
 */
#ifndef _LEKA_OS_LIB_SCREEN_H_
#define _LEKA_OS_LIB_SCREEN_H_

#include "mbed.h"

#include "LekaLCD.h"
#include "PinNames.h"

class Screen
{
  public:
	Screen();
	~Screen() {};

	void start(void);

  private:
	PwmOut _brightness;
	LekaLCD _lcd;
};

#endif
